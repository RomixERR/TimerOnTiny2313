/*
 Name:		Timer.ino
 Created:	19.01.2022 23:48:37
 Author:	RomixERR
*/
#include <GyverTM1637.h>
//#include "MyIncl2313.ino"
#define OCR0ASET 249
#define PREDIV 25
#define KEY10_PIN 8		//КНОПКА УПРАВЛЕНИЯ ПОЛЁТОМ +10сек (от земли, поддяжка штанов к + уже реализована в коде)
#define KEY1_PIN 9		//КНОПКА УПРАВЛЕНИЯ ПОЛЁТОМ +1сек (от земли, поддяжка штанов к + уже реализована в коде)
#define RELE_PINOUT 13	//СИЛОВОЕ РЕЛЕ (просто выход)
#define ALARM_PINOUT 12 //ЗВОНОК (просто выход, мелодию реализуй сам уж)
#define CLK 4			//пин 7-ми сегментного индикатора
#define DIO 5			//пин 7-ми сегментного индикатора
#define AlarmDur 5		//Время работы звонка в секундах
#define DelayOnStart 4 //Время задержки пуска (при вводе времени) в секундах 4
#define HalfTimePeriod 5 //5 = 500мС Половина секунды в миллисекундах (можно уменьшать для теста 100мС например)

#pragma region MyRegion


/* Registers and associated bit numbers. */

#define USIBR _SFR_IO8(0x000)
#define USIBR0 0
#define USIBR1 1
#define USIBR2 2
#define USIBR3 3
#define USIBR4 4
#define USIBR5 5
#define USIBR6 6
#define USIBR7 7

#define DIDR _SFR_IO8(0x001)
#define AIN0D 0
#define AIN1D 1

#define UBRRH _SFR_IO8(0x002)
#define UBRR8 0
#define UBRR9 1
#define UBRR10 2
#define UBRR11 3

#define UCSRC _SFR_IO8(0x003)
#define UCPOL 0
#define UCSZ0 1
#define UCSZ1 2
#define USBS 3
#define UPM0 4
#define UPM1 5
#define UMSEL0 6
#define UMSEL1 7

/* When in MSPIM mode */
#define UCPHA 1
#define UDORD 2

#define PCMSK1 _SFR_IO8(0x004)
#define PCINT8 0
#define PCINT9 1
#define PCINT10 2

#define PCMSK2 _SFR_IO8(0x005)
#define PCINT11 0
#define PCINT12 1
#define PCINT13 2
#define PCINT14 3
#define PCINT15 4
#define PCINT16 5
#define PCINT17 6

#define PRR _SFR_IO8(0x006)
#define PRUSART 0
#define PRUSI 1
#define PRTIM0 2
#define PRTIM1 3

#define __AVR_HAVE_PRR	((1<<PRUSART)|(1<<PRUSI)|(1<<PRTIM0)|(1<<PRTIM1))
#define __AVR_HAVE_PRR_PRUSART
#define __AVR_HAVE_PRR_PRUSI
#define __AVR_HAVE_PRR_PRTIM0
#define __AVR_HAVE_PRR_PRTIM1

#define BODCR _SFR_IO8(0x007)
#define BPDSE 0
#define BPDS 1

#define ACSR _SFR_IO8(0x008)
#define ACIS0 0
#define ACIS1 1
#define ACIC 2
#define ACIE 3
#define ACI 4
#define ACO 5
#define ACBG 6
#define ACD 7

#define UBRRL _SFR_IO8(0x009)
#define UBRR0 0
#define UBRR1 1
#define UBRR2 2
#define UBRR3 3
#define UBRR4 4
#define UBRR5 5
#define UBRR6 6
#define UBRR7 7

#define UCSRB _SFR_IO8(0x00A)
#define TXB8 0
#define RXB8 1
#define UCSZ2 2
#define TXEN 3
#define RXEN 4
#define UDRIE 5
#define TXCIE 6
#define RXCIE 7

#define UCSRA _SFR_IO8(0x00B)
#define MPCM 0
#define U2X 1
#define UPE 2
#define DOR 3
#define FE 4
#define UDRE 5
#define TXC 6
#define RXC 7

#define UDR _SFR_IO8(0x00C)
#define UDR0 0
#define UDR1 1
#define UDR2 2
#define UDR3 3
#define UDR4 4
#define UDR5 5
#define UDR6 6
#define UDR7 7

#define USICR _SFR_IO8(0x00D)
#define USITC 0
#define USICLK 1
#define USICS0 2
#define USICS1 3
#define USIWM0 4
#define USIWM1 5
#define USIOIE 6
#define USISIE 7

#define USISR _SFR_IO8(0x00E)
#define USICNT0 0
#define USICNT1 1
#define USICNT2 2
#define USICNT3 3
#define USIDC 4
#define USIPF 5
#define USIOIF 6
#define USISIF 7

#define USIDR _SFR_IO8(0x00F)
#define USIDR0 0
#define USIDR1 1
#define USIDR2 2
#define USIDR3 3
#define USIDR4 4
#define USIDR5 5
#define USIDR6 6
#define USIDR7 7

#define PIND _SFR_IO8(0x010)
#define PIND0 0
#define PIND1 1
#define PIND2 2
#define PIND3 3
#define PIND4 4
#define PIND5 5
#define PIND6 6

#define DDRD _SFR_IO8(0x011)
#define DDD0 0
#define DDD1 1
#define DDD2 2
#define DDD3 3
#define DDD4 4
#define DDD5 5
#define DDD6 6

#define PORTD _SFR_IO8(0x012)
#define PORTD0 0
#define PORTD1 1
#define PORTD2 2
#define PORTD3 3
#define PORTD4 4
#define PORTD5 5
#define PORTD6 6

#define GPIOR0 _SFR_IO8(0x013)
#define GPIOR00 0
#define GPIOR01 1
#define GPIOR02 2
#define GPIOR03 3
#define GPIOR04 4
#define GPIOR05 5
#define GPIOR06 6
#define GPIOR07 7

#define GPIOR1 _SFR_IO8(0x014)
#define GPIOR10 0
#define GPIOR11 1
#define GPIOR12 2
#define GPIOR13 3
#define GPIOR14 4
#define GPIOR15 5
#define GPIOR16 6
#define GPIOR17 7

#define GPIOR2 _SFR_IO8(0x015)
#define GPIOR20 0
#define GPIOR21 1
#define GPIOR22 2
#define GPIOR23 3
#define GPIOR24 4
#define GPIOR25 5
#define GPIOR26 6
#define GPIOR27 7

#define PINB _SFR_IO8(0x016)
#define PINB0 0
#define PINB1 1
#define PINB2 2
#define PINB3 3
#define PINB4 4
#define PINB5 5
#define PINB6 6
#define PINB7 7

#define DDRB _SFR_IO8(0x017)
#define DDB0 0
#define DDB1 1
#define DDB2 2
#define DDB3 3
#define DDB4 4
#define DDB5 5
#define DDB6 6
#define DDB7 7

#define PORTB _SFR_IO8(0x018)
#define PORTB0 0
#define PORTB1 1
#define PORTB2 2
#define PORTB3 3
#define PORTB4 4
#define PORTB5 5
#define PORTB6 6
#define PORTB7 7

#define PINA _SFR_IO8(0x019)
#define PINA0 0
#define PINA1 1
#define PINA2 2

#define DDRA _SFR_IO8(0x01A)
#define DDA0 0
#define DDA1 1
#define DDA2 2

#define PORTA _SFR_IO8(0x01B)
#define PORTA0 0
#define PORTA1 1
#define PORTA2 2

#define EECR _SFR_IO8(0x01C)
#define EERE 0
#define EEPE 1
#define EEMPE 2
#define EERIE 3
#define EEPM0 4
#define EEPM1 5

#define EEDR _SFR_IO8(0x01D)
#define EEDR0 0
#define EEDR1 1
#define EEDR2 2
#define EEDR3 3
#define EEDR4 4
#define EEDR5 5
#define EEDR6 6
#define EEDR7 7

#define EEAR _SFR_IO8(0x01E)
#define EEAR0 0
#define EEAR1 1
#define EEAR2 2
#define EEAR3 3
#define EEAR4 4
#define EEAR5 5
#define EEAR6 6

#define PCMSK _SFR_IO8(0x020)
#define PCMSK0 _SFR_IO8(0x020)
#define PCINT0 0
#define PCINT1 1
#define PCINT2 2
#define PCINT3 3
#define PCINT4 4
#define PCINT5 5
#define PCINT6 6
#define PCINT7 7

#define WDTCR _SFR_IO8(0x021)
#define WDP0 0
#define WDP1 1
#define WDP2 2
#define WDE 3
#define WDCE 4
#define WDP3 5
#define WDIE 6
#define WDIF 7

#define TCCR1C _SFR_IO8(0x022)
#define FOC1B 6
#define FOC1A 7

#define GTCCR _SFR_IO8(0x023)
#define PSR10 0

#define ICR1 _SFR_IO16(0x024)

#define ICR1L _SFR_IO8(0x024)
#define ICR1L0 0
#define ICR1L1 1
#define ICR1L2 2
#define ICR1L3 3
#define ICR1L4 4
#define ICR1L5 5
#define ICR1L6 6
#define ICR1L7 7

#define ICR1H _SFR_IO8(0x025)
#define ICR1H0 0
#define ICR1H1 1
#define ICR1H2 2
#define ICR1H3 3
#define ICR1H4 4
#define ICR1H5 5
#define ICR1H6 6
#define ICR1H7 7

#define CLKPR _SFR_IO8(0x026)
#define CLKPS0 0
#define CLKPS1 1
#define CLKPS2 2
#define CLKPS3 3
#define CLKPCE 7

#define OCR1B _SFR_IO16(0x028)

#define OCR1BL _SFR_IO8(0x028)
#define OCR1BL0 0
#define OCR1BL1 1
#define OCR1BL2 2
#define OCR1BL3 3
#define OCR1BL4 4
#define OCR1BL5 5
#define OCR1BL6 6
#define OCR1BL7 7

#define OCR1BH _SFR_IO8(0x029)
#define OCR1BH0 0
#define OCR1BH1 1
#define OCR1BH2 2
#define OCR1BH3 3
#define OCR1BH4 4
#define OCR1BH5 5
#define OCR1BH6 6
#define OCR1BH7 7

#define OCR1A _SFR_IO16(0x02A)

#define OCR1AL _SFR_IO8(0x02A)
#define OCR1AL0 0
#define OCR1AL1 1
#define OCR1AL2 2
#define OCR1AL3 3
#define OCR1AL4 4
#define OCR1AL5 5
#define OCR1AL6 6
#define OCR1AL7 7

#define OCR1AH _SFR_IO8(0x02B)
#define OCR1AH0 0
#define OCR1AH1 1
#define OCR1AH2 2
#define OCR1AH3 3
#define OCR1AH4 4
#define OCR1AH5 5
#define OCR1AH6 6
#define OCR1AH7 7

#define TCNT1 _SFR_IO16(0x02C)

#define TCNT1L _SFR_IO8(0x02C)
#define TCNT1L0 0
#define TCNT1L1 1
#define TCNT1L2 2
#define TCNT1L3 3
#define TCNT1L4 4
#define TCNT1L5 5
#define TCNT1L6 6
#define TCNT1L7 7

#define TCNT1H _SFR_IO8(0x02D)
#define TCNT1H0 0
#define TCNT1H1 1
#define TCNT1H2 2
#define TCNT1H3 3
#define TCNT1H4 4
#define TCNT1H5 5
#define TCNT1H6 6
#define TCNT1H7 7

#define TCCR1B _SFR_IO8(0x02E)
#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4
#define ICES1 6
#define ICNC1 7

#define TCCR1A _SFR_IO8(0x02F)
#define WGM10 0
#define WGM11 1
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7

#define TCCR0A _SFR_IO8(0x030)
#define WGM00 0
#define WGM01 1
#define COM0B0 4
#define COM0B1 5
#define COM0A0 6
#define COM0A1 7

#define OSCCAL _SFR_IO8(0x031)
#define CAL0 0
#define CAL1 1
#define CAL2 2
#define CAL3 3
#define CAL4 4
#define CAL5 5
#define CAL6 6

#define TCNT0 _SFR_IO8(0x032)
#define TCNT0_0 0
#define TCNT0_1 1
#define TCNT0_2 2
#define TCNT0_3 3
#define TCNT0_4 4
#define TCNT0_5 5
#define TCNT0_6 6
#define TCNT0_7 7

#define TCCR0B _SFR_IO8(0x033)
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM02 3
#define FOC0B 6
#define FOC0A 7

#define MCUSR _SFR_IO8(0x034)
#define PORF 0
#define EXTRF 1
#define BORF 2
#define WDRF 3

#define MCUCR _SFR_IO8(0x035)
#define ISC00 0
#define ISC01 1
#define ISC10 2
#define ISC11 3
#define SM0 4
#define SE 5
#define SM1 6
#define PUD 7

#define OCR0A _SFR_IO8(0x036)
#define OCR0A_0 0
#define OCR0A_1 1
#define OCR0A_2 2
#define OCR0A_3 3
#define OCR0A_4 4
#define OCR0A_5 5
#define OCR0A_6 6
#define OCR0A_7 7

#define SPMCSR _SFR_IO8(0x037)
#define SPMEN 0
#define PGERS 1
#define PGWRT 2
#define RFLB 3
#define CTPB 4

#define TIFR _SFR_IO8(0x038)
#define OCF0A 0
#define TOV0 1
#define OCF0B 2
#define ICF1 3
#define OCF1B 5
#define OCF1A 6
#define TOV1 7

#define TIMSK _SFR_IO8(0x039)
#define OCIE0A 0
#define TOIE0 1
#define OCIE0B 2
#define ICIE1 3
#define OCIE1B 5
#define OCIE1A 6
#define TOIE1 7

#define EIFR _SFR_IO8(0x03A)
#define GIFR _SFR_IO8(0x03A)
#define PCIF1 3
#define PCIF2 4
#define PCIF0 5
#define INTF0 6
#define INTF1 7

#define GIMSK _SFR_IO8(0x03B)
#define PCIE1 3
#define PCIE2 4
#define PCIE0 5
#define INT0 6
#define INT1 7

#define OCR0B _SFR_IO8(0x03C)
#define OCR0_0 0
#define OCR0_1 1
#define OCR0_2 2
#define OCR0_3 3
#define OCR0_4 4
#define OCR0_5 5
#define OCR0_6 6
#define OCR0_7 7


/* Interrupt vectors */
/* Vector 0 is the reset vector */
#define INT0_vect_num  1
#define INT0_vect      _VECTOR(1)  /* External Interrupt Request 0 */
#define INT1_vect_num  2
#define INT1_vect      _VECTOR(2)  /* External Interrupt Request 1 */
#define TIMER1_CAPT_vect_num  3
#define TIMER1_CAPT_vect      _VECTOR(3)  /* Timer/Counter1 Capture Event */
#define TIMER1_COMPA_vect_num  4
#define TIMER1_COMPA_vect      _VECTOR(4)  /* Timer/Counter1 Compare Match A */
#define TIMER1_OVF_vect_num  5
#define TIMER1_OVF_vect      _VECTOR(5)  /* Timer/Counter1 Overflow */
#define TIMER0_OVF_vect_num  6
#define TIMER0_OVF_vect      _VECTOR(6)  /* Timer/Counter0 Overflow */
#define USART0_RX_vect_num  7
#define USART0_RX_vect      _VECTOR(7)  /* USART, Rx Complete */
#define USART_RX_vect_num  7
#define USART_RX_vect      _VECTOR(7)  /* alias */
#define USART0_UDRE_vect_num  8
#define USART0_UDRE_vect      _VECTOR(8)  /* USART Data Register Empty */
#define USART_UDRE_vect_num  8
#define USART_UDRE_vect      _VECTOR(8)  /* alias */
#define USART0_TX_vect_num  9
#define USART0_TX_vect      _VECTOR(9)  /* USART, Tx Complete */
#define USART_TX_vect_num  9
#define USART_TX_vect      _VECTOR(9)  /* alias */
#define ANA_COMP_vect_num  10
#define ANA_COMP_vect      _VECTOR(10)  /* Analog Comparator */
#define PCINT0_vect_num  11
#define PCINT0_vect      _VECTOR(11)  /* Pin Change Interrupt Request 0 */
#define PCINT_B_vect_num  11
#define PCINT_B_vect      _VECTOR(11)  /* alias */
#define TIMER1_COMPB_vect_num  12
#define TIMER1_COMPB_vect      _VECTOR(12)  /*  */
#define TIMER0_COMPA_vect_num  13
#define TIMER0_COMPA_vect      _VECTOR(13)  /*  */
#define TIMER0_COMPB_vect_num  14
#define TIMER0_COMPB_vect      _VECTOR(14)  /*  */
#define USI_START_vect_num  15
#define USI_START_vect      _VECTOR(15)  /* USI Start Condition */
#define USI_OVERFLOW_vect_num  16
#define USI_OVERFLOW_vect      _VECTOR(16)  /* USI Overflow */
#define EEPROM_Ready_vect_num  17
#define EEPROM_Ready_vect      _VECTOR(17)  /* EEPROM Ready */
#define WDT_OVERFLOW_vect_num  18
#define WDT_OVERFLOW_vect      _VECTOR(18)  /* Watchdog Timer Overflow */
#define PCINT1_vect_num  19
#define PCINT1_vect      _VECTOR(19)  /* Pin Change Interrupt Request 1 */
#define PCINT_A_vect_num  19
#define PCINT_A_vect      _VECTOR(19)  /* alias */
#define PCINT2_vect_num  20
#define PCINT2_vect      _VECTOR(20)  /* Pin Change Interrupt Request 2 */
#define PCINT_D_vect_num  20
#define PCINT_D_vect      _VECTOR(20)  /* alias */

#define _VECTOR_SIZE 2 /* Size of individual vector. */
#define _VECTORS_SIZE (21 * _VECTOR_SIZE)


/* Constants */
#define SPM_PAGESIZE (32)
#define RAMSTART     (0x60)
#define RAMSIZE      (128)
#define RAMEND       (RAMSTART + RAMSIZE - 1)
#define XRAMSTART    (NA)
#define XRAMSIZE     (0)
#define XRAMEND      (RAMEND)
#define E2END        (0x7F)
#define E2PAGESIZE   (4)
#define FLASHEND     (0x7FF)


/* Fuses */
#define FUSE_MEMORY_SIZE 3

/* Low Fuse Byte */
#define FUSE_CKSEL0  (unsigned char)~_BV(0)  /* Select Clock Source */
#define FUSE_CKSEL1  (unsigned char)~_BV(1)  /* Select Clock Source */
#define FUSE_CKSEL2  (unsigned char)~_BV(2)  /* Select Clock Source */
#define FUSE_CKSEL3  (unsigned char)~_BV(3)  /* Select Clock Source */
#define FUSE_SUT0  (unsigned char)~_BV(4)  /* Select start-up time */
#define FUSE_SUT1  (unsigned char)~_BV(5)  /* Select start-up time */
#define FUSE_CKOUT  (unsigned char)~_BV(6)  /* Clock output */
#define FUSE_CKDIV8  (unsigned char)~_BV(7)  /* Divide clock by 8 */
#define LFUSE_DEFAULT (FUSE_CKDIV8 & FUSE_SUT0 & FUSE_CKSEL3 & FUSE_CKSEL2 & FUSE_CKSEL0)

/* High Fuse Byte */
#define FUSE_RSTDISBL  (unsigned char)~_BV(0)  /* External reset disable */
#define FUSE_BODLEVEL0  (unsigned char)~_BV(1)  /* Brown-out Detector trigger level */
#define FUSE_BODLEVEL1  (unsigned char)~_BV(2)  /* Brown-out Detector trigger level */
#define FUSE_BODLEVEL2  (unsigned char)~_BV(3)  /* Brown-out Detector trigger level */
#define FUSE_WDTON  (unsigned char)~_BV(4)  /* Watchdog Timer Always On */
#define FUSE_SPIEN  (unsigned char)~_BV(5)  /* Enable Serial programming and Data Downloading */
#define FUSE_EESAVE  (unsigned char)~_BV(6)  /* EEPROM memory is preserved through chip erase */
#define FUSE_DWEN  (unsigned char)~_BV(7)  /* debugWIRE Enable */
#define HFUSE_DEFAULT (FUSE_SPIEN)

/* Extended Fuse Byte */
#define FUSE_SELFPRGEN  (unsigned char)~_BV(0)  /* Self Programming Enable */
#define EFUSE_DEFAULT (0xFF)


/* Lock Bits */
#define __LOCK_BITS_EXIST


/* Signature */
#define SIGNATURE_0 0x1E
#define SIGNATURE_1 0x91
#define SIGNATURE_2 0x0A


/* Device Pin Definitions */
#define RXD_DDR   DDRD
#define RXD_PORT  PORTD
#define RXD_PIN   PIND
#define RXD_BIT   0

#define TXD_DDR   DDRD
#define TXD_PORT  PORTD
#define TXD_PIN   PIND
#define TXD_BIT   1

#define PA1_DDR   DDRXTAL
#define PA1_PORT  PORTXTAL
#define PA1_PIN   PINXTAL
#define PA1_BIT   XTAL2

#define PA0_DDR   DDRXTAL
#define PA0_PORT  PORTXTAL
#define PA0_PIN   PINXTAL
#define PA0_BIT   XTAL1

#define INT0_DDR   DDRD
#define INT0_PORT  PORTD
#define INT0_PIN   PIND
#define INT0_BIT   2

#define XCK_DDR   DDRD
#define XCK_PORT  PORTD
#define XCK_PIN   PIND
#define XCK_BIT   2

#define CKOUT_DDR   DDRD
#define CKOUT_PORT  PORTD
#define CKOUT_PIN   PIND
#define CKOUT_BIT   2

#define INT1_DDR   DDRD
#define INT1_PORT  PORTD
#define INT1_PIN   PIND
#define INT1_BIT   3

#define T0_DDR   DDRD
#define T0_PORT  PORTD
#define T0_PIN   PIND
#define T0_BIT   4

#define T1_DDR   DDRD
#define T1_PORT  PORTD
#define T1_PIN   PIND
#define T1_BIT   5

#define OC0B_DDR   DDRD
#define OC0B_PORT  PORTD
#define OC0B_PIN   PIND
#define OC0B_BIT   5

#define ICP_DDR   DDRD
#define ICP_PORT  PORTD
#define ICP_PIN   PIND
#define ICP_BIT   6

#define AIN0_DDR   DDRB
#define AIN0_PORT  PORTB
#define AIN0_PIN   PINB
#define AIN0_BIT   0

#define AIN1_DDR   DDRB
#define AIN1_PORT  PORTB
#define AIN1_PIN   PINB
#define AIN1_BIT   1

#define OC0A_DDR   DDRB
#define OC0A_PORT  PORTB
#define OC0A_PIN   PINB
#define OC0A_BIT   2

#define OC1A_DDR   DDRB
#define OC1A_PORT  PORTB
#define OC1A_PIN   PINB
#define OC1A_BIT   3

#define OC1B_DDR   DDRB
#define OC1B_PORT  PORTB
#define OC1B_PIN   PINB
#define OC1B_BIT   4

#define MOSI_DDR   DDRB
#define MOSI_PORT  PORTB
#define MOSI_PIN   PINB
#define MOSI_BIT   5

#define DI_DDR   DDRB
#define DI_PORT  PORTB
#define DI_PIN   PINB
#define DI_BIT   5

#define MISO_DDR   DDRB
#define MISO_PORT  PORTB
#define MISO_PIN   PINB
#define MISO_BIT   6

#define DO_DDR   DDRB
#define DO_PORT  PORTB
#define DO_PIN   PINB
#define DO_BIT   6

#define SCK_DDR   DDRB
#define SCK_PORT  PORTB
#define SCK_PIN   PINB
#define SCK_BIT   7

#define SCL_DDR   DDRB
#define SCL_PORT  PORTB
#define SCL_PIN   PINB
#define SCL_BIT   7

#define SLEEP_MODE_IDLE (0x00<<4)
#define SLEEP_MODE_STANDBY (0x04<<4)
#define SLEEP_MODE_PWR_DOWN (0x05<<4)


#pragma endregion

//Операционная система реального времени (шутка) Реализация тактирования
uint32_t myTimer1, myTimer2; 
uint16_t period1 = 100, period2 = HalfTimePeriod, alarmCount, myPreDiv= PREDIV;
//Библиотека дисплея стартуем
GyverTM1637 disp(CLK, DIO);
//Всякие переменные
uint16_t timeLeft=0;
uint8_t const countExitDefault = DelayOnStart * 1000 / period1;
uint8_t const countAlarmDefault = AlarmDur * 1000 / period1;
uint8_t countExit = countExitDefault;							//countExit == countExitDefault - условие входа (начала работы)
bool flagIsPress, flagPoint = true;		//flagIsPress - флаг того что кнопка удерживается, flagPoint - двоеточие или точка на экране
bool flagKey1Pressed, flagKey10Pressed;


// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(KEY10_PIN, INPUT_PULLUP); 
	pinMode(KEY1_PIN, INPUT_PULLUP);
	pinMode(RELE_PINOUT, OUTPUT);   
	pinMode(ALARM_PINOUT, OUTPUT);
	InitialTIMER0();
	disp.clear();
	disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)
}

void loop() {
}

ISR(TIMER0_COMPA_vect) {
	// ваш код
	OCR0A = OCR0ASET; //регистр сравнения Верхняя граница счета. Диапазон от 0 до 255. 
	myPreDiv--;
	if (myPreDiv == 0) {
		myPreDiv = PREDIV;
		PeriodForSet();
		period2--;
		if (period2 == 0) {
			period2 = HalfTimePeriod;
			PeriodForTime();
		}
	}
}


void InitialTIMER0() {
	// Timer/Counter 0 initialization
	//Частота прерываний будет = Fclk / (2 * N * (1 + OCR0A)) / 2    где N - коэф. предделителя (1, 8, 64, 256 или 1024)
	interrupts();
	OCR0A = OCR0ASET; //регистр сравнения Верхняя граница счета. Диапазон от 0 до 255. ОБНОВИ В ISR(TIMER0_COMPA_vect)
	TCCR0A = (1<<WGM01); //CTC (Сброс при совпадении)
	TCCR0B = (1<<CS02); // CLK/256
	TIMSK =  (1 << OCIE0A);  // Разрешить прерывание по совпадению
}

void PeriodForSet() {			//УСТАНОВОЧНЫЙ ЦИКЛИЧНЫЙ ПРОХОД
	flagKey1Pressed = !digitalRead(KEY1_PIN);
	flagKey10Pressed = !digitalRead(KEY10_PIN);
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
				if (flagKey10Pressed || flagKey1Pressed) {					//обработка тыков юзвера
						if (!flagIsPress) {
							flagIsPress = true;
								countExit = countExitDefault - 1; //Сброс или запуск начала отсчёта до пуска таймера
								if (flagKey1Pressed) timeLeft+=60; //Уставка времени В секуднах (1 мин = 60 сек)
								if (flagKey10Pressed) timeLeft += 600;
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
				if (flagKey10Pressed || flagKey1Pressed) {		//экстренное завершение тык юзера
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