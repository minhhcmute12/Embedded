/*
 * tutorial_rtc_project.c
 *
 *  Created on: Jan 30, 2024
 *      Author: Minh
 */
/**
*===Project: RTC on LCD
+ RTC+and+LCD+interfacing: sơ đồ nối dây
  DS1307 <---I2C---> STM32 ---GPIOs---> LCD 16x2

*==Real time clock on LCD(V263)
+ DS1307: tài liệu về module Real Time Clock : Các thông tin cần chú ý
 ^Absolute maximum ratings
  ~Voltage Range: -0.5V to +7V

 ^Recommended DC Operating Conditions
  ~Suplly Voltage(Vcc): Min(4.5V) và TYP(5.0) và Max(5.5V)
  ~Vùng Logic 1(V_IH): Min(2.2V) và Max(Vcc+0.3)
  ~Vùng Logic 1(V_IL): Min(-0.2V) và Max(+0.8)
   ->Việc xác định vùng logic để xác định vùng Logic giữa 2 thiết bị có tương thích hay không, từ đó có sử dụng
   hay không "Logic Level Converter"

  ^AC Electrical character
   ~SCL Clock Frequency(f_CLK): 0(Min) và 100(Max) kHz

  ^Clock and Calendar
   ~Timekeeper Register: việc trích xuất ngày và giờ

  ^Pin Description: Giải thích về các pin có trong con chip thiết bị

  ^Oscillator Circuit: DS1307 sử dụng thạch anh tần số 32.768kHz, tần số này ta dùng để tính toán thời gian

  ^I2C Data Bus: Xác định địa chỉ Ds1307
   ~Slave Receive Mode: 7-bit DS1307 có address = 1101000

+ Các lớp để lập trình RTC:
 ^Application layer: rtc_lcd.c   (Application layer)

 ^This layer abstract the usage of RTC and LCD : lcd.c, ds1307.c, lcd.h, ds1307.h (BSP layer)
  gồm các hàm: lcd_init(), lcd_write(), get_current_time(), get_date()

 ^This layer abstract theusage of MCU peripheral: GPIO_Driver, I2C_Driver  (HAL,LL  layer)

+ Lập trình DS1307 (ds1307.h)
 ^Contains device(RTC) related information - Lấy thông tin liên quan đến thiết bị (RTC) trong Datasheet:
  ~Đọc thật kỹ các thông tin đc cung cấp trong Datasheet của thiết bị muốn lập trình
  ~I2C address
  ~Register address
  ~Data Structure to handle the information - Tạo Cấu trúc dữ liệu để xử lý thông tin
  ~Function prototypes which are exposed to application - Các nguyên mẫu chức năng được đưa vào ứng dụng
  ~Application configurable items - Các mục có thể cấu hình ứng dụng

*==DS1307 RTC register(V264) - Tìm hiểu về module RTC DS1307
+ Xem xet bảng Timekeeper Register
 ^Cột Address : chứa tên thanh ghi
 ^Cột "Bit7" to "Bit0" : Chứa vị trí bit dùng để cấu hình thời gian
 ^Cột Function: cho biêt ta đang cấu hình đơn vị thời gian nào
 ^Cột Range: giói hạn cho phép cấu hình đơn vị thời gian

+Lưu ý: chip sẽ ko lưu giá trị thời gian ban đầu nên lập trình viên phải khởi tạo thời gian ban đầu đó

+ Vd về chuyển đổi thời gian:
 ^30 giây: có mã nhị phân là '000111100'(không sử dụng)
 Nhưng trong cấu hình thanh ghi RTC thì cần dạng mã BCD: 30= 3(0011) 0(0000) -> 0011_0000(BCD)

+ Giải thích về RTC register:
 ^00h register(8bit): 00h=0x00 địa chỉ define trong lập trình
  ~Bit8(CH bit): 1(disable), nghĩa là dừng bộ dao động đếm thời gian
				 0(enable), nghĩa là cho bộ dao động đếm thời gian, sau đó thời gian bị reset thành 01/01/00 01 00:00:00
				(MM/DD/YY DOW HH:MM:SS)
  ~Bit[6:4]: giá trị hàng chục giây
  ~Bit[3:0]: giá trị hàng giây

 ^0xh register(8bit): theo đơn vị giải thích trong datasheet

 ^07h register: đọc phần CONTROL REGISTER

+ Hướng dẫn lập trình thư viện DS1307(8p55s)

*==RTC code implementation(V265)
+Hàm memset() trong lập trình C được sử dụng để ghi một giá trị nhất định vào một vùng bộ nhớ.
 Cú pháp của hàm memset() như sau: void *memset(void *ptr, int value, size_t num);
 Hãy thận trọng khi sử dụng các đoạn mã. Tìm hiểu thêm
 Trong đó: ptr là con trỏ trỏ đến vùng bộ nhớ cần ghi.
           value là giá trị cần ghi vào vùng bộ nhớ.
		   num là số lượng byte cần ghi.
 Hàm memset() sẽ ghi giá trị value vào num byte đầu tiên của vùng bộ nhớ được trỏ bởi ptr.
 Ví dụ:
	char str[10] = "Hello";
	memset(str, '0', 5);
	printf("%s\n", str); // In ra "00000Hello"

 Trong ví dụ trên, hàm memset() sẽ ghi 5 ký tự '0' vào 5 byte đầu tiên của chuỗi str.

 Hàm memset() có thể được sử dụng cho nhiều mục đích khác nhau, chẳng hạn như:
	Khởi tạo một vùng bộ nhớ.
	Xóa dữ liệu trong một vùng bộ nhớ.
	Ghi một giá trị cụ thể vào một vùng bộ nhớ.
	Hàm memset() là một hàm rất hữu ích trong lập trình C.

 Dưới đây là một số lưu ý khi sử dụng hàm memset():
	Hàm memset() không kiểm tra xem vùng bộ nhớ được trỏ bởi ptr có đủ lớn để ghi num byte hay không.
	Hàm memset() sẽ ghi đè lên bất kỳ dữ liệu nào đã có trong vùng bộ nhớ được trỏ bởi ptr.

+ Thao khảo datasheet DS1307: định dạng "Frame Data"
 ^Data Write(ghi thời gian vào thanh ghi DS1307) do MCU ghi vào thanh ghi DS1307 - Slave Receiver Mode
 ^Data Read(đọc thời gian trong thanh ghi DS1307) và truyền dạng I2C đến MCU - Slave Transmitter Mode
 ^Data Read(Write Pointer, Then Read) - Slave Receive and Transmit
  Trường hợp này ta có thể vừa Write sau đó Read trong cùng một Frame Data gửi đi

*==RTC coding for set time and date(V266)
*==RTC coding for get time and date(V267)
*==RTC coding: BCD and Binary manipulation functions(V268)
+ Cách chuyển đổi sang dạng BCD và nhị phân:
 ^Vd: số 12 có (b)0001100 và dạng (BCD)0001_0010 với 0001 = 1 và 0010 = 2
 ^Vd: số x=41 , trong máy tính sẽ lưu số 42 dưới dạng nhị phân
 if(x>10){
	 m = x/10; 	//4 -> m << 4 : 01000000
	 n = x%10;	//1 -> n : 		00000001
	 bcd = (m << 4) | n; //->   01000001
 }

*==Writing RTC application(V269)
+Con trỏ hàm là một biến lưu trữ địa chỉ của một hàm trong bộ nhớ. Nó cho phép bạn thực hiện các thao tác
với hàm như:
 ^Truyền hàm như một tham số cho một hàm khác. Ví dụ, bạn có thể viết một hàm sắp xếp có thể nhận một hàm
 so sánh làm tham số.
 ^Lưu trữ địa chỉ của một hàm trong một biến. Điều này có thể hữu ích nếu bạn muốn gọi hàm đó sau này hoặc
 truyền nó cho một hàm khác.
 ^Thay đổi hành vi của một chương trình đang chạy. Ví dụ, bạn có thể sử dụng con trỏ hàm để thay đổi cách
 một hàm xử lý dữ liệu.
 ^Ví dụ:
    char* time_to_string(RTC_time_t *rtc_time){};
	RTC_time_t rtc_time;
	char *time_string;

	// Lấy thời gian hiện tại
	rtc_get(&rtc_time);

	// Chuyển đổi thời gian thành chuỗi
	time_string = time_to_string(&rtc_time);

	// In ra chuỗi thời gian
	printf("%s\n", time_string);

 Trong ví dụ này, con trỏ hàm time_to_string được sử dụng để chuyển đổi thời gian được lưu trữ trong cấu trúc
 RTC_time_t thành một chuỗi biểu diễn thời gian.

 ^Một số lợi ích của việc sử dụng con trỏ hàm:
  ~Tính linh hoạt: Con trỏ hàm cho phép bạn viết mã linh hoạt hơn và dễ dàng tái sử dụng hơn.
  ~Hiệu quả: Con trỏ hàm có thể giúp bạn cải thiện hiệu suất của chương trình bằng cách cho phép bạn tránh sao chép mã.
  ~Khả năng mở rộng: Con trỏ hàm cho phép bạn dễ dàng thêm các chức năng mới vào chương trình của mình.

 ^Một số lưu ý khi sử dụng con trỏ hàm:
  ~Kiểu dữ liệu: Bạn cần đảm bảo rằng kiểu dữ liệu của con trỏ hàm khớp với kiểu dữ liệu của hàm mà bạn muốn gọi.
  ~Quyền truy cập: Bạn cần đảm bảo rằng bạn có quyền truy cập vào hàm mà bạn muốn gọi.
  ~Bộ nhớ: Bạn cần đảm bảo rằng bạn giải phóng bộ nhớ được cấp phát bởi con trỏ hàm khi bạn không còn cần sử dụng nó nữa.
  ~Con trỏ hàm là một công cụ mạnh mẽ có thể giúp bạn cải thiện mã của mình theo nhiều cách. Tuy nhiên, bạn cần
  sử dụng nó một cách cẩn thận để tránh gặp lỗi.

*==RTC and Systick(V270)
+ Systick Interrupt: tiến hành lập trình để thiết lập ngắt mỗi 1 giây để cập nhật đồng hồ
 ^Datasheet_stm32f407x: khối "Cortex System Timer"
 ^Khi bạn cài đặt timer, nó sẽ tiến hành đếm ngược và khi bộ đếm về 0 thì có ngắt xảy ra

 ^Chúng ta cần tính toán sẽ phải set "count value" là bao nhiêu vào bộ Systick Timer để cứ 1s ta có một ngắt
 ^Systick -> 16MHz(HSI) => mỗi (1/(16*10^6)) 0.0625 us = 1 single down count timer(đếm ngược)
  => vậy 1s = x_? down count timer
  => x = 1 /(0.0625 * 10^-6) = 16 *10^6 Hz / 1Hz => X = 16*10^6
  => Cứ khi value X đếm về 0 thì có một ngắt interrupt xảy ra

*==LCD connections(V271)
+ LCD 16x2 có 16 pins: (Kết nối theo project bài học)
 ^V_SS nối GND , V_CC nối 5V
 ^R_S nối PD0(GPIO) , R/W nối PD1(GPIO) , En nối PD2(GPIO)
 ^D1,D2,D3,D4: là các đừng data kết hợp với D4,D5,D6,D7 để tạo dữ liệu song song 8bit (Trong bài này ko sử dụng)
 ^D4 nối PD3(GPIO), D5 nối PD4(GPIO), D6 nối PD5(GPIO), D7 nối PD6(GPIO) dùng để tạo dữ liệu song song 4bit
 ^Anode+ Led nối 5V, Anode- Led nối GND : điều khiển đèn nền của LCD
 ^V_E dùng để điều chỉnh độ tương phản: nối với biến trở 10K, V_E nối Wiper(chân giữa biến trở),
 Hai chân còn lại của biến trở một nối 5V một nối GND

*==Significance of LCD pins(V272)
+ HF44780: tài liệu về LCD 16x2
+ Các phần cần chú ý:
 ^Pin function
  ~RS pin: dùng để phân biệt giữa "Command-0" và "Data-1"
  ~E pin: cho phép việc read/write trên bộ điều khiển LCD module

*==LCD initialization(V273)
*==LCD initialization flowchart(V274)
+ Datasheet HF44780: xem thông tin về 8-Bit(use 8pins) Interface và 4-Bit(use 4pín) Interface
 ^Tham khảo sơ đồ các bước để cấu hình LCd khi khởi động

*==Sending command and data to LCD(V275)
+ LCD command: các chức năng có thể thực hiện
 1. Function set
 2. Display on/off, cursor(con trỏ) on/off and blink control
 3. Entry mode set
 4. LCD clear display
 5. Cursor return home
 6. Set co-ordinates
 7. Display right/left shift
 8. Cursor on/off, blink on/off
 9. Address counter read/write

+ Sending command/data
 ^Một byte lệnh(command) hoặc dữ liệu(data) có độ dài 8 bit (1 byte)
 ^Bạn có thể gửi tất cả 8 bit trong một lần qua 8 dòng dữ liệu hoặc bạn có thể chia thành 2 lần truyền dữ liệu,
 mỗi lần 4 bit
 ^Để truyền dữ liệu(data transmission) 4 bit, bạn chỉ cần 4 đường dữ liệu được kết nối giữa LCD và MCU
 ^Để truyền dữ liệu 4 bit, bạn phải sử dụng các dòng dữ liệu D4, D5 D6, D7

+ Sending a command - Cách gửi một command
 1. Tạo mã lệnh(command9)
 2. Đặt chân RS ở mức thấp
 3. Đặt chân RnW ở mức thấp
 4. Trước tiên hãy gửi đoạn mã cao hơn(higher nibble) (4-bit) của mã lệnh(command code) tới các dòng dữ liệu
 5. Đặt chân kích hoạt LCD (LCD enable) từ cao xuống thấp (khi LCD phát hiện sự chuyển đổi từ cao xuống thấp trên chân
 kích hoạt, nó sẽ đọc dữ liệu từ các đường dữ liệu)
 6. Tiếp theo gửi đoạn mã lệnh phía dưới(lower nibble) tới các dòng dữ liệu
 7. Đặt chân kích hoạt LCD từ cao xuống thấp (khi LCD phát hiện sự chuyển đổi từ cao xuống thấp trên chân kích hoạt,
 nó sẽ đọc dữ liệu từ các đường dữ liệu)
 8. Đợi thời gian thực hiện lệnh trước khi gửi lệnh tiếp theo hoặc xác nhận màn hình LCD "not busy" bằng cách đọc
 trạng thái cờ busy trên chân D7

 ^VD: 0x28 ---BCD--> 0010 1000
				bit: 7654 3210
		| ------------- D4--------------->|
	MCU	| --------------D5--------------->|	LCD
		| --------------D6--------------->|
		| --------------D7--------------->|

  ~Gửi higher nibble(4-bit) 0010(bit: 7654) và sau đó là lower nibble(4-bit) 1000(bit: 3210)
  Khi thực hiện truyền theo:  bit7,bit3 truyền trên D7 line; bit6,bit2 truyền trên D6 line
							  bit5,bit1 truyền trên D5 line; bit4,bit0 truyền trên D4 line

+ Sending a byte - Cách gửi một byte data
 1. Đặt chân RS lên cao
 2. Đặt chân RnW ở mức thấp
 3. Trước tiên hãy gửi dữ liệu có độ phân giải cao hơn (4 bit) tới các dòng dữ liệu
 4. Đặt chân kích hoạt LCD(LCD enable) từ cao xuống thấp (khi LCD phát hiện sự chuyển đổi từ cao xuống thấp trên chân
 kích hoạt, nó sẽ đọc dữ liệu từ các đường dữ liệu)
 5. Tiếp theo gửi phần dưới của dữ liệu tới các dòng dữ liệu
 6. Đặt chân kích hoạt LCD từ cao xuống thấp (khi LCD phát hiện sự chuyển đổi từ cao xuống thấp trên chân kích hoạt,
 nó sẽ đọc dữ liệu từ các đường dữ liệu)

*==Creating LCD command code(V276)
+ Tài liệu datasheet: Table6. Introduction
 ^Danh sách các "command code" và mã nhị phân của lệnh trên từng line
 Vd: lệnh "Clear Display" có mã từ DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0 là 0 0 0 0 | 0 0 0 1 ---BCD---> 0x 0 1

 ^Một số "command code" có các bit lựa chọn nên cần kết hợp các các cột "Description" và bảng phụ để tạo ra mã bit
 có chức năng mong muốn, sau đó define mã command code đúng với biến đại diện

*==Testing(V277)
+ Tìmm hiểu về cách di chuyển con trỏ về vị trí mong muốn trên LCD và cách nó lưu trên DRAM
 ^Vd: LCD 16x2 có 2 row và 16 column và ta liên tưởng đến trục toạ độ
  ~Có vị trí đầu tiền là (1,1)(row1-column1), hoặc là vị trí (2,3)(row2-column3)
  ~Theo datasheet LCD module thì Vị trí đầu tiền (1,1) đc liên kết địa chỉ của DRAM là 00,
  hoặc là vị trí (2,3) đc liên kết địa chỉ của DRAM là 42
   -> Nếu làm việc với nhiều loại LCD module khác nhau thì bắt buộc phải tham khảo datasheet của LCD module đó để nắm đc
   cách phân chia row-column liên kết với địa chỉ trên ô nhớ trên DRAM

  ~Để trỏ tới vị trí mong muốn trên LCD thì cũng đồng nghĩa với việc thay đổi liên kết địa chỉ ô nhớ DRAM
  Ta sẽ sử dụng lệnh command "Set DDRAM address"
   B1: Trên LCD 16x2 thì đối với lệnh command "Set DDRAM address" 3 bit RS R/W DB7 lần lượt có mặc định là 0 0 1
   Ta sẽ tiến hành thay đổi liên kết địa chỉ ô nhớ vs DDRAM bằng các bit từ DB6~DB0

   B2: Theo datasheet thì địa chỉ 2 vị trí đầu tiên của mỗi row là 00 và 40
   Vd: Ta thực hiện set vị trí (0,0) có địa chỉ ô nhớ liên kết là 00
    -> 0(RS) 0(R/W) 1(DB7) 0(DB6) 0(DB5) 0(DB4) 0(DB3) 0(DB2) 0(DB1) 0(DB0) --> 1000 000 -> 0x80
	Ta thực hiện set vị trí (1,0) có địa chỉ ô nhớ liên kết là 40
	-> 0(RS) 0(R/W) 1(DB7) 1(DB6) 0(DB5) 0(DB4) 0(DB3) 0(DB2) 0(DB1) 0(DB0) --> 1100 000 -> 0xC0
    -> Chọn vị trí colum mong muốn và tiến hành pp dịch bit vs vị trí gốc để đc column mong muốn
	dịch bit column |= 0x80(row1) và column |= 0xC0(row2)
	Lưu ý: giá trị column chọn phải trừ đi 1 đơn vị do địa chỉ cột ô nhớ trên DRAM bắt đầu từ 0

+ Cách code macros để vô hiệu hóa một các dòng lệnh
*/

