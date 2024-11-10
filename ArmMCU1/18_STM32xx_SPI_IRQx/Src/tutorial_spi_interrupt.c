/*
 * tutorial_spi_interrupt.c
 *
 *  Created on: Dec 4, 2023
 *      Author: Minh
 */
/**
*===SPI Interrupt(S44)
*==SPI peripheral interrupting the processor(V162)
+ During SPI communication an interrupt can be generated by the following event(Trong quá trình giao tiếp SPI,
sự kiện sau có thể tạo ra ngắt):
 .Transmit Tx Buffer ready to be loaded
 .Data received in Rx Buffer
 .Master mode fault(in single master case you must avoid this error happening)
 (Lỗi chế độ Master (trong trường hợp chỉ có 1 master, bạn phải tránh xảy ra lỗi này))
 .Overrun error(Lỗi tràn)
Interrupt can be enabled and disable separately(Ngắt có thể được kích hoạt và vô hiệu hóa riêng biệt)

+ SPI interrupt request
Interrupt event								Event flag			Eanble Control Bit
------------------------------------------------------------------------------------
Transmit Tx Buffer ready to be loaded		TXE					TXEIE
Data received in Rx Buffer					RXNE				RXNEIE
Master Mode fault event						MODF				ERRIE
Overrun error								OVR					ERRIE
CRC error									CRCERR				ERRIE
TI frame format error						FRE					ERRIE

 ^Nếu bạn muốn cấu hình ngắt-interrupt khi các "Event Flag" đc kích hoạt thì trước đó bạn phải cấu hình các bit tương
 ứng ở bên cột "Eanble Control Bit" nằm bên trong thanh ghi Data Register

+ SPI Interrupting the Processor
 ^Sơ đồ:
	SPI_1-------->|------|
	SPI_2-------->| NVIC | <---> Processor Core(ARM CortexM4)
	SPI_3-------->|------|

 ^Giải thích:
  ~Không giống với GPIO Interrupt(các GPIO pin phải thông qua EXTI Block và EXTIx(interrupt) để kết nối NVIC Register)
  thì các SPI_x Interrupt sẽ đc phép kết nối trực tiếp với NVIC Register(Vd: SPI_1 kết nối trực tiếp NVIC Register)
  Đây đc gọi là đường ngắt interrupt trực tiếp từ Periperal. Nếu bạn có 5 SPI Interrupt thì có thể kết nối trực tiếp
  với 5 IRQx của NVIC
  -> Cần định nghĩa các SPI_x nối với các IRQx của NVIC Register

+ Exercise1: Complete SPI IRQ number definition macros in MCU specific header file
 ^Hoàn thành các macro định nghĩa số SPI IRQ trong tệp tiêu đề cụ thể của MCU
 ^Cần dựa vào datasheet Vector Table để có thể thiết lập define này
 ^Nghĩa là cần xác định các IRQx có thể sử dụng trên MCU, vì khi làm GPIO Interrupt đã xác định rồi nên qua phần SPI
 không cần làm(stm32f407xx.h:  18_IRQ(Interrupt request) Number of SRTM32F407x MCU(V111))

+ Exercise2: Complete the SPI IRQ Configuaration APIs Implemetation(reuse code from GPIO Driver)
 ^Hoàn thành quá trình định nghĩa API(các hàm) cấu hình SPI IRQ (sử dụng lại mã từ Trình điều khiển GPIO)
 ^Đã đc code trước đó:  stm32f407xx_driver_spi.h -> phần IRQ Configuration and ISR Handling for SPI

 ^Triển khai code các hàm:
  ~Hàm SPI_IRQInterruptConfig() có nội dung giống hàm SPOI_IRQInterruptConfig()

*===SPI Interrupt mdoe APIs(S45)
*==SPI Interrupt mode API implementation and changes to handle structure(V163)
*==SPI Send Data with Interrupt API Implemetation(V164)
+ Code hàm: uint8_t SPI_SendData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)

*==SPI Receive Data with Interrupt API Implemetation(V165)
+ Code hàm: uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)

*===SPI Driver API: IRQ Hanling Function(S46)
*==SPI: Handling of Interrupt(V166)
+ Handling SPI Interrupt in the Code:
				Enter ISR(Cho phép kích hoạt ngắt)
									|
		Understand which event caused interrupt to Trigger(check SR)
			|						|							|
 Interrupt is due to 		Interrupt is due to 			Interrupt is due to(bởi vì, do)
 setting of RXNE flag		setting of TXE flag				Settings of ERROR flag
			|						|							|
 Handle RXNE event			Handle TXE flag					Handle ERROR

+ Handling TXE Interrupt
							Handle TXE Interrupt
									|
				|----------8bit or 16bit mode?----------|
			8bit|										|16bit
				|										|
		Write one byte to SPI 					Write 2 byte to SPI Data
		Data register(DR)						register(DR)
				|										|
			Len-- <----------------------------------> 	Len-=2
									|
								Len=0? ---------No(Tranmission Not Over)--->|
			   Yes(Tranmission over)|										|
							Close the SPI TX					Wait till another TXE Interrupt

*==SPI IRQ Hangler implementation: Part1(V167)
+ Code hàm void SPI_IRQHangling(SPI_Handle_t *pHandle)

+ Lỗi Overrun flag(OVR): là tình trạng tràn data khi master hoặc slave hoàn thành việc nhận khung dữ liệu tiếp
theo trong khi thao tác đọc khung data trước đó từ bộ đệm Rx buffer chưa hoàn thành(lưu ý cờ RXNE đã đc set)
 ^Dẫn đến việc SPI peripheral sẽ loại bỏ các dữ liệu đc gửi đến gây tràn đó, điều này làm mất mát dữ liệu
 ^Lúc này RxBuffer sẽ ko nhận dữ liệu mới, việc truyền dữ liệu tiếp theo sẽ bị dừng hay thất bại
 ^Để tiếp tục truyền thì cần xóa cờ OVR bit trong SPI_DR register

*==SPI IRQ Hangler implementation: Part2(V168)
+ Từ khóa "static" khi khai báo hàm
 ^Mục đich là thông báo các hàm "static" là các hàm phụ nhằm trợ giúp cho các hàm chính trong file đó, và nếu
 app (hàm main) cố gắng gọi nó để sử dụng thì trình biên dịch sẽ báo lỗi

*==SPI IRQ Hangler implementation: Part3(V169)
+(void)temp; : mục đích của dòng này là cho trình biên dịch thấy là biến này có sử dụng

*== Exercise(V170)
Đề: STm32 discovery board (Master) receive the message from the Arduino board(Slave) over SPI
1) User enter the message using Arduino serial monitor terminal
2) Arduino board notifies the STM32 board about message availability
3) STM32 device reads and prints the message
						| 	<------SPI(SCLK,MISO,MOSI,NSS)--->	|
Master(STM32): Receive	| PD6	<---SPI interrupt------> Pin8 	|	Tranmitter: Slave(Arduino)
																				(use "Serial Monitor")

*===Common problem in SPI(S47)
*==Common problem in SPI and Debugging Tips(V171)
+ Cross check these settings(Kiểm tra chéo các cài đặt)
 ^Bit chế độ Master phải được "Enable" trong thanh ghi cấu hình nếu bạn đang định cấu hình thiết bị ngoại vi là Master
 ^Bit "SPI peripheral enable" phải đc enabled
 ^Bit "SPI peripheral clock" phải đc enable. Theo mặc định, "clock" hầu hết cho tất cả các peripheral trong vđk sẽ đc
 "disable" để tiết kiệm năng lượng.

+ Common Problems in SPI and Debugging Tips - Các vấn đề thường gặp trong SPI và mẹo gỡ lỗi
 ^Case1: Master can not able to produce clock and data - Master không thể tạo ra xung clock và dữ liệu
  ~Trạng thái: Khi đặt byte data vào TxBuffer việc truyền ko xảy ra
  ~Nguyên nhân 1: Có thể Master không kích xung Clock
   ->Giải quyết: Xem lại cài đặt cấu hình board Master

  ~Nguyên nhân 2: GPIO I/O pin mà bạn sử dụng cho CLock và chức năng MOSI ko đc cấu hình "Alternate" đúng cách
   ->Giải quyết: Kiểm tra lại "GPIO Configuaration register", để kiểm tra chân đó đã đc cấu hình đúng cho chức năng
   Alternate Pin hay chưa.

  ~Nguyên nhân 3: Ghi đè lên cấu hình(Configuaration Overriding). Mặc dù bạn đã cấu hình đúng các thanh ghi cần thiết,
  nhưng khi bạn tham gia vào các dự án lơn thì có khả năng các thanh ghi đó bị ghi đè bởi mã code cấu hình khác.
   -> Giải quyết: Loại bỏ tất cả nội dung cấu hình register cần thiết ngay trước khi bạn bắt đầu truyền, đông thời tạo
   ra một trình tự code thống nhất giữa các thành viên trong dự án

 ^Cas2: Master is sending data but Slave is not receiving data!
  ~Nguyên nhân 1: KO kéo pin "Slave Select" xuống GND trước khi gửi dữ liệu
   ->Giải quyết: Thăm dò thông qua bộ phân tích logic "Logic Analyzer" để xác nhận đường chọn phụ thực sự ở mức thấp
   trong quá trình truyền dữ liệu

  ~Nguyên nhân 2: Không cấu hình đúng I/O pin Alternate cho chức năng SPI Periperal
   -> Giải quyết: Kiểm tra lại cấu hình các thanh ghi

  ~Nguyên nhân 3: Trong trường hợp Slave đang trong quá trình "Receive Data" dựa trên Interrupt thì có khả năng là
  chức năng SPI IRQ Number không đc NVIC kích hoạt.
   -> Kiểm tra lại cấu hình thanh ghi "NVIC Interrupt Mask" với bit SPI ngắt có đc kích hoạt hay không

 ^Case3: SPI Interrupt are not gettings triggerd
  ~Nguyên nhân 1: Không kích hoạt các bit "TXE hoặc RXNE interrupt" trong cấu hình SPI register
   ->Kiểm tra lại các cấu hình SPI Register xem đã kích hoạt các bit TXEIE và RXNEIE hay chưa.

  ~Nguyên nhân 2: Không kích hoạt đc chức năng SPI IRQ Number trong NVIC register
   -> Kiểm tra lại cấu hình thanh ghi "NVIC Interrupt Mask" với bit SPI ngắt có đc kích hoạt hay không

 ^Case4: Master is producing righ data but Slave is receiving the different data
  ~Trạng thái: Master đang tạo ra dữ liệu đúng nhưng Slave đang nhận được dữ liệu khác
  ~Nguyên nhân : Sử dụng dây dài trong truyền thông tần số cao(gây ra vấn đề nghiêm trọng như là có độ trễ tín hiệu)
  hoặc sử dụng tần số quá cao(>4MHz)
   ->Gải quyết: Sử dụng các dây có độ dài ngắn và sử dụng ở tần số khoảng 500KHz
*/
