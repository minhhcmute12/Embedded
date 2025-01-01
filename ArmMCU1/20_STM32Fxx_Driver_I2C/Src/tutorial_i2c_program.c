/*
 * tutorial_i2c_prgram.c
 *
 *  Created on: Jan 3, 2024
 *      Author: Minh
 */

/**
*===I2C Driver API requirements and Config Structure(S53)
*==I2C Driver API requirements(V182)
+ Sơ đồ: 						  -->	I2C Initialization / Periperal Clock Control
								  -->	I2C Master TX
			I2C Driver <--> APIs  -->	I2C Master RX
								  -->	I2C Slave TX
								  -->	I2C Slave RX
								  -->	I2C Error Interrupt  handling
								  ->>	I2C Events Interrupt handling
								  -->	Other I2C Management APIs
 ^Đây là các hàm cơ bản phục vụ lập trình cấu hình I2C trong lớp I2C APIs_Driver

+ I2Cs Periperals:
								|	I2C_Speed			|
	 I2Cx_Peripheral	------>	|	I2C_DeviceAddres	|
								|	I2C_ACKControl		|
								|	I2C_FMDutyCycle		|
 ^Các Items(thông số) trên là các mục(các hàm lập trình) có thể dùng cho cấu hình cho ứng dụng người dùng
 ^I2C_Speed: cấu hình tốc độ xung nhịp cho I2C
 ^I2C_DeviceAddres: cấu hình địa chỉ cho các thành phần Slave và Master
 ^I2C_ACKControl: cấu hình tính năng tự động xác thực là đc bật hay tắt
 ^I2C_FMDutyCycle: cấu hình chu kỳ nhiệm vụ dùng cho trường hợp truyền tốc độ cao Fast_Mode

+ Exercise: 1.Create stm32f407xx_i2c_driver.c and stm32f407xx_i2c_driver.h
 2. Add I2Cx related(có liên quan) details to MCU specific header file
  I. I2C peripheral register definition structure
  II. I2Cx base address macros
  III. I2Cx peripheral definition macros
  IV. Macros to enable and disable I2Cx peripheral clock
  V. Bit position definition of I2C peripheral

*==I2C Handle and Configuration structure(V183)
+ Thiết lập viết các cấu hình macros trong file "stm32f407xx.h"

*==I2C User Configuration macros(V184)
+ Thiết lập viết các cấu hình macros trong file "stm32f407xx_i2c_driver.h"

*==I2C API Prototypes(V185)
+ Thiết lập các hàm khai báo Prototypes trong file "stm32f407xx_i2c_driver.h"

*==Steps for I2C init implementation(V186)
+ Các bước Triển khai I2C_Init() API
 1. Cấu hình chế độ I2C sử dụng (Standard or Fast)
 2. Cấu hình tốc độ Speed của Serial Clock(SCL) theo cấu hình Mode ở bước 1(Standard: 100KHz or Fast: 400KHz)
 3. Cấu hình Device Address (Nếu thiết bị bạn cho là Slave)
 4. Enable the Acking
 5. Định cấu hình thời gian tăng(the rise time) cho các chân I2C (sẽ thảo luận sau)
 The rise time: là thời gian cần thiết để tín hiệu từ mức GND đạt đến mức VCC

 Lưu ý: Tất cả các cấu hình trên phải được thực hiện khi thiết bị ngoại vi bị vô hiệu hóa(disable) trong thanh ghi điều khiển

*===I2C Serial Clock Discussion(SCLK) (S54)
*==I2C Serial Clock Setting with explanation(V187)
+ Trong thiết bị ngoại vi STM32F4x I2C, các thanh ghi CR2 và CCR được sử dụng để điều khiển cài đặt đồng hồ nối tiếp(serial
clock setting) I2C và các bộ định thời I2C khác như thời gian thiết lập(Setup time) và thời gian giữ(hold time)
 ^I2C_CR2: bit FREQ[5:0] - tùy theo giá trị 5 bit sẽ thiết lập tốc độ khác nhau (tham khảo datasheet)
 ^I2C_CCR: bit CCR[11:0]

+ Giải thích về cơ chế SLC trên I2C:
 ^Xung Clock 16MHz từ bộ Processor sẽ qua bộ chia APB1 prescaler(vì I2C nằm ở APB1 Bus). Vì APB1 prescaler ta set bằng 1 nên
 tần số output vẫn là 16MHz. Trước khi từ APB1 Bus vào I2C Periperal thì xung Clock sẽ đi qua thêm một khối "CR2_FREQ",
 số bit đc cấu hình ở khối "CR2_FREQ" sẽ quyết định tốc độ CLK của I2C Periperal.
 
+ Tần số (Hz): Đây là đơn vị đo số lần lặp lại của một sự kiện trong một giây. Ví dụ, 16MHz có nghĩa là một sự kiện lặp lại 
16*10^6 =  16.000.000(Hz) lần trong 1 giây. 
  Nếu bạn muốn biết thời gian để hoàn thành 1 chu kỳ của một tín hiệu có tần số 16MHz, bạn có thể tính như sau:
  Chu kỳ: 1 / (16*10^6) Hz = 0.0000000625 giây = 62.5 nano giây (ns) = 62.5 * 10^-9 second

+ Bài tập 1: In Standard Mode, generate a 100KHz SCL frequency APB1 Bus Clock(PCLK) = 16KHz
 ^Các bước tiến hành:
  1. Cấu hình chế độ trong CCR register(bit 15)
  2. Lập trình trường FREQ của CR2 với giá trị PCLK1= 16KHz = 62.5 * 10^-9 second
  3. Tính toán và lập trình giá trị CCR trong trường CCR của thanh ghi CCR
  T_high = CCR * T_PCLK1 hoặc T_low = CCR * T_PCLK1
  Trong chế độ Standard Mode thì thời gian chu kỳ xung của T_high và T_low là tương đối bằng nhau

 ^Sử dụng bit15 F/S của Thanh ghi I2C_CCR để cấu hình chế độ là Standard Mode(0)
 -> Ta có Standard Mode = 100KHz = 100.000 Hz -> Chu kỳ T = 1 / 100.000 = 0.00001 giây = 10 micro giây (µs)
 -> thời gian cho một chu kỳ xung(T_high + T_low) là 10 micro second
 -> Thời gian hoạt động cạnh xung lên 	T_high(SCL) = 5ms
 -> Thời gian hoạt động cạnh xung xuống T_low(SCL) = 5ms
 -> Công thức: T_high = CCR * T_PCLK1 <=> 5*10^-6 = CCR * 62.5 * 10^-9 -> CCR = (5/62.5)*1000 = 80 = 0x50
 => Ta tiến hành cấu hình thanh ghi CR2 với trường bit FREQ = 16 và thanh ghi CCR với trường bit CCR = 80

+ Bài tập 2: In Fast Mode, generate a 200KHz SCL frequency APB1 Bus Clock(PCLK) = 16KHz
 ^Các bước tiến hành:
  1. Cấu hình chế độ trong CCR register(bit 15)
  2. Lựa chọn "the duty cycle" của Fast Mode SCL trong thanh ghi CCR(bit 14)
  3. Lập trình trường FREQ của CR2 với giá trị PCLK1= 16KHz = 62.5 * 10^-9 second
  4. Tính toán và lập trình giá trị CCR trong trường CCR của thanh ghi CCR
  If Duty=0 -> T_high = CCR * T_PCLK1 và T_low = 2* CCR * T_PCLK1
  If Duty=1(400KHz) -> T_high = 9* CCR * T_PCLK1 và T_low = 16 * CCR * T_PCLK1
  Có sự khác biệt này là do trong chế độ Fast Mode thì thời gian chu kỳ xung của T_high và T_low là khác nhau
	T_low = 2*T_high hoặc T_low = 1.8*T_high
 => Khi tiến hành lập trình I2C cho các chup khác nhau cần xác định thời gian chu kỳ xung thông qua Datasheet
 Thường thì trong datasheet nằm trong bảng tên là "Characteristics of SDA and SCL Bus line"

 ^Sử dụng bit15 F/S của Thanh ghi I2C_CCR để cấu hình chế độ là Fast Mode(1)
 -> Sử dụng bit14 Duty của Thanh ghi I2C_CCR để cấu hình Duty(0)
 -> Vì Duty=0 nên: Chu kỳ xung T_high +T_low = 3*CCR*PCLK1
 -> Fast Mode = 200KHz -> Chu kỳ T = 1 / 200.000 = 0.000005 giây = 5 micro giây (µs) = 5*10^-6 second
 -> PCLK1 = 16KHz = 62.5 * 10^-9
 => CCR = (5*10^-6 / (3*62.5 * 10^-9)) = 26
 => Ta tiến hành cấu hình thanh ghi CR2 với trường bit FREQ = 16 và thanh ghi CCR với trường bit CCR = 26

*==Clock Stretching(V188)
+ Clock Stretching: Kéo giãn đồng hồ đơn giản có nghĩa là giữ clock(trên SCL line) ở mức 0 hoặc GND level
+ Đồng hồ xung clock trên SCL line thời điểm được giữ ở mức thấp, sau đó toàn bộ giao diện I2C tạm dừng cho đến khi
đồng hồ được đưa về mức hoạt động bình thường, nghĩa là chờ trên SDA line xử lý xong việc gì đó
+ Mục đích: Thiết bị I2C, Master hoặc Slave, sử dụng tính năng này để làm chậm quá trình giao tiếp bằng cách kéo dài
SCL xuống mức thấp, điều này ngăn đồng hồ tăng cao trở lại và giao tiếp i2c dừng trong một thời gian. Trường hợp hay
đc sử dụng là Có những tình huống I2C Slave không thể hợp tác với tốc độ xung nhịp do Master đưa ra và cần phải chậm
lại một chút. Nếu Slave cần thời gian, thì nó tận dụng lợi thế của việc kéo dài đồng hồ và bằng cách giữ đồng hồ ở
mức thấp, nó sẽ tạm dừng hoạt động I2C trong giây lát.

*===I2C Driver API: I2C Init(S55)
*==Implemetation of I2C init API: Part1,Part2,Part3(V189,V190,V191)
+ Triển khai hàm I2C init trong file "stm32f407xx_i2c_driver.c"
+ Công thức rút gọn: (Standard Mode) CCR = F_PCLK1 / (2*F_SCL)
(Fast Mode) DUTY = 0 -> CCR = F_PCLK1 / (3 * F_SCL)
			DUTY = 1 -> CCR = F_PCLK1 / (25* F_SCL)

*===I2C Driver API: I2C Master send data(S56)
*==I2C Transfer Sequence Diagram for Master Sending Data(V192)
+Sơ đồ khối việc gửi dữ liệu từ Master:
			 S|___|Address|A|_________| Data1 | A | Data2 | A |___....| DataN | A |_____|P
			   EV5|		    |EV6|EV8_1| EV8   |   | EV8   |   |EV8|               |EV8_2|

 ^Legend: S=Start, SR= Repeated start, P=Stop, A=Acknowledge
 ^EVx: Event(cần xóa hoàn thành sự kiện để tiếp tục chương trình)(with interrupt if ITEVFEN = 1)
 ^EV5: SB=1 ,Clear by reading SR1 register followed by writting DR register
 Xóa sự kiện bằng cách đọc thanh ghi SR1, sau đó ghi vào thanh ghi DR

 ^Address 8bit: Slave Addrress[7:1] + r/nw bit[0]=0 (write=0 <==> send data)

 ^EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2
 Address ở đây là địa chỉ Slave muốn gửi đến, nếu đúng địa chỉ, Slave sẽ trả về tín hiệu ACK và set ADDR=1, nếu ko đúng
 sẽ ko set ADDR. Sau đó thực hiện điều kiện tiếp tục -> Xóa sự kiện bằng cách đọc thanh ghi SR1, sau đó đọc SR2.

 ^EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR
 Điều kiện vượt qua EV8_1: Cờ TxE=1, shift register và data register đều trống

 ^EV8: TxE=1, shift register empty, data register empty, cleared by writting DR register
 Cờ TxE=1, shift register ko trống(có dữ liệu để ghi vào data register), data register trống(sẵn sàng nhận từ shift register)
 Điều kiện vượt qua EV8: ghi Data1 vào Data register

 ^EV8_2: TxE=1, BTF=1, Program stop request, TxE and BTF are cleared by hardware by the stop condition.
 Vd nếu hoàn thành chuyển byte cuối cùng DataN xong thì cờ TxE=1, DR=null, SR=null cộng với BTF=1 thì tạo điều kiện Stop
 Nguyên nhân vì khi EV8 xảy ra thì các SCL line đều ỏ mức Low(gián đoạn tạm thời chương trình), vì vậy lúc này ta cần
 cấu hình BTF(Byte Tranfer Finish) lên high, để thông báo cho processor đã truyền dữ liệu xong.
 Nếu ko cấu hình BTF=1, thì lúc này ko thể xóa sự kiện EV8(ko còn data để ghi vào DR) dẫn đến chương trình bị treo.

 ^Lưu ý: The EV5,EV6,EV8_1,EV8_2,EV9 events xảy ra thì chúng sẽ kéo xung trên SCL line xuống Low cho đến khi hết sự kiện
 Nghĩa là vd sau khi có điều kiện S(Start) thì EV5 xảy ra sẽ kéo SCL xuống low làm gián đoạn chương trình. Khi EV5 hoàn
 tất(bit SB bị xóa) thì SCL trở lại high, chương trình sẽ tiếp đến khối Address. Nếu có sự cố nào đó bit SB ko bị xóa,
 thì chương trình sẽ bị treo ở sự kiện EV5.

 ^Lưu ý 2: Sự kiện EV8 stretching sẽ kéo SCL xuống mức thấp nếu chuỗi phần mềm không hoàn thành trước khi kết thúc quá
 trình truyền byte tiếp theo.

+ Lúc này thanh ghi cần quan tâm là I2C Status register(SR1) với các trường bit ADDR,BTF, TxE,...

*==Implemetation of I2C Master Sending Data API: Part1(V193)
*==Implemetation of I2C Master Sending Data API: Part2(V194)
+ Mẹo: Bit field TIMEOUT[14] của I2C_SR1 Register dùng để kiểm tra thời gian bị stretching, nếu trong trường hợp bị
stretching quá lâu(quá 25ms hoặc điều kiện khác) thì sẽ tự động thực hiện "STOP Condition"

*==Implemetation of I2C Master Sending Data API: Part3(V195)
*==Implemetation of I2C Master Sending Data API: Part4(V196)
*==Implemetation of I2C Master Sending Data API: Part5(V197)

*===I2C pull up resistance, rise time and bus capacitance(S57)
*==I2C pull up resistance, rise time and bus capacitance discussion(V198)
+ Thảo luận về điện trở kéo lên của I2C, thời gian tăng và điện dung bus
+ Datasheet: UM10204 -> Table. Characteristics of SDA and SCL bus lines
 ^Trong bảng sẽ cung cấp các giá trị t_r, C_b theo từng chế độ I2C cho ta lựa chọn
+ Pull-up Register(R_p) Calculation
 ^R_p(min) = (VCC - V_oL(max)) / I_oL
  V_oL = Low level output voltage(với STM32 thường là 0.4V)
  I_oL = Low Level output current	(với STM32 thường là 3mA)

 ^R_p(max) = t_r / (0.8473 * C_b)
  t_r = rise time of both SDA and SCL signals - thời gian tăng của cả tín hiệu SDA và SCL, STM32:
	Standard Mode:  t_r(max) = 1000 nano second
	Fast Mode:  t_r(max) = 300 nano second
	Fast Mode Plus:  t_r(max) = 120 nano second
	Dựa vào bảng datasheet để tra thông số kỹ thuật nếu dùng các loại chip khác STM32F407VGT6
  C_b = capacitive load for each bus line -tải điện dung cho mỗi bus line

+ rise time(t_r) là khoảng thời gian mà cạnh tăng(xung đi lên) cần để đạt biên độ 70% từ biên độ 30% đối với SDA và SCL
 ^Thời gian t_r này ko đc quá cao vì nếu cao sẽ ảnh hưởng đến quá trình giao tiếp I2C
 ^Giá trị điện trở kéo lên và điện dung của bus line sẽ ảnh hưởng đến t_r
  ~Nếu giá trị R hoặc điện dung C_b tăng thì sẽ làm tăng t_r(value R_p, C_p tỷ lệ thuận với t_r). Tuy nhiên điều này dẫn
  tới trường hợp nếu bạn dùng điện trở quá cao thì xung điện áp cạnh lên sẽ ko thể đạt biên độ điện áp 70%(đc tính làm
  mức high level), dẫn đến Slave sẽ nhận đc những giá trị ko xác định.

  ~Nếu giá trị R hoặc điện dung C_b giảm thì sẽ làm giảm t_r, điều này khá tốt, nhưng nếu bạn giảm R_p thì dòng điện tiêu
  thụ sẽ tăng lên, gây ảnh hướng đến các ứng dụng muốn tiết kiệm năng lượng

  ~Vì vậy bạn cần tính toán R_p(min) và R_p(max), sau đó sẽ chọn giá trị ở giữa 2 giá trị này để đạt đc R_p cần dùng

+ I2C bus capacitance - điện dung (C_b): là các tụ có tác dụng làm mức điện áp trên đường tín hiệu ko thể thay đổi ngay lập
tức mà sẽ nạp xả từ từ theo tính năng của tụ điện, khống chế mức điện áp đc sử dụng trên thiết bị
 ^Vd ta có 2 line SDA và SCL, trên SDA kết nối 4 thiết bị, mỗi thiết bị có kết nối với một tụ điện(gọi là tụ điện ký sinh)
  ~Các tụ điện ký sinh này nếu giá trị quá cao có thể là rào cản để ko làm tín hiệu tăng từ trạng thái 0 lên high level.
  ~Khi tính toán R_p thì bạn cũng phải lựa chọn C_b cho hợp lý

 ^Điện dung của bus là tập hợp các điện dung pin riêng lẻ wrt gnd, điện dung giữa sda và scl, tụ điện ký sinh,
 điện dung được thêm vào bởi các thiết bị treo trên bus, chiều dài bus(dây), vật liệu điện môi, v.v.
 ^Điện dung của bus giới hạn thời gian nối dây i2c của bạn và số lượng thiết bị bạn có thể kết nối trên xe buýt
 ^Bạn phải kiểm tra bảng thông số kỹ thuật của con chip để lựa chọn C_b tối đa cho phép ở từng chế độ I2C

+ Phải tính toán điều này vì mỗi Master sẽ kết nối nhiều Slave khác nhau và mỗi Slave lại có các giá trị điện áp, tần số,
điện dung khác nhau. Nên ngoài chọn các thiết bị có hỗ trợ i2C thì cũng cần kiểm tra thông số kỹ thuật của Slave

+Bài tập: Tính toán value R_p(select) khi sử dụng I2C ở chế độ Fast mode với C_b = 150pF, VCC = 3.3V
 R_p(min) = (3.3 -0,4) / (3*10^-3) = 996 Ohm
 R_p(max) = 300 *10^-9 / (0.8473 x 150*10^-12) = 2.3 KOhm
 => 996 Ohm < R_p(select) <= 2.3 KOhm => Thường chọn mức R_p = 2.2 KOhm

+ Lưu ý: bạn có thể cấu hình t_r bằng cách cấu hình thanh ghi TRISE(I2C_TRISE 27.6.9)

*==I2C rise time calculation(V199)
+ Tiến hành cấu hình value t_r

*===I2C Exercise(S58)
*==Exercise: Introduction - Giới thiệu(V200)
+ Bài tập: I2C Master(STM32 Discovery) and I2C Slave(Arduino board) communication
When Button on the STM32 board (master) is pressed, master should send data to the Arduino board(slave). The data received
by the Arduino board will be displayed on the serial monitor terminal of the Arduino IDE
 1. Use I2C SCL = 100 kHz(Standard Mode)
 2. Use external pull up resistor(3,3kOhm) for SDA and SCL line
 Note: if you do not have external pull up resistor, you can also try with activating the STM32 I2C pin is terminal pull
 up resistor.

 Giao tiếp I2C Master(STM32 Discovery) và I2C Slave(board Arduino)
Khi nhấn Nút trên bo mạch STM32 (Master), chủ nên gửi dữ liệu đến bo mạch Arduino (Slave). Dữ liệu nhận được của bo mạch
Arduino sẽ được hiển thị trên serial monitor terminal của Arduino IDE
  1. Sử dụng I2C SCL = 100 kHz (Chế độ tiêu chuẩn)
  2. Sử dụng điện trở kéo lên bên ngoài (3,3kOhm) cho đường SDA và SCL
  Lưu ý: nếu bạn không có điện trở kéo lên bên ngoài, bạn cũng có thể thử kích hoạt chân STM32 I2C là đầu cuối kéo
  điện trở lên.

  ^STM32 board: SCL(PB6), SDA(PB9) -> Cần thiết lập Alternate Function
   Nối mỗi nhánh SDA và SCL line với Pull-Up resistor 3.3kOhm, đầu còn lại nối Vdd 3V
  ^Sử dụng Logic Level convertor làm trung gian
  ^Arduino board: A5(SCL),A4(SDA)
  ^Sơ đồ nối dây: V200 6p00s

+ External pull up resistor calculation
 R_p(max) = t_r / (0.8473 * C_b)
 ^Theo datasheet: UM10204 -> Table. Characteristics of SDA and SCL bus lines thì t_r=1000ns, C_b=400pF
  R_p(max) = (1000 * 10^-9) / (0.8473 * 400*10^-12) = 3 kOhm (Standard Mode)
  -> Giá trị điện trở kéo lên có thể sử dụng là 3.3kOhm hoặc tối đa là 4.7kOhm

*==Exercise: Coding Part1,Par2(V201,V202)
+ Triển khai code file "08_i2c_master_tx_testing.c"

*==Exercise: Testing(V203)
+ Code bài chưa tối ưu nên có một số vấn đề xảy ra khi gửi dữ liệu
 ^Sau khi kiểm tra: có sự gián đoạn trên SDA line(PB9), Nguyên nhân là PB9 pin trên STM32407 Board đang cùng thực hiện
 một chức năng khác của mạch trên board dẫn đến đôi lúc PB9 busy. Cách khắc phục là chọn một pin khác(ko thực hiện chức
 năng trên board- not busy) để cấu hình SDA line -> Sau khi tra datasheet chọn PB7

 ==>Việc sử dụng các phần mềm hoặc osciloscope để quan sát mạch tín hiệu là rất quan trọng

*===I2C Driver API: I2C Master Receive Data(S59)
*==I2C Tranfer Sequence Diagram for Master Receiving Data(V204)
+ Trình bày làm cách nào để Master nhận dữ liệu từ Slave bằng giao tiếp I2C
+Sơ đồ khối việc Master nhận dữ liệu từ Slave:
			 S|___|Address|A|___| Data1 | A(1) | Data2 | A |___....| DataN | NA |P
			   EV5|		    |EV6|              | EV7|      |EV7|   | EV7|       |EV7 |

 ^Legend: S=Start, SR= Repeated start, P=Stop, A=Acknowledge, NA=Non-Acknowledge
 ^EVx: Event(cần hoàn thành sự kiện để tiếp tục chương trình)(with interrupt if ITEVFEN = 1)
 ^EV5: SB=1 ,Clear by reading SR1 register followed by writting DR register
 Hoàn thành(Clear) sự kiện bằng cách đọc thanh ghi SR1, sau đó ghi vào thanh ghi DR

 ^Address 8bit: Master Addrress[7:1] + r/nw bit[0]=0 (read=1 <==> receive data)

 ^EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2. In 10-bit master receiver mode, this sequence
 should be followed bt writing CR2 with START=1. In case of the reception of 1 byte, the Acknowledge disable must be
 performed during EV6 event, i.e before clearing ADDR flag.
 Address ở đây là địa chỉ Slave muốn gửi đến, nếu đúng địa chỉ, Slave sẽ trả về tín hiệu ACK và set ADDR=1, nếu ko đúng
 sẽ ko set ADDR. Sau đó thực hiện điều kiện tiếp tục -> Xóa sự kiện bằng cách đọc thanh ghi SR1, sau đó đọc SR2.
 Trong chế độ Master là nhận data 10 bit, trình tự này phải được theo sau bằng cách ghi CR2 với START=1.
 Trong trường hợp nhận 1 byte, việc tắt "the Acknowledge" phải được thực hiện trong sự kiện EV6, tức là trước khi xóa cờ ADDR.

 ^EV7: RxNE=1, Cleared by reading DR Register - Xóa bằng cách đọc thanh ghi DR

 ^EV7_1: RxNE=1, Cleared by reading DR Register, program ACK=0 and STOP request
 Xóa bằng cách đọc thanh ghi DR, lập trình ACK=0 và yêu cầu STOP condition

 ^A(1): A=Acknowledge(có dữ liệu từ Slave->Master) hoặc NA=Non-Acknowledge(không có dữ liệu từ Slave->Master)

 ^Lưu ý:
  1. If a single byte is received, it is NA - Nếu nhận được một byte đơn thì đó là NA
  2. The Ev5,EV6 and EV9 event stretch SCL low until the end of the corresponding software sequence
  Sự kiện Ev5, EV6 và EV9 kéo dài stretch SCL ở mức thấp cho đến hết chuỗi phần mềm tương ứng
  3. The EV7 event stretches SCL low if the software sequence is not completed before the end of the next byte reception
  Sự kiện EV7 kéo dài SCL xuống mức thấp nếu chuỗi phần mềm không được hoàn thành trước khi kết thúc quá trình nhận byte tiếp theo
  4. The EV7_1 software sequence must be completed before the ACK pulse of the current byte transfer
  Chuỗi phần mềm EV7_1 phải được hoàn thành trước xung ACK của quá trình truyền byte hiện tại

+ Trường hợp chỉ có 1 byte data đc gửi đến Master
			 S|___|Address|A|___| Data1 | NA |P
			   EV5|		    |EV6|            | EV7|
 ^A: lúc này là Ack, thông báo có data đc gửi đến
 ^NA: Nack,thông báo đã ko còn data đc gửi đến master đông thời sẽ làm Stop Condition

+ Trường hợp chỉ có n byte data đc gửi đến Master
			S|___|Address|A|___| Data1 |A| Data2|A|.....| DataN| NA |P
			  EV5|		   |EV6|         | EV7|         | EV7_1|    | EV7|
 ^Thấy rằng nếu còn data muốn gửi đến Master thì phải cấu hình bit tiếp sau data đã gửi là bit Ack.
 ^Khi không còn data muốn gửi đến Master thì cần cấu hình bit cuối của data cuối cùng đã gửi là bit Nack đồng thời kích hoạt
 Stop Condition để kết thúc chu trình gửi data đến master.
 Sơ đồ: ....| Data(N-1)| A| DataN| NA |P
 -> Nghĩa là trong xử lý "| Data(N-1)| A|" thì cần cấu hình Bit Nack và Stop Condition trước khi vào gửi byte cuối cùng để byte
 cuối cùng có cấu trúc bit cuối là Nack và có điều kiện STOP Condition.

*==Assignment: I2C master receive data API  implementation(V205)
+ Tạo cấu trúc cơ bản của hàm gửi data từ Slave đến Master

*==Implemetation of I2C Master Receive Data API: Part1,Part2(V206,V207)

*===I2C Exercise(S60)
*==Exercise: Reading data from the I2C Slave(V208)
+ Phần cứng cần dùng: Arduino Board, ST Board, dây dẫn, Test Board, 2*pull-up Resistor 4.7KOhm
+ Mô hình: ST Board <====> Arduino Board
 Ở St Board sẽ in các data nhận đc thông qua hàm printf cho ST sử dụng semihosting

+ Exercise: I2C Master(STM) and I2C Salve(Arduino) communication
When button on the master is pres, master should read and display data from Arduino Slave connected. First master has get the
length of the data from the slave to read subsequent(tiếp theo) data from the slave
 1. Use I2C SCL = 100KHz(Standard Mode)
 2. Use internal pull resistor for SDA and SCL lines
Bài tập: Giao tiếp I2C Master(STM) và I2C Salve(Arduino)
Khi nhấn nút trên master, master sẽ đọc và hiển thị dữ liệu từ Arduino Slave được kết nối. Bước đầu tiên Master lấy được độ
dài của dữ liệu từ Slave để đọc dữ liệu tiếp theo từ Slave
  1. Sử dụng I2C SCL = 100KHz(Chế độ tiêu chuẩn)
  2. Sử dụng điện trở kéo bên trong cho đường SDA và SCL

 ^Quy trình đọc dữ liệu từ Arduino Slave:
  1. Master gửi mã lệnh 0x51 để đọc độ dài Len(1byte) của dữ liệu gửi từ salve
  vd: Đầu tiên, Master send data "S|<Slave_addr>1101000|<write>0|A|<commandcode>0x51|A|P" đến Slave
  Sau đó, Master receive data "S|<Slave_addr>1101000|<read>1|A|<data>Len|NA|P" từ Slave

  2. Master gửi mã lệnh 0x52 để đọc dữ liệu hoàn chỉnh từ Slave
  vd: Đầu tiên, Master send data "S|<Slave_addr>1101000|<write>0|A|<commandcode>0x52|A|P" đến Slave
  Sau đó, Master receive data "S|<Slave_addr>1101000|<read>1|A|<data1>|.....A|<dataN>|NA|P" từ Slave

*==Exercise: Coding Part1(V209)
*==Exercise: Coding Part2(V210)
+ Gỡ lỗi code , quan sát osciloscope và kiểm tra việc truyền dữ liệu có chính xác hay không
*==Exercise: Coding Part3(V211)
+ Thêm các hàm printf để hiển thị data do Master nhận đc
*==Exercise: Testing repeated start(V212)
+ Xem lại quá trình truyền data
+ Giải quyết vấn đề nhận data từ Slave: Cứ mỗi lần tạo Stop Condition thì sẽ ngừng kết nối MasterA<->Slave, mặc dù ko sai
nhưng nó sẽ tạo ra một thời gian trống chờ MasterA kết nối lại với Slave để tiếp tục công việc nhận data. Vấn đề ở đây xuất
hiện, vì có thời gian not Busy nên có thể một MasterB khác sẽ chiếm quyền bus line, dẫn đến gián đoạn trong việc nhận data
của MasterA, tạo ra độ trễ và ko nhận data đúng thời hạn. Dẫn đến công việc tổng thể bị sai số hoặc gây lỗi nghiêm trọng.
 ^Hỏi: Có cách nào để master kết nối vs Slave cho đến khi hoàn thành việc nhận data mà ko cần dùng Stop condition nhiều lần?
 ^Trả lời: Thay vì sử dụng Stop Condition ở mọi dòng thì đặt điều kiện đẻ chỉ sử dụng Stop Condition khi đã hoàn thành toàn
 bộ việc nhận data từ Slave(Các Start Condition sẽ đc lặp lại)
*/

