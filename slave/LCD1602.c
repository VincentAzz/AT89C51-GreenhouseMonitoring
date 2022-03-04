#include <reg51.h>
#include <intrins.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U8INT unsigned char
#define U16INT unsigned int
#define U32INT unsigned long int
#define LCD_Port P0

sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;

void delay_ms(U16INT x)//延时
{
    U8INT i=0;
    while(x--){
        for(i=0;i<120;i++);
    }
}

void Busy_Wait()//检查忙时
{
    U8INT LCD_Status=0x00;
    do{
        LCD_Port=0xFF;//lcd拉高
        EN=0;RS=0;RW=1;
        EN=1;_nop_();//EN设置下降沿来执行命令
        LCD_Status=LCD_Port;
        EN=0;_nop_();
    }
    while(LCD_Status&0x80);
}

void LCD_Write_CMD(U8INT cmd)//写命令
{
    Busy_Wait();//检查 忙
    EN=0;RS=0;RW=0;
    LCD_Status=cmd;
    EN=1;_nop_();
    EN=0;_nop_();
    Busy_Wait();//检查 忙
}

void LCD_Write_Data(U8INT dat)//写数据
{
    Busy_Wait();//检查 忙
    EN=0;RS=1;RW=0;
    LCD_Status=cmd;
    EN=1;_nop_();
    EN=0;_nop_();
    Busy_Wait();//检查 忙
}

void LCD_Init()
{
    LCD_Write_CMD(0x38);
    LCD_Write_CMD(0x0C);
    LCD_Write_CMD(0x06);
    LCD_Write_CMD(0x01);//清屏
}

//显示字符串
void Display_String(U8INT row, U8INT col, U8INT*str)
{
    U8INT UD_ADD[]={0x80,0xC0};
    U8INT i=0x00;
    LCD_Write_CMD(UD_ADD[row]|col);
    for(i=0;i<16&&str[i];i++)
    {
        LCD_Write_Data(str[i]);
    }
    for(;i<16;i++)//不够16格用空格填充
    {
        LCD_Write_Data(' ');
    }
}