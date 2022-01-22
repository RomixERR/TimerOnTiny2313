/*
    GyverTM1637 - ���������� ��� 7 ����������� ������� �� ���� TM1637 � ����� ��������
    ������������:
    GitHub: https://github.com/GyverLibs/GyverTM1637
    �����������:
    - ����� ���� �������� ��� ���������
    - ����� ���� �� ������ ��������� (������ ����) �������� ��� ���������
    - ��������� ������� ������ ����� � ����� (���� ��� ���� �����, ������ � ����)
    - ����� ����� �� -999 �� 9999 � ������ �����
    - ������� ������� ������� ������
    - ������� ����� ������� � ��������� ��������� ������������� ��������� �������
    - ������� ���������� �������� � �������� ������������ ���������
    - ������� ���������� �������� � �������� ����������� (����� ���� ��� �������)

    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    �������� �� ������������ ���������� "Grove_4Digital_Display" �� Frankie.Chu
    https://github.com/Seeed-Studio/Grove_4Digital_Display

    ������:
    v1.4 - ���������� ���� ������ � ������, ��������� ������������� � ESP
    v1.4.1 - ������������� ESP32
    v1.4.2 - ���� ��������� ����� �����, ����� �� ���������
*/

#ifndef _GyverTM1637_h
#define _GyverTM1637_h
#include <inttypes.h>
#include <Arduino.h>
class GyverTM1637 {
public:

    GyverTM1637(uint8_t clk, uint8_t dio);									// ���������� � �������������

    void display(uint8_t DispData[]);										// ������� ����� �������� �� �������. �� 0 �� 9 (byte values[] = {3, 5, 9, 0}; )
    void display(uint8_t BitAddr, uint8_t DispData);							// ������� ����� DispData � ��������� ������ ������� BitAddr
    void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// ���� ���� ��������� ������, ������� ����� � ������

    void displayByte(uint8_t DispData[]);									// ������� ���� ���� 0xe6 � �����-��������� ���� _a , _b .... ��������
    void displayByte(uint8_t BitAddr, uint8_t DispData);						// ������� ���� ���� 0xe6 � �����-��������� ���� _a , _b .... � ������
    void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// ���� ���� ��������� ������, ������� ����� � ������

    void displayClock(uint8_t hrs, uint8_t mins);							// ������� ���� � ������
    void displayTimer(uint8_t mins, uint8_t sec);							// ������� ������ � ���
    
    void displayInt(int value);												// ������� ����� �� -999 �� 9999 (��, �� ������ �����)

    
    void clear(void);														// �������� �������

    void point(boolean PointFlag, bool upd = true);											// ��� / ���� ����� (POINT_ON / POINT_OFF)

    void brightness(uint8_t bright, uint8_t = 0x40, uint8_t = 0xc0);		// ������� 0 - 7	

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
uint8_t digToHEX(uint8_t digit);		// ����� ��� ����� ��� displayByte

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

/************** ����� � ������� *****************/


#endif