/*
 * tutorial_i2c_bus.c
 *
 *  Created on: Dec 17, 2023
 *      Author: Minh
 */

/**
*===I2C Introduction and Differentces with SPI(S48)
*==I2C Introduction and Differentces with SPI(V172)
+ I2C viết tắt của từ "Inter-Integrated Circuit"
+ I2C Protocol(Giao thức): Nó chỉ là một giao thức để đạt được giao tiếp dữ liệu nối tiếp giữa các mạch tích hợp (IC)
rất gần nhau. (nhưng giao thức này nghiêm túc hơn SPI vì các công ty đã đưa ra thiết kế một thông số kỹ thuật(design
a specification) cho việc giao tiếp sử dụng I2C)
+ Chi tiết giao thức I2C (cách gửi dữ liệu, cách nhận dữ liệu, cách thức kết nối(bắt tay - hand shaking) giữa người gửi
và người nhận, xử lý lỗi, ..) phức tạp hơn SPI. (Nói cách khác SPI là giao thức đơn giản so với I2C)

+ Different between SPI and I2C:
 ^Specifications(Thông số kỹ thuật):
  ~I2C là một protocols dựa trên đặc điểm kỹ thuật chuyên dụng chung đc thiết kế bởi hãng NXP
  Link: https://www.nxp.com/docs/en/user-guide/UM10204.pdf
  I2C có khả năng Multi-Master(Đa chủ) capable(bằng dây cab), nghĩa là việc phân xử giữa các master đc phần cứng(cable)
  đảm nhận một cách tự động.

  ~SPI: là một protocols không dựa trên đặc điểm kỹ thuật chuyên dụng, mà do các hãng thiết kế riêng để sử dụng. Có thể tìm
  một số Specifications của SPI của các hãng TI, Motorola,..
  SPI không có khả năng Multi-Master(Đa chủ) capable. Vì cậy trong SPI nó phụ thuộc vào các thiết kế hoặc nhà cung cấp vđk.
  Vd: Có thể sử dụng ngoại vi SPI vs STM32 với cấu hình multi-master nhưng việc phân xử giữa nhiều master phải đc xử lý ở
  phần mềm với code programming.

 ^ACKing:
  ~I2C: Phần cứng I2C tự động ACKs mỗi byte nhận được
  ~SPI: Phần cứng SPI không hỗ trợ bất cứ tự động ACKing nào, nếu muốn phải dùng phần mền thực hiện

 ^Pins:
  ~I2C: Số lượng chân đc sử dụng cho giao tiếp I2C chỉ cần 2 pins là SCL(Sender Clock) và SDA(Sender Data)
  Vd: nếu có 2 Master và 2 Slave thì số chân pin là 2 chân, 1 chân của các Master và các Slave đều nối SCL, chân còn lại của
  các Master và các Slave sẽ nối vs SDA
  Vcc-----------|-----------|--------------------------
				|			|
				R1			R2
	SCL-----|---|-----------|---|------------|-----------|-----------
	SDA-----|------|--------|---|---|--------|--|--------|--|--------
			|	   |			|   |		 |  |		 |	|
			Master-1			Slave-1		Slave-2		Master-2

   Các chân SCL và SDA đc kéo về VCC bằng cách dùng điện trở kéo lên

  ~SPI: Số lượng chân cơ bản(ko tính trường hợp rút gọn) cho giao tiếp SPI là 4 chân hoặc nhiều hơn nếu sử dụng Multi-Slave
  Vd: nếu có 1 Master và 3 Slave thì số chân cần sử dụng là 6 chân(MISO,MOSI,SCK,IO1,IO2,IO3)

 ^Addressing:
  ~I2C: I2C Master trò chuyện(talks) giao tiếp với Slaves dựa trên địa chỉ của Slave đó.
  Vd: nếu có 2 Master và 2 Slave thì các Master sẽ giao tiếp với Slave dựa trên địa chỉ mà Slave đó đc cấu hình trước

  ~SPI: trong giao tiếp này, một chân của Master đc sử dụng để làm việc select Slave cần kết nối

 ^Communication:
  ~I2C: giao tiếp sử dụng tiêu chuẩn "Half-duplex"
  ~SPI: giao tiếp sử dụng tiêu chuẩn "Full-duplex"

 ^Speed: vì I2C sử dụng half-duplex nên tốc độ giao tiếp sẽ không bằng SPI sử dụng full-duplex
  ~I2C: tốc độ tối đa max speed của I2C là 4Mhz đc sử dụng ở cấu hình "ultra speed plus" ở một số vđk đc thiết kế chuyên dụng.
  Nếu sử dụng STM microcontroller thì tốc độ max chỉ có thể đạt 400KHz
  ~SPI: tốc độ tối đa sẽ là f_CLK/2, nghĩa là "peripheral clock" là 20MHz thì tốc độ tối đa giao tiếp SPI có thể là 10MHz
  -> Chuẩn SPI thực sự rất nhanh so với chuẩn I2C

 ^Slave is control over serial clock: Slave được điều khiển trên serial clock
  ~I2C: Slave có thể khiến Master chờ đợi(đưa master vào trạng thái 'waiting') bằng cách giữ xung clock nếu nó đang ở trạng
  thái busy, đó là nhờ tính năng "to clock stretching - kéo dài xung clock" của i2c. Hầu hết các dòng chip đều hỗ trợ tính
  năng này.
  ~SPI: Các Slave không có quyền kiểm soát serial clock, các lập trình viên sẽ sử dụng mẹo lập trình để có thể thực hiện tính
  năng "clcok stretching" vs chuẩn SPI.

+ Data rate: Tốc độ dữ liệu
 ^Data rate(Số bits đc truyền từ người gửi đến người nhận trong 1 giây) trong I2C ít hơn rất nhiều so với SPI(Đây là lợi thế
 lớn nhất của chuẩn SPI, SPI thực sự truyền dữ liệu rất nhanh. Vì vậy, đây là lý do tại sao bạn không thể thay thế tất cả các
 ứng dụng dựa trên chuẩn SPI thành chuẩn I2C)
  ~Vd: giả sử để truyền dữ liệu, bạn yêu cầu sử dụng "Data rate" rất cao là 10mbs. Lúc này chuẩn I2C không thể đáp ứng mức
  tốc độ dữ liệu này nên bạn phải sử dụng chuẩn SPI.
  ~I2C sẽ phù hợp vs các hệ thống yêu cầu tốc độ dữ liệu ở mức thấp như thu thập dữ liệu từ các cảm biến

 ^Ví dụ bạn có một vđk dựa trên dòng STM32F4xx và nó có "peripheral clock" là 40MHz thì với chuẩn I2C bạn có thể đạt "Data
 rate" ở tốc độ 400Kbpz nhưng với chuẩn SPI bạn có thể đạt "Data rate" lên tới 20Mbps
 Trong vd trên thì chuẩn SPI đã nhanh hơn chuẩn I2C là 50 lần

+ Các thuật ngữ(terminology) dùng cho định nghĩa I2C:
 ^Tranmitter: thiết bị gửi data đến bus
 ^Receiver: thiết bị nhận data từ bus
 ^Master: thiết bị bắt đầu(initiates) quá trình truyền, tạo(generates) tín hiệu đồng hồ và kết thúc(terminates) quá trình truyền
 ^Slave: thiết bị được định địa chỉ bởi Master
 ^Multi-master: nhiều thiết bị Master có thể cố gắng điều khiển bus cùng lúc mà không làm hỏng thông báo
 ^Arbitration: Thủ tục để đảm bảo rằng, nếu có nhiều hơn một Master đồng thời cố gắng điều khiển bus thì chỉ một người được phép làm
 như vậy và tin thông báo truyền thành công không bị hỏng. Thủ tục này do phần cứng I2C tự động xử lý
 ^Synchronization: quy trình đồng bộ hóa tín hiệu đồng hồ của hai hoặc nhiều thiết bị

*==I2C SDA and SCL signal(V173)
+ Sơ đồ nối dây:
  Vcc-----------|-----------|--------------------------
				|			|
				R1			R2
	SCL-----|---|-----------|---|------------|-----------|-----------
	SDA-----|------|--------|---|---|--------|--|--------|--|--------
			|	   |			|   |		 |  |		 |	|
			Master-1			Slave-1		Slave-2		Master-2

+ Cả SDA và SCL đều là các đường hai chiều được kết nối với điện áp cung cấp dương thông qua điện trở kéo lên(pull-up resistor).
Khi bus rảnh rỗi, cả hai tuyến đều được giữ ở mức cao.
+ Các tầng đầu ra của thiết bị kết nối với bus phải có cấu hình open-drain hoặc open-collector
+ Điện dung của bus(The bus capacitance) giới hạn số lượng giao diện(interface) được kết nối với bus

+ Pins I2C:  đc sử dụng cho cả dây SDA và SCL
 ^Pins cho giao thức I2C phải ở chế độ Open-Drain(ko dùng P-Mos và sử dụng N-Mos) cùng với điện trở kéo lên để có thể đạt đc hai
 trạng thái cao hoặc thấp.
 ^Bất cứ khi nào bạn muốn kết nối thiết bị với I2C Bus thì cả hai chân đều phải ở chế độ Open-Drain
 ^Để bus I2C hoạt động bình thường, giá trị điện trở kéo lên phải được tính theo công thức I2C
 ^Lưu ý: Khi bus ko hoạt động thì cả  dây SDA và SCL đều đc kéo về +Vdd
 ^Mẹo kiểm tra: Bất cứ khi nào bạn gặp phải vấn đề về I2C, hãy thăm dò dòng SDA và SCL sau khi khởi tạo I2C. Nó phải được giữ ở
 mức CAO-HIGH (3,3V hoặc 1,8V tùy thuộc vào mức điện áp IO của bo mạch của bạn). Nghĩa là bạn lấy đồng hồ Voltage kế kiểm tra điện
 áp giữa SDA với GND và SCL với GND.

*===I2C Mode(S49)
*==I2c Standard and Fast Mode(V174)
+ Có 4 chế độ I2C:
	Mode					Data rate					Notes
Standard Mode			Up to 100 Kbits/sec			Supported by SYM32F4x
Fast Mode				Up to 400Kbits/sec			Supported by STM32F4x
Fast Mode +				Up to 1Mbits/sec			Supported by some STM32F4x MCUs(Refer RM)
Hight Speed Mode		Up to 3.4 Mbits/sec			Not Supported by F4x

+ Standard Mode: là chế độ giao tiếp mà data rate(tốc độ truyền dữ liệu) có thể đạt tới đa 100kbits/sec
 ^Là chế độ đầu tiên được giới thiệu khi thông số kỹ thuật i2c đầu tiên được phát hành
 ^Tuy nhiên, các thiết bị Chế độ Tiêu chuẩn không tương thích trở lên. Nghĩa là không thể giao tiếp với các thiết bị ở
 Fast mode hoặc chế độ cao hơn.
 Vd: cảm biến của bạn chỉ hỗ trợ "Standard Mode" thì không thể giao tiếp vs Master ở Fast Mode
 ^Hầu hết các cảm biến cơ bản như RTC, cảm biến nhiệt độ đều hỗ trợ "Standard Mode"(vì chúng ko cần truyền dữ liệu tốc độ nhanh)

+ Fast Mode: là một chế độ của giao thức I2C
 ^Thiết bị giao tiếp ở "Fast Mode" có thể receive và transmit data với tốc độ lên đến 400kbits/sec
 ^Các thiết bị trong "Fast Mode" có thể tương thích hướng xuống, nghĩa là nó có thể giao tiếp ở chế độ "Standard Mode" với
 tốc độ từ 0 đến 100kbits/sec I2C Bus system
 ^Tuy nhiên, các thiết bị "Standard Mode" không tương thích trở lên. Chúng không nên được kết hợp trong hệ thống I2C -Bus ở
 "Fast Mode" vì chúng không thể tuân theo tốc độ truyền cao hơn và các trạng thái không thể đoán trước sẽ xảy ra. Vì vậy, để đạt
 được tốc độ truyền dữ liệu lên tới 400kbits/giây, bạn phải đặt các thiết bị I2C ở "Fast Mode"

*===Understanding I2C Protocol(Giao thức)(S50)
*==I2C Protocol explanation - Giải thích về giao thức I2C(V175)
+ Mô hình data trên SDA line of I2C:
 S-A7-A6-A5-A4-A3-A2-A1-R/nW-ACK1-D7-D6-D5-D4-D3-D2-D1-D0-ACK2-P

 ^S: gọi là Start Condition - Điều kiện bắt đầu truyền dữ liệu (Master quản lý)
 ^A7-A6-A5-A4-A3-A2-A1-R/nW: đây là phase 8bit
  ~A7-A6-A5-A4-A3-A2-A1: địa chỉ address của thiết bị truyền/nhận Slave(7bit)
  ~Nếu R/nW='1' thì yêu cầu READ data Slave -> Master(đọc data từ Slave)
  ~Nếu R/nW='0' thì truyền data WRITE, Master -> Slave(nhận data từ Master)

 ^ACK1(hoặc NACK bit): bit xác nhận địa chỉ có khớp với địa chỉ Slave muốn truyền/nhận hay không và đc gửi đến Master
 ^D7-D6-D5-D4-D3-D2-D1-D0: nội dung data(1byte-8bit)
 ^ACK2(hoặc NACK bit): bit xác nhận Slave đã nhận đc dữ liệu hay chưa và đc gửi đến Master(R/nW='0')
 hoặc bit xác nhận Master đã nhận đc dữ liệu hay chưa và đc gửi đến Slave(R/nW='1')
 ^P: gọi là Stop Condition - Điều kiện ngừng truyền dữ liệu (Master quản lý)

+ Việc truyền dữ liệu luôn đc khởi tạo bởi Master trên SDA line
+ Mỗi byte đặt trên dòng SDA phải dài 8 bit
+ Mỗi byte phải được theo sau bởi Bit xác nhận
+ Dữ liệu được truyền với Bit quan trọng nhất (MSB) trước tiên chỉ ra yêu cầu

*==I2C START and STOP Condition(V176)
+ Tất cả các Transaction(Giao dịch truyền hoặc nhận) trong I2C đều bắt đầu bằng START(S) và kết thúc bằng STOP(P)
+ START Condition: Quá trình chuyển đổ dạng sóng từ Cao-High xuống Thấp-Low trên SDA line trong khi SCL Line ở mức Cao-High sẽ
đc xác định làm điều kiện START
+ STOP Condition: Quá trình chuyển đổi dạng sóng từ Thấp-Low lên Cao-High trên SDA line trong khi SCL Line ở mức Cao-High sẽ
đc xác định làm điều kiện STOP(Nghĩa là lúc này cả SDA và SCL đều đc Pull-Up lên High)

+ Giải thích mô hình sóng trong I2C: V176 - 3p45s
+ Khi đã truyền xong một mô hình data trên SDA line, Nếu Master muốn bắt đầu lại một vòng truyền data mới thì nó ko thiết lập
điều kiện STOP mà sẽ làm tiếp với một bit START

+ Chú ý:
 ^START và STOP Condition đều luôn do Master tạo ra
 ^Bus system sẽ chuyển trạng thái Busy(Bận) sau khi nhận đc START Condition
 ^Bus system sẽ chuyển lại trạng thái Free(rảnh) sau khi nhận đc STOP Condition
 ^Khi Bus System đang ở trạng thái Free thì một Master khác có thể yêu cầu Bus để giao tiếp data
 ^Bus sẽ luôn ở trạng thái Busy nếu đc lập lại bằng một lệnh START Condition thay vì STOP Condition
 ^Hầu hết MCUs I2C peripheral đều hỗ trợ cả Master và Slave. Bạn sẽ ko cần cấu hình mode bởi vì nếu Peripheral tạo START condition
 thì nó tự động trở thành Master và khi tạo STOP Condition, nó sẽ quay trở lại chế độ Slave
 Vd: ta có một cảm biến(hầu hết cảm biến là Slave mode), và một MCUs. Bạn ko cần thiết lập đâu là thiết bị Master đâu là thiết
 bị Slave. Khi bạn sử dụng I2C peripheral thì thiết bị nào tạo START Condition là Master, và khi tạo ra STOP Condition sẽ tự
 động quay lại Slave Mode. Nghĩa là ban đầu xem như hai thiết bị đều là Slave Mode, và khi có sự kiện giao tiếp thì mới thiết lập
 thiết bị Master mode và thiết bị Slave mode

*==ACK và NACK(V177)
+ I2C Protocol: Address Phase((8bit)
 ^Vùng 7bit nằm phía sau bitS(START) đc gọi là Slave Address
 ^Vùng 1bit phía sau cùng đc gọi là R/W

+ I2C Protocol: ACK/NACK
 ^Bit nằm phía ngay sau bit "R/W" đc gọi là ACK bit(hoặc NACK bit)
 ^Tại vị trí này(cạnh xung thứ 9 SCL line):
  ~Nếu SCL line ở mức High và SDA line ở mức Low thì nó là bit ACK(đc chấp nhận), xác nhận địa chỉ chính xác
  ~Nếu SCL line ở mức High và SDA line ở mức High thì nó là bit NACK(ko đc chấp nhận), xác nhận địa chỉ sai

+ Lưu ý:
 ^Việc xác nhận diễn ra sau mỗi byte
 ^Bit xác nhận cho phép máy thu(received) báo hiệu cho máy phát(transmitter) rằng byte đã được gửi thành công và một byte khác
 có thể được gửi
 ^Master tạo ra tất cả các xung đồng hồ, bao gồm cả xung đồng hồ thứ chín được xác nhận

*==I2C Data validity(tính hợp lệ)(V178) - Tìm hiểu về tính hợp lệ của dữ liệu đc sử dụng
+ Dữ liệu trên SDA line phải ổn định trong thời gian xung clock ở SCL line ở mức High
+ Trạng thái High hoặc Low của dữ liệu trên SDA line có thể bị thay đổi nếu tín hiệu Clock trên SLC line ở mức Low
+ Chính vì sự bất ổn định khi clock ở mức Low nên khi Clock ở mức High thì bất kỳ quá trình chuyển đổi trạng thái nào(từ High->Low
hoặc từ Low->High) đều đc xem như lúc đó là START Condition hay STOP Condition -> Việc giao tiếp dữ liệu(nếu đc xác nhận Ack) sẽ
luôn thực hiện khi xung Clock của SLC line ở mức High
-> Dữ liệu đc xem là hợp lệ khi đc thực hiện khi xung Clock của SCL line ở mức High

*===I2C Master and Slave communication(S51)
*==Example of master writing/reading to slave(V179)
+ Ví dụ về việc truyền dữ liệu từ master sang slave:
SCL: Mỗi vị trí trên SDA tương ứng với 1 chu kỳ xung Clock
SDA: S-A7-A6-A5-A4-A3-A2-A1-W-ACK1-D7-D6-D5-D4-D3-D2-D1-D0-D7-D6-D5-D4-D3-D2-D1-D0-ACK2-P
 ^D7-D6-D5-D4-D3-D2-D1-D0-D7-D6-D5-D4-D3-D2-D1-D0: 2 byte dữ liệu mà Master sẽ gửi đến Slave
 ^A7-A6-A5-A4-A3-A2-A1: byte dữ liệu địa chỉ của slave
 ^ACK: byte phản hồi(slave->master)
 ^W: yêu cầu Writting
 ^P: điều kiện dừng truyền dữ liệu, do master tạo ra, nếu có n byte dữ liệu truyền đi thì khi truyền xong master sẽ tự
 động tạo điều kiện dừng này.

+ Ví dụ về việc đọc dữ liệu của master từ slave:
SCL: Mỗi vị trí trên SDA tương ứng với 1 chu kỳ xung Clock
SDA: S-A7-A6-A5-A4-A3-A2-A1-R-ACK1-D7-D6-D5-D4-D3-D2-D1-D0-ACK2(NACK)-P
 ^Slave sẽ gửi tổng cộng 3 byte dữ liệu gồm, byte đầu tiên(D7-D6-D5-D4-D3-D2-D1-D0) đc gửi đến master, một ACK2 đc gửi
 từ Master xác nhận việc read bắt đầu, tiếp theo Slave sẽ gửi byte(A7-A6-A5-A4-A3-A2-A1) để xác nhận địa chỉ cho
 Master, cuối cùng sẽ gửi byte thứ 3(D7-D6-D5-D4-D3-D2-D1-D0) là data Msster muốn đọc.
 ^Trong trường hợp còn data muốn đọc thì tiếp tục sẽ có một ACK2 đc gửi từ Master, nếu hết thì Slave sẽ tạo ra một NACK
 gửi đến Master thông báo kết thúc gửi data cho Master đọc. Master sẽ tạo P để dừng việc đọc/truyền dữ liệu

 ^Lưu ý: Bất cứ khi nào Slave nhận được ACK từ Master, đó là dấu hiệu cho thấy Slave gửi thêm một byte cho Master
 Bất cứ khi nào Master nhận được ACK từ Slave, đó là dấu hiệu cho thấy Master đã nhận được dữ liệu thành công

*==Understanding repeated START condition(V180)
+ Nghĩa là bất đầu lại mà không cần điều kiện dừng(Start again without Stop). Việc này giúp cho Master đang giao tiếp với
Slave ko cần tạo ra điều kiện dừng sau mỗi lần giao tiếp Read hoặc Write tạo tính liên tục trong xử lý data

+ Giả sử bạn có một vđk  MCU và một EEPROM tại địa chỉ 0x45 đc kết nối với nhau qua Bus I2C
 ^Ta sẽ thực hiện cả việc Read và việc Write trên địa chỉ 0x45. Theo nguyên tắc I2C thông thường thì khi ta thực hiện việc
 Read xong(hoặc ngược lại là Write) thì sẽ tạo ra điều kiện dừng. Việc tạo ra điều kiện dừng làm lúc này Bus đang Empty,
 nghĩa là tạo cơ hội cho peripheral khác chiếm bus để sử dụng truyền data. Nếu trường hợp có peripheral khác chiếm Bus,
 thì việc Write data trên địa chỉ 0x45 sẽ phải chờ cho đến khi Bus I2C trống.
 ^Như vậy điều này sẽ làm ngừng tính liên tục trong việc đọc/viết data tại địa chỉ 0x45.
 -> Repeated Start Condition đc sử dụng để giải quyết vấn đề trên

 ^Cách làm: ngay khi kết thúc việc read(hoặc write) thì trên SDA tạo lại một điều kiện START mới(SCL_High, SDA_Low) để Bus
 ko empty và tiếp tục cho việc write(read) data

*===STM32 I2C function Block Diagram(S52)
*==I2C functional block diagram(V181)
+ Tìm hiểu về sơ đồ khối I2C
+ Tài liệu Datasheet: RM407 -> 27. Inter-integrated circuit (I2C) interface -> Figure 239. I2C block diagram for STM32F40x/41x
+ Noise filter: bộ lọc nhiễu tín hiệu, làm min tín hiệu trên SCL và SDA
+ Thanh ghi "Shift Register" của stm32f407x hỗ trợ lến đến 16bit
+ Thanh ghi "Data Register": nơi lưu dữ liệu đã read hoặc write và giao tiếp với "Data Shift Register"
+ I2C Bus sẽ đc kết nối với Processor thông qua APB1 Bus(I2C1,I2C2,I2C3)
+ Own address register: thanh ghi dùng để lưu địa chỉ nếu thiết lập MCU là một Slave
+ SCL sẽ đc nói với khối "Clock Control", khối này đc điều khiển bởi thanh ghi CCR(Clock Control Register)
+ CR1 Và CR2 là các thanh ghi điều khiển thông số của Periperal I2C
+ SR1 và SR2 là các thanh ghi thể hiện trạng thái của Periperal I2C
*/
