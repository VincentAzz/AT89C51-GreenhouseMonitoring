#include<reg51.h>
#include<stdio.h>
#include<string.h>
#include<intrins.h>
#include<stdlib.h>

#define U8INT unsigned char 
#define U16INT unsigned int 
#define U32INT unsigned long int

//定义引脚
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

//初始化状态：均为停止状态
//main函数中调用
void Control_Pins_Init()
{
	Key1_Wind=1;
	Key2_Light=1;
	Key3_Pump=1;
	Motor_Wind_IN1=1;
	Motor_Wind_IN2=1;
	Motor_Light_IN3=1;
	Motor_Light_IN4=1;
	Pump_CONPin=1;
	LED1_Wind=0;
	LED2_Light=0;
	LED3_Pump=0;
}


//各部件的运行状态函数

void Motor_Wind_Run()
{
	Motor_Wind_IN1=1;//电压差 高
	Motor_Wind_IN2=0;//低
	LED1_Wind=1;//LED高电平点亮
}
void Motor_Wind_stop()
{
	Motor_Wind_IN1=1;//两端电压相同，停止
	Motor_Wind_IN2=1;
	LED1_Wind=0;
}
void Motor_Light_Run()
{
	Motor_Light_IN3=1;
	Motor_Light_IN4=0;
	LED2_Light=1;
}
void Motor_Light_stop()
{
	Motor_Light_IN3=1;
	Motor_Light_IN4=1;
	LED2_Light=0;
}
void Pump_Run()
{
	Pump_CONPin=0;//低电平启动
	LED3_Pump=1;
}
void Pump_Stop()
{
	Pump_CONPin=1;//高电平停止
	LED3_Pump=0;
}

