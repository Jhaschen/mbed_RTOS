#include "mbed.h"
#include "Sharp_GP2Y.h"
#include "LCD_Touch_SSD1289.h"

GP2Y DF(P1_31); // Infrarot vorn
GP2Y DB(P0_2); // Infrarot hinten

/*****Pin Connections to**************/
/*****Data Bus, LCD, Touch**/
BusOut dataBus( p26, p25, p24, p23, p22, p21, P2_6, P2_7); // 8 pins
SSD1289_LCD lcd( P0_22, p15, P0_24, P0_21, P0_19, P0_20, &dataBus, P0_25 );// control pins and data bus CS, RS, WR, DIR, EN, LE, DATA_PORT, RD
TouchScreenADS7843 touch(p5, p6, p7, p8, P2_13, &lcd);// ADS7843 -> mosi, miso, sclk, cs, irq, SPI_TFT

// Mutex
Mutex read_mutex; // Create and initialize a Mutex object
Mutex write_mutex; // Create and initialize a Mutex object





void Task_1_readInfrarot1(void)
{
    // Variable
uint16_t distance_front;
char stringBuffer[50];
while(1) {



read_mutex.lock(); // Schützt die Variable
distance_front=DF.getRange();
sprintf(stringBuffer,"Inf_vorne= %2d cm", distance_front);
read_mutex.unlock();

write_mutex.lock(); // Schützt die Variable
lcd.Print(stringBuffer, 0, 0, COLOR_RED,-1);
write_mutex.unlock();

ThisThread::sleep_for(100);
}
}

void Task_2_readInfrarot2(void)
{
uint16_t distance_back;
char stringBuffer1[50];
while(1) {

read_mutex.lock(); // Schützt die Variable
distance_back=DB.getRange();
sprintf(stringBuffer1,"Inf_hinten= %2d cm", distance_back);
read_mutex.unlock();

write_mutex.lock(); // Schützt die Variable
lcd.Print(stringBuffer1, 0, 30,COLOR_RED,-1 );
write_mutex.unlock();

ThisThread::sleep_for(100);
}
}

// MAIN
int main()
{
Thread thread_Task_1(osPriorityNormal, OS_STACK_SIZE, NULL, NULL); // memory-pointer, name
Thread thread_Task_2(osPriorityNormal, OS_STACK_SIZE, NULL, NULL);

lcd.Initialize(LANDSCAPE);
lcd.FillScreen(COLOR_BLACK);
lcd.SetFont(&TerminusBigFont);

thread_Task_1.start(Task_1_readInfrarot1); // thread mit der Funktion readInfrarot verknüpfen und starten
thread_Task_2.start(Task_2_readInfrarot2);

while(1) {
ThisThread::sleep_for(2000);
}
}