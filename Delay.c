//延时函数 由STC-ISP的“软件延时计算器”生成
void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} 
		while (--i);
	}
}