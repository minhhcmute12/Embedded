/*
 * ds1307.h
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 */

#ifndef DS1307_H_
#define DS1307_H_

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include "stm32f407xx_rcc_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "stm32f407xx_usart_driver.h"

/* Application configurable itmes - Cấu hình sẵn các thông số kết nối MCU*/
#define DS1307_I2C 				I2C1				//Muốn kết nối RTC với I2C1 peripheral của MCU
#define DS1307_I2C_PORT 		GPIOB				//Muốn kết nối RTC với PortB của MCU
#define DS1307_I2C_SDA_PIN 		GPIO_PIN_NO_7		//Muốn kết nối RTC_SDA với pin 7(PortB) của MCU
#define DS1307_I2C_SCL_PIN 		GPIO_PIN_NO_6		//Muốn kết nối RTC_SDA với pin 6(PortB) của MCU
#define DS1307_I2C_SPEED 		I2C_SCL_SPEED_SM	//Muốn kết nối RTC với speed của MCU
#define DS1307_I2C_PUPD			GPIO_PIN_PU			//Muốn kết nối RTC với internal pullups

/*Các thông số ở trên có thể thay đổi tùy cấu hình kết nối mà lập trình viên mong muốn*/
//#define DS1307_I2C_PUPD 		GPIO_NO_PUPD		//No internal pullups
//GPIO_NO_PUPD: việc ko dùng là do ta sẽ ko dung PUPD nội mà dùng PUPD ngoại, nghĩa là kết nối chặp 3 với
//mỗi dây SDA và SCL với R=3.3KOhm và Vdd=5V

/* Register address */
#define DS1307_ADDR_SEC 	0x00
#define DS1307_ADDR_MIN 	0x01
#define DS1307_ADDR_HRS 	0x02
#define DS1307_ADDR_DAY 	0x03	//thứ mấy
#define DS1307_ADDR_DATE 	0x04
#define DS1307_ADDR_MONTH 	0x05
#define DS1307_ADDR_YEAR 	0x06

/* TIME FORMAT */
#define TIME_FORMAT_12HRS_AM	0
#define TIME_FORMAT_12HRS_PM	1
#define TIME_FORMAT_24HRS		2

/* DS1307 ADDRESS */
#define DS1307_I2C_ADDRESS	0x68	//1101000(b)=68(hex)

/* DAY(Thứ) INFORMATION(03h regoster) */
#define SUNDAY			1		//Bạn có thể tùy chọn cặp day-number mà bạn mong muốn
#define MONDAY			2		//Trong bài học thì thầy chọn Sunday làm 1
#define TUESDAY			3
#define WEDNESDAY		4
#define THURSDAY		5
#define FRIDAY			6
#define SATURDAY		7

/* RTC Date Structure */
typedef struct
{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
}RTC_date_t;

/* RTC Time Structure */
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t time_format;
}RTC_time_t;

//Function prototypes
uint8_t ds1307_init();
void ds1307_set_current_time(RTC_time_t *rtc_time);
void ds1307_get_current_time(RTC_time_t *rtc_time);

void ds1307_set_current_date(RTC_date_t *rtc_date);
void ds1307_get_current_date(RTC_date_t *rtc_date);


#endif /* DS1307_H_ */
