/*
 * usart_tutorial.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 */
/**
*===UART Essentials(S68)
*==Intro UART vs USART(V238)
+ UART: là viết tắt của Universal(Phổ quát) Asynchronous(không đồng bộ) Receive Tranmitter
  Nghĩa là UART peripheral chỉ hỗ trợ Asynchronous Mode

  USART: là viết tắt của Universal Synchronous(đồng bộ) Asynchronous Receive Tranmitter
  Nghĩa là USART peripheral hỗ trợ cả Asynchronous Mode và Synchronous Mode

  ->USART là chuẩn truyền thông nâng cấp so với UART do có hỗ trợ truyền/nhận đồng bộ

+ Không có cổng cụ thể(specific port) cho giao tiếp USART. Chúng thường được sử dụng kết hợp với
các giao thức như RS-232, RS-434, USB, v.v.
+ Trong truyền đồng bộ(Asynchronous), The Clock được gửi riêng biệt với luồng dữ liệu và không sử dụng
bit start/stop
+ USART hardware components(thành phần):
 ^ Baud rate generator - Bộ tạo tốc độ Baud
 ^ TX and RX shift registers - Thanh ghi dịch chuyển TX và RX
 ^ Transmit/Receive control blocks - Khối điều khiển truyền/nhận
 ^ Transmit/Receive buffers - Bộ đệm truyền/nhận
 ^ First-in, First-out(FIFO) buffer memory - Bộ nhớ đệm hoạt động theo chuẩn FIFO

*==Understanding UART pins(V239)
+ Sơ đồ:   		TX	------>	RX
		UART1	RX	<------	TX		UART2
				RTS ------>	CTS
				CTS	<------	RTS

 ^Giao tiếp hai chiều UART yêu cầu tối thiểu hai chân là TX và RX. Nhận data vào module UART qua chân RX
 và truyền data ra khỏi module UART qua chân TX
 ^Nếu ko truyền bất cứ thứ gì thì TX line sẽ đc giữ ở mức HIGH(đây là trạng thái ko hoạt động của TX line)
 ^Để nhận data trên RX line, module UART sẽ liên tục lấy mẫu RX line để phát hiện bit start của Frame data
 ^Khi bit start đc phát hiện, quá trình tiếp nhận "Frame Data" sẽ bắt đầu cho đến khi nó phát hiện RX line
 đã empty(đã truyền xong, ko còn data trên RX line)

 ^RTS và CTS là 2 chân đc yêu cầu khi bạn sử dụng chức năng "Hardware flow control- điều khiển luồng phần cứng"
 ^CTS(Clear to Send), là một "active low pin - hành động khi chân ở mức low", khi sử dụng "Hardware flow control",
 việc truyền dữ liệu trên TX line sẽ chỉ xảy ra nếu chân CTS đc kéo xuống mức Low(kéo xuống GND), nếu ko việc
 truyền data sẽ đc giữ kéo dài cho đến khi chân này đc kéo xuống mức thấp.
 ^RTS(Request to Send), là một "active low pin", Mô-đun UART sử dụng dòng này để yêu cầu dữ liệu từ thiết bị kết
 nối khác rằng nó cần data

 ^Sơ đồ ở trên là cách kết nối dây giữa 2 thiết bị
  ~Khi kết nối RTS(1) -> CTS(2), khi RTS(1) ở mức thấp sẽ khiến CTS(2) cũng sẽ xuống mức thấp,thì lúc này data
  sẽ được truyền từ thiết bị 2 -> thiết bị 1 trên TX(2)->RX(1)
  ~Nghĩa là với vd ở trên thì data sẽ đc thiết bị 2 truyền sang thiết bị 1

*==UART Frame format(V240)
+ Có 2 kiểu Frame thường dùng trong UART là
 ^Frame 1: Start bit|Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Stop bit|Next start bit(nếu có)|Next Data Frame(nếu có)
 Ở định dạng Frame1 thì Data Frame có độ dài là 9bit(bit0(LSB)->bit8(MSB)), với bit 8 là Optional Parity Bit

 ^Frame 2: Start bit|Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Stop bit|Next start bit(nếu có)|Next Data Frame(nếu có)
 Ở định dạng Frame1 thì Data Frame có độ dài là 8bit(bit0(LSB)->bit7(MSB)), với bit 7 là Optional Parity Bit

 ^Các Start bit luôn ở mức low và Stop bit luôn ở mức high
 ^Khi bạn muốn sử dụng bao nhiêu bit(Data bit:5bit,6bit,7bit,8bit,9bit) thì bạn sẽ tiến hành cấu hình các UART Periperal
 Register
 ^Optional Parity Bit: bạn sẽ chọn 2 cấu hình là bit chẵn(0) hay lẻ(1)
 ^Stop bit có thể là 1 Stop Bit, 1.5 Stop Bit, 2 Stop Bit

*==Baud Rate(V241)
+ Tốc độ truyền(Baud rate) là tốc độ truyền dữ liệu qua đường nối tiếp. Nó thường được biểu thị bằng đơn
vị bit-per-second (bps). Nếu bạn đảo ngược tốc độ baud, bạn có thể biết được mất bao lâu để truyền một bit. Giá trị
này xác định thời gian máy phát(transmitter) giữ đường dây nối tiếp ở mức cao hay thấp
 ^Vd: baudrate = 9600bps, ta cần truyền 4 bit gồm start bit và 3 bit data
  -> Thời gian truyền 1 bit cần 1/9600 = 0.000104s = 104us => Để truyền 4bit cần tổng cộng 104*4=416us
  -> Nếu thời gian truyền 1 bit càng nhanh thì tời gian truyền hết data của bạn càng nhanh

+ Baud rate có thể là bất kỳ số nào nhưng có yêu cầu duy nhất quan trọng là cả Transmitter và Receive đều phải hoạt
động trên cùng một tốc độ Baud Rate

+ Các Baud Rate phổ biến thường đc sử dụng: 2400,4800,9600,19200,38400,57600,115200
 ^Tốc độ Baud Rate càng nhanh thì tốc độ data đc gửi càng nhanh, tuy nhiên có những giới hạn tốc độ truyền dữ liệu.
 Tốc độ baudrate sẽ phụ thuộc vào tần số clock xung nhịp ngoại vi của thiết bị hỗ trợ UART peripheral

*==Synchronization bits(V242)
+ Bit Đồng Bộ là 2 hoặc 3 bit đặc biệt đc truyền cùng với mỗi đoạn dữ liệu. Vd Start bit và Stop Bit

+ Sơ đồ Frame: Start bit| 5 to 9 data bits |Stop bit(s)
 ^Start bit là bit báo hiệu bắt đầu của một frame data, Stop bit(s) là bit báo hiệu kết thúc của một frame data
 ^Thường trong một frame chỉ sử dụng 1 start bit và 1 stop bit nhưng trong trường hợp bạn truyền với tốc độ cao
 (lên tới megabit) thì nên sử dụng 2 stop bit

+ Thêm:
 ^Trong lập trình UART, synchronization bits là các bit được sử dụng để đồng bộ hóa dữ liệu giữa hai thiết bị UART.
 Synchronization bits thường được sử dụng trong các ứng dụng có tốc độ truyền dữ liệu cao, nơi nhiễu điện từ có thể
 gây ra lỗi dữ liệu.

 ^Có hai loại synchronization bits phổ biến:
  ~Start bit: Bit start là bit đầu tiên của mỗi khung dữ liệu UART. Bit start thường được thiết lập thành mức thấp(0).
  ~Stop bit: Bit stop là bit cuối cùng của mỗi khung dữ liệu UART. Bit stop thường được thiết lập thành mức cao(1).

 ^Việc sử dụng synchronization bits trong lập trình UART như sau:
  ~Khi một thiết bị UART muốn bắt đầu truyền dữ liệu, nó sẽ gửi một bit start. Bit start sẽ được thiết lập thành mức
 thấp (0).
  ~Thiết bị nhận sẽ nhận bit start và đồng bộ hóa với tốc độ truyền dữ liệu của thiết bị truyền.
  ~Sau bit start, thiết bị truyền sẽ gửi dữ liệu. Dữ liệu có thể được gửi dưới dạng các bit đơn lẻ hoặc các nhóm bit.
  ~Cuối cùng, thiết bị truyền sẽ gửi một bit stop. Bit stop sẽ được thiết lập thành mức cao (1).
  ~Thiết bị nhận sẽ nhận bit stop và kết thúc quá trình truyền dữ liệu.
  ~Ví dụ, trong một ứng dụng có tốc độ truyền dữ liệu 115 kbps, một khung dữ liệu UART có thể bao gồm 8 bit dữ liệu,
  1 bit start, và 1 bit stop. Khung dữ liệu này sẽ được truyền trong khoảng thời gian 100 micro giây.

 ^Synchronization bits có thể giúp cải thiện độ tin cậy của truyền dữ liệu UART trong các ứng dụng có tốc độ truyền
 dữ liệu cao. Tuy nhiên, việc sử dụng synchronization bits cũng có thể làm giảm tốc độ truyền dữ liệu(vì làm tăng số
 lượng bit cần truyền).

*==UART Parity Bit(V243)
+ Việc thêm Parity bit là phương pháp phát hiện lỗi đơn giản nhất. Parity chỉ đơn giản là số lượng những số bit "1"
xuất hiện dưới dạng nhị phân của một số. Vd: số 55 có dạng nhị phân là 0b00110111 -> Parity = 5
+ Có 2 lựa chọn cho Parity là "Even(Chẵn) Parity" và "Odd(Lẻ) Parity"
 ^Even Parity: Trong parity chẵn, tổng số bit 1 trong khung dữ liệu phải là số chẵn
	55 --Binary--> 00110111(Data bit) + 1(Parity bit) => Tạo thành tập hợp có số lượng bit 1 chẵn
	54 --Binary--> 00110110(Data bit) + 0(Parity bit) => Tạo thành tập hợp có số lượng bit 1 chẵn

 ^Odd Parity: Trong parity lẻ, tổng số bit 1 trong khung dữ liệu phải là số lẻ.
	55 --Binary--> 00110111(Data bit) + 0(Parity bit) => Tạo thành tập hợp có số lượng bit 1 lẻ
	54 --Binary--> 00110110(Data bit) + 1(Parity bit) => Tạo thành tập hợp có số lượng bit 1 lẻ

+ Parity bit: đc sử dụng để phát hiện lỗi
 ^Giả sử đang truyền data có số thập phân '167->1101011', sau đó ta thêm vào bit "1" để tạo Even Parity bit
 -> Data đc gửi có dạng "1101011_1"

 ^Nếu trong quá trình truyền data bị hỏng, và một trong những bit bị thay đổi, giả sử data hỏng có dạng "1100011_1"
 ->Ta thấy rằng bit thứ 4 bị hỏng(từ 1->0) -> lúc này số lượng bit 1 bao gồm bit Parity là 5bit(là 1 số lẻ)
 =>Vì đã cài đặt Even Parity bit(cả hai thiết bị đều đc cấu hình giống nhau) nên khi có sự khác biệt về Parity bit
 thì ta có thể nhận ra rằng data bị hỏng và cuối cùng loại bỏ data đó

+ Việc sử dụng parity bit trong lập trình nhúng như sau:
 ^Khi một thiết bị muốn truyền dữ liệu, nó sẽ tính toán parity bit cho dữ liệu.
 ^Thiết bị nhận sẽ nhận dữ liệu và parity bit.
 ^Thiết bị nhận sẽ kiểm tra parity bit để xem dữ liệu có bị lỗi hay không.
 Nếu tổng số bit 1 trong dữ liệu nhận được không khớp với parity bit, thì dữ liệu bị lỗi.

+ Parity bit có thể giúp cải thiện độ tin cậy của truyền dữ liệu trong các ứng dụng có nhiễu điện từ. Tuy nhiên, việc sử
dụng parity bit cũng có thể làm giảm tốc độ truyền dữ liệu.

+ Dưới đây là một số ví dụ về cách sử dụng parity bit trong lập trình nhúng:
 ^Trong các ứng dụng kết nối cảm biến, parity bit có thể được sử dụng để phát hiện lỗi dữ liệu do nhiễu điện từ.
 ^Trong các ứng dụng kết nối thiết bị, parity bit có thể được sử dụng để phát hiện lỗi dữ liệu do nhiễu điện từ
 hoặc các vấn đề khác trong đường truyền dữ liệu.

*===UART functional block and Peripheral Clock(S69)
*==Exploring UART function block(V244)
+ Giới thiệu về sơ đồ khối của UART peripheral

*==UART Peripheral Clock(V245)
+ USART1 và USART6 đc kết nối với APB2 Bus (84MHz)
 ^Xung clock: HSI ---16MHz--->Bộ chia---16MHz--->APB2 Bus(84MHz)---16MHz/ F_clock--->USART1,USART6

+ USART2,USART3,USART4,USART5 đc kết nối với APB1 Bus(43 MHz)

+ Các USART ở APB2 Bus có khả năng chạy ở tần số cao hơn
+ Vì trong bài học sử dụng bộ dao động nội có tần số là 16MHz nên các USART hầu hết cũng sẽ hoạt động ở tần số này
+ Tần số đc USART Peripheral sử dụng để tạo ra các Baud rate khác nhau

*===UART Communication(S70)
*==UART Transmitter(V246)
+ Thành phần quan trọng nhất của khối UART Transmitter là "Transmit Shift Register", dữ liệu dạng song song đc chuyển đổi
thành chuỗi dữ liệu nối tiếp. "TSR" sẽ nhận data từ "Transmit Data Register" do phần mềm tải vào trước đó.
+ "TSR" sẽ được tải dữ liệu mới sau khi bit stop của frame cuối cùng được gửi thành công vào TDR

+ Cách dữ liệu đc truyền đi bằng UART peripheral
 1. Lập trình "M bit" nằm trong USART_CR1 Register để xác định độ dài của từ muốn truyền(word length)(8bit hay 9bit)
 2. Lập trình số "stop bit" trong USART_CR2 Register
 3. Lựa chọn "Baud rate" mong muốn bằng cách cấu hình USART_BRR Register
 Lưu ý: trước khi lựa chọn baud rate bạn phải truy cập bảng "Table 136. Error calculation for programmed baud rate"
 và cùng với tần số xung clock để lựa chọn baud rate cho đúng. Nguyên nhân vì tần số clock sẽ đặt giới hạn cho baud rate
 4. Cấu hình "TE bit" trong USART_CR1 Register để cho phép kích hoạt(enable) "Transmit block"
 5. Kích hoạt(enable_high_1) USART bằng cách cấu hình "UE bit" trong USART_CR1 Register
 6. Nếu "TxE Flag" đc set, thì ta sẽ tiến hành "write_ghi" data byte để gửi vào trong USART_DR Register, lặp lại cho đến
 khi gửi hết tất cả data byte muốn write
 7. Sau khi đã tiến hành write_ghi data vào trong USART_DR Register, ta chờ đến khi TC flag đc set lên 1
 TC(Transmition Complete)=1, chỉ ra rằng việc truyền Frame cuối cùng đã hoàn tất

*==UART Receiver(V247)
+ Dữ liệu sẽ đc nhận theo sơ đồ: "Receive Shift Register-RSR" -> "Receive Data Register-RDR" -> Processor
+ Cách dữ liệu đc nhận bằng UART peripheral:
 1. Lập trình bit field "M bit" trong USART_CR1 Register để xác định độ dài từ đc nhận
 Cả hai Tranmitter và Receiver đều phải cùng đồng ý độ dài của từ
 2. Lập trình số "bit stop" muốn sử dụng trong USART_CR2 Register
 3. Lựa chọn tốc độ "baud rate" mong muốn bằng cách sử dụng USART_BRR Register
 4. Enable(Kích hoạt) USART bằng cách cấu hình(writing) "UE bit" trong USART_CR1 Register
 5. Set "RE bit" trong USART_CR1 Register, nghĩa là cho phép sử dụng "receive block" của USART Peripheral
 6. Khi một ký tự(a character) đc nhận, ta sẽ đợi cho đến khi "RXNE bit" đc set(nghĩa là nội dung của RSR đang đc chuyển
 tới RDR) và sau đó phần mềm đọc đc "data byte" từ RDR
 Khi "RXNE flag" thì sẽ kích hoạt sẽ làm gián đoạn, bạn sẽ phải kích hoạt "RXNEIE bit"
 Mỗi lần nhận một "Frame data" hoàn chỉnh thì "RXNE flag" sẽ đc set
 7. "RXNE bit" sẽ phải bị xóa bởi việc Processor đọc RDR trước khi kết thúc quá trình nhận ký tự tiếp theo để tránh lỗi
 tràn "overrun error", nghĩa là RDR phải empty trước khi RSR hoàn thành việc nhận ký tự tiếp theo

 Lưu ý: ^Khi không còn data để đọc(nhận) thì RX line sẽ giữ ở múc High(trạng thái không hoạt động)
 ^Khối "Receiver Block" sẽ liên tục lấy mẫu "RX line" để phát hiện lần bắt đầu tiếp theo. Điều này sẽ dẫn đến trường hợp
 khi không có data trên RX line thì sẽ gây tiêu tốn điện năng. Vì vậy sau khi hoàn tất việc nhận data, tốt hơn nên tắt
 "Receiver Block" theo nhu cầu lập trình của bạn nếu muốn tiết kiệm năng lượng.

*===USART Driver Development(S71)
*==USART Driver Development configurable items(V248)
+ Sơ đồ: 						  	-->	USARTx Initialization / Periperal Clock Control
									-->	USARTx TX
			USARTx Driver <--> APIs -->	USARTx RX
									-->	USARTx Interrupt config & handling
									-->	Other USARTx Management APIs
 ^Đây là các hàm cơ bản phục vụ lập trình cấu hình USARTx trong lớp USARTx APIs_Driver

+ USARTx Periperals: (User Configuaration)
								|	USART_Mode				|
	 USARTx_Peripheral	------>	|	USART_Baud				|
								|	USART_NoOfStopBit		|
								|	USART_WordLength		|
								|	USART_ParityControl		|
								|	USART_HWFlowControl		|
 ^Các Items(thông số) trên là các mục(các hàm lập trình) có thể dùng cho cấu hình cho ứng dụng người dùng
 ^USART_Mode: Chọn chế độ Tranmitter hay Receiver hay cả hai chế độ
 ^USART_Baud: Chọn tốc độ "Baud rate"
 ^USART_NoOfStopBit: Chọn số "Stop bit"
 ^USART_WordLength: Chọn độ dài từ (8bit hay 9bit)
 ^USART_ParityControl: Chọn chế độ Even Parity hay Odd Parity hay không sử dụng Parity bit
 ^USART_HWFlowControl: Kiểm soát luồng phần cứng, nghĩa là cài đặt RTS và CTS khi 2 chân đc
 yêu cầu khi bạn sử dụng chức năng "Hardware flow control- điều khiển luồng phần cứng"

+ Mô hình Structure của USART Peripheral cho "Configuaration" và "Hangling"
 ^Configuaration structure for USARTx peripheral
 typedef struct
 {
	 uint8_t 	USART_Mode;
	 uint32_t 	USART_Baud;
	 uint8_t	USART_NoOfStopBit;
	 uint8_t	USART_WordLength;
	 uint8_t	USART_ParityControl;
	 uint8_t	USART_HWFlowControl;
 }USART_Config_t;

 ^Handle structure for USARTx peripheral
 typedef struct
 {
	 USART_RegDef_t	*pUSARTx;			//Struct đại diện các thanh ghi
	 USART_Config_t USART_Config;		//Struct đại diện User Configuaration
 }USART_Hangle_t

+ Exercise:
1.Create USART driver header file and Source file - Tạo tệp tiêu đề trình điều khiển USART và tệp Nguồn
2.Complete USART register definition structure and other macros(peripheral base addresses, Device definition,
clock en, clock di,etc) in MCU specific header file - Hoàn thành cấu trúc định nghĩa thanh ghi USART và các macro
khác (địa chỉ cơ sở ngoại vi, Định nghĩa thiết bị, đồng hồ en, đồng hồ di, v.v.) trong tệp tiêu đề cụ thể của MCU
3.Also add USART register bit definition macros in MCU specific header file - Ngoài ra, thêm macro định nghĩa bit
đăng ký USART trong tệp tiêu đề cụ thể của MCU
4.Add USART Configuration structure and USART handle structure in USART header file - Thêm cấu trúc cấu hình USART
và cấu trúc xử lý USART trong tệp tiêu đề USART

*==USART driver APIs Prototypes(V249)
*==Configuaration option and USART register(V250)
+ Datasheet_stm32f407x: Dm00031020 -> 30.3.4 Fractional baud rate generation
 Công thức tính baud rate

*==Configuaration option and USART register(V250)
*==USART Driver API: USART Init(V251)
*==USART Driver API: Tx and RX Assignment(V252)
*==USART Driver API: Send Data(V253)

*===USART oversampling and baudrate(S72)
*==USART Oversampling - Tìm hiểu việc lấy mẫu quá mức(V254)
+ Bộ thu(Receiver) triển khai các kỹ thuật lấy mẫu quá mức do người dùng định cấu hình khác nhau (ngoại trừ ở chế độ đồng bộ)
để phục hồi dữ liệu bằng cách phân biệt giữa dữ liệu đến hợp lệ và nhiễu(noise)

+ Phương pháp lấy mẫu quá mức có thể được chọn bằng cách lập trình QUÁ 8 bit trong thanh ghi USART_CR1 và có thể gấp 16 hoặc
8 lần tốc độ truyền đồng hồ

+ Phương pháp lấy mẫu quá mức có thể định cấu hình theo 16 hoặc 8 để mang lại sự linh hoạt giữa tốc độ và dung sai xung nhịp

+ Việc lấy mẫu(Slampled) và Noise
 +Nếu giá trị Slampled Value là 000 hoặc 111 thì Receiver sẽ tương ứng nhận đc là 0 và 1 (NE status = 0)
 +Nếu giá trị Slampled Value 001,010,011,100,101,110 thì có nhiếu tác động vào data gây lỗi nhiễu (NE status = 1 ở USART_SR_NF)

+ Noise Erro -> Khi phát hiện thấy nhiễu trong frame:
 ^Bit cờ NF được thiết lập bằng phần cứng khi phát hiện nhiễu trên frame nhận được
 ^Dữ liệu không hợp lệ được chuyển từ thanh ghi Shift sang thanh ghi USART_DR
 ^Ứng dụng có thể xem xét hoặc loại bỏ frame dựa trên logic ứng dụng

+ Phương pháp để chọn "oversampling" thích hợp:
 ^Nếu bạn chọn lấy mẫu quá mức 8 (OVER8 = 1) thì bạn có thể đạt được tốc độ truyền tối đa lên tới F_pCLK/8 nhưng trong trường
 hợp này, dung sai tối đa của máy thu đối với độ lệch đồng hồ sẽ giảm. Nghĩa là bộ dao động lấy mẫu 8 thường là các RC bộ tạo
 dao động nội và chúng sẽ bị ảnh hưởng bởi nhiệt độ dẫn đến việc độ lệch clock sẽ lớn và gây ra lỗi khi receive

 ^Nếu bạn chọn lấy mẫu quá mức 16(OVER8 = 0) thì bạn có thể đạt được tốc độ baud tối đa lên tới F_pCLK /16, Trong trường hợp
 này, dung sai tối đa của máy thu đối với độ lệch đồng hồ sẽ tăng lên. Nghĩa là độ lệch dao động xung clock với máy thu
 sẽ ở mức chịu đựng cao hơn

 -> Nếu bạn sử dụng USART trong mỗi trường "Very Noise" thì hãy sử dụng tính năng lấy mẫu ở mức 16.
 -> Nếu bạn sử dụng USART trong mỗi trường "ít hoặc không có Noise" thì hãy sử dụng tính năng lấy mẫu ở mức 8.

*==USART Baudrate calculation (V255,V256)
+ Công thức: Tx/Rx Baud = f_CK / (8 x USART_DIV)  (if OVER8=1)
			 Tx/Rx Baud = f_CK / (16 x USART_DIV) (if OVER8=0)
		Với: USART_DIV: Divide factor to generate different baud rate - Bộ Chia hệ số để tạo ra tốc độ truyền khác nhau

 ^Tốc độ truyền cho máy thu và máy phát (Rx và Tx) đều được đặt thành cùng giá trị như được lập trình trong giá trị
 Mantissa(4 bit đầu thanh ghi USART_BRR) và Fraction(12 bit sau của thanh ghi USART_BRR) đối với value USART_DIV

+ Công thức chung: Tx/Rx Baud = f_CK / (8 x (2 - OVER8) x USART_DIV)
			OVER8=1: nếu lấy mẫu mức 8 và OVER8=0 nếu lấy mẫu mức 16
 ^Lưu ý: value f_CK sẽ phụ thuộc vào bus mà nó đang kết nối, vd trên STM32407_MCU thì các USARTx đc kết nối ở 2 bus
 ABPx khác nhau nên f_CK trên 2 bus khác nhau nên cần xác định USARTx muốn sử dụng đang dùng APBx bus nào để xác
 định value f_CK cho đúng

+ Vd: Muốn tạo Baudrate=9600bps và sử dụng F_CK(USART Peripheral clock) = 16MHZ và OVER8=0(lấy mẫu mức 16)
 -> USART_DIV = 16M /(8*2*9600) = 104.1875 -> Bạn sẽ cấu hình số này USART_BRR = 104.17

 ^Thanh ghi USART_BRR : với 2 phần là Mantissa(4 bit đầu[3:0]) và Fraction(12 bit sau[15:4]) thanh ghi USART_BRR
 ^Với value USART_DIV = 104.1775 thì bạn sẽ lưu phần '104' vào Mantissa và lưu '1775' vào phần Fraction
 ^Cách làm: DIV_Fraction = 0.1875 * 16(OVER8=0) = 3
			DIV_Mantissa = 104 = 0x68(Hex)
			==> USART_DIV = 0x683 -> Cấu hình value này vào trong USART_BRR register để tạo baudrate = 9600bps

+ Vd2: Muốn tạo Baudrate=115200bps và sử dụng F_CK(USART Peripheral clock) = 16MHZ và OVER8=1(lấy mẫu mức 8)
 ^Cách làm: -> USART_DIV = 16M /(8*1*115200) = 17.361
			DIV_Fraction = 0.361 * 8(OVER8=1) = 2.88=3
			DIV_Mantissa = 17 = 0x11(Hex)
			==> USART_DIV = 0x113 -> Cấu hình value này vào trong USART_BRR register để tạo baudrate = 115200bps

+ Cách tính khác về USART_DIV = (F_CK / (8 * BaudRate)) * 100 <=>  (25* F_CK) / (2 * BR) = X
 ^DIV_Fraction(I) = X/100
 ^DIV_Mantissa = X - (I * 100) = Y -sau đó-> (Y* 8) + Số_làm_tròn

 ^Vd: USART_DIV = 471 -> I = 471 / 100 = 4 -> ghi vào phần DIV_Fraction
					  -> F = 471 - (4*100) = 71 *8 = (568 + 50(số làm tròn)) / 100 = 6 -> ghi vào phần DIV_Mantissa

*==USART baudrate coding(V257)

*===Exercise(S73)
*==Exercise: USART send data to arduino(V258)
+ Write a program to send some message over UART from STM32 board to Arduino board. The Arduino board will display the
message (on Arduino serial monitor) sent from the ST board.
 Baudrate: 115200bps
 Frame format: 1 stop bit, 8 bit, no parity

*==Communicating with PC over UART(V259)
+ Sơ đồ nối: PC <---> CP2102 USB to UART convertor Rx <---- TX(PA2)	USART(STM32)
												   TX ----> RX(PA3)
												   GND ---> GND
 ^CP2102 USB to UART convertor: đc gọi là thiết bị chuyển đổi

*===USART interrupt(S74)
*==USART interrupt discussion(V260)
+ Trong USART communication thì các sự kiện sau đây có thể làm Interrupt của Processor
Interrupt Events									Event flag				Enable control bit
Transmit Data Register Empty							TXE						TXEIE
CTS flag												CTS						CTSIE
Transmission Complete									TC						TCSIE
Receive Data Ready to be Read							RXNE					RXNEIE
Overrun Error Detected									ORE						RXNEIE
Idle Line Detected 										IDLE					IDLEIE
Parity Error											PE						PEIE
Break Flag												LBD						LBDIE
Noise Flag, Overrun error and Framing Error in 			NF or ORE or FE			EIE
multibuffer communication

 ^Cột "Enable control bit- bit kiểm soát", đây là các bit cần kích hoạt trước khi kích hoạt các bit cột "Event flag"
 Việc kích hoạt các bit cột "Enable control bit" sẽ ko làm interrupt of processor
 ^Các bit cột "Event flag" đc kích hoạt(set made 1) là tiền đề để thực hiện Interrupt of Processor
 Nghĩa là các sự kiện khi thực hiện I2C communication có thể làm Interrupt of Processor

+ Tham khảo datasheet: Fugure 320. USART interrupt mapping diagram

*==Exercise(261)
Write a program for stm32 board which transmits different messages to the Arduino board over UART communication
For every messages STM32 board sends, arduino code will change the case of alphabets (lower case to upper case
and vice versa) and sends messages back to the stm32 board.
The stm32 board should capture the reply from the arduino board and display using semi hosting

*/

