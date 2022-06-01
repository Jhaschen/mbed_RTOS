#include "mbed.h"
#include "LCD_Touch_SSD1289.h"
#include "Sharp_GP2Y.h"

Mutex displayMutex;
Mutex adcMutex;

GP2Y IV(P1_31); // Infrarotsensor vorne
GP2Y IR(P1_30); // Infrarotsensor rechts 
/*****Pin Connections to**************/
/*****Data Bus, LCD, Touch**/
BusOut dataBus( p26, p25, p24, p23, p22, p21, P2_6, P2_7); // 8 pins
SSD1289_LCD lcd( P0_22, p15, P0_24, P0_21, P0_19, P0_20, &dataBus, P0_25 );// control pins and data bus  CS, RS, WR, DIR, EN, LE, DATA_PORT, RD
TouchScreenADS7843 touch(p5, p6, p7, p8, P2_13, &lcd);     // ADS7843 -> mosi, miso, sclk, cs, irq, SPI_TFT

void Task_1(void){
    
    uint16_t Inf_v;
    char buffer[50];
    
    while(1)
    {
        adcMutex.lock();
        Inf_v = IV.getRange();
        adcMutex.unlock();
        sprintf(buffer,"Abstand vorne: %3d cm",Inf_v);
        displayMutex.lock();
        lcd.Print(buffer,0,50);
        displayMutex.unlock();
        ThisThread::sleep_for(100);
    }
}

void Task_2(void){
    
    uint16_t Inf_r;
    char buffer[50];
    
    while(1)
    {
        adcMutex.lock();
        Inf_r = IR.getRange();
        adcMutex.unlock();
        sprintf(buffer,"Abstand rechts: %3d cm",Inf_r);
        displayMutex.lock();
        lcd.Print(buffer,0,75);
        displayMutex.unlock();
        ThisThread::sleep_for(100);
    }
}

int main()
{
    lcd.Initialize(LANDSCAPE);
    lcd.FillScreen(COLOR_WHITE);
    lcd.SetFont(&TerminusBigFont);
    //touch.setCalibrationwithoutUser();
    //touch.TouchPanel_Calibrate();
    lcd.Print( "Abstandsmessung", 0, 25 );
    
    Thread thread_Task_1(osPriorityNormal,OS_STACK_SIZE,NULL,NULL) ;
    Thread thread_Task_2(osPriorityNormal,OS_STACK_SIZE,NULL,NULL) ;
    thread_Task_1.start(Task_1);
    thread_Task_2.start(Task_2);
    
    while(1)
    {
        ThisThread::sleep_for(59);
    }
    
}

