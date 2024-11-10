/*
 * tutorial_i2c_interrupt.c
 *
 *  Created on: Jan 9, 2024
 *      Author: Minh
 */
/**
*===I2C Interrupt and IRQ bumbers(S61)
*==I2C IRQs and Interrupt discussion(V213)
+ Trong I2C communication thì các sự kiện sau đây có thể làm Interrupt của Processor
 Interrupt Events									Event flag				Enable control bit
Start bit sent(Master)									SB
Address sent(Master) or Address matched(Slave)			ADDR
10-bit header sent(Master)								ADDD10					ITEVFEN
Stop received(Slave)									STOPF
Data byte tranfer finished								BTF
----------------------------------------------------------------------------------------------------
Receive buffer not empty								RxNE					ITEVFEN and ITBUFEN
Transmit buffer empty									TxE
----------------------------------------------------------------------------------------------------
Bus error												BERR
Arbitration los(Master)									ARLO
Acknowledge failure										AF
Overrun/Underrun										OVR						ITRRREN
PEC error												PECERR
Timeout/Tlow error										TIMEOUT
SMBus Alert												SMBALERT

 ^Cột "Enable control bit- bit kiểm soát", đây là các bit cần kích hoạt trước khi kích hoạt các bit cột "Event flag"
 Việc kích hoạt các bit cột "Enable control bit" sẽ ko làm interrupt of processor
 ^Các bit cột "Event flag" đc kích hoạt(set made 1) là tiền đề để thực hiện Interrupt of Processor
 Nghĩa là các sự kiện khi thực hiện I2C communication có thể làm Interrupt of Processor

+ Interrupt of I2C communication: chia làm 2 loại là Interrupt_Event và Interrupt_Error
 ^Input Interrupt_Event là 2 line tượng trưng như là bit cột Event flag(Event)=1 và Enable control bit(ITEVFEN)=1
  Output Interrupt_Event line sẽ đc thiết kế kết nối với ngõ IRQn_3x của NVIC Reigster của Processor(ARM Cortex M4)

 ^Input Interrupt_Event là 2 line tượng trưng như là bit cột Event flag(Error)=1 và Enable control bit(ITRRREN)=1
  Output Interrupt_Error line sẽ đc thiết kế kết nối với ngõ IRQn_3x+1 của NVIC Reigster của Processor(ARM Cortex M4)

+ I2C Interrupt the Processor: Tham khảo Datasheet_stm32f407x: Vector Table (I2Cx_EV và I2Cx_ER)
 ^Interrupt_Event(I2C1) line sẽ đc thiết kế kết nối với ngõ IRQn_31 của NVIC Reigster của Processor(ARM Cortex M4)
  Interrupt_Error(I2C1) line sẽ đc thiết kế kết nối với ngõ IRQn_32 của NVIC Reigster của Processor(ARM Cortex M4)
 ^Interrupt_Event(I2C2) line sẽ đc thiết kế kết nối với ngõ IRQn_33 của NVIC Reigster của Processor(ARM Cortex M4)
  Interrupt_Error(I2C2) line sẽ đc thiết kế kết nối với ngõ IRQn_34 của NVIC Reigster của Processor(ARM Cortex M4)
 ^Interrupt_Event(I2C3) line sẽ đc thiết kế kết nối với ngõ IRQn_35 của NVIC Reigster của Processor(ARM Cortex M4)
  Interrupt_Error(I2C3) line sẽ đc thiết kế kết nối với ngõ IRQn_36 của NVIC Reigster của Processor(ARM Cortex M4)

*==I2C Error and Importance of BUSY flag(V214)
+ Tìm hiểu các lỗi khác nhau có thể xảy ra trong giao tiếp I2C
 ^Bus Error: Lỗi này xảy ra khi Interface Application phát hiện xung cạnh lên hoặc xung cạnh xuống trên SDA line
 trong khi xung trên SCL line đang ở mức high. Điều này xảy ra ở một vị trí không hợp lệ trong quá trình chuyển byte.
 Nghĩa là: trong qus trình truyền byte thì nếu có bất kỳ chuyển đổi nào không hợp lệ xảy ra trên SDA line thí lỗi
 "Bus Error" đc kích hoạt và "Bus Error of Flag" trong thanh ghi trạng thái(SR) sẽ đc thiết lập. Và nếu Interrupt
 đc bật thì ngắt này sẽ đc kích hoạt trên dòng IRQn ngắt của Interrupt_Error.

 ^Arbitration Loss Error(Lỗi phân xử thất bại): Lỗi này xảy ra khi Interface Application mất quyền Arbitration(phân
 xử) của Bus sang cho một Master khác. Nghĩa là việc chuyển giao bus từ Master này sang Master khác để thực hiện
 việc truyền data bị lỗi và lỗi này chỉ xảy ra trong trường hợp đa Master.

 ^ACK Failure Error(Lỗi ACK thất bại): Lỗi này xảy ra khi không có bit ACK nào được trả về cho byte đã gửi

 ^Overrun Error(Lỗi tràn): Xảy ra trong quá trình tiếp nhận, khi một byte mới được nhận và thanh ghi dữ liệu chưa
 được đọc và byte mới nhận được bị mất.
 Vd: ta có một Data Register(DR) và Shift Register(SR) in I2C, data sẽ đc truyền từ Shift Register->Data Register
 Khi data(1byte) đang ở trong Data Register thì bằng một vấn đề nào đó(vd: bus line busy do Master khác đang chiếm,...)
 byte này ko đc đọc nên vẫn nằm trong DR.
 Khi có một byte mới vào SR, nhưng vì trong DR vẫn còn chứa byte cũ nên byte mới từ SR ko thể gửi vào DR.
 Khi có một byte mới thứ 3 thì lúc này do SR lấn DR đang còn lưu các byte trước đó nên ko thể chứa byte mới thứ 3.
 Điều này sẽ dẫn đến "Overrun Error"
 Cách khắc phục: lỗi này sẽ ko xảy ra nếu tính năng I2C Stretching đc bật(cờ BTF đc set), nghĩa là khi đó dù cả DR
 và SR đều đang lưu byte nhưng vì BTF=1 sẽ tự động kéo dài xung clock(cả Slave và Master sẽ đều chuyển sang trạng
 thái chờ nên gửi hoặc nhận byte mới thứ 3) cho đến khi Processor thực hiện đọc data ở DR rồi SR.

 ^Under-run Error: Xảy ra khi đang truyền khi một byte mới phải được gửi và thanh ghi Data Register chưa được ghi
 và cùng một byte được gửi hai lần.

 ^PEC Error: Xảy ra khi có CRC không khớp, nếu bạn đã bật tính năng CRC

 ^Time-Out Error: Xảy ra khi Master hoặc Slave kéo dài đồng hồ, bằng cách giữ nó ở mức thấp ở khoảng thời gian hơn
 mức khoảng thời gian được đề xuất. Nghĩa là thời gian thực hiện việc stretching vượt quá khoảng thời gian cài đặt
 cho phép stretching.

+ Tìm hiểu về "BTF flag in TX" và việc preventing(ngăn chặn) Under-run
 ^Trong quá trình tuyền Txing của một byte data, nếu bit TXE=1 thì điều này có nghĩa là Data Register đang empty
 ^Và nếu firmware chưa ghi bất kỳ byte nào vào Data Register trước khi Shift Register trở nên empty (trước
 đó bằng cách truyền), thì cờ BTF sẽ được đặt và đồng hồ sẽ được kéo dài để ngăn chặn việc chạy under-run

 ^Chu trình: Ta có SR <=>DR<--firmware/software. Đầu tiên ta có 1 byte A đc software->DR, sau đó byteA đc sao chép
 từ DR->SR, từ SR sẽ truyền data ra bên ngoài. Lúc này DR là empty(TXE=1), tuy nhiên trong khi TXE=1 thì firmware
 lại đưa vào DR một byteB, sau đó sao chép byteB vào SR. byteB này ko phải là byte dữ liệu đúng, dẫn đến việc
 truyền dữ liệu bị sai(gọi là under-run).(firmware:byte do phần cứng tạo/software: byte do user muốn truyền)
 ->Để tránh việc một byteB (ko mong muốn đc truyền do firmware tạo ra) thì nếu software ko gửi byte tiếp theo vào DR,
 thì khi SR đã hoàn thành việc gửi byteA thì BTF flag sẽ đc set(BTF=1) và clock sẽ Stretching. Ta có thể hiểu là
 khi TXE=1 và BTF=1 thì cả DR và SR đang empty và cùng với đó là clock sẽ stretching

+ Tìm hiểu về "BTF flag in RX" và việc preventing(ngăn chặn) Overrun
 ^Nếu RXNE = 1, điều đó có nghĩa là dữ liệu mới đang chờ trong DR và nếu firmwaren chưa đọc byte dữ liệu trước khi SR
 được lấp đầy một dữ liệu byte mới khác thì cờ BTF cũng sẽ được đặt và đồng hồ sẽ được kéo dài để ngăn chặn việc Overrun
 Nghĩa là chỉ đọc các data đc gửi từ Slave->SR->DR, ngăn chặn việc đọc các data do firmware tạo ra gửi vào DR
 Việc các data do slave gửi đến và do firmware tạo ra lẫn vào nhau và cùng đc đọc thì dẫn đến Overrun Error

+ I2C Handle Strcuture modification - Sửa đổi lại cấu hình Strcuture của I2C Handle(V215)
+ I2C ađing interrupt related and interrupt APIs(V216)

*===I2C Interrupt based APIs(S62)
*==Assignmen(Bài tập)t: I2C Interrupt APIs Implemetation(V217)
*==Implemetation of I2C interrupt based APIs(V218)
+ Code hàm "I2C_MasterSendDataIT"

*===I2C IRQ Handler Implemetation(S63)
*==I2C IRQ Handler Implemetation Part1(V219)
+ Một I2C Interrupt sẽ bao gồm 2 hành động là: xử lý events and xử lý lỗi error
 ^ISR1: Interrupt handling for interrupt generated by I2C events(I2C_EV_IRQHangling)
 ^ISR2: Interrupt handling for interrupt generated by I2C error(I2C_ER_IRQHangling)

+ Việc điều khiển ngắt I2C sẽ chủ chủ yếu dựa vào "I2C Status register 1 (I2C_SR1)"
+ Lưu ý: điều kiện để ngắt RxNE và TxE xảy ra là các "Enable Control Bit" của cả "ITEVFEN và ITRRREN" đều đc set
+ Code triển khai

*==I2C IRQ Handler Implemetation Part2(V220),Part3(V221),Part4(V222),Part5(V223)
+ Triển khai các code điều kiện "if" kiểm tra các flag trong hàm "I2C_EV_IRQHangling"
+ Mẹo: bạn có thể kiểm tra MCU đang ở chế độ Master hay Slave bằng cách kiểm tra bit MSL của I2C_SR2

*==I2C IRQ Handler Implemetation Part6(V224),Part7(V225),Part8(V226)
*==I2C Error IRQ Handler Implemetation(V227)

*===Exercise(S64)
*==Exercise: Testing i2c Interrupt APIs(V228,V229)
+ Đề bài giống với đề I2C Send-Receive Data based
+ Việc Interrupt sẽ cần 2 điều kiện là việc cấu hình ngắt(I2C_MasterSendDataIT và I2C_MasterReceiveDataIT nếu là nhận)
và việc thực hiện hành động ngắt(nếu là event "I2C_EV_IRQHangling" hoặc nếu là error "I2C_ER_IRQHangling")
 ^Hàm Thực hiện hành động ngắt sẽ kết hợp với hàm Interrupt có sẵn nằm trong file startupcode_stm32xxx

+ Khi thực hiện việc nhận data thì có trường hợp chưa xử lý xong việc nhận mà chương trình đã xử lý việc khác
Nguyên nhân là do code chưa tối ưu, ko set trạng thái busy khi thực hiện sự kiện, thường chỉ nhận 1 byte sau đó
sẽ giải phóng Bus empty nên cần tối hiệu chỉnh lại code

*===I2C Slave Programming(S65)
*==I2C Slave programming discussion(V230)
+ Trong bài này ta sẽ thay đổi là STM32 là Slave và Arduino là Master
+ Sơ đồ:  I2C Master	<------------> 		I2C Slave
	(START,STOP,READ,WRITE)				 (Request(Send) for Data, Receive Data)
 ^Vì Arduino sẽ là Master nên tất cả các công việc sẽ do phía arduino lập trình
 ^Bên STM32 là Slave nên chỉ cần thực hiện hai hàm nhiệm vụ là SendData(khi có yêu cầu Read từ Master) và ReceiveData(khi
 có yêu cấu Write từ Master)

*==I2C transfer sequence diagram for Slave transmitter(V231)
+ Sơ đồ khối việc gửi dữ liệu từ Slave(STM32) đến Master(Read):
			 S|Address|A|_________| Data1 | A | Data2 | A |___....| DataN | NA|_____|P
			   		    |EV1|EV3_1| EV3   |   | EV3   |   |EV3|               |EV3_2|

 ^Legend: S=Start, SR= Repeated start, P=Stop, A=Acknowledge, NA=Not Acknowledge
 ^EV1: ADDR=1, cleared by reading SR1 followed(theo sau đó là) by reading SR2
 ^EV3-1: TxE=1, shift register empty, data register empty, write Data1 in DR
 ^EV3: TxE=1, shift register not empty, data register empty , scear by writting DR
 ^EV3-2: AF=1, AF is cleared by writtng '0' in AF bit of SR1 register

 ^Chú ý 1: The EV1 and EV3_1 events stretch SCL low untill the end of the corresspoding software sequence
 ^Chú ý 2: The RV3 event stretches SCL low if the software sequence is not completed before the end of the next byte transmission
 ^Các sự kiện EV là chắc chắn xảy ra và ta cần thực hiện các bước cấu hình bit(hướng dẫn phía sau dấu phẩy ở mỗi EVx) để chương
 trình tiếp tục thực hiện, nếu ko chương trình sẽ bị stretching
 ^Chú ý 3: Bất cứ khi nào Slave nhận được 'NACK', đó là dấu hiệu cho thấy Slave ngừng gửi thêm dữ liệu đến Master và Slave phải
 kết thúc việc truyền dữ liệu đến Master

+ Sơ đồ khối việc nhận dữ liệu từ Master(Write) đến Slave(STM32) :
			 S|Address|A|___| Data1 | A | Data2 | A |___....| DataN | A|P  |
			   		    |EV1|   		| EV2   |   |EV2|              |EV2||EV4|

 ^Legend: S=Start, SR= Repeated start, P=Stop, A=Acknowledge, NA=Not Acknowledge
 ^EVx= Event (with interrupt if ITEVFEN=1)
 ^EV1: ADDR=1, cleared by reading SR1 followed(theo sau đó là) by reading SR2 register
 ^EV2: RxNE=1, cleared by reading DR register
 ^EV4: STOPF=1, cleared by reading SR1 register followed(theo sau đó là) by writing to the CR1 register

 ^Chú ý: ~The EV1 events stretch SCL low untill the end of the corresspoding software sequence
  ~The EV12 events stretch SCL low if the software sequence is not completed before the end of the next byte reception
  ~After checking the SR1 register content, the user should perform the complete clearing sequence for each flag found set
  Thus, for ADDR and STOPF flags, the following sequence is required inside the I2C interrupt routine READ SR1
  if(ADDR==1){READ SR1; READ SR2}
  if(STOPF==1){READ SR1; WRITE CR1}
  The purpose is to make surre that both ADDR and STOPF flags are cleared if both are found set
  ~Các sự kiện EV2 kéo dài SCL xuống mức thấp nếu chuỗi phần mềm không được hoàn thành trước khi kết thúc quá trình nhận byte tiếp theo
  ~Sau khi kiểm tra nội dung thanh ghi SR1, người dùng nên thực hiện trình tự xóa hoàn chỉnh cho từng bộ cờ được tìm thấy
  Do đó, đối với các cờ ADDR và STOPF, trình tự sau đây là bắt buộc trong quy trình ngắt I2C READ SR1
  if(ADDR==1){ĐỌC SR1; ĐỌC SR2}
  if(STOPF==1){ĐỌC SR1; VIẾT CR1}
  Mục đích là để đảm bảo rằng cả hai cờ ADDR và ​​STOPF đều bị xóa nếu cả hai đều được tìm thấy.

*==I2C Slave Support in Drive(V232)

*===Exercise(S66)
*==Exercise: I2C Slave programming(V233)
+ Đề: I2C Master(Arduino) and I2C Slave(STM32) communication
 Master should read and display data from
 STM32 Slave connected. First master has to get the length of the data from the Slave to read subsequent data from the Slave
 1. Use I2C SCL=100KHz(Standart mode)
 2. Use Internal pull resistor for SDA and SCL lines

*==Exercise: Coding(V234)
*==Exercise: Testing(V235)
*==Exercise: Modifying I2C Transaction - Sửa đổi giao dịch I2C(V236)
+ Sửa đổi để chương trình có thể truyền hơn 32 byte tối đa

*===Commmon problem in I2C(S67)
*==Common Problems in I2C and Debugging Tips(V237)
+ Problems: SDA and SCL line not held HIGH Voltage after I2C pin initialization
  ^Reason-1: Not activating the pull-up resistor if you are using the internal pull upn resistor of an I/O line
  Debug Tip: Worth cheking the configuration register of an I/O line to see whether the pull-ups are really activated or not,
  best way is to dump the regisetrr content.

+ Problems 2: ACK failure(ko nhận đc ACK)
  ^Reason-1: Generating the address phase with wrong slave address
  Debug Tip: Verify the Slave address appearing on the SDA line by using logic analyzer hoặc osciloscope

  ^Reason-2: Not enabling the ACKing feature in the I2C control register
  Debug Tip: Cross check the I2C Control register ACK enable field

+ Problems 3: Master is not roducing the clock
  ^Debug Tip 1: First check whether I2C peripheral clock is enable and set to at leat 2MHz to produce standard mode i2c serial
  clock frequency
  ^Debug Tip 2: Check whether GPIOs which you used for SCL and SDA functionality are configured properly for the alternate functionality
*/

