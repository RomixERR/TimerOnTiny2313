/*
    GyverTM1637 - бибилотека для 7 сегментного дисплея на чипе TM1637 с кучей приколюх
    Документация:
    GitHub: https://github.com/GyverLibs/GyverTM1637
    Возможности:
    - Вывод цифр массивом или прицельно
    - Вывод букв из списка доступных (листай ниже) массивом или прицельно
    - Отдельная функция вывода часов и минут (часы без нуля слева, минуты с нулём)
    - Вывод числа от -999 до 9999 с учётом знака
    - Готовая функция бегущей строки
    - Функции смены яркости и состояния двоеточия автоматически обновляют дисплей
    - Функция обновления значения с эффектом вертикальной прокрутки
    - Функция обновления значения с эффектом скручивания (лучше один раз увидеть)

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Основано на оригинальной библиотеке "Grove_4Digital_Display" от Frankie.Chu
    https://github.com/Seeed-Studio/Grove_4Digital_Display

    Версии:
    v1.4 - поправлены типы данных и ошибки, добавлена совместимость с ESP
    v1.4.1 - совместимость ESP32
    v1.4.2 - чуть переделан вывод точки, можно не обновлять
*/

#ifndef _GyverTM1637_h
#define _GyverTM1637_h
#include <inttypes.h>
#include <Arduino.h>
class GyverTM1637 {
public:

    GyverTM1637(uint8_t clk, uint8_t dio);									// объявление и инициализация

    void display(uint8_t DispData[]);										// выводит цифры массивом по ячейкам. От 0 до 9 (byte values[] = {3, 5, 9, 0}; )
    void display(uint8_t BitAddr, uint8_t DispData);							// выводит цифру DispData в указанную ячейку дисплея BitAddr
    void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// если лень создавать массив, выводит цифры в ячейки

    void displayByte(uint8_t DispData[]);									// выводит байт вида 0xe6 и буквы-константы вида _a , _b .... массивом
    void displayByte(uint8_t BitAddr, uint8_t DispData);						// выводит байт вида 0xe6 и буквы-константы вида _a , _b .... в ячейку
    void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// если лень создавать массив, выводит байты в ячейки

    void displayClock(uint8_t hrs, uint8_t mins);							// выводит часы и минуты
    void displayTimer(uint8_t mins, uint8_t sec);							// выводит минуты и сек
    
    void displayInt(int value);												// выводит число от -999 до 9999 (да, со знаком минус)

    
    void clear(void);														// очистить дисплей

    void point(boolean PointFlag, bool upd = true);											// вкл / выкл точку (POINT_ON / POINT_OFF)

    void brightness(uint8_t bright, uint8_t = 0x40, uint8_t = 0xc0);		// яркость 0 - 7	

    uint8_t PointData;

    

private:
    uint8_t lastData[4];
    void update();
    int  writeByte(int8_t wr_data);
    void start(void);
    void stop(void);

    void sendByte(uint8_t BitAddr, int8_t sendData);
    void sendArray(uint8_t sendData[]);

    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;


    uint8_t Clkpin;
    uint8_t Datapin;
};

void swapBytes(byte* newByte, byte oldByte, byte oldP, byte newP);
uint8_t digToHEX(uint8_t digit);		// вернёт код цифры для displayByte

//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
#define STARTADDR  0xc0

/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   1
#define POINT_OFF  0

/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

/************** БУКВЫ И СИМВОЛЫ *****************/


#endif