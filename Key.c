#include <REGX52.H>
#include "Delay.h"

/**
  * @brief  获取独立按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
  */

unsigned char Key()
{
	unsigned char KeyNumber=0;									//无按键按下时键码为0
		
	if(P3_1==0){Delay(10);while(P3_1==0);Delay(10);KeyNumber=1;}//当P3_1端口输出低电平时，输出按键键码为1（K1按键）
	if(P3_0==0){Delay(10);while(P3_0==0);Delay(10);KeyNumber=2;}//当P3_0端口输出低电平时，输出按键键码为2（K2按键）
	if(P3_2==0){Delay(10);while(P3_2==0);Delay(10);KeyNumber=3;}//当P3_2端口输出低电平时，输出按键键码为3（K3按键）
	if(P3_3==0){Delay(10);while(P3_3==0);Delay(10);KeyNumber=4;}//当P3_3端口输出低电平时，输出按键键码为4（K4按键）
		
	return KeyNumber;											//返回键码值
}

