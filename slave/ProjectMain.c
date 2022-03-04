#include<reg51.h>
#include<stdio.h>
#include<stdlib.h>
#include<intrins.h>
#include<string.h>
#define U8INT unsigned char
#define U16INT unsigned int
#define U32INT unsigned long int
#define max_cmd_length 20

sbit Key1_Wind=P1^5;
sbit Key2_Light=P1^6;
sbit Key3_Pump=P1^7;

sbit LED1_Wind=P2^5;
sbit LED2_Light=P2^6;
sbit LED3_Pump=P2^7;
sbit Pump_CONPin=P2^4;

sbit Motor_Wind_IN1=P1^0;
sbit Motor_Wind_IN2=P1^1;
sbit Motor_Light_IN3=P1^2;
sbit Motor_Light_IN4=P1^3;

extern void delay_ms(U16INT x);
extern void Display_String(U8INT row,U8INT col,U8INT *str);
extern void LCD_Init();
extern U8INT Temp_Byte[];
extern U8INT Get_Temp16bits();
extern void Motor_Wind_Run();
extern void Motor_Wind_Stop();
extern void Motor_Light_Run();
extern void Motor_Light_Stop(); 
extern void Pump_Run();		    
extern void Pump_Stop();
extern void Control_Pins_Init();		
U8INT Display_TempString[17];

volatile U8INT Received_Char_Buff[max_cmd_length+1];
volatile U8INT Buff_Index=0;
volatile U8INT Receive_Receipt=0; 



void main()
{
	float tempf=0.00f;

	EA=1;
	EX0=1;
	IT0=1;

	Control_Pins_Init();//初始化各引脚连接的部件
	Serial_Port_Init();

	LCD_Init();
	Display_String(0,0,"TempControlSyste");//第0行
	Get_Temp16bits();//读温度
	delay_ms(1000);
	while(1)
	{
		if(Get_Temp16bits())//如果读取成功
		{
			tempf=(int)((Temp_Byte[1]<<8)|Temp_Byte[0])*0.0625;
			sprintf(Display_TempString,"Temp:%5.1f",tempf);//	\xDF\x43
			strcat(Display_TempString,"\r\n");
			Transmit_String(Display_TempString);
			strcat(Display_TempString,"\xDF\x43");
			Display_String(1,4,Display_TempString);//第一行
			delay_ms(10);//延时10 去抖
			if(Receive_Receipt==1)
			{
				Receive_Receipt=0;
				if(strcmp(Received_Char_Buff,"WIND_OPEN")==0)
				{
					  Motor_Wind_Run();
				}
				else if(strcmp(Received_Char_Buff,"WIND_CLOSE")==0)
				{
					  Motor_Wind_Stop();
				}
				else if(strcmp(Received_Char_Buff,"LIGHT_OPEN")==0)
				{
					  Motor_Light_Run();
				}
				else if(strcmp(Received_Char_Buff,"LIGHT_CLOSE")==0)
				{
					  Motor_Light_Stop();
				}
				else if(strcmp(Received_Char_Buff,"PUMP_OPEN")==0)
				{
					  Pump_Run();
				}
				else if(strcmp(Received_Char_Buff,"PUMP_CLOSE")==0)
				{
					  Pump_Stop();
				}
			}



            //温度＞26度 通风电机启动
            //也可以手动启动
			if(tempf>26)
			{
				Motor_Wind_Run();
			}//温度＞18 <26度 通风电机关闭
			else if(tempf>18&&tempf<=26)
			{
				Motor_Wind_Stop();
			}  
		}

	}
}



void Transmit_String(char *str)
{
	U8INT i=0;
	while(str[i])
	{
		SBUF=str[i];
		while(TI==0);
		TI=0;
		i++;
	}
}

void Received_DataSPort(void) interrupt 4
{
	static U8INT i=0;
	if(RI==0)
	return;
	RI=0;

	if(SBUF=='$')
	{
		i=0;
		return;
	}
	if(SBUF==0x0D)//上位机到下位机传输的总是回车+换行
	{
		return;
	}
	if(SBUF==0x0A)
	{
		Receive_Receipt=1;
	}
	else//有效数据
	{
		Received_Char_Buff[i]=SBUF;
		Received_Char_Buff[++i]='\0';
	}
}

//外部中断0 函数
void EX0_Interrupt(void) interrupt 0
{
	if(Key1_Wind==0)
	{
		delay_ms(10);
		if(Key1_Wind==0)
		{
			if(LED1_Wind==1)
			{
			 	Motor_Wind_Stop();
			}
			else
			{
				Motor_Wind_Run();
			}
		}
	}
	if(Key2_Light==0)
	{
		delay_ms(10);
		if(Key2_Light==0)
		{
			if(LED2_Light==1)
			{
			 	Motor_Light_Stop();
			}
			else
			{
				Motor_Light_Run();
			}
		}
	}
	if(Key3_Pump==0)
	{
		delay_ms(10);
		if(Key3_Pump==0)
		{
			if(LED3_Pump==1)
			{
			 	Pump_Stop();
			}
			else
			{
				Pump_Run();
			}
		}
	}	
}


//串口初始化
void Serial_Port_Init()
{
	SCON=0x50;//串口选择1方式
	TMOD=0x20;//选择T1第二种方式 TCON？
	PCON=0x80;//使用倍频
	TH1=0xFA;//T1初始值
	TL1=0xFA;
	EA=1;//中断总开关
	ES=1;
	EX0=1;//外部中断0开关
	TR1=1;//开启定时器
	IT0=1;//触发电平类型，IT=1 低电平触发中断 IT=1 下降沿触发中断
}
