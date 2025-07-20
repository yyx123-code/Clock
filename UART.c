#include <REGX52.H>

/**
  * @brief  串口初始化，4800bps@11.0592MHz
  * @param  无
  * @retval 无
  */

void UART_Init()
{
	SCON=0x40;		//工作方式1 
	PCON &= 0x7F;	//波特率不倍速
	TMOD &= 0x0F;	//设置定时器模式 配置Timer1 高四位清零
	TMOD |= 0x20;	//设置定时器模式
	TL1 = 0xFA;		//设定定时初值
	TH1 = 0xFA;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */

void UART_SendByte(unsigned char Byte)
{
	SBUF=Byte;		//赋值要发送的一个字节数据
	while(TI==0);	//防止Byte没发完就发送第二个导致数据错误
	TI=0;			//发送完成标志位置0
}