/*************************************************************************/
#include"msp430f6638.h"
/*******************************************
P5.3----E                       P57  P56  P55  P54  P53  P52  P51  P50
P5.4----RS           
P5.5----RW                    P57  P56  RW   RS     E    P52  P51  P50
P2OUT---wdata
********************************************/
#define uchar unsigned char;
#define uint unsigned int;
//*********显示字符串可将字符串放入数组，例如显示ancd efdg******/
/**定义一个数组**/
/**uchar code ttt[]="ancd efdg";//字符串用双引号引起来，空格计入字符串的元素
/*uchar code kkk[]="ffba  ggddmm";//上句加空格9个元素，该句加两个空格12个元素
/**********************************元素个数在循环显示时要使用*****
/*********************为循环显示字符串还得定义一个变量**/
// uchar  n;/定义变量n
void   delay(int time)   //延迟函数
  {
      while(time--); //延迟时间
  }
void   writeCommandLcd(unsigned char comd)// 写入命令
  {
    P5OUT &= ~0x10; //RS清0    (运算后P5=xxx0 xxxx,即RS=0其他位状态不变） 
    P5OUT &= ~0x20; //RW清0   (运算后P5=xx0x xxxx, 即RW=0其他位状态不变)
    P5OUT &= ~0x08; //使能E清0   (运算后P5=xxxx 0xxx, 即E=0其他位状态不变) 
              //上面三个语句执行完后P5OUT=xx000xxx,用P5OUT &=~0x38；可一次完成。
    P2OUT  =  comd;  //送命令到P2OUT准备输出进入1602，等E下降沿来即可进入1602.
    P5OUT |=  0x08;  //使能E置1，P5OUT= xxxx xxxx+0000 1000=xxxx1xxx，E=1.
    delay(10);
    P5OUT &= ~0x08; //使能E清0，这样E从1变0，产生一个下降,写入命令到1602。
  }
 void   writeDataLcd(unsigned char dat)// 写入命令
  {
    P5OUT  |= 0x10; //RS置1    (运算后P5=xxx1 xxxx,即RS=1其他位状态不变） 
    P5OUT &= ~0x20; //RW清0   (运算后P5=xx0x xxxx, 即RW=0其他位状态不变)
    P5OUT &= ~0x08; //使能E清0   (运算后P5=xxxx 0xxx, 即E=0其他位状态不变) 
    P2OUT  =  dat;  //送数据到P2OUT准备输出进入1602，等E下降沿来即可进入1602.
    P5OUT |=  0x08;  //使能E置1，P5OUT= xxxx xxxx+0000 1000=xxxx1xxx，E=1.
    delay(10);
    P5OUT &= ~0x08; //使能E清0，这样E从1变0，产生一个下降,写入命令到1602。
  } 
void lcd_init(void) //初始化LCD
  {
    delay(10);
    writeCommandLcd(0x38);//显示模式设置为1602，5X7点阵显示。8位数据接口。
    delay(10);
    writeCommandLcd(0x08);//打开显示液晶
    writeCommandLcd(0x01);//清屏，光标回到起点
    //writeCommandLcd(0x0C);//开显示，关光标，即不显示光标。
    //writeCommandLcd(0x0E);//开显示，开光标，有光标但不闪烁。
    writeCommandLcd(0x0F);//开显示，开光标，有光标且光标闪烁。
    writeCommandLcd(0x06);//读写操作时地址自动加1。   
  }
 int main(void)
 {
 WDTCTL=WDTPW+WDTHOLD;
    P2DIR=0xff;
    P5DIR=0xff;
    lcd_init();
      /********************字符串显示代替单字显示*************************************
      for (n=0;n<9;n++)
      {
      writeData(ttt[n]);//从第一行第一位显示（第2行显示需80h+40h）
      delay(10);      
      }
      weitecommandLcd(0x80+0x40);//第2行显示必须从第2行首地址开始
      for (n=0;n<12;n++)
      {
      writeData(ttt[n]);//从第一行第一位显示（第2行显示需80h+40h）
      delay(10);      
      }
      while(1);
      ********************闪烁动态效果可将延迟时间加长*****************************/
    while(1)
    {
    writeCommandLcd(0x80);//从第一行第一位显示（地址是80h）
/*其实1行1列地址是00h，但在输入地址是时要保证D7位为1，其余位确定的是实际地址，这样
00h=0100 0000b要保证D7为1，就变成了1000 0000b，即80h，相当于给实际地址加8，如第2行2列，
实际地址为41h=0100 0000b，高位变1就成了11000000b=c0h*/
     delay(10);
    writeDataLcd(0x30+0);//30为0在1602字库的存储地址，依次类推1、2、3....
      delay(10);
    writeDataLcd(0x30+1);
    delay(10);
    writeDataLcd(0x30+2);
    delay(10);
    writeDataLcd(0x30+3); 

    while(1);
    }
 }/*************************************************************************/