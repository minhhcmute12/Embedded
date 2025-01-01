/*
 * lcd.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 */


#include "lcd.h"
#include "stm32f407xx_gpio_driver.h"

static void write_4_bits(uint8_t value);
static void lcd_enable(void);
static void mdelay(uint32_t cnt);
static void udelay(uint32_t cnt);

/****************************************************************
 * @fn			- lcd_send_command
 *
 * @brief		- Gửi lệnh command điều khiển LCD module
 *
 * @param[in]	- cmd: nội dung lệnh command
 *
 * @return		- none
 *
 * @Note 		- none
 */
void lcd_send_command(uint8_t cmd)
{
	/* RS=0 for LCD command */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/*R/nW = 0, for write */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(cmd >> 4);		//4bit higher nibble
	write_4_bits(cmd & 0x0F);	//4bit low nibble, 0x0F= 0000 1111, truyền 4 bit thấp hơn

}

/****************************************************************
 * @fn			- lcd_print_char
 *
 * @brief		- Gửi một byte data đến hiển thị trên LCD
 *
 * @param[in]	- data: nội dung 1 ký tự
 *
 * @return		- none
 *
 * @Note 		- This function sends a character to the LCD
 *				- Here we used 4 bit parallel data transmission.
 *				- First higher nibble of the data will be sent on to the data lines D4,D5,D6,D7
 *				- Then lower nibble of the data will be set on to the data lines D4,D5,D6,D7
 */
void lcd_print_char(uint8_t data)
{
	/* RS=1 for LCD user data */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);

	/*R/nW = 0, for write */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	write_4_bits(data >> 4);  /*Higher nibble*/
	write_4_bits(data & 0x0F); /*Lower nibble*/

}

/****************************************************************
 * @fn			- lcd_print_string
 *
 * @brief		- in thônng tin chuỗi lên LCD
 *
 * @param[in]	- message: địa chỉ nội dung tin nhắn chuỗi
 *
 * @return		- none
 *
 * @Note 		- Nó sẽ tách từng ký tự trong chuỗi rồi gửi hiển thị, và dừng khi gặp ký tự '\0'
 */
void lcd_print_string(char *message)
{
      do
      {
          lcd_print_char((uint8_t)*message++);
      }
      while (*message != '\0');

}

/****************************************************************
 * @fn			- lcd_init
 *
 * @brief		- Hàm khởi tạo lập kết nối giữa MCU và LCD
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- Cấu hình phần cứng chuyển đổi sang lệnh code
 * 				- Tham khảo datasheet phần "4-Bit Interface"
 */
void lcd_init(void)
{
	//1. Configure the gpio pins which are used for LCD connections
	//Định cấu hình các chân gpio được sử dụng cho kết nối LCD
	GPIOx_Handle_t lcd_signal;				//Struct đại diện Cấu hình chân GPIO

	//Cấu hình portD kết nối và các thông số cho từng pin kết nối
	lcd_signal.pGPIOx = LCD_GPIO_PORT;								//PortD

	lcd_signal.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RS;			//RS - PD0
	lcd_signal.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	lcd_signal.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	lcd_signal.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_RW;			//RW - PD1
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_EN;			//EN - PD2
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D4;			//D4 - PD3
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D5;			//D5 - PD4
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D6;			//D6 - PD5
	GPIO_Init(&lcd_signal);

	lcd_signal.GPIO_PinConfig.GPIO_PinNumber = LCD_GPIO_D7;			//D7 - PD6
	GPIO_Init(&lcd_signal);

	//Cấu hình ban đầu giữ các pins LCD đều ở mức Low(0)
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);

	//2. Do the LCD initialization -  phần "4-Bit Interface"
	mdelay(40);

	/* RS = 0 , For LCD command */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);

	/* RnW = 0, Writing to LCD */
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	//Cấu hình ban đầu 4-bit interface theo flowchart lần 1
	write_4_bits(0x3);	//0x3=0011 , nghĩa là RS-RW-DB7-DB6-DB5-DB4 = 0-0-0-0-1-1

	mdelay(5);			//Chờ khoảng 4.1 ms để LCD module cập nhật thông số

	write_4_bits(0x3);	//Cấu hình ban đầu 4-bit interface  một lần 2

	udelay(150);		//Chờ khoảng 100us để LCD module cập nhật thông số

	write_4_bits(0x3);	//Cấu hình ban đầu 4-bit interface  một lần 3

	write_4_bits(0x2);	//0x2=0010 , nghĩa là RS-RW-DB7-DB6-DB5-DB4 = 0-0-0-0-1-0

	/*3. Cấu hình LCD một cách từng bước theo các lệnh Command Code */
	//function set command
	lcd_send_command(LCD_CMD_4DL_2N_5X8F);

	//disply ON and cursor ON
	lcd_send_command(LCD_CMD_DON_CURON);

	//Display Clear, đảm bảo khi khởi tạo LCD ko lưu hiển thị các công việc trước đó
	lcd_display_clear();

	//entry mode set
	lcd_send_command(LCD_CMD_INCADD);

}

/****************************************************************
 * @fn			- write_4_bits
 *
 * @brief		- write_ghi 4 bit dữ liệu/lệnh vào các dòng D4,D5,D6,D7
 *
 * @param[in]	- value: giá trị bit đại diện cho đường cần sử dụng
 *
 * @return		- none
 *
 * @Note 		- vd: 0x3=0011, nghĩa là yêu cầu sử dụng đường D5,D4 còn D7,D6 là 0
 */
static void write_4_bits(uint8_t value)
{
	//Cấu hình từng chân Data LCD module
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D4, ((value >> 0) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D5, ((value >> 1) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D6, ((value >> 2) & 0x1) );
	GPIO_WriteToOutputPin(LCD_GPIO_PORT,LCD_GPIO_D7, ((value >> 3) & 0x1) );

	/*
	 * Giải thích: vd value đưa vào là 0x3=0011, (MSB)D7=D6=0 , D5=D4(LSB)=1
	 * Th1: Có: value >> 0, nghĩa là dịch đến bit0(tính từ LSB) của value
	 * sau đó lấy giá trị tại vị trí bit0 'and' với 0x1=1
	 * có: bit[0]=1 & 1 = 1
	 *
	 * Th1: Có: value >> 3, nghĩa là dịch đến bit3(tính từ LSB) của value
	 * sau đó lấy giá trị tại vị trí bit3 'and' với 0x1=1
	 * có: bit[3]=0 & 1 = 0
 	 */

	//Kích hoạt Enable pin
	lcd_enable(); 	//Theo nguyên tăc khi bạn kích hoạt các Data pins thì bạn cũng cần một lệnh
					//chốt đối với data bên trong LCD, cho phép data vào LCD module
}

/****************************************************************
 * @fn			- lcd_display_clear
 *
 * @brief		-
 *
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note 		- none
 */
void lcd_display_clear(void)
{
	//Display clear
	lcd_send_command(LCD_CMD_DIS_CLEAR);

	/*
	 * check page number 24 of datasheet - kiểm tra trang số 24 của bảng dữ liệu datasheet..
	 * display clear command execution wait time is around 2ms
	 * thời gian chờ thực hiện lệnh  "command clear" hoàn thành là khoảng 2ms
	 */

	mdelay(2);
}

/****************************************************************
 * @fn			- lcd_display_return_home
 *
 * @brief		- Hàm thiết lập  vị trí Con trỏ trở về vị trí ban đầu
 *
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note 		- Cursor returns to home position
 */
void lcd_display_return_home(void)
{

	lcd_send_command(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of datasheet.
	 * return home command execution wait time is around 2ms
	 */
	mdelay(2);
}

/****************************************************************
 * @fn			- lcd_set_cursor
 *
 * @brief		- Set vị trí con trỏ ký tự hiển thị trên LCD
 *
 * @param[in]	- row: vị trí hàng ngang(y=1~2)
 * @param[in]	- column: vị trí hàng dọc(x=1~16)
 *
 * @return		- none
 *
 * @Note 		- Set Lcd to a specified location given by row and column information
 * 				Đặt Lcd ở một vị trí được chỉ định theo thông tin hàng và cột
 * 				- Row Number (1 to 2)
  *   			- Column Number (1 to 16) Assuming a 2 X 16 characters display
 */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
  column--;			//trừ đi 1 vì vị trí bắt đầu ô nhớ tròn DRAM bắt đầu từ vị trí 0

  switch (row)		//chọn row để xác định tham số dịch bit đúng
  {
    case 1:		//hàng thứ 1
      /* Set cursor(con trỏ) to 1st row address and add index*/
      lcd_send_command((column |= 0x80));
      break;
    case 2:		//hàng thứ 2
      /* Set cursor to 2nd row address and add index*/
        lcd_send_command((column |= 0xC0));
      break;
    default:
      break;
  }
}


/****************************************************************
 * @fn			- lcd_enable
 *
 * @brief		- Kích hoạt LCD enable
 *
 * @param[in]	- none
 *
 * @return		- none
 *
 * @Note 		- chuyển trạng thái mức điện áp từ high->low->high
 * 				- Chỉ đc phép truyền data trên các line Dx khi Enable line ở mức low
 */
static void lcd_enable(void)
{
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	udelay(10);
	GPIO_WriteToOutputPin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	udelay(100);	/* execution time > 37 micro seconds */
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

/****************************************************************
 * @fn			- udelay
 *
 * @brief		- Hàm delay theo micro seconds
 *
 * @param[in]	-
 *
 * @return		- none
 *
 * @Note 		- none
 */
static void udelay(uint32_t cnt)
{
	for(uint32_t i=0 ; i < (cnt * 1); i++);
}
