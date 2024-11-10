/**
*===SPI Introduce and Bus Details - Giới thiệu về SPI và Bus(S31)
*==Introduce to SPI Bus(V121)
+ SPI là Serial Peripheral Interface(Giao diện ngoại vi nối tiếp)
+ Là một giao thức đc sử dụng để kết nối giữa Master và Slaves trong hệ thống nhúng
+ Sơ đồ: Giả sử ta có một Master kết nối Slave
								SPI Bus
				SCLK  ------ Serial Clock       ------>	SCLK
	Master|		MOSI  ----- Master Out Slave In ------> MOSI	|Slave
				MISO  <-----Master In Slave Out ------- MISO
				GPIO1 ----- Slave Control       ------> ss

 ^Khối Slave có thể là các cảm biến(Sensor), chip EEFROM, SD-card, DISPLAY, kết nối không dây(Bluetooth,Wifi),...
 ^Nếu ta muốn thiết bị chính Master giao tiếp với thiết bị phụ Slave thì nghĩa là Master đang muốn gửi mốt số tín
 hiệu đến slave hoặc là muốn lấy data từ Slave. SPI chính là một trong số giao thức kết nối đc sử dụng cho việc này
 ^Ngoài SPI ta cũng có một số giao thưc kết nối khác như Can, I2C, Ethernet, USB,RSS232, RS485
 ^Trong bo mạch sẽ có một số chân Pin và SPI Bus đc thiết kế cho giao tiếp chuẩn SPI

+ Tìm hiểu về pin của SPI Bus
 ^Bốn I/O pins đc dành riêng cho giao tiếp SPI với các thiết bị bên ngoài -> Khi dùng SPI cần sử dụng 4 pin
 ^"MISO pin" : là "Master In/ Slave Out" Data. Đây là chân dữ liệu(Data Pin). Chân này đc sử dụng để truyền dữ liệu
 ở Slave Mode và nhận dữ liệu khi ở Master Mode. Nghĩa là Slave sẽ gửi dữ liệu và Master nhận dữ liệu thông qua MISO.

 ^"MOSI pin" : là "Master Out/ Slave In" Data. Đây là chân dữ liệu(Data Pin). Chân này đc sử dụng để truyền dữ liệu
 ở Master Mode và nhận dữ liệu khi ở Slave Mode. Nghĩa là Master sẽ gửi dữ liệu và Slave nhận dữ liệu thông qua MOSI.

 ^"SCK pin" : là "Serial Clock" pin. Chân đầu ra Serial Clock cho SPI Master và Chân đầu vào cho SPI Slave
 Trong gioa thức SPI thì Master là bên tạo ra SCLK,bên Slave chỉ nhận Xung Clock, vì vậy Slave ko có quyền kiểm soát
 trong việc tạo ra Serial Clock. Và ko có xung Clock thì việc giao tiếp SPI cũng ko thành công.
 Mẹo: Khi gặp lỗi liên quan đén SPI ẩn, thì bạn có thể kiểm tra Master có tạo ra xung Clock hay không

 ^"ss hoặc NSS" : "Slave Select" pin. Tùy thuộc vào cài đặt SPI và NSS(ss), chân này có thể được sử dụng để chọn một
 thiết bị Slave riêng lẻ để liên lạc, giao tiếp. Nghĩa là khi bạn có nhiều Slave riêng lẻ khác nhau thì bạn có quyền
 cài đặt là Master có muốn kết nối với Slave đó hay không qua cài đặt chân NSS này.
 Ngoài ra, Chân Slave Select (SS) giúp giảm nhiễu trong hệ thống bằng cách ngăn chặn các Slave không được chọn nhận
 hoặc gửi dữ liệu. Điều này có thể giúp cải thiện chất lượng dữ liệu được truyền.

*==SPI comparison with other protocols - So sánh SPI với các giao thức khác(V122)
+ Bảng so sánh các giao thức truyền dữ liệu

Protocol 		Type Max 			Distance (feet)		Max Speed(bps) 			Typical usage
USB 3.0 		dual simplex 		9(typical) 				5G			 	Mass storage, Video
				serial 				(upto 49 with
									5 hubs)

USB 2.0			half duplex			16(98 ft. with		1.5M,12M,			Keyboard, Mouse,Drive
									5 hubs)				480M				Speaker,Printer,Camera

Ethernet		serial				1600				10G					Netwwork Comunication

I2C				synchronous			18					3.4M in High		Microcontroller communication
				serial									Speed mode

RS232			asynchronous		50-100				20k					Modern, Mouse,instrumentation
				serial

RS485			asynchronous		4000				10M					Data acquisition and Control
				serial														system SPI

SPI 			synchronous			10					fPCLK/2				Sensor Data, Display, Leđ Flash
				serial

 ^Qua bảng so sánh ta thấy thì khoảng cách truyển của SPI khá ngắn chỉ khoảng 10 feet(3 mét). và tốc độ truyền dữ
 liệu sẽ phụ thuộc vòa tốc độ truyền xung ngoại vi fPCLK/2 của SPI Bus
 ^Vd: Nếu thiết bị ngoại vi SPI của bộ vđk của bạn có xung nhip ngoại vi là 50MHz, thì tốc độ tối đa cho xung Clock
 của SPI là 25mbps
 => SPI hoạt động ở "khoảng cách ngắn" nhưng bạn có thể đạt đc "tốc độ truyền dữ liệu cao hơn, tốc độ cao hơn"

+ Lưu ý:
 1.Hiện tại hầu hết trường hợp cần truyền tốc độ cao trong khoảng cách ngắn thì lập trình viên sẽ ưu tiên sử
 dụng I2C hơn là SPI. Nguyên nhân là vì I2C cung cấp nhiều tính năng hơn và khoảng cách/tốc độ sẽ khả thi hơn trong
 thực tế khi cần truyền dũ liệu.

 2. SPI sẽ đc sử trong trong một bo mạch tổng quát(kết nối các Sensor, Display, EEFROM,..trên cùng một bo mạch với
 Processor) dùng để tester, phát triển ứng dụng test.

 3. Trong ô tô hoặc các máy công nghiệp,có độ bao phủ lớn, khoảng cách lớn và bạn cần giao tiếp các bộ phần kết nối
 với nhau thì sẽ ưu tiên sử dụng các chuẩn kêt nối khác như CAN,Ethernet,RS232,RS485 hơn là sử dụng SPI,I2C

*==Importance of SPI Slave Select pin(V123)
+ Trong chuẩn giao tiếp SPI, chân Slave Select (SS) được sử dụng để chọn một thiết bị Slave cụ thể bởi Master.
Nếu Master kéo đường SS của một Slave nào đó xuống mức thấp(có thể là kéo về GND) thì việc giao tiếp sẽ xảy ra
giữa Master và Slave đó.
+ Ở trạng thái idle(nhàn rỗi), không truyền tải, dòng slave select được giữ ở mức điện áp cao. Nhiều chân SS / CS
có thể có sẵn trên thiết bị master cho phép đấu dây song song nhiều slave.

+ Cụ thể, tác dụng của chân Slave Selcect Pin trong chuẩn giao tiếp SPI của Hệ thống nhúng là:
 ^Chọn một Slave cụ thể để giao tiếp
 ^Cho phép Master và Slave đồng bộ hóa với nhau
 ^Giúp giảm nhiễu trong hệ thống

+ Trong giao tiếp SPI, Master là thiết bị chịu trách nhiệm điều khiển quá trình truyền dữ liệu. Master tạo ra xung clock
trên chân SCK (Serial Clock) để đồng bộ hóa truyền dữ liệu giữa Master và Slave. Master cũng chịu trách nhiệm kích hoạt
chân Slave Select (SS) của từng thiết bị slave để chọn thiết bị nào sẽ giao tiếp.
+ Khi Master kéo chân SS của một Slave xuống mức thấp, Slave đó sẽ biết rằng mình được chọn để giao tiếp. Slave đó sẽ
đồng bộ hóa với xung clock của Master và bắt đầu nhận hoặc gửi dữ liệu.
+ Khi Master muốn ngừng giao tiếp với một Slave, nó sẽ kéo chân SS của Slave đó lên mức cao. Slave đó sẽ biết rằng mình
không được chọn để giao tiếp nữa và sẽ ngừng nhận hoặc gửi dữ liệu.
+ Chân Slave Select (SS) giúp giảm nhiễu trong hệ thống bằng cách ngăn chặn các Slave không được chọn nhận hoặc gửi dữ liệu.
Điều này có thể giúp cải thiện chất lượng dữ liệu được truyền.
+ Trong một hệ thống nhúng, có thể có nhiều Slave được kết nối với một Master. Trong trường hợp này, chân Slave Select (SS)
là cần thiết để chọn một Slave cụ thể để giao tiếp.

*==SPI Minimum Bus Configuaration(V124)
+ SPI Bus cho phép giao tiếp giữa một Master device(Thiết bị chính) và một hoặc nhiều Slave device(thiết bị phụ).
+ Trong một số ứng dụng, SPI Bus chỉ bao gồm 2 dây, một dây là Clock Signal và một dây truyền dữ liệu đồn bộ.
Các tín hiệu khác có thể đc thêm vào tùy thuộc vào việc trao đổi dữ liệu giữa SPI node và quản lý tín hiệu Slave Selcect.

+ Giả sử ta có một Master kết nối Slave. Slave luôn chỉ nhận dữ liệu và ko gửi dữ liệu cho Master
								SPI Bus
				SCLK  ------ Serial Clock       ------>	SCLK
	Master|		MOSI  ----- Master Out Slave In ------> MOSI	|Slave
				MISO   									MISO
				GPIO1 							GND <---ss
 ^Trong trường hợp này, bạn sẽ kết nối chân ss về GND để có mức 0, nhằm cho phép Master chọn Slave đó là kết nối.
 ^Sau đó bạn sẽ sử dụng 2 dây là SCLK và MOSI để truyền dữ liệu từ Master -> Slave
 -> Tùy vào ứng dụng thực tế mà bạn sẽ lựa chọn việc truyền SPI sử dụng bao nhiêu chân

*==SPI Bus Behind the Scene Data Comunication principle - Nguyên tắc truyền dữ liệu hậu trường của Bus SPI(V125)
+ Giả sử ta có một Master kết nối Slave. Slave luôn chỉ nhận dữ liệu và ko gửi dữ liệu cho Master
+ Cạnh xung Clock:  Cạnh xuống- cạnh lên
  Master: 			(MSB)Shift Register(LSB)	----MOSI---->   (MSSB)Shift Register (LSB)		: Slave
												<----MISO----
												----PCLK---->
  Shift Register là một thanh ghi 8bit data:
					(MSB)A7A6A5A4A3A2A1A0(LSB) 					(MSB)B7B6B5B4B3B2B1B0(LSB)
  MOSI: A0(LSB) -nối-> (MSB)B7
  MISO: B0(LSB) -nối-> (MSB)A7

 ^Mục tiêu là gửi dữ liệu đang lưu trong thanh ghi của Master tới thanh ghi của Slave và ngược lại
 ^Hoạt động: Data lưu trong các thanh ghi sẽ thay đổi theo chu kỳ CLock
  ~Ở chu kỳ xung CLK đầu tiên, A0(LSB) sẽ truyền qua MOSI và chiếm vị trí B7, đồng thời B0 sẽ bị đẩy khỏi thanh ghi
  và truyền tới thanh ghi của Master ở vị trí trước A7
  Thanh ghi bên Slave lúc này:  (MSB)A0B7B6B5B4B3B2B1(LSB)
  Thanh ghi bên Master lúc này: (MSB)B0A7A6A5A4A3A2A1(LSB)

  ~Tuần tự mỗi chu kỳ xung Clock cho đến khi hoàn thành việc truyền dữ liều từ Master sang Slave và ngược lại
  Thanh ghi bên Slave cuối cùng:  (MSB)A7A6A5A4A3A2A1A0(LSB)
  Thanh ghi bên Master cuối cùng: (MSB)B7B6B5B4B3B2B1B0(LSB)
  ->Cần 8 chu kỳ xung CLK cho việc truyền data này
  Lưu ý;  khi kết nối MISO đc thiết lập thì chắc chắn bạn sẽ nhận đc data từ Slave -> Master
*/

/**
*===SPI Bus configuration and functional block diagram(S32)
*==SPI Bus configuration discussion(thảo luận): full-duplex, half-duplex, simplex(V126)
+ SPI cho phép bộ vdk giao tiếp bằng các cấu hình khác nhau, tùy thuộc vào thiết bị, mục tiêu và yêu cầu ứng dụng.
+ Full-duplex communication(giao tiếp song công hoàn toàn):
 ^Mặc định, SPI là giao tiếp song công hoàn toàn. Bởi vì bạn sẽ có 2 dòng dữ liệu chuyên dụng là MISO và MOSI.
 Nghĩa là khi data đc truyền đi(MOSI) thì bạn cũng có thể nhận đc dữ liệu(MISO).
 ^Trong Full-duplex, các thanh ghi dịch chuyển(Shift Register) của Master và Slave đc liên kết bằng hai đường một
 chiều giữa các pin là MOSI và MISO.
 ^Trong quá trình giao tiếp, data đc dịch chuyển đồng bộ dựa trên các cạnh xung Clock do Master cung cấp.
 ^Master truyền data gửi đến Slave qua MOSI line và nhận data từ Slave bằng MISO line

+ Half-duplex communication(Giao tiếp song công bán phần):
								SPI Bus
				SCLK  ------ Serial Clock       ------>	SCLK
	Master|		MOSI  ----- -------Rx			------> MOSI	|Slave
				MISO  <---- 		|---------  ------- MISO
				GPIO1 -----  Slave Control      -------> ss
 ^Bằng cách sử dụng giao tiếp kết nối này thì bạn có thể tiết kiệm đc một số pin cần kết nối.
 ^Trong cấu hình này, một đường kết nối duy nhất đc sử dụng để liên kết các Shift Register giữa Master và Slave.
 ^Data sẽ đc chuyển đồng bộ dựa trên các cạnh xung Clock do Master cung cấp.
 ^Vì chỉ có một đường dữ liệu để sử dụng nên việc truyền dữ liệu từ Master đến Slave và ngược lại đều sử dụng
 đường line kết nối này. Nghãi là đường này vừa là MOSI line và MISO line. Luu ý: ở chính giữa đường line truyền
 data này sẽ PHẢI đặt một con điện trở Rx(~1KOhm).
 ^Để sử dụng giao tiếp này bạn phải cài đặt cấu hình đâu là Tranmitter(máy phát) đâu là Receptor(máy nhận)
  ~Nếu truyền Master->Slave thì cài đặt Master là Tranmitter và Slave là Receptor
  ~Nếu truyền Master<-Slave thì cài đặt Master là Receptor và Slave là Tranmitter
 ^Bạn phải check Datasheet để xác định bộ vđk khiển có cho phép sử dụng Half-duplex hay không

+ Simplex communication(Giao tiếp đơn công)
 ^Được chia làm hai loại là Tranmit-Only(Chỉ truyền) và Receiver-Only(Chỉ nhận)
 ^Vd: Cấu hình Master ở Tranmit-Only và cấu hình Slave ở chế độ Receiver-Only. Nghĩa là lúc này chỉ có đường MOSI
 mà KHÔNG có MISO line, Master sẽ ko nhận bất cứ data nào từ Slave
 ^Khi ở cấu hình Simplex, bạn có thể ngắt kết nối các line ko cần thiết
 ^Các chân pin ko sử dụng kết nối bạn có thể cấu hình thành các "Alternate Pin", vd như thành GPIO pin để mở rộng
 chức năng cho hệ thống nhúng đang lập trình trong trường hợp thiếu GPIO Pin.

*==SPI function Block Diagram explanation(V127)
+ Tìm hiểu về sơ đồ khối SPI
+ Tài liệu Datasheet: RM407 -> 28. Serial peripheral interface (SPI) -> Figure 246. SPI block diagram
+ Thanh ghi "Shift Register" của stm32f407x hỗ trợ lến đến 16bit
+ SPI Bus sẽ đc kết nối với Processor thông qua APBx Bus
  Ta có data line nhận : MISO -> Shift Register -> Rx Buffer -> APBx Bus -> Processor
  Ta có data line gửi :  Processor -> APBx Bus -> Tx Buffer -> Shift Register -> MOSI
+ SCK sẽ đc quản lý bởi khối "Baud rate generator"
+ Bộ điều khiển SS và SPI Register(cấu hình SPI Register) sẽ nằm xung quanh và kết nối với khối "Communication
Control"
*/

/**
*===STM32 NSS pin Setting and Management(Quản lý)(S33)
*==NSS settings in STM32 Master and Slave Modes(V128)
+ Slave Device: Khi thiết bị ở chế độ Slave Mode, NSS hoạt động như là một "standard chip select input - chân đầu vào chọn
chip tiêu chuẩn" và cho phép Slave Device đó giao tiếp với Master.
+ Master Device: NSS pin có thể đc sử dụng làm Output  hoặc Input.
 ~Với vai trò Input thì nó có thể ngăn chặn xung đột giữa các "Multi-Master" Bus, nghĩa là có thể thiết lập một chế độc
 điều khiển Đa Master khi sử dụng chuẩn giao tiếp SPI.
 ~Với vai trò Output thì có thể điều khiển tín hiệu "Slave Select" của một Slave đc chọn.

+ Sơ đồ: Giả sử ta có một Master(STM32) kết nối Slave(STM32) , SR(Shift Register)
											SPI Bus
								SCLK  ------ Serial Clock       ------>	SCLK
	Master| (MSB)8bit SR(LSB)	MOSI  ----- Master Out Slave In ------> MOSI	(MSB)8bit SR(LSB)	|Slave
	STM32						MISO  <-----Master In Slave Out ------- MISO						STM32
								NSS   <----- Slave Control      ------> NSS

 ^Trong trường hợp chỉ có một Master một Slave thì ko cần thiết kết nối hai pin NSS
 ^Nếu bạn vẫn muốn kết nói hai pin NSS với nhau thì NSS pin của Slave phải cài đặt làm Input(chân Input này làm nhiệm
 vụ Chip Select Input) và từ phía Master thì chân NSS phải đc cấu hình là Output. Nếu lúc này cài đặt Master là một
 Input thì sẽ xảy ra vấn đề xung đột "Multi-Master" Bus gây lỗi trong hệ thống.

*==STM32 SPI of 2 Types Hardware and Software Slave Management(V129)
+ Sơ đồ: Giả sử ta có một Master(STM32) kết nối Slave(STM32) , SR(Shift Register)
											SPI Bus
								SCLK  ------ Serial Clock       ------>	SCLK
	Master| (MSB)8bit SR(LSB)	MOSI  ----- Master Out Slave In ------> MOSI	(MSB)8bit SR(LSB)	|Slave
	STM32						MISO  <-----Master In Slave Out ------- MISO						STM32
								NSS   <----- Slave Control      ------> NSS
 ^Như đã biết trong trường hợp một Master một Slave thì ko cần nối hai chân NSS, và nếu muốn kết nối Master với Slave thì
 ta sẽ nối chân NSS của Slave xuống GND.

 Software: cần cấu hình chân NSS của Slave mà ko cần cấu hình chân NSS của Master
 ^Trường hợp kết nối hai chân và quản lý chân NSS bằng phần mềm thì Thanh ghi cấu hình cho NSS pin là SPIx_CR1 Register
 ^Vị trí bit cần cấu hình trong thanh ghi là SSM bit viết tắt của "Software NSS Management(SSM=1)"
  ~Với SSM=1 thì cho phép quản lý cấu hình Slave là Input bằng phần mềm

 ^Vị trí bit tiếp theo cần cấu hình trong thanh ghi là SSI bit viết tắt của "Software NSS Internal(SSI=0)"
  ~Với SSI=0 thì kết nối chân NSS của Slave với GND by Software (cho phép slave kết nối giao tiếp với Master)
  ~Với SSI=1 thì kết nối chân NSS của Slave với VDD by Software (ko cho slave kết nối giao tiếp với Master)

 Hardware: cần cấu hình chân NSS của Slave và chân NSS của Master
 ^Trường hợp kết nối hai chân và quản lý chân NSS bằng phần cứng thì Thanh ghi cấu hình cho NSS pin là SPIx_CR1 Register
 ^Vị trí bit cần cấu hình trong thanh ghi là SSM bit viết tắt của "Software NSS Management(SSM=0)"
  ~Với SSM=0(cả hai bên Master và Slave) thì đang cấu hình đường kết nối bằng phần cứng
 ^Sau đó cần set NSS của Master là Output. Và cấu hình chân NSS của Slave ở mức thấp.

 ^Điểm khác nhau giữa 2 kiểu kết nối Software và Hardware là khi nối kiểu Software thì chỉ cần cấu hình một chân NSS của
 bên Slave, còn khi kết nối kiểu Hardware thì cần cấu hình chân NSS của cả Master và Slave.

+ Trong trường hợp một Master nhưng có nhiều Slave thì bạn ko thể sử dụng kiểu "Software Slave" mà bạn phải sử dụng kiểu
"Hardware Slave" để quản lý các Slave. Vì lúc này bạn điều khiển, kết nối các Slave_x bằng cách sử dụng các IOx khác nhau
mà không sử dụng chân NSS của Master

Vd: bạn có chân IO1 kết nối với NSS của Slave1, chân IO2 kết nối với NSS của Slave2 , chân IO3 kết nối với NSS của Slave3
 ^Master: Khi muốn kết nối với Slave2 thì ta sẽ cấu hình cho line IO2 xuống Low và hai line còn lại lên mức High
 ^Slave: ta sẽ cấu hình cho chân NSS của Slave2 xuống mức thấp (Low -> GND)

 Lưu ý: trong trường hợp này ko sử dụng chân NSS của Master, vì thế ta nên kết nối chân NSS này với VDD để tránh một số lỗi.
 hoặc bạn có thể thiết lập nó là tính năng "Alternate GPIO". Tuy nhiên tốt nhất vẫn là nối VDD cho chân NSS này
*/

/**
*===SPI Comunication format - Định dạng truyền thông SPI(S34)
*==SPI CPOL và CPHA discussion(Thảo luận)(V130)
+ SPI Comunication format gồm 3 loại: 	SCLK PHASE(CPHA), SCLK POLARITY(CPOL), Data frame format(DFF)
+ Trong giao tiếp SPI. việc hoạt động nhận(receive) và truyền(transmit) data đc thực hiện đồng thời
+ The Serial Clock - Đồng hồ nối tiếp(SCK) đồng bộ hóa việc dịch chuyển và lấy mẫu thông tin trên dòng dữ liệu
+  Comunication format phụ thuộc vào "Clock Phase","Clock Polarity","Data frame format". Để có thể giao tiếp
với nhau thì Master và Slave phải tuân theo cùng một định dạng giao tiếp "Comunication format". Nếu không việc
truyền dữ liệu từ Master sẽ ko thành công, và Slave ko đọc đc dữ liệu hợp lệ và ngược lại.
+ Data format có thể là định dạng dữ liệu 16bit hoặc 8bit(Mặc định ban đầu là 8bit)

+ CPOL(Clock Polarity - Cực đồng hồ)
 ^The CPOL bit điều khiển kiểm soát giá trị trạng thái rảnh(idle state) của đồng hồ khi không có dữ liệu nào được truyền
 ^Nếu CPOL đc reset, the SCLK pin sẽ ở trạng thái "low-level idle state"
  Nếu CPOL đc set, the SCLK pin sẽ ở trạng thái "high-level idle state"
 ^Trong thanh ghi điều khiển SPI sẽ có một bit gọi là CPOL bit và bạn có thể đặt bit đó lên HIGH (1) hoặc LOW(0)
  ~Trạng thái xung Clock CPOL = 0: 				__|--|__|--|__|--|__|--|__
  ~Trạng thái xung Clock CPOL = 1(idle state):  __    __    __    __    __
												  |__|  |__|  |__|  |__|

+ CPHA(Clock Phase - Pha đồng hồ)
 ^CPHA bit điều khiển, kiểm soát cạnh xung Clock nào của SCLK (1st) hoặc (2nd), và data sẽ đc lấy mẫu bởi Slave.
  ~SCLK (1st): nửa chu kỳ đầu xung clock , SCLK (2nd) là nửa chu kỳ sau của xung clock.
  ~CPHA = 0: SCLK (1st) và CPHA = 1: SCLK (2nd)

+ Sự kết hợp giữa các bit CPOL và CHPA sẽ chọn cạnh xung clock ^ thời điểm chu kỳ truyền-nhận dữ liệu. Nghĩa là khi có cạnh
 xung xuống(CPOL=1) hoặc cạnh xung lên(CPOL=0) xảy ra thì trong khoảng thời gian nửa đầu chu kỳ xung CPHA = 0 hoặc là nửa
 sau chu kỳ CPHA = 1 thì có việc "Data Tranmition" hoặc "Data Receiver" đc thực hiện. (xem video có sơ đồ để hiểu rõ)

+ Lưu ý: Việc lựa chọn CPHA và CPOL phải phụ thuộc vào các yêu cầu và thiết kế ứng dụng của bạn
 ^Vd: một số ứng dụng đòi hỏi xung CLK phải ở mức High trong trạng thái hoạt động thì bạn nên chọn CPOL=1
 ^Bạn cũng có thể chọn mặc định CPHA=0 và CPOL=0 trong các ứng dụng thông thường.

+ Diferent SPI Mode: có 4 chế độ SPI
	Mode	CPOL	CPHA
	0		0		0
	1		0		1
	2		1		0
	3		1		1
 ^Nếu CPHA=1 thì thì việc lấy mẫu sẽ đc thực hiện ở nửa sau chu kỳ xung CLK
 ^Nếu CPHA=0 thì thì việc lấy mẫu sẽ đc thực hiện ở nửa đầu chu kỳ xung CLK
 ^Việc lấy mẫu là việc thực hiện MOSI và MISO giữa Master và Slave

*==SPI CPOL and CPHA Waveform example(V131)
+ Video cho xem một số vd về việc lựa chọn xung SCL trong giao tiếp SPI
+ Việc test giao tiếp sẽ giúp ta xác định đc thời gian cần truyền hoặc nhận data, độ dài của dây, nhiễu mỗi trường, từ đó thay
 đổi hoặc tối ưu phương pháp thực hiện giao tiếp để đáp ứng như cầu của ứng dụng muốn làm.

 Vấn đề: giao tiếp SPI thường dùng tần số lớn nhưng độ dài dây dẫn cần ngắn(3mét)
 Nguyên nhân là khi có tần số lớn thì cho phép chu kỳ xung sẽ lớn, lúc này thì phía Slave sẽ có thời gian cho việc lấy mẫu data,
 việc chu kỳ xung ngắn có thể làm mất mát dữ liệu hoặc đọc sai data khi Slave chưa lấy mẫu xong. Tuy nhiên việc tăng tần số đồng
 nghĩa với việc cần tăng năng lượng sử dụng cho việc truyền-nhận nên việc sử dụng dây dẫn dài và nhiều môi trường sẽ làm cho việc
 hao hụt đường dây xảy ra từ đó việc truyền data cũng sẽ ko chính xác.
*/

/**
*===SPI Serial Clock Discussion(S35)
*==SPI Periperal of Your Microcontroller(V132)
+ Bạn cần mở Datasheet và Block Diagram để kiểm tra các SPI trong vđk
+ Với STM32407_MCU thì có 3 peripheral SPI đc cài đặt: SPI1(APB2 Bus), SPI2,SPI3(APB1 Bus)

*==SPI Serial Clock Frequency(V133)
+ Hỏi: Tốc độ xung nhịp tối đa của SPI Periperal có thể đặt đc trên một bộ vđk là bao nhiêu?
 Trả lời:
 ^Đầu tiên bạn cần phải biết tốc độ của APBx bus mà SPI peripheral đc kết nối
  ~Hiện tại trong tất cả dự án thì đang sử dụng HSI 16MHz
  ~Với APB1 Bus(chứa SPI2,SPI3), và theo sơ đồ bộ chia thì từ HSI ---> APB1 Bus có bộ chia là 1
  -> f_PCLK(APB1 Bus) = 16MHz

  ~Tuy nhiên bên trong khối SPI2/SPI3 có bộ chia gọi là Min Prescalar = 2
  Lưu ý: Min Prescalar = 2 là giá trị mặc định nhưng có thể thay đổi theo yêu cầu.
  -> SCLK(SPI2/SPI3) = 16 / 2 = 8MHz
  => Tốc độ xung nhip nối tiếp đa mà thiết bị ngoại vi SPI có thể tạo ra là 8MHz ở APB1 Bus
  Tốc độ này cũng là tốc đô cho SPI1 ở ABP2 Bus

 ^Trong một số trường hợp f_PCLK(APB1 Bus) có thể lên đến 42MHz và f_PCLK(APB2 Bus) có thể lên đến 84MHz
 -> SCLK(SPI2/SPI3) có thể tối đa là 21MHz  và SCLK(SPI1) có thể tối đa 42MHz

 ^Các thanh ghi cấu hình tốc độ Baud  chính là cấu hình Min Prescalar
*/

