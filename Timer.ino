/*
 Name:		Timer.ino
 Created:	19.01.2022 23:48:37
 Author:	RomixERR
*/
#include <GyverTM1637.h>
#include <MyIncl2313.ino>
#define KEY10_PIN 8		//������ ���������� ��˨��� +10��� (�� �����, �������� ������ � + ��� ����������� � ����)
#define KEY1_PIN 9		//������ ���������� ��˨��� +1��� (�� �����, �������� ������ � + ��� ����������� � ����)
#define RELE_PINOUT 13	//������� ���� (������ �����)
#define ALARM_PINOUT 12 //������ (������ �����, ������� �������� ��� ��)
#define CLK 4			//��� 7-�� ����������� ����������
#define DIO 5			//��� 7-�� ����������� ����������
#define AlarmDur 5		//����� ������ ������ � ��������
#define DelayOnStart 4 //����� �������� ����� (��� ����� �������) � ��������
#define HalfTimePeriod 500 //500�� �������� ������� � ������������� (����� ��������� ��� ����� 100�� ��������)


//������������ ������� ��������� ������� (�����) ���������� ������������
uint32_t myTimer1, myTimer2; 
uint16_t period1 = 100, period2 = HalfTimePeriod, alarmCount; 
//���������� ������� ��������
GyverTM1637 disp(CLK, DIO);
//������ ����������
uint16_t timeLeft=0;
uint8_t const countExitDefault = DelayOnStart * 1000 / period1;
uint8_t const countAlarmDefault = AlarmDur * 1000 / period1;
uint8_t countExit = countExitDefault;							//countExit == countExitDefault - ������� ����� (������ ������)
bool flagIsPress, flagPoint = true;		//flagIsPress - ���� ���� ��� ������ ������������, flagPoint - ��������� ��� ����� �� ������
bool flagKey1Pressed, flagKey10Pressed;


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(KEY10_PIN, INPUT_PULLUP); 
	pinMode(KEY1_PIN, INPUT_PULLUP);
	pinMode(RELE_PINOUT, OUTPUT);   
	pinMode(ALARM_PINOUT, OUTPUT);
	InitialTIMER0();
	disp.clear();
	disp.brightness(7);  // �������, 0 - 7 (������� - ��������)
}

// the loop function runs over and over again until power down or reset
void loop() {

	//if (millis() - myTimer1 >= period1) {   // ���� ������� 
		myTimer1 += period1;                  // ����� ��������
		// ��������� �������� 1
		PeriodForSet();
	//}
	//if (millis() - myTimer2 >= period2) {   // ���� ������� 
		myTimer2 += period2;                  // ����� ��������
		// ��������� �������� 2
		PeriodForTime();
	//}
}

ISR(TIMER0_COMPA_vect) {
	// ��� ���
	digitalWrite(RELE_PINOUT, !digitalRead(RELE_PINOUT);
}

void InitialTIMER0() {
	// Timer/Counter 0 initialization
	//������� ���������� ����� = Fclk / (N * (1 + OCR0A))    ��� N - ����. ������������ (1, 8, 64, 256 ��� 1024)
	OCR0A = 124; //������� ��������� ������� ������� �����. �������� �� 0 �� 255. 
	TCCR0A = WGM01; //CTC (����� ��� ����������)
	TCCR0B = (0<<CS02) | (0<<CS00); // CLK/1024
	TIMSK = (1 << OCIE0A);  // ��������� ���������� �� ����������
	
}

void PeriodForSet() {			//������������ ��������� ������
	flagKey1Pressed = !digitalRead(KEY1_PIN);
	flagKey10Pressed = !digitalRead(KEY10_PIN);
			if (countExit > 0) {
				if (countExit != countExitDefault) {			 //������ �� ����� �������
					countExit--;
				}
				else {										      //countExit == countExitDefault - ������� ����� (������ ������)
					if (alarmCount>0) {							 //alarmCount - ���� ����� � �� ������� alarmCount, �� �������� ������
						digitalWrite(ALARM_PINOUT, HIGH);
						alarmCount--;							//������� �� ���������� ������
 					}else digitalWrite(ALARM_PINOUT, LOW);		//��������� ������ �����  period1 * countAlarmDefault
				}
				if (flagKey10Pressed || flagKey1Pressed) {					//��������� ����� ������
						if (!flagIsPress) {
							flagIsPress = true;
								countExit = countExitDefault - 1; //����� ��� ������ ������ ������� �� ����� �������
								if (flagKey1Pressed) timeLeft+=60; //������� ������� � �������� (1 ��� = 60 ���)
								if (flagKey10Pressed) timeLeft += 600;
								if (timeLeft > 5940) {				//���� ����� ����� ����� ��������� � ������� ����������� �������� �������� (>99 �����) ��
									timeLeft = 0;					//����� ����� �� �����
									countExit = countExitDefault;   //������ ������
								}
						}
				}
				else
				{
					flagIsPress = false;
				}
				DisplayTime(timeLeft);
				disp.point(true);

			}
}

void PeriodForTime() {			//������� ���ר� ������
		if (countExit == 0) {
			if (flagPoint) {						//��� �������� ������ �� ������� ��������� (�����)
				alarmCount = countAlarmDefault;     //
				if (flagKey10Pressed || flagKey1Pressed) {		//���������� ���������� ��� �����
					timeLeft = 0;
					alarmCount = 0;					//��� �� �������� �����, � �� � ����� ��� ��������
				}
				if (timeLeft == 0) {				//������� ���������� ����� timeLeft==0 (��� �� ���������� ���������� ���������� )
					digitalWrite(RELE_PINOUT, LOW);
					countExit = countExitDefault;   //����� � ������ ������
					flagIsPress = true;				//��� ������������� ������� ������ ������
				}
				else {
					digitalWrite(RELE_PINOUT, HIGH); //����� ������� --
					timeLeft--;
					flagPoint = false;
				}
			}
			else {
				flagPoint = true;	//�������� ��������� (�����)
			}
			DisplayTime(timeLeft);	//����������� �������
			disp.point(!flagPoint);	//����������� �����
		}
	
}
//void Display(uint16_t _Val)
//{
//	int8_t bt3 = _Val % 10;
//	int8_t bt2 = _Val /10 % 10;
//	int8_t bt1 = _Val /100 % 10;
//	int8_t bt0 = _Val /1000 % 10;
//	
//	disp.display(bt0, bt1, bt2, bt3);
//}
void DisplayTime(uint16_t _Seconds) //��� ������� ����������� ����� ������ � ���:���
{
	uint8_t bt0 = _Seconds / 600 % 10;
	uint8_t bt1 = _Seconds / 60 % 10;
	uint8_t bt2 = (_Seconds - 60 * bt1 - 600 * bt0) / 10 % 10;
	uint8_t bt3 = _Seconds % 10;
	
	disp.display(bt0, bt1, bt2, bt3); //GyverTM1637 MM:SS
}
