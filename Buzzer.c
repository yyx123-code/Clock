#include <REGX52.H>
#include <INTRINS.H>

sbit Buzzer=P2^5;			//蜂鸣器端口

/**
  * @brief  蜂鸣器VIP专享延时函数500us
  * @param  无
  * @retval 无
  */
void Buzzer_Delay500us()	//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 247;
	while (--i);
}

/**
  * @brief  蜂鸣器发声
  * @param  ms 发声的时长
  * @retval 无
  */
void Buzzer_Time(unsigned int ms)//蜂鸣器发声函数
{
	unsigned int i;
	for(i=0;i<ms*2;i++)			//发声时长
		{
			Buzzer=!Buzzer;		//蜂鸣器翻转
			Buzzer_Delay500us();//延时500us
		}
}