#include <REGX52.H>				//引入单片机头文件
#include "LCD1602.h"			//LCD1602显示屏
#include "DS1302.h"				//DS1302时钟
#include "Key.h"				//按键
#include "Timer0.h"				//定时器0
#include "Delay.h"				//延时函数
#include "Buzzer.h"				//蜂鸣器
#include "WaterFallLight.h"		//流水灯
#include "UART.h"				//UART串口
#include "TimeShow.h"			//时间显示

unsigned int CountDown=10,c;	//定义变量
unsigned char KeyNum,MODE,TimeSetSelect,TimeSetFlashFlag,KeyFlag;

void TimeSet(void)				//更改时间设置
{
	if(KeyNum==2)				//按下K2按键
	{
		TimeSetSelect++;		//选中需要更改时间的位
		TimeSetSelect%=6;		//越界清零
	}
	
	if(KeyNum==3)										//按下K3按键
	{
		DS1302_Time[TimeSetSelect]++;					//选中位按下K3时+1
														//对参数进行合法性检查
		if(DS1302_Time[0]>99){DS1302_Time[0]=0;}		//年设置超过99时置为0
		if(DS1302_Time[1]>12){DS1302_Time[1]=1;}		//月设置超过12时置为1
		if(DS1302_Time[1]==1||DS1302_Time[1]==3||		//当月份为“一三五七八十腊”时
		DS1302_Time[1]==5||DS1302_Time[1]==7||DS1302_Time[1]==8||DS1302_Time[1]==10||DS1302_Time[1]==12)
		{
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}	//日期设置超过31天时置为1
		}
		else if(DS1302_Time[1]==4||						//当月份为“四六九冬”时
		DS1302_Time[1]==6||DS1302_Time[1]==9||DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}	//日期设置超过30天时置为1
		}
		else if(DS1302_Time[1]==2)						//当月份为2时特殊讨论闰年平年
		{
			if(DS1302_Time[0]%4==0)						//年份能被4整除，闰年（年份在0—99范围内不考虑百年不闰）
			{
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}//日期设置超过29天时置为1（闰年2月有29天）
			}
			else										//其他年份，平年
			{
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}//日期设置超过28天时置为1（平年2月有28天）
			}
		}
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}		//时设置超过23时置为0
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}		//分设置超过59时置为0
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}		//秒设置超过59时置为0

	}
	
	if(KeyNum==4)										//按下K4按键
	{
		DS1302_Time[TimeSetSelect]--;					//选中位按下K4时-1
														//对参数进行合法性检查
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}		//年设置小于0时置为99
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}		//月设置小于1时置为12
		if(DS1302_Time[1]==1||DS1302_Time[1]==3||		//当月份为“一三五七八十腊”时
			DS1302_Time[1]==5||DS1302_Time[1]==7||DS1302_Time[1]==8||DS1302_Time[1]==10||DS1302_Time[1]==12)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=31;}	//日设置小于1时置为31
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}	//同上，日期设置超过31天时置为1
		}
		else if(DS1302_Time[1]==4||DS1302_Time[1]==6||	//当月份为“四六九冬”时
			DS1302_Time[1]==9||DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=30;}	//日期设置小于1天时置为30
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}	//同上，日期设置超过30天时置为1
		}
		else if(DS1302_Time[1]==2)						//二月
		{
			if(DS1302_Time[0]%4==0)						//闰年
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=29;}//日期设置小于1天时置为29
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}//日期设置超过29天时置为1
			}
			else
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=28;}//日期设置小于1天时置为28
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}//日期设置超过28天时置为1
			}
		}
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}		//时设置小于0时置为23
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}		//分设置小于0时置为59
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}		//秒设置小于0时置为59
	}
	//设置时间时，if显示空格else显示时间，实现闪烁效果
	if(TimeSetSelect==0 && TimeSetFlashFlag==1){LCD_ShowString(1,1,"  ");}//年
	else{LCD_ShowNum(1,1,DS1302_Time[0],2);}
	if(TimeSetSelect==1 && TimeSetFlashFlag==1){LCD_ShowString(1,4,"  ");}//月
	else{LCD_ShowNum(1,4,DS1302_Time[1],2);}
	if(TimeSetSelect==2 && TimeSetFlashFlag==1){LCD_ShowString(1,7,"  ");}//日
	else{LCD_ShowNum(1,7,DS1302_Time[2],2);}
	if(TimeSetSelect==3 && TimeSetFlashFlag==1){LCD_ShowString(2,1,"  ");}//时
	else{LCD_ShowNum(2,1,DS1302_Time[3],2);}
	if(TimeSetSelect==4 && TimeSetFlashFlag==1){LCD_ShowString(2,4,"  ");}//分
	else{LCD_ShowNum(2,4,DS1302_Time[4],2);}
	if(TimeSetSelect==5 && TimeSetFlashFlag==1){LCD_ShowString(2,7,"  ");}//秒
	else{LCD_ShowNum(2,7,DS1302_Time[5],2);}
}

void main()							//主函数
{
	LCD_Init();						//LCD1602初始化
	DS1302_Init();					//DS1302初始化
	Timer0_Init();					//定时器0初始化
	UART_Init();					//UART串口初始化
	
	LCD_ShowString(1,1,"  -  -  ");	//在LCD上显示年月日之间的—
	LCD_ShowString(2,1,"  :  :  ");	//在LCD上显示时分秒之间的：
	LCD_ShowString(1,10,"Count");	//显示字符串
	LCD_ShowString(2,10,"Down:");	//CountDown意即倒计时
	
	DS1302_SetTime();				//将DS1302_Time数组的数字设置到DS1302中
	
	while(1)
	{			
		KeyNum=Key();				//获取独立按键键码
		if(KeyNum)					//按下任意按键
		{
			WaterFallLight();		//流水灯效果
			Buzzer_Time(50);		//蜂鸣器响
		}	
		
		switch(MODE)				//基于不同模式执行不同函数（替换if-else 更简洁）
		{
			case 0:TimeShow();break;//该情况显示时间
			case 1:TimeSet();break;	//该情况更改时间
		}
		if(KeyNum==1)				//按下按键K1
		{	
			if(MODE==0){MODE=1;TimeSetSelect=0;}		//切换模式 选中位置0
			else if(MODE==1){MODE=0;DS1302_SetTime();}	//切换模式 写入新设置的时间
		}
		
		if(MODE==0){LCD_ShowNum(2,15,CountDown,2);}		//该模式下启用倒计时
		else if(MODE==1){LCD_ShowNum(2,15,10,2);}		//不启用倒计时

		if(MODE==0 && KeyNum==4){KeyFlag=1;}			//该模式下倒计时标志位为1（后续用于启动倒计时）
		else if(MODE==0 && KeyNum==3){KeyFlag=0;}
		if(MODE==1){KeyFlag=0;}							//该模式下倒计时标志位为0

	}
}

void Timer0_Routine() interrupt 1	//中断函数
{
	static unsigned int T0Count;	//记录定时器中断的次数
	TL0 = 0x18;						//低字节初值
	TH0 = 0xFC;						//高字节初值（设置定时初值，调整定时时间为1ms）
	T0Count++;						//次数增加
	if(MODE==1)						//调时模式
	{
		if(T0Count>=350)					//350ms
		{
		T0Count=0;							//清零
		TimeSetFlashFlag=!TimeSetFlashFlag;	//闪烁标志位
		}
	}
	
	if(KeyFlag==1)							//倒计时标志位为1时
	{
		if(T0Count>=1000)					//1000ms即1s
		{
			T0Count=0;						//清零
			CountDown--;					//倒计时，秒数递减
			UART_SendByte(CountDown);		//将倒计时数据通过串口发送到上位机
			if(CountDown==0 && KeyFlag==1)	//当倒计时到0且不处于更改时间模式下
			{
				Buzzer_Time(100);			//蜂鸣器响
				WaterFallLight();			//流水灯效果
				CountDown=10;				//倒计时重新从10s开始
				KeyFlag=0;
			}

		}
	}

}