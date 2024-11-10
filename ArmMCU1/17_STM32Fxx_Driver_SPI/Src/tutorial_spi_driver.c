/*
 * tutorial_spi_driver.c
 *
 *  Created on: Dec 3, 2023
 *      Author: Minh
 */

/**
*===SPI Driver API Requirement and Configuaration Structure(S36)
*==SPI API Requirement and configuration items(V134)
+ Sơ đồ: 						  -->	SPI initialization / Periperal Clock Control
								  -->	SPI TX
			SPI Driver <--> APIs  -->	SPI RX
								  -->	SPI Interrupt config & handling
								  -->	Other SPI Management APIs
 ^Đây là các hàm cơ bản phục vụ lập trình cấu hình SPI  trong lớp SPI APIs_Driver

+ SPIs Periperals:
						|SPI_DeviceMode	|
						|SPI_BusConfig	|
						|SPI_DFF		|
SPIx_Peripheral	------>	|SPI_CPHA		|
						|SPI_CPOL		|
						|SPI_SSM		|
						|SPI_Speed		|
 ^Các Items trên là các mục(các hàm lập trình) có thể dùng cho cấu hình cho ứng dụng người dùng
 ^SPI_DeviceMode: cấu hình thiết bị đó là là Master mode hay là Slave mode
 ^SPI_BusConfig: lựa chọn giao tiếp muốn dùng, là full-duplex, half-duplex, simplex
 ^SPI_DFF: Data frame format, cấu hình định dạng data mong muốn 8bit hay 16bit
 Vd: nếu là 8bit thì "Shift Register" có định dạng là 8bit

 ^SPI_CPHA: Cấu hình phần chu kỳ sử dụng clock để lấy mẫu
 ^SPI_CPOL: Cấu hình định dạng cạnh(lên hay xuống) xung clock sử dụng để lấy mẫu
 Cả hai giá trị này có cấu hình mặc định là đều bằng '0'

 ^SPI_SSM: cấu hình dạng Slave là Software Slave hay Hardware Slave

 ^SPI_Speed(SCLK Speed): cấu hình tốc độ xung nhịp cần thiết để truyền hoặc nhận data bằng giao tiếp SPI

 ^Giống với cấu hình GPIO, ta cũng sẽ thiết lập SPI dựa trên là một Struct SPI với các thành phần là
 các hàm SPIx_Peripheral ở trên. -> Biến SPI Struct: SPI_Config_t

 ^Biến Struct dùng để thực hiện xử lý giao tiếp SPI : SPI_Handle_t
  Với thành phần là  SPI_Config_t và SPI_RegDef_t(địa chỉ SPIx(x:0,1,2) muốn dùng)

*==Updating MCU Specific header file with SPI(V135)
+ Tạo các file header và source cho SPI Driver

*==SPI adding API Prototypes to Driver header file(V136)
+ Tiến hành code khai báo các Prototypes trong file header và sẽ đc triển khai trong file source

*===SPI Driver API Implemetation: Clock Control(S37)(V137)
+ Code triển khai hàm "SPI_PeriClockControl"

*===SPI Driver API Implemetation: SPI Init(S38)
*==SPI User configuration options writting and register bit definition macros(V138)
+ Tìm hiểu về các thanh ghi cấu hình cho SPI:
+ Tài liệu Datasheet : RM407 -> 28.5 SPI and I2S registers
 ^SPI status register (SPI_SR): thanh ghi chứa các flag(cờ) trạng thái khác nhau.
  ~Cờ trạng thái là trạng thái bit mà chúng ta có thể dựa vào để xác định các hoạt động đang xảy ra trên SPI
  Vd: nếu có lỗi trong việc truyền nhận data thì trạng thái đó sẽ đc thể hiển bit trên thanh ghi này

+ Lưu ý: 1.Việc xác định thiết bị là Master là Slave để xác định thiết bị nào sẽ tạo xung Clock hoạt động
 Vd: nếu Master là Arduino và Slave là STM32 thì board cần cấu hình xung Clock là bên phía Arduino, còn bên phía
 STM32 là Slave nên ko cần cấu hình thanh phần "SPI_Speed(SCLK Speed)".

 2.Ngoài ra, chỉ khi và khi nào có xung Clock từ Master thì việc truyền nhận data với Slave mới đc bắt đầu
 Vd: nếu bên STM32(Slave) có lệnh truyền data đến Arduino(Master) nhưng lúc này phía Arduino chưa enable xung Clock
 hoặc việc đã tạo xung clock nhung giữa chừng ngừng cấp xung nên việc truyền data sẽ ko xảy ra hoặc bị gián đoạn
 => Việc truyền nhận trong SPI phụ thuộc vào CLOCK của Master Device

 3. Các thành phần biến cần thiết cho việc cấu hình sử dụng SPI Peripheral của "SPI_Config_t"
 Các thành phần biến này thực chất là các bit sẽ đc sử dụng để cấu hình SPI thuộc các thanh ghi SPI Peripheral
 Các thanh ghi SPI Peripheral đã đc định nghĩa "SPI_RegDef_t" trong file stm32f407xx.h
 Thanh ghi quan trọng nhất khi cấu hình SPI là SPI_RegDef_t->CR1
 Dựa vào datasheet của CR1 ta có thể tạo các trạng thái define cho các thành phần SPI_Config_t

*==SPI Implemetation of SPI Init API: part1(V139)
+ Mẹo: vì các vị trí bit trong thanh ghi đã đc thiết kế cố định nên ta hãy define tất cả bit trong thanh ghi để dễ
dàng cấu hình vị trí bit cũng như quản lý vị trí bit thanh ghi khi lập trình
 Vd: vì nhiều vị trí bit thanh ghi CR1 của SPI peripheral sẽ đc sử dụng nên ta hãy define các bit của CR1 register
 header file: #define CR1_MSTR	15
  ---sau đó lập trình--> Source file: tempreg &= ~(1 << CR1_MSTR);
*==SPI Implemetation of SPI Init API: part2(V140)
+ Chỉ cần định nghĩa các vị trí bit thuộc thanh ghi CR1, CR2, SR để cấu hình SPI
+ Viết hàm SPI_De-Init

*===SPI Driver API Implemetation: Send Data(S39)
*==Implemetation of SPI send data API: Part1(V141)
+ Sơ đồ hoạt đọng Send Data: 	  START
									|
						|-------> Len=0? ---Yes---> Exit from the function -> END
						|			|No
						|		Wait untill the TxBuffer is empty
						|			|
						|		DFF? ------ ==1 16bit-------> Load DR with 2 byte of Data And increment the buffer address--|
						|			|==0, 8bit																				|
						|		Load DR with 1 byte of Data And increment the buffer address								|
						|			|																					  Len--
						|<---------- Len--	<-------------------------------------------------------------------------------|
 ^Giải thích cách sơ đồ hoạt động:
  ~Nếu ko có(hoặc đã gửi hết) dữ liệu truyền(Len==0) thì thoát khỏi chương trình send data
  ~Nếu Len > 0 thì bắt đầu bước tiếp theo là chờ cho thanh ghi đệm TxBuffer đc trống("TXE[1]==1")
  ~Kiểm tra định dạng dữ liệu DFF là 8bit hay 16 bit
  ~Tải SPI_DR(SPI Data Register) với 1 byte dữ liệu và tăng địa chỉ bộ đệm(nghĩa là trừ độ dài đi 1 đơn vị(đã gửi đi một đơn vị)
  nếu là 8bit và trừ độ dài 2 đơn vị nếu là 16bit
  ~Kiểm tra xem đã gửi hết dữ liệu chưa. Nếu chưa thì tiếp tục chương trình gửi nếu Len==0 thì kết thúc chương trình gửi

 ^Cách xác định TxBuffer có đang trống hay không, ta sẽ dựa vào thanh ghi "SPI status register" cờ bit "TXE[1]"
  ~TXE[1]=0: not empty
  ~TXE[1]=1: empty -> phải chờ đến khi bit "TXE[1]==1"

+ Datasheet: RM407 -> 28.5.4 SPI data register (SPI_DR) (16bit)
 ^Thanh ghi dữ liệu được chia thành 2 bộ đệm - một bộ đệm để ghi (Bộ đệm truyền) và một bộ đệm khác để đọc (Nhận bộ đệm).
 Việc ghi vào thanh ghi dữ liệu sẽ ghi vào bộ đệm Tx và đọc từ thanh ghi dữ liệu sẽ trả về giá trị được giữ trong bộ đệm Rx.
 ^Nghĩa là đây là thanh ghi chứa dữ liệu đc truyền hoặc đc nhận

 ^Lưu ý: Những lưu ý này áp dụng cho chế độ SPI:
  ~Tùy thuộc vào bit chọn định dạng khung dữ liệu (DFF trong thanh ghi SPI_CR1), dữ liệu gửi hoặc nhận là 8 bit hoặc 16 bit.
  Lựa chọn này phải được thực hiện trước khi kích hoạt SPI để đảm bảo hoạt động chính xác.
  ~Đối với khung dữ liệu 8 bit, bộ đệm là 8 bit và chỉ LSB của thanh ghi(SPI_DR[7:0]) được sử dụng để truyền/nhận. Khi ở chế
  độ tiếp nhận, MSB của thanh ghi (SPI_DR[15:8]) bị buộc về 0.
  ~Đối với khung dữ liệu 16 bit, bộ đệm là 16 bit và toàn bộ thanh ghi SPI_DR[15:0] là dùng để truyền/nhận.

+ Con đường truyền và nhận data:
 ^Firmware Write -> Data Register -> APBx Bus -> TX_Buffer -> Shift Register -> Send Data
 ^Receiver Data -> Shift Register ->  RX_Buffer -> APBx Bus -> Data Register -> Firmware Read

 ^Khi bạn tiến hành thao tác gửi data thì data đang lưu ở Data Register sẽ tự động gửi vào TX_Buffer
 ^Khi bạn tiến hành đọc data ở Data Register thì tự động dữ liệu đang lưu ở RX_Buffer sẽ gửi vào Data Register

*==Implemetation of SPI send data API: Part2(V142)
+ "while(! (pSPIx->SR & (1 << 1) ));"
 ^Giải thích: giả sử bit 1(TXE) đang là 0 và khi AND với 1 cũng sẽ là 0
 ^Biện luận: lặp đến khi nào khác 0 -> nghĩa là TXE[bit1] = 1 thì thoát vòng lặp while

+ Tùy vào ứng dụng mà bạn sẽ chọn định nghĩa và kiểm tra các flag nào bên trong thanh ghi SPI_SR

*==Implemetation of SPI send data API: Part3(V143)
+ Triển khai hàm "SPI_SendData"

+ Lưu ý: 1. bạn nên thêm một vòng While để kiểm tra giới hạn len đc phép truyền nhận
  vd: nếu len có độ dài quá 1000 ký tự thì ko đc phép send

  2. Nếu trong quá trình kiểm tra "while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);"
  Có thể có trường hợp vì Flag_TXE = 0 nên ko thể thoát vòng lặp while. Vì vậy bạn cần có cơ chế số lần kiểm tra flag,
  nếu quá số lần kiểm tra flag mà chưa thoát đc while thì lập tức ngừng hàm send

*===Exercise: SPI Send Data(S40)
*==Exercise to Test SPI Send Data API(V145)
1. Test the SPI_SendData API to send the string "Hello World" and use the below configuration
 1. SPI-2 Master mode
 2.SCLK = max possible
 3. DFF = 0 and DFF = 1

+ Ta sẽ thiết lập board STM32 làm Master, cấu hình 2 chân MOSI và SCLK, hai chân MISO và NSS không sử dụng vì ko có
Slave nào cần điều khiển

*==Finding out microcontroller pins to communicatie over SPI2(V146)
+ Tìm chân vi điều khiển để giao tiếp qua SPI2
+ Tìm hiểu Datasheet_stm32f407x -> 3. Pinout and pin -> Table 9: Alternate function mapping
 ^Trong sở đồ này ta tìm các chân có thể hoạt động tính năng Alternate để cấu hình SPI2
 ^Qua bảng ta có thể có SPI2_SCLK là chân PB10, SPI2_NSS là chân PB12, SPI2_MISO là chân PB14 ,PC2...(ta có x chân
 có thể làm SPI2_MISO),SPI2_MOSI là chân PC3,PB15...
 ->Thông qua bảng ta có thể xác định có bao nhiêu chân có thể hoạt động để cấu hình SPI2

 ^Chọn chân pin để cấu hình SPI2: SPI2_MOSI là PB15, SPI2_MISO là PB14, SPI2_SCLK là PB13, SPI2_NSS là chân PB12

*==Exercise: Code Implemetation: Part1(V147)
+ Thiết lập hàm Alternate Function để chuyển đổi chân pin từ chức năng GPIO sang SPI2

*==Exercise: Code Implemetation: Part2(V148)
*==Exercise: Code Implemetation: Part3(V149)
+ Sau khi đã cấu hình cho SPI2 nhưng lúc này SPI2 vẫn sẽ chưa hoạt động mà cần phải kích hoạt thêm bit SPE[6] của SPI_CR1
 ^SPE: SPI Enable Với 0=Disable và 1=Enable
 ^Mặc định bit này sẽ bằng 0 và né sẽ tắt giao tiếp SPIx

*==Exercise: Testing(V150)
+ Nếu có lỗi xảy ra thì ta nên vừa Debug vừa kiểm tra các thanh ghi đã đc cấu hình liên quan đến chức năng
+ Trong SPI bạn có thể kiểm tra thanh ghi SR(Status Register) để xem cờ nào đang set thì đó có thể là lỗi liên quan
đền cờ đó đang xảy ra.
 ^Lỗi liên quan đến "MODF", khi cờ MODF đc set có thể reset lại bit MSTR(bit set Master Device) làm cho thiết bị trở
 thành Slave, vì thành Slave nên sẽ ko tạo ra SCLK dẫn đến ko thể gửi tín hiệu
 ^Cách giải quyết: mặc dù ko sử dụng SPI2_NSS nhưng vì nếu NSS là mức thấp sẽ kích hoạt biến MODF lên 1 dẫn đến lỗi
 Nên ta sẽ tiến hành cấu hình NSS pin nối với VDD, lên mức cao_high

*===Exercise: STM32 master and Arduino Slave Communication(S41)
*==Exercise : Communicating with Arduino Slave(V151)
+ Exercise: SPI Master(STM32) and SPI Slave(Arduino) communication.
When the button on the master is pressed, master should send string of data to the Arduino Slave connnected.
The data received by the Arduino will be displayed on the Arduino serial port.
 1.Use SPI Full duplex mode
 2.ST board will be in SPI master mode and Arduino will be configured for SPI Slave Mode
 3.Use DFF=0
 4.Use Hardware Slave management (SSM = 0)
 5. SCLK Speed = 2MHz, fclk = 16MHz
In this exercise master is not going to receive anything for the slave. So you may not configure the MISO pin
NOte: Slave does not know how many bytes of data master is going to send. So master first sends the number bytes
info which slave is going to receive next.

+ Bài tập: Giao tiếp SPI Master(STM32) và SPI Slave(Arduino).
Khi nhấn nút trên master, master sẽ gửi chuỗi dữ liệu đến Arduino Slave được kết nối.
Dữ liệu mà Arduino nhận được sẽ được hiển thị trên cổng nối tiếp Arduino.
  1.Sử dụng chế độ song công hoàn toàn SPI
  Bảng 2.ST sẽ ở chế độ SPI master và Arduino sẽ được cấu hình cho Chế độ SPI Slave
  3.Sử dụng DFF=0
  4.Sử dụng quản lý nô lệ phần cứng (SSM = 0)
  5. Tốc độ SCLK = 2 MHz, fclk = 16 MHz
Trong bài tập này, chủ nhân sẽ không nhận được bất cứ thứ gì cho nô lệ. Vì vậy bạn không thể cấu hình chân MISO
LƯU Ý: Slave không biết chủ sẽ gửi bao nhiêu byte dữ liệu. Vì vậy, trước tiên chủ nhân sẽ gửi số byte
thông tin mà nô lệ sẽ nhận được tiếp theo.

+ Sơ đồ khối kết nối: ST board <--SPI--> Arduino Board <--USB--> PC(Arduino Serial Monitor)

+ Step1: Connect Arduino and STM32 discovery board SPI pins as shown
 ^Sử dụng bộ "Logic level Converter- Bộ ổn định điện áp", vì nguồn cấp cho STM32(3.3V) và Arduino(5V) nên sẽ có sự
 chênh lệch điện áp giữa các pin STM32 và Arduino(Xem lại bài giải thích về sự chênh lệch điện áp giữa Master và Slave).
 ^Không đc phép kết nối trực tiếp từ STM32 -> Arduino do sự sai lệch về điện áp, dẫn đến việc truyền dữ liệu có thể bị hỏng
 ^Vì sẽ truyền từ STM->Arduino nên ta cần tăng điện áp từ 3.3V->5V và sử dụng bộ "Logic level Converter"
 ^Sơ đồ nối dây sử dụng Logic level Converter(LLC): ST board <--SPI, LLC--> Arduino Board (Video chi tiết 6p21s)

+Step2: Power your Arduino board and downdload SPI Slave sketch to Arduino
Sketch name: 001SPISlaveRXString.ino (Đã viết sẵn file lập trình phía Arduino)

*==Exercise: Coding Part1(V152)
+Sơ đồ:
											Logic Level Converter
								SCLK(PB13)  <----- A1 - B1 ------> (13)SCLK
	Master| (MSB)8bit SR(LSB)	MOSI(PB15)  <----- A0 - B0 ------> (11)MOSI	(MSB)8bit SR(LSB)	|Slave
	STM32						MISO(PB14)  <----- Not Use ------> (12)MISO						Arduino
								NSS(PB12)   <----- A2 - B2 ------> (10)NSS
								5V 			<----- VCC_B
								3V          <----- VCC_A
								GND         <----- GND

+Lưu ý: TH1: SSM=0, SPE=1 => NSS o/p=0(Low) (SSOE=1)
		TH2: SSM=0, SPE=0 => NSS o/p=1(High)(SSOE=0)
 -> Để send data thì ta cần cấu hình NSS theo trường hợp 1
 Có thể tham khảo thêm datasheet để hiểu phần cấu hình NSS

*==Exercise: Coding Part2(V153)
*==Exercise: Coding Part3(V154)
+ Tìm hiểu về flag "BSY" của thanh ghi SPI_SR
 ^Đây là flag(bit) cho ta biết SPI có đàn truyền tín hiệu hay không
 ^0: SPI not busy và 1: SPI is busy in communication or Tx buffer is not empty
*==Exercise: Testing(V155)

*===SPI Driver API: Receive Data(S42)
*==Implement of SPI data receive API: Part1(V156)
+ Sơ đồ hoạt đọng Receive Data: 	START
									|
						|-------> Len=0? ---Yes---> Exit from the function -> END
						|			|No
						|		Wait untill the RxBuffer is empty
						|			|
						|		DFF? ------ ==1 16bit-------> Read DR with 2 byte of Data And increment the buffer address--|
						|			|==0, 8bit																				|
						|		Read DR with 1 byte of Data And increment the buffer address								|
						|			|																					  Len--
						|<---------- Len--	<-------------------------------------------------------------------------------|

*==Implement of SPI data receive API: Part2(V157)
+ Code hàm SPI_ReceiveData()

*===Exercise: SPI Receive Data(S43)
*==Exercise: SPI command and respondse based communication(V158)
Đề: SPI Master(STM32) and SPI Slave(Arduino) command & repondse based communication.
When the button on the master is pressed, master sends a command to the slave and slave and slave responds as per the command
implementation
 1. Use SPI Fill-duplex mode
 2. ST board will be in SPI master mode and Arduino will be configured for SPI slave mode
 3. Use DFF = 0
 4. Use Hardware slave management(SSM=0)
 5. SCLK speed = 2MHz, f_CLK = 16Mhz

+ Step 1: Thiết lập Sơ đồ:
											Logic Level Converter
								SCLK(PB13)  <----- A1 - B1 ------> (13)SCLK
	Master| (MSB)8bit SR(LSB)	MOSI(PB15)  <----- A0 - B0 ------> (11)MOSI	(MSB)8bit SR(LSB)	|Slave
	STM32						MISO(PB14)  <----- A3 - B3 ------> (12)MISO						Arduino
								NSS(PB12)   <----- A2 - B2 ------> (10)NSS
								5V 			<----- VCC_B
								3V          <----- VCC_A
								GND         <----- GND

+ Step2: Arduino file: 002SPÍlaveCmdHandling.ino
+ Quy trình truyền nhận:
  Master														Slave
	|															 |
	|-------------Master sends a command------------------------>|
	|<------------Slave responds-----------------ACK or NACK Byte|		NACK(0x05) or ACK(0xF5)
	|															 |
	|--If ACK -------------------------------------------------->|
	|															 |
	|--If NACK ----> Display error message    	Take action according to command
	|											(Thực hiện hành động theo lệnh)
    |<----------Slave repondse for data read command-------------|

+ Commmand formats: <command_code(1)>	<arg1> 	<arg2>
 1) CMD_LED_CTRL <pin no> <Value> : cho phép bật tắt pin Arduino kỹ thuật số từ 0 đến 9
    <pin no> : digital pin number of the arduino board (0 to 9)(1 byte)
	<value> : 1 = ON , 0 = OFF (1 byte)
	Slave Action : control the digital pin ON or OFF
	Slave returns: Nothing

 2) CMD_SENSOR_READ <analog pin number> : cho phép đọc value tại chân pin Arduino analog két nối vs cảm biến
    <analog pin number> : analog pin number of the arduino board A0 to A5)(1 byte)
	Slave Action : Slave should red the analog value of the supplied pin
	Slave returns: 1 byte of the analog read value

 3) CMD_LED_READ <pin no>  : cho phép đọc value trạng thái pin Arduino kỹ thuật số từ 0 đến 9
    <pin no> : digital pin number of the arduino board (0 to 9)(1 byte)
	Slave Action : read the status of the supplied pin number
	Slave returns: 1 byte of the leds status, 1=ON, 0=OFF

 4) CMD_PRINT <len> <message> : gửi một số dữ liệu đến Arduino Slave
    <len> : 1 byte of length information of the message to follow
	<message> : message of 'len' bytes
	Slave Action : receive the message and display via serial port
	Slave returns: Nothing

 5) CMD_ID_READ : Arduino slave sẽ trả về mã ID của nó và gửi tới Master
    No argument for this commands
	Slave return: 10 bytes of id string

+ Cách thức hoạt động của Application
				Start
				 |
				Enter main()
				 |
				All Inits Function
				 |
			|-> Wait till button is pressed
			|	 |
			|	Execute CMD_LED_CTRL
			|	 |
			|	Wait till button is pressed
			|	 |
			|	Execute CMD_SENSOR_READ
			|	 |
			|	Wait till button is pressed
			|	 |
			|	Execute CMD_LED_READ
			|----|

*==Exercise: Coding Part1(159)
*==Exercise: Coding Part2(160)
*==Exercise: Coding Part3(161)
*/
