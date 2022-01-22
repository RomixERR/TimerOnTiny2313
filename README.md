# TimerOnTiny2313
Таймер времени реализованый на микроконтроллере 2Kb Falsh с кварцем 16МГц.
Таймер обратного отсчёта для микроконтроллеров ATTiny2313 или подобных. + дисплей 7-сегментный на базе TM1637.
===
Написано для ARDUINO платформы. Использовался IDE Visual Studio 2022 с Visual Micro.
Код таймера умещается всего в 2 038 bytes (used 100% of a 2 048 byte maximum). Со включенной LTO - оптимизацией Option-4.
Требует для работы #include <GyverTM1637.h>
===
Настройки для среды Visual Studio 2022 с Visual Micro или ArduinoIDE
No Bootloader шъётся через USBasp.
ClockSourse = 16MHz external (16 МГц кварц)
Initial Sec Timers = no
LTO Enable Option 4
mills/millis Enabel
Save Eeprom - Not Retained
===
Timer.ino - основной файл с кодом.
