/*
 * 15_rtc_ds1307_lcd_testing.c
 *
 *  Created on: Jan 30, 2024
 *      Author: Minh
 */

#include <stdio.h>
#include "ds1307.h"
#include "lcd.h"

#define SYSTICK_TIM_CLK 16000000UL		//HSI block

/* Định nghĩa tên Macros muốn vô hiệu hóa */
#define PRINT_LCD

static void mdelay(uint32_t cnt);

/****************************************************************
 * @fn			- init_systick_timer
 *
 * @brief		- Thiết lập bộ đếm timer ngắt 1s để cập nhật đồng hồ
 *
 * @param[in]	- tick_hz : giá trị cứ mỗi bao giây thì ngắt
 *
 * @return		- none
 *
 * @Note 		- none
 */
void init_systick_timer(uint32_t tick_hz)
{
	//Cài đặt các địa chỉ timer của hệ thống
	uint32_t *pSRVR = (uint32_t*)0xE000E014;
	uint32_t *pSCSR = (uint32_t*)0xE000E010;

    /* calculation of reload value */
    uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz)-1;

    //Clear the value of SVR
    *pSRVR &= ~(0x00FFFFFFFF);

    //load the value in to SVR
    *pSRVR |= count_value;

    //do some settings
    *pSCSR |= ( 1 << 1); //Enables SysTick exception request:
    *pSCSR |= ( 1 << 2);  //Indicates the clock source, processor clock source

    //enable the systick
    *pSCSR |= ( 1 << 0); //enables the counter

}

/****************************************************************
 * @fn			- get_day_of_week
 *
 * @brief		- Chuyển đổi dạng số đc lưu sang dạng ký tự string
 *
 * @param[in]	- i : thứ đc lưu dạng số
 *
 * @return		- thứ đc chuyển đổi sang string
 *
 * @Note 		- none
 */
char *get_day_of_week(uint8_t i)
{
	//Chuỗi chứa ký tự thứ có thể có
	char* days[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

	return days[i-1];	//vì i đã đc định nghĩa trong file "ds1307.h"
}

/****************************************************************
 * @fn			- number_to_string
 *
 * @brief		-
 *
 * @param[in]	- num: dạng số cần chuyển đổi sang string
 * @param[in]	- buf: mảng con trỏ muốn lưu sau khi chuyển đổi
 *
 * @return		- none
 *
 * @Note 		- none
 */
void number_to_string(uint8_t num, char* buf)
{
	if(num > 10)
	{
		buf[0] = '0';
		buf[1] = num + 48;
	}
	else if(num >= 10 && num < 99)
	{
		buf[0] = (num / 10) + 48;
		buf[1] = (num % 10) + 48;
	}

	//buf[0] và buf[1] ở đây ko phải là vị trí 0 và 1 của buf[9] đưa vào
	//mà là vị trí 0 và 1 bắt đầu từ địa chỉ vị trí mà &buf[] đưa vào

	//48: số chênh lệch vị trí trong bảng mã ASCII giữa giá trị số(9) và ký tự số(9)
}

/****************************************************************
 * @fn			- time_to_string
 *
 * @brief		- Chuyển đổi time đc lưu sang dạng ký tự string
 *
 * @param[in]	- rtc_time : giá trị time đọc đc từ DS1307
 *
 * @return		- time đc chuyển đổi sang string
 *
 * @Note 		- hh:mm:ss -
 */
char* time_to_string(RTC_time_t *rtc_time)
{
	static char buf[9]; //h h : m m : s s /0

	buf[2] = ':';
	buf[5] = ':';

	number_to_string(rtc_time->hours,buf);
	number_to_string(rtc_time->minutes,&buf[3]);
	number_to_string(rtc_time->seconds,&buf[6]);

	buf[8] = '\0';  //kết thúc chuỗi luôn là một ký tự null

	return buf;
}

/****************************************************************
 * @fn			- date_to_string
 *
 * @brief		- Chuyển đổi date đc lưu sang dạng ký tự string
 *
 * @param[in]	- rtc_date : giá trị date đọc đc từ DS1307
 *
 * @return		- date đc chuyển đổi sang string
 *
 * @Note 		- dd/mm/yy
 */
char* date_to_string(RTC_date_t *rtc_date)
{
	static char buf[9]; //d d : m m : y y /0

	buf[2] = ':';
	buf[5] = ':';

	number_to_string(rtc_date->date,buf);
	number_to_string(rtc_date->month,&buf[3]);
	number_to_string(rtc_date->year,&buf[6]);

	buf[8] = '\0';  //kết thúc chuỗi luôn là một ký tự null

	return buf;
}

int main(void)
{
	RTC_time_t current_time;
	RTC_date_t current_date;

#ifndef PRINT_LCD
	printf("RTC test\n");
#else
	/* LCD init */
	lcd_init();

	/* Vd1: LCD test*/
	lcd_print_string("LCD test....");
	mdelay(2);		//hiển thị 2 s
	lcd_display_clear();	//xóa hiển thị
	lcd_display_return_home();  //đưa con trỏ về vị trí ban đầu 00

	/* Khi bạn tạo macros ở trên thì dòng lệnh tương ứng vs PRINT_LCD tức là "printf("RTC test\n");"  bị vô hiệu hóa
	 * Và khi bạn chuyển macros sang trạng thái comment "//#define PRINT_LCD" thì các dòng lệnh trong else
	 * sẽ bị vô hiệu hóa
	 * */
#endif



	/* Kiểm tra DS1307 có cho phép clock hoạt động hay không */
	if(ds1307_init())
	{
		printf("RTC init has field\n");
		while(1);
	}

	/* Cấu hình timer để ngắt cập nhật đồng hồ */
	init_systick_timer(1);			//mỗi 1s cập nhật đồng hồ

	/* Cấu hình thời gian ban đầu */
	current_date.day = FRIDAY;
	current_date.date = 15;
	current_date.month = 1;
	current_date.year = 21;

	current_time.hours = 4;
	current_time.minutes = 25;
	current_time.seconds = 41;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

	ds1307_set_current_date(&current_date);
	ds1307_set_current_time(&current_time);

	//Đọc thời gian đc cấu hình
	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	//Biểu diễn thời gian
	char *am_pm;		//lưu ký tự biểu diễn PM hoặc AM
	if(current_time.time_format != TIME_FORMAT_24HRS)
	{
		am_pm = (current_time.time_format) ? "PM" : "AM";
		//printf("Current time = %s %s \n",time_to_string(&current_time),am_pm); //Vd: 04:25:41 PM

		/* Vd2: hiển thị thời gian */
		lcd_print_string(time_to_string(&current_time));
		lcd_print_char(' ');
		lcd_print_string(am_pm);
	}
	else
	{
		//printf("Current time = %s \n",time_to_string(&current_time)); //Vd: 04:25:41

		/*Vd2: hiển thị thời gian */
		lcd_print_string(time_to_string(&current_time));
	}

	//Biểu diễn ngày tháng: 15/01/21 <friday>
	//printf("Current date: %s <%s>\n",date_to_string(&current_date),get_day_of_week(current_date.day));

	/* Vd2: hiển thị thời gian */
	lcd_set_cursor(2, 1);			//đưa con trỏ xuống hàng dưới
	lcd_print_string(date_to_string(&current_date));
	lcd_print_char(' ');
	lcd_print_string(get_day_of_week(current_date.day));

	while(1);

}

/****************************************************************
 * @fn			- SysTick_Handler
 *
 * @brief		- Xử lý công việc khi có Interrup Timer xảy ra mỗi 1s
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- Hàm cập nhật hiển thị đồng hồ
 * 				- Việc đếm thời gian sẽ do DS1307 module thực hiện, ta chỉ cần lệnh cho MCU đọc giá trị
 * 				từ DS1307 rồi hiển thị kết quả nhận đc.
 */
void SysTick_Handler(void)
{
	RTC_time_t current_time;
	RTC_date_t current_date;

	//Đọc thời gian đc cấu hình
	ds1307_get_current_time(&current_time);
	ds1307_get_current_date(&current_date);

	lcd_set_cursor(1, 1);  //row1 column1
	char *am_pm;		//lưu ký tự biểu diễn PM hoặc AM
	if(current_time.time_format != TIME_FORMAT_24HRS)
	{
		am_pm = (current_time.time_format) ? "PM" : "AM";
		//printf("Current time = %s %s \n",time_to_string(&current_time),am_pm); //Vd: 04:25:41 PM

		/* Vd2: hiển thị thời gian */
		lcd_print_string(time_to_string(&current_time));
		lcd_print_char(' ');
		lcd_print_string(am_pm);
	}
	else
	{
		//printf("Current time = %s \n",time_to_string(&current_time)); //Vd: 04:25:41

		/* Vd2: hiển thị thời gian */
		lcd_print_string(time_to_string(&current_time));
	}

	//Biểu diễn ngày tháng: 15/01/21 <friday>
	//printf("Current date: %s <%s>\n",date_to_string(&current_date),get_day_of_week(current_date.day));

	/* Vd2: hiển thị thời gian */
	lcd_set_cursor(2, 1);			//đưa con trỏ xuống hàng dưới //row2 column1
	lcd_print_string(date_to_string(&current_date));
	lcd_print_char(' ');
	lcd_print_string(get_day_of_week(current_date.day));
}

/****************************************************************
 * @fn			- mdelay
 *
 * @brief		- Hàm delay theo mili seconds
 *
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note 		- none
 */
static void mdelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1000); i++);
}
