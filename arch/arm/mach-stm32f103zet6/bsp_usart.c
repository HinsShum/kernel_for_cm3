/**
  * @file bsp_usart.c
  *
  * @brief usart initialization
  *
  * @author Hins Shum
  *
  * @date 2016/09/23 23:26:20
  *
  * @version v1.0
  */
/* include header file */
#include "mach/bsp_usart.h"

/******************************************************************************/
/*                            USART1                                          */
/*                         Configuration                                      */
/******************************************************************************/
/**
  * @brief  initialize usart1 gpio
  * @author Hins Shum
  * @date   2016/09/23
  * @param  None
  * @retval None
  */
static void bsp_usart1_gpio_init(void)
{
    /* usart1 gpio register */
    GPIO_InitTypeDef GPIO_InitStructure;
    /* enable usart1 gpio rcc clock */
    RCC_APB2PeriphClockCmd(USART1_GPIO_PORT_CLK, ENABLE);
    /* configure usart1 tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);
    /* configure usart1 rx as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);
} /* end bsp_usart1_gpio_init */

/**
  * @brief  write configuration value to register of usart1
  * @author Hins Shum
  * @date   2016/09/23
  * @param  None
  * @retval None
  */
static void bsp_usart1_register_init(void)
{
    /* usart1 register */
    USART_InitTypeDef USART_InitStructure;
    /* enable usart1 rcc clock */
    RCC_APB2PeriphClockCmd(USART1_RCC_CLK, ENABLE);
    /* usart1 configured as follow:
            - baudrate = 115200 baud
            - word length = 8 Bits
            - one stop bit
            - no parity
            - hardware flow control disabled (RTS and CTS signals)
            - receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    /* write configuration value to register of usart1 */
    USART_Init(USART1, &USART_InitStructure);
    /* enable usart1 */
    USART_Cmd(USART1, ENABLE);
} /* end bsp_usart1_register_init */

/**
  * @brief  initialize usart1
  * @author Hins Shum
  * @date   2016/09/23
  * @param  None
  * @retval successful value of usart1 initialization
  */
int32_t bsp_usart1_init(void)
{
    /* initialize usart1 gpio */
    bsp_usart1_gpio_init();
    /* initialize usart1 register */
    bsp_usart1_register_init();

    return 0;
} /* end bsp_usart1_init */

/**
  * @brief  send a character on serial port
  * @author Hins Shum
  * @date   2016/09/24
  * @param  c : a character will be send on serial port
  * @retval None
  */
void bsp_serial_putc(const char c)
{
    /* wait the content of the TDR register has been transferred into the shift register */
    while(RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE));
    /* output the character */
    USART_SendData(USART1, c);
    /* if \n, also do \r */
    if('\n' == c) {
        bsp_serial_putc('\r');
    }
} /* end bsp_serial_putc */

/**
  * @brief  send a string on serial port
  * @author Hins Shum
  * @date   2016/09/24
  * @param  s : a sting will be send on serial port
  * @retval len
  */
unsigned int serial_puts(const char *s, unsigned int len)
{
	unsigned int i = 0;
	
	for(i = 0; (i < len) && s[i]; i++) {
		bsp_serial_putc(s[i]);
	}
	
	return i;
}

/******************************************************************************/
/*                            USART1                                          */
/*                             End                                            */
/******************************************************************************/



