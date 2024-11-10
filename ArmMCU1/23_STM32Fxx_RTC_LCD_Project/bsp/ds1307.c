/*
 * ds1307.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 *   File này chứa các lập trình MCU để đọc đc các giá trị từ DS1307 gửi về khi kết nối I2C
 */

#include <stdint.h>			//thư viên cho khai báo các kiểu biến
#include <string.h>			//thư viện cho hamd memset()
#include "ds1307.h"

/*
 * Static Function Prototype
 */
static void ds1307_i2c_pin_config();
static void ds1307_i2c_config(void);
static void ds1307_write(uint8_t value, uint8_t reg_addr);
static uint8_t ds1307_read(uint8_t reg_addr);
static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);

I2C_Handle_t g_ds1307I2CHandle;		//biến toàn cục

/****************************************************************
 * @fn			- ds1307_init
 *
 * @brief		- Khởi tạo thông số I2C cho RTC DS1307 module
 *
 * @param[in]	- none
 *
 * @return		- 1: CH=1 -> init failed
 *				  0: CH=0 -> init success
 * @Note 		- none
 */
uint8_t ds1307_init()
{
	//1. Initialize the i2c pins
	ds1307_i2c_pin_config();

	//2. Initialize the i2c peripheral
	ds1307_i2c_config();

	//3. Enable the I2C peripheral
	I2C_PeripheralControl(DS1307_I2C, ENABLE);

	//4. Make clock halt = 0, cấu hình dừng đồng hồ tại 0, cấu hình thanh ghi 00h với value 0
	ds1307_write(0x00, DS1307_ADDR_SEC);

	//5. Read back clock halt bit(đọc value thanh ghi 00h)
	uint8_t clock_state = ds1307_read(DS1307_ADDR_SEC);

	return ((clock_state >> 7) & 0x1);		//Đọc CH bit[7] và trả về giá trị CH bit
}

/****************************************************************
 * @fn			- ds1307_set_current_time
 *
 * @brief		- Cấu hình thời gian đồng hồ (giờ, phút, giây,kiểu giờ)
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- Cấu hình các thanh ghi 00h,01h,02h
 */
void ds1307_set_current_time(RTC_time_t *rtc_time)
{
	//Xử lý thanh ghi 00h liên quan đến second và bit7[CH]
	uint8_t seconds;
	seconds = binary_to_bcd(rtc_time->seconds);			//chuyển đổi dạng nhị phân sang bcd
	seconds &= ~(1 << 7);			//xóa bit7[CH] của 00h register về 0, đảm bảo rằng cho phép clock hoạt động
	ds1307_write(seconds,DS1307_ADDR_SEC);				//set second 00h regíter

	//minute
	ds1307_write(binary_to_bcd(rtc_time->minutes),DS1307_ADDR_MIN);		//set minute 01h register

	//hour
	uint8_t hrs;
	hrs = binary_to_bcd(rtc_time->hours);

	if(rtc_time->time_format == TIME_FORMAT_24HRS)
	{
		hrs &= ~(1 << 6);			//format 24h: clear bit6 của 01h register
	}
	else
	{
		hrs |= (1 << 6);			//format 12h: set bit6 của 01h register

		//toán tử ba ngôi cấu hình bit5 của 01h register
		hrs = (rtc_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1 << 5) : hrs & ~(1 << 5);
	}
	ds1307_write(hrs,DS1307_ADDR_HRS);				//set hour 02h regíter
}

/****************************************************************
 * @fn			- ds1307_get_current_time
 *
 * @brief		- Đọc giá trị thời gian(giờ,phút,giây,kiểu giờ) hiện tại
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- Giá trị đọc đc lưu vào con trỏ rtc_time
 */
void ds1307_get_current_time(RTC_time_t *rtc_time)
{
	//second
	uint8_t seconds;
	seconds = ds1307_read(DS1307_ADDR_SEC);			//lệnh read thanh ghi 00h register
	seconds &= ~(1 << 7);							//chắc chắn rằng bit7[CH] đc clear
	rtc_time->seconds = bcd_to_binary(seconds);		//chuyển đổi và set seconds vào con trỏ

	//minute
	rtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADDR_MIN));  //lệnh read thanh ghi 01h register
																	//chuyển đổi và set minutes vào con trỏ

	//hour
	uint8_t hrs;
	hrs = ds1307_read(DS1307_ADDR_HRS);				//lệnh read thanh ghi 02h register
	//Biện luận bit6 của 02h register đế xác định kiểu thời gian đang lưu
	if(hrs & (1 << 6))	//bit6=0
	{
		//set 12hrs format, check bit5 nếu 0:giờ bình thường or 1:Kiểu giờ AM-PM
		rtc_time->time_format = !((hrs & (1 << 5)) == 0);  //nếu bit5là 0 thì nó sẽ đảo lên thành 1
														  //biến nó thành kiểu giờ AM-PM
		hrs &= ~(0x3 << 5);	//0x3=0011 : clear 6 and 5 bắt đầu từ vị trí bit5
	}
	else
	{
		//set 24hrs format
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}
	rtc_time->hours = bcd_to_binary(hrs);				//chuyển đổi và set hour vào con trỏ
}

/****************************************************************
 * @fn			- ds1307_set_current_date
 *
 * @brief		- Cấu hình thời gian ngày tháng năm
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- none
 */
void ds1307_set_current_date(RTC_date_t *rtc_date)
{
	//set date 04h register
	ds1307_write(binary_to_bcd(rtc_date->date),DS1307_ADDR_DATE);

	//set month 05h register
	ds1307_write(binary_to_bcd(rtc_date->month),DS1307_ADDR_MONTH);

	//set year 06h register
	ds1307_write(binary_to_bcd(rtc_date->year),DS1307_ADDR_YEAR);

	//set day(thứ) 03h register
	ds1307_write(binary_to_bcd(rtc_date->day),DS1307_ADDR_DAY);
}

/****************************************************************
 * @fn			- ds1307_get_current_date
 *
 * @brief		- đọc giá trị thứ ngày tháng năm
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		-  Giá trị đọc đc lưu vào con trỏ rtc_date
 */
void ds1307_get_current_date(RTC_date_t *rtc_date)
{
	//date
	rtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADDR_DATE));

	//month
	rtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADDR_MONTH));

	//year
	rtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADDR_YEAR));

	//day
	rtc_date->day = bcd_to_binary(ds1307_read(DS1307_ADDR_DAY));
}

/****************************************************************
 * @fn			- ds1307_i2c_pin_config
 *
 * @brief		- Cấu hình ban đầu I2C Pin nối với STM32f407 MCU
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- 1. Tùy vào cấu hình phần cứng mà lập trình viên có thể chọn chân AF
 * 				Nếu thay đổi cấu hình AF thì cần viết lại code hàm này
 * 				- 2. Vì hàm sử dụng GPIOx_Driver của chip STM32F407xx nên hàm ds1307_i2c_pin_config chỉ dùng
 * 				cho chip này, nếu thay đổi driver chip khác cũng cần viết lại code hàm
 */
static void ds1307_i2c_pin_config()
{
	GPIOx_Handle_t i2c_sda,i2c_scl;

	//hàm memset() sẽ ghi số 0 vào bao nhiêu vị trí sizeof(x) byte của con trỏ struct i2c_x
	memset(&i2c_sda,0,sizeof(i2c_sda));		//tất cả value trong i2c_sda đều là 0
	memset(&i2c_scl,0,sizeof(i2c_scl));		//tất cả value trong i2c_scl đều là 0

	/*
	 * I2C1_SCL(DS1307) ==> PB6(STM32)
	 * I2C1_SDA(DS1307) ==> PB7(STM32)
	 */
	i2c_sda.pGPIOx = DS1307_I2C_PORT;								//Cấu hình port dùng kết nối DS1307->MCU
	i2c_sda.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_sda.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_sda.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SDA_PIN;
	i2c_sda.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_sda.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_sda.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&i2c_sda);

	i2c_scl.pGPIOx = DS1307_I2C_PORT;
	i2c_scl.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	i2c_scl.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	i2c_scl.GPIO_PinConfig.GPIO_PinNumber = DS1307_I2C_SCL_PIN;
	i2c_scl.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	i2c_scl.GPIO_PinConfig.GPIO_PinPuPdControl = DS1307_I2C_PUPD;
	i2c_scl.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&i2c_scl);
}

/****************************************************************
 * @fn			- ds1307_i2c_config
 *
 * @brief		- Cấu hình ban đầu I2Cx_MCU kết nối ds1307_i2c
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- 1. Tùy thuộc vào lựa chọn I2Cx_MCU mà lập trình viên sẽ triển khai code khác nhau
 * 				- 2. Vì hàm sử dụng GPIOx_Driver của chip STM32F407xx nên hàm ds1307_i2c_pin_config chỉ dùng
 * 				cho chip này, nếu thay đổi driver chip khác cũng cần viết lại code hàm
 */
static void ds1307_i2c_config(void)
{
	g_ds1307I2CHandle.pI2Cx = DS1307_I2C;	//Cấu hình I2Cx_MCU sử dụng kết nối DS1307
	g_ds1307I2CHandle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	g_ds1307I2CHandle.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;
	I2C_Init(&g_ds1307I2CHandle);
}

/****************************************************************
 * @fn			- ds1307_write
 *
 * @brief		- Ghi thời gian vào thanh ghi ds1307
 *
 * @param[in]	- value : giá trị thời gian
 * @param[in]	- reg_addr: địa chỉ thanh ghi muốn ghi
 *
 * @return		- none
 *
 * @Note 		- Frame data gửi đến DS1307: S|<Slave_address>|0(W)|Ack|<Word_Address>|A|<Data(n)>|...|A|P
 */
static void ds1307_write(uint8_t value, uint8_t reg_addr)
{
	uint8_t tx[2];		//Một mảng 2 phần tử, lưu 2 byte thông số
	tx[0] = reg_addr;
	tx[1] = value;

	//Gửi thông số muốn ghi đến DS1307
	I2C_MasterSendData(&g_ds1307I2CHandle, tx, 2, DS1307_I2C_ADDRESS, 0);	//0: ko có lặp lại Frame Data
}

/****************************************************************
 * @fn			- ds1307_write
 *
 * @brief		- Đọc thời gian thanh ghi ds1307
 *
 * @param[in]	- reg_addr: địa chỉ thanh ghi muốn đọc
 *
 * @return		- none
 *
 * @Note 		- Frame read data DS1307: S|<Slave_address>|1(R)|Ack|<Data(n)>|...|/A|P
 */
static uint8_t ds1307_read(uint8_t reg_addr)
{
	uint8_t data;  //biến lưu giá trị đọc đc từ nhận từ Ds1307

	//Gửi đi lệnh đọc thanh ghi tại địa chỉ reg_addr tới DS1307
	I2C_MasterSendData(&g_ds1307I2CHandle, &reg_addr, 1, DS1307_I2C_ADDRESS, 0);

	//Nhận giá trị vào biến data từ địa chỉ reg_addr của Ds1307
	I2C_MasterReceiveData(&g_ds1307I2CHandle, &data, 1, DS1307_I2C_ADDRESS, 0);

	return data;
}

/****************************************************************
 * @fn			- bcd_to_binary
 *
 * @brief		- Chuyển đổi số từ BCD sang Binary
 *
 * @param[in]	- value: giá trị số bcd cần chuyển đổi
 *
 * @return		- Số Binary nhị phân
 *
 * @Note 		- Vd: Số 12 có (b)0001100 và dạng (BCD)0001_0010 với 0001 = 1 và 0010 = 2
 * 				- Lưu ý: Máy tính sẽ hoạt động dưới dạng nhị phân và DS1307 lưu số dạng BCD
 */
static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m , n;
	m = (uint8_t) ((value >> 4 ) * 10);
	n =  value & (uint8_t)0x0F;
	return (m+n);
}

/****************************************************************
 * @fn			- binary_to_bcd
 *
 * @brief		- Chuyển đổi số từ Binary sang BCD
 *
 * @param[in]	- value: giá trị số binary cần chuyển đổi
 *
 * @return		- Số BCD
 *
 * @Note 		- Vd: Số 12 có (b)0001100 và dạng (BCD)0001_0010 với 0001 = 1 và 0010 = 2
 * 				- Lưu ý: Máy tính sẽ hoạt động dưới dạng nhị phân nên 'value' đưa vào sẽ đc máy tính
 * 				chuyển sang nhị phân và và DS1307 lưu số dạng BCD
 */
static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m , n;
	uint8_t bcd;

	bcd = value;
	if(value >= 10)
	{
		m = value /10;
		n = value % 10;
		bcd = (m << 4) | n ;
	}

	return bcd;
}
