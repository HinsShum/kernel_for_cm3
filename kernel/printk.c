/**
 * /kernel/printk.c
 *
 * Copyright (C) 2017 HinsShum
 *
 * printk.c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*---------- includes ----------*/
#include <kernel/kernel.h>
#include <kernel/bug.h>
#include <kernel/mutex.h>
#include <kernel/semaphore.h>
#include <kernel/init.h>

/*---------- marcos ----------*/
#define CONFIG_LOG_BUF_SHIFT        (9)
#define __LOG_BUF_LEN               (1 << CONFIG_LOG_BUF_SHIFT) /* 512 bytes */

/**
 * for_each_console() allows you to iterate on each console
 */
#define for_each_console(con)   \
            for(con = console_driver; con != NULL; con = con->next)

/**
 * printk's without a loglevel use this..
 */
#define DEFAULT_MESSAGE_LOGLEVEL    (4)     /* KERN_WARNING */
#define DEFAULT_CONSOLE_LOGLEVEL    (7)     /* anything MORE serious than KERN_DEBUG */

#define LOG_BUF_MASK        (log_buf_len - 1)
#define LOG_BUF(index)      (log_buf[(index) & LOG_BUF_MASK])

/*---------- variables ----------*/
int console_printk[2] = {
    DEFAULT_CONSOLE_LOGLEVEL,   /* console_loglevel */
    DEFAULT_MESSAGE_LOGLEVEL    /* default_message_loglevel */
};

#ifdef CONFIG_PRINTK
/**
 * The indices into log_buf are not constrained to log_buf_len - they
 * must be masked before subscripting
 */
static unsigned int con_start = 0;  /* Index into log_buf: next char to be sent to consoles */
static unsigned int log_end = 0;    /* Index into log_buf: most-recently-written-char + 1 */

static char __log_buf[__LOG_BUF_LEN];
static char *log_buf = __log_buf;
static int log_buf_len = __LOG_BUF_LEN;

static DECLARE_MUTEX(logbuf_lock);

static DECLARE_SEMAPHORE(console_sem);

static int new_text_line = 1;
static char printk_buf[512];

/*---------- functions ---------*/
static void emit_log_char(char c)
{
    LOG_BUF(log_end) = c;
    log_end++;
    if(log_end - con_start > log_buf_len) {
        con_start = log_end - log_buf_len;
    }
}

/**
 * try_acquire_console_sem() - Try to get console semaphore
 * without waiting to actually show the kernel messages from a
 * 'printk'.
 *
 * Get the console semaphore return 0, otherwise return -1.
 */
static int try_acquire_console_sem(void)
{
    int retval = -1;

    if(semaphore_down(&console_sem)) {
        retval = 0;
    }
    return retval;
}

/**
 * Check if we have any console registered that can be called early in boot.
 */
static int have_callable_console(void)
{
/*
    struct console *con = NULL;

    for_each_console(con) {
        if(con->flags & CON_ANYTIME) {
            return true;
        }
    }
*/
    return true;
}

/**
 * Can we actually use the console at this time?
 *
 * Console drivers may assume that per-cpu resources have
 * been allocated. So unless they're explicitly marked as
 * being able to cope (CON_ANYTIME) don't call them.
 */
static inline int can_use_console(void)
{
    return have_callable_console();
}

/**
 * Try to get console ownership to actually show the kernel
 * messages from a 'printk'. Return true (and with the
 * console_semaphore held, and 'console_locked' set) if it
 * is successful, false otherwise.
 *
 * This gets called with the 'logbuf_lock' mutexlock held. It
 * should return with 'lockbuf_lock' released.
 */
static int acquire_console_semapgore_for_printk(void)
{
    int retval = false;

    if(!try_acquire_console_sem()) {
        retval = true;

        /**
         * If we can't use the console, we need to release
         * the console semaphore by hand to avoid flushing
         * the buffer. We need to hold the console semaphore
         * in order to do this test safely.
         */
        if(!can_use_console()) {
            semaphore_up(&console_sem);
            retval = false;
        }
    }

    mutex_unlock(&logbuf_lock);
    return retval;
}

extern unsigned int serial_puts(const char *s, unsigned int len);
static void __call_console_driver(unsigned int start, unsigned int end)
{
/*
    struct con *con = NULL;

    for_each_console(con) {
        if((con->flags & CON_ENABLE) && (con->flags & CON_ANYTIME) && con->write) {
            con->write(con, &LOG_BUF(start), end - start);
        }
    }
*/
    // 临时用来往串口发送字符串用的函数, 将来要删除, 用上面注释掉的代码来实现这个功能
    serial_puts(&LOG_BUF(start), end - start);
}

/**
 * Write out chars from start to end - 1 inclusive
 */
static void _call_console_driever(unsigned int start, unsigned int end, int msg_log_level)
{
    if((msg_log_level < console_loglevel) && (start != end) /*&& console_driver*/) {
        if((start & LOG_BUF_MASK) > (end & LOG_BUF_MASK)) {
            /* wrapped write */
            __call_console_driver(start & LOG_BUF_MASK, log_buf_len);
            __call_console_driver(0, end & LOG_BUF_MASK);
        } else {
            __call_console_driver(start, end);
        }
    }
}

/**
 * Call the console drivers, asking them to write out
 * log_buf[start] to log_buf[end - 1].
 * The console_sem must be held.
 */
static void call_console_drivers(unsigned int start, unsigned int end)
{
    unsigned int cur_index = 0, start_print = 0;
    static int msg_level = -1;

    BUG_ON(((int)(start - end)) > 0);

    cur_index = start;
    start_print = start;
    while(cur_index != end) {
        if((msg_level < 0) && ((end - cur_index) > 2) &&
                LOG_BUF(cur_index + 0) == '<' &&
                LOG_BUF(cur_index + 1 >= '0') &&
                LOG_BUF(cur_index + 1 <= '7') &&
                LOG_BUF(cur_index + 2 == '>')) {
            msg_level = LOG_BUF(cur_index + 1) - '0';
            cur_index += 3;
            start_print = cur_index;
        }
        while(cur_index != end) {
            char c = LOG_BUF(cur_index);

            cur_index++;
            if(c == '\n') {
                if(msg_level < 0) {
                    /**
                     * printk() has already given us loglevel tags in
                     * the buffer.  This code is here in case the
                     * log buffer has wrapped right round and scribbled
                     * on those tags
                     */
                    msg_level = default_message_loglevel;
                }
                _call_console_driever(start_print, cur_index, msg_level);
                msg_level = -1;
                start_print = cur_index;
                break;
            }
        }
    }
    _call_console_driever(start_print, end, msg_level);
}

/**
 * release_console_sem() - unlock the console system
 *
 * Releases the semaphore which the caller holds on the console system
 * and the console driver list.
 *
 * While the semaphore was held, console output may have been buffered
 * by printk().  If this is the case, release_console_sem() emits
 * the output prior to releasing the semaphore.
 */
static void release_console_sem(void)
{
    unsigned int _con_start = 0, _log_end = 0;

    _con_start = con_start;
    _log_end = log_end;
    con_start = log_end;    /* Flush */

    call_console_drivers(_con_start, _log_end);
    semaphore_up(&console_sem);
}

int vprintk(const char *fmt, va_list args)
{
    int printed_len = 0, current_log_level = default_message_loglevel;
    char *p = NULL;

    /* lock log buf */
    if(!logbuf_lock) {
        return false;
    }
    mutex_lock_blocked(&logbuf_lock);
    printed_len = vscnprintf(printk_buf, sizeof(printk_buf), fmt, args);

    p = printk_buf;

    /* Do we have a loglevel in the string? */
    if(p[0] == '<') {
        unsigned char c = p[1];
        if(c && p[2] == '>') {
            switch(c) {
                case '0' ... '7' :  /* loglevel */
                    current_log_level = c - '0';
                /* Fallthrough - make sure we're on a new line */
                case 'd' : /* KERN_DEFAULT */
                    if(!new_text_line) {
                        emit_log_char('\n');
                        printed_len += 1;
                        new_text_line = 1;
                    }
                /* Fallthrough - skip the loglevel */
                case 'c' :
                    p += 3;
                    printed_len -= 3;
                    break;
            }
        }
    }

    /**
     * Copy the output into log_buf.  If the caller didn't provide
     * appropriate log level tags, we insert them here
     */
    for(; *p; p++) {
        if(new_text_line) {
            emit_log_char('<');
            emit_log_char(current_log_level + '0');
            emit_log_char('>');
            printed_len += 3;
            new_text_line = 0;
        }

        emit_log_char(*p);
        if(*p == '\n') {
            new_text_line = 1;
        }
    }
    /**
     * Try to acquire and then immediately release the
     * console semaphore. The release will do all the
     * actual magic (print out buffers, wake up klogd,
     * etc).
     *
     * The acquire_console_semaphore_for_printk() function
     * will release 'logbuf_lock' regardless of whether it
     * actually gets the semaphore or not.
     */
    if(acquire_console_semapgore_for_printk()) {
        release_console_sem();
    }

    return printed_len;
}

/**
 * printk - print a kernel message
 * @fmt: format string
 *
 * This is printk().  It can be called from any context.  We want it to work.
 *
 * We try to grab the console_sem.  If we succeed, it's easy -
 * we log the output and call the console drivers.  If we fail
 * to get the semaphore we place the output into the log buffer
 * and return. The current holder of the console_sem will notice
 * the new output in release_console_sem() and will send it to
 * the consoles before releasing the semaphore.
 *
 * One effect of this deferred printing is that code which calls printk() and
 * then changes console_loglevel may break. This is because console_loglevel
 * is inspected when the actual printing occurs.
 *
 * See also:
 * printf(3)
 *
 * See the vsnprintf() documentation for format string extensions over C99.
 */
int printk(const char *fmt, ...)
{
    va_list args;
    int len = 0;

    va_start(args, fmt);
    len = vprintk(fmt, args);
    va_end(args);

    return len;
}

/**
 * 临时用的, 将来要删除
 */
static int __init semaphore_mutex_init(void)
{
    mutex_init(&logbuf_lock);
    semaphore_binary_init(&console_sem);
    semaphore_up(&console_sem);

    return 0;
}
arch_initcall(semaphore_mutex_init);

#endif
