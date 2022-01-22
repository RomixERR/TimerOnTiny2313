/*
 Name:		Timer.ino
 Created:	19.01.2022 23:48:37
 Author:	RomixERR
*/
//#include "GyverTM1637.h"
#include <GyverTM1637.h>
#define KEY_PIN 8		//КНОПКА УПРАВЛЕНИЯ ПОЛЁТОМ (от земли, поддяжка штанов к + уже реализована в коде)
#define RELE_PINOUT 13	//СИЛОВОЕ РЕЛЕ (просто выход)
#define ALARM_PINOUT 12 //ЗВОНОК (просто выход, мелодию реализуй сам уж)
#define CLK 4			//пин 7-ми сегментного индикатора
#define DIO 5			//пин 7-ми сегментного индикатора
#define AlarmDur 5		//Время работы звонка в секундах
#define DelayOnStart 4 //Время задержки пуска (при вводе времени) в секундах
#define HalfTimePeriod 500 //500мС Половина секунды в миллисекундах (можно уменьшать для теста 100мС например)

//Операционная система реального времени (шутка) Реализация тактирования
uint32_t myTimer1, myTimer2; 
uint16_t period1 = 100, period2 = HalfTimePeriod, alarmCount; 
//Библиотека дисплея стартуем
GyverTM1637 disp(CLK, DIO);
//Всякие переменные
uint16_t timeLeft=0;
uint8_t const countExitDefault = DelayOnStart * 1000 / period1;
uint8_t const countAlarmDefault = AlarmDur * 1000 / period1;
uint8_t countExit = countExitDefault;							//countExit == countExitDefault - условие входа (начала работы)
bool flagIsPress, flagPoint = true;		//flagIsPress - флаг того что кнопка удерживается, flagPoint - двоеточие или точка на экране


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(KEY_PIN, INPUT_PULLUP); 
	pinMode(RELE_PINOUT, OUTPUT);   
	pinMode(ALARM_PINOUT, OUTPUT);
	disp.clear();
	disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (millis() - myTimer1 >= period1) {   // ищем разницу 
		myTimer1 += period1;                  // сброс счётчика
		// выполнить действие 1
		PeriodForSet();
	}
	if (millis() - myTimer2 >= period2) {   // ищем разницу 
		myTimer2 += period2;                  // сброс счётчика
		// выполнить действие 2
		//digitalWrite(RELE_PINOUT, !digitalRead(RELE_PINOUT)
		PeriodForTime();
	}
}


void PeriodForSet() {			//УСТАНОВОЧНЫЙ ЦИКЛИЧНЫЙ ПРОХОД
	uint16_t ts;
			if (countExit > 0) {
				if (countExit != countExitDefault) {			 //отсчёт до пуска таймера
					countExit--;
				}
				else {										      //countExit == countExitDefault - условие входа (начала работы)
					if (alarmCount>0) {							 //alarmCount - если вошли с не нулевым alarmCount, то включаем звонок
						digitalWrite(ALARM_PINOUT, HIGH);
						alarmCount--;							//считаем до выключения звонка
 					}else digitalWrite(ALARM_PINOUT, LOW);		//выключаем звонок через  period1 * countAlarmDefault
				}
				if (!digitalRead(KEY_PIN)) {					//обработка тыков юзвера
						if (!flagIsPress) {
							flagIsPress = true;
								countExit = countExitDefault - 1; //Сброс или запуск начала отсчёта до пуска таймера
								if (timeLeft < 1200){             //Уставка времени В секуднах (1 мин = 60 сек)
									ts = 120;					  
								}
								else{
									ts = 300;					  
								}
								timeLeft += ts;
								if (timeLeft > 5940) {				//Если тупой юзвер вдруг обосрался и натыкал запредельно огромное значение (>99 минут) то
									timeLeft = 0;					//сброс время по кругу
									countExit = countExitDefault;   //начало работы
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

void PeriodForTime() {			//РАБОЧИЙ ОТСЧЁТ ПРОХОД
		if (countExit == 0) {
			if (flagPoint) {						//все действия только по миганию двоеточия (точки)
				alarmCount = countAlarmDefault;     //
				if (!digitalRead(KEY_PIN)) {		//экстренное завершение тык юзера
					timeLeft = 0;
					alarmCount = 0;					//шоп не сработал аларм, а то у юзера ухи порвутся
				}
				if (timeLeft == 0) {				//обычное завершение когда timeLeft==0 (или по нисходящей экстренное завершение )
					digitalWrite(RELE_PINOUT, LOW);
					countExit = countExitDefault;   //сброс в начало работы
					flagIsPress = true;				//для игнорирования нажатой сейчас кнопки
				}
				else {
					digitalWrite(RELE_PINOUT, HIGH); //режим таймера --
					timeLeft--;
					flagPoint = false;
				}
			}
			else {
				flagPoint = true;	//инверсия двоеточия (точки)
			}
			DisplayTime(timeLeft);	//отображение времени
			disp.point(!flagPoint);	//отображение точек
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
void DisplayTime(uint16_t _Seconds) //Моя функция переработки общих секунд в МИН:СЕК
{
	uint8_t bt0 = _Seconds / 600 % 10;
	uint8_t bt1 = _Seconds / 60 % 10;
	uint8_t bt2 = (_Seconds - 60 * bt1 - 600 * bt0) / 10 % 10;
	uint8_t bt3 = _Seconds % 10;
	
	disp.display(bt0, bt1, bt2, bt3); //GyverTM1637 MM:SS
}
