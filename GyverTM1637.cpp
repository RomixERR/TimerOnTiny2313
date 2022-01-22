#include "GyverTM1637.h"
#include <Arduino.h>
const uint8_t digitHEX[] = { 0x3f, 0x06, 0x5b, 0x4f,
    0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x00, 0x40
};//0~9, ,-

GyverTM1637::GyverTM1637(uint8_t clk, uint8_t dio)
{
    Clkpin = clk;
    Datapin = dio;
    pinMode(Clkpin, OUTPUT);
    pinMode(Datapin, OUTPUT);
}

uint8_t digToHEX(uint8_t digit) {
    return digitHEX[digit];
}

int GyverTM1637::writeByte(int8_t wr_data)
{
    uint8_t i, count1;
    for (i = 0; i < 8; i++) //sent 8bit data
    {
        digitalWrite(Clkpin, LOW);
        if (wr_data & 0x01)digitalWrite(Datapin, HIGH); //LSB first
        else digitalWrite(Datapin, LOW);
        wr_data >>= 1;
        digitalWrite(Clkpin, HIGH);

    }
    digitalWrite(Clkpin, LOW); //wait for the ACK
    digitalWrite(Datapin, HIGH);
    digitalWrite(Clkpin, HIGH);
    pinMode(Datapin, INPUT);

    delayMicroseconds(50);
    uint8_t ack = digitalRead(Datapin);
    if (ack == 0)
    {
        pinMode(Datapin, OUTPUT);
        digitalWrite(Datapin, LOW);
    }
    delayMicroseconds(50);
    pinMode(Datapin, OUTPUT);
    delayMicroseconds(50);

    return ack;
}
//send start signal to GyverTM1637
void GyverTM1637::start(void)
{
    digitalWrite(Clkpin, HIGH); //send start signal to GyverTM1637
    digitalWrite(Datapin, HIGH);
    digitalWrite(Datapin, LOW);
    digitalWrite(Clkpin, LOW);
}
//End of transmission
void GyverTM1637::stop(void)
{
    digitalWrite(Clkpin, LOW);
    digitalWrite(Datapin, LOW);
    digitalWrite(Clkpin, HIGH);
    digitalWrite(Datapin, HIGH);
}

// ************************** ФОРМИРУЕМ ДАННЫЕ *****************************
void GyverTM1637::display(uint8_t DispData[])
{
    uint8_t SegData[4];
    for (byte i = 0; i < 4; i++) {
        //if (DispData[i] == 0x7f) SegData[i] = 0x00;
        //else 
        //{
        lastData[i] = digitHEX[DispData[i]];
        SegData[i] = digitHEX[DispData[i]] + PointData;
        //}
    }
    sendArray(SegData);
}
void GyverTM1637::displayByte(uint8_t DispData[])
{
    uint8_t SegData[4];
    for (byte i = 0; i < 4; i++) {
        //if (DispData[i] == 0x7f) SegData[i] = 0x00;
        //else SegData[i] = DispData[i];
        //{
        lastData[i] = DispData[i];
        SegData[i] = DispData[i] + PointData;
        //}	
    }
    sendArray(SegData);
}

void GyverTM1637::display(uint8_t BitAddr, uint8_t DispData)
{
    uint8_t SegData;
    //if (DispData == 0x7f) SegData = 0x00;
    //else
    //{  
    lastData[BitAddr] = digitHEX[DispData];
    SegData = digitHEX[DispData] + PointData;
    //}  
    sendByte(BitAddr, SegData);
}

void GyverTM1637::displayByte(uint8_t BitAddr, uint8_t DispData)
{
    uint8_t SegData;
    //if (DispData == 0x7f) SegData = 0x00;
    //else
    //{  
    lastData[BitAddr] = DispData;
    SegData = DispData + PointData;
    //}  
    sendByte(BitAddr, SegData);
}

// ************************** ОТПРАВКА НА ДИСПЛЕЙ *****************************
void GyverTM1637::sendByte(uint8_t BitAddr, int8_t sendData) {
    start();          //start signal sent to GyverTM1637 from MCU
    writeByte(ADDR_FIXED);//
    stop();           //
    start();          //
    writeByte(BitAddr | 0xc0); //
    writeByte(sendData);//
    stop();            //
    start();          //
    writeByte(Cmd_DispCtrl);//
    stop();           //
}

void GyverTM1637::sendArray(uint8_t sendData[]) {
    start();          //start signal sent to GyverTM1637 from MCU
    writeByte(ADDR_AUTO);//
    stop();           //
    start();          //
    writeByte(Cmd_SetAddr);//
    for (byte i = 0; i < 4; i++) {
        writeByte(sendData[i]);        //
    }
    stop();           //
    start();          //
    writeByte(Cmd_DispCtrl);//
    stop();           //
}
// ******************************************
void GyverTM1637::displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3) {
    uint8_t dispArray[] = { bit0, bit1, bit2, bit3 };
    displayByte(dispArray);
}
void GyverTM1637::display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3) {
    uint8_t dispArray[] = { bit0, bit1, bit2, bit3 };
    display(dispArray);
}

void GyverTM1637::clear(void)
{
    display(0x00, 0x7f);
    display(0x01, 0x7f);
    display(0x02, 0x7f);
    display(0x03, 0x7f);
    lastData[0] = 0x00;
    lastData[1] = 0x00;
    lastData[2] = 0x00;
    lastData[3] = 0x00;
}
void GyverTM1637::update(void)
{
    displayByte(lastData);
}

void GyverTM1637::brightness(uint8_t brightness, uint8_t SetData, uint8_t SetAddr)
{
    Cmd_SetData = SetData;
    Cmd_SetAddr = SetAddr;
    Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
    update();
}


void GyverTM1637::point(boolean PointFlag, bool upd)
{
    if (PointFlag) PointData = 0x80;
    else PointData = 0;
    if (upd) update();
}

// ************************** ВСЯКИЕ ФУНКЦИИ *****************************
void GyverTM1637::displayClock(uint8_t hrs, uint8_t mins) {
    if (hrs > 99 || mins > 99) return;
    uint8_t disp_time[4];
    if ((hrs / 10) == 0) disp_time[0] = 10;
    else disp_time[0] = (hrs / 10);
    disp_time[1] = hrs % 10;
    disp_time[2] = mins / 10;
    disp_time[3] = mins % 10;
    GyverTM1637::display(disp_time);
}

void GyverTM1637::displayTimer(uint8_t mins, uint8_t sec) {
    if (mins > 99 || sec > 59) return;
    uint8_t disp_time[4];
    disp_time[0] = (mins / 10);
    disp_time[1] = mins % 10;
    disp_time[2] = sec / 10;
    disp_time[3] = sec % 10;
    GyverTM1637::display(disp_time);
}

void GyverTM1637::displayInt(int value) {
    if (value > 9999 || value < -999) return;
    boolean negative = false;
    boolean neg_flag = false;
    byte digits[4];
    if (value < 0) negative = true;
    value = abs(value);
    digits[0] = (int)value / 1000;      	// количесто тысяч в числе
    uint16_t b = (int)digits[0] * 1000; 	// вспомогательная переменная
    digits[1] = ((int)value - b) / 100; 	// получем количество сотен
    b += digits[1] * 100;               	// суммируем сотни и тысячи
    digits[2] = (int)(value - b) / 10;  	// получем десятки
    b += digits[2] * 10;                	// сумма тысяч, сотен и десятков
    digits[3] = value - b;              	// получаем количество единиц

    if (!negative) {
        for (byte i = 0; i < 3; i++) {
            if (digits[i] == 0) digits[i] = 10;
            else break;
        }
    }
    else {
        for (byte i = 0; i < 3; i++) {
            if (digits[i] == 0) {
                if (digits[i + 1] == 0) {
                    digits[i] = 10;
                }
                else {
                    digits[i] = 11;
                    break;
                }
            }
        }
    }
    GyverTM1637::display(digits);
}






void swapBytes(byte* newByte, byte oldByte, byte newP, byte oldP) {
    byte newBit = 0;
    if (oldByte & (1 << oldP)) newBit = 1;
    else newBit = 0;
    *newByte = *newByte | (newBit << newP);
}
