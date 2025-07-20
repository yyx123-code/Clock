#include <REGX52.H>
#include <Delay.h>

void WaterFallLight()	//流水灯函数
{
	unsigned int i;
	for(i=0;i<5;i++)	//执行五次
	{	
		P2=~(0x01<<i);	//移位计算 实现流水效果
		Delay(18);		//延时18ms
	}
	P2=0x00;			//全亮
	P2=0x1F;;			//回归初始状态（高位三个LED常亮）
}
