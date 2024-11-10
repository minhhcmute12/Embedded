/*
 * tutorial2_gpio_structure.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Minh
 */
/**
*===GPIO Programming Structure(cấu trúc) and Register(S15)
*==GPIO programming structure(V55)
+ Mô hình:
			|<--->|Port Direction(Mode) Register------->|       |
            |<--->|Port Speed Register----------------->|       |
		Bus |<--->|Port Output type Register----------->|I/OPort|<----->MCU I/O Pin
			|<--->|Some the configuration Register----->|	    |
			|<--->|Port Output Data Register----------->|	    |
			|<--->|Port Input Data Register------------>|	    |
+ Trong microcontroller thì cổng GPIO đc quản lý bởi các thanh ghi(register)
 ^Direction(Mode) Register(Thanh ghi điều hướng): là thanh ghi mà bạn sẽ tìm thấy bên trong bất kỳ bộ vđk nào,
 bằng cách sử dụng thanh ghi này bạn có thể đặt hướng(direction) hoặc thiết lập chế độ cho port là input,output,
 analog,vv...
 ^Input Data Register : được sử dụng để đọc data từ GPIO port.
 ^Output Data Register: được sử dụng để ghi data từ GPIO port.
 ^.....
+ Tham khảo tài liệu về GPIO register của STM32F4xx: DM00031020.pdf -> 8.4 GPIO register
 ^Trong Block Diagram thì trên Stm32f4xxx thì các Port GPIOx đc kết nối với Processor thông qua AHB1 Bus(168MHz)
  Tham khảo tài liệu: Datasheet_stm32f407x -> 2.2 Device overview -> F5: Stm32F4xx block diagram

*==Exploring(Khám phá) GPIO Port and Pins of STM32F4xxx Discovery Board(V56)
+ Bộ vđk STM32F407VGT6 hỗ trợ tối đa 9 cổng GPIO(từ GPIOA -> GPIOI)
+ Mỗi GPIO là một nhóm gồm 16 chân(pin). Và mỗi GPIO port sẽ có một Register cấu hình riêng cho port đó
 -> GPIO sẽ hỗ trợ tối đa 144pin cho mục đích chung(có thể là output hoặc input)
+ Trên board lập trình STM32F407VGT6 thì nhà sản xuất đã hàn sẵn các chân từ GPIOA->GPIOE(80 pins).
Như vậy lập trình viên có thể input hoặc output ngay các ứng dụng lập trình của mình

*==GPIO Mode Register(used to set mode for a pin-đc sử dụng để set chế độ cho một chân pin)(V57)
+ Tài liệu: RM407 -> 8.4.11 Table 39. GPIO register map and reset values
Phần này cho ta thấy cái nhìn tổng quan về các GPIO register trong VĐK
+ Tài liệu: RM407 -> 8.4 GPIO register -> 8.4.1 GPIO port mode register
 ^Name: GPIOx_MODER (x: có thể thay bằng từ A-I port muốn cấu hình-> Vd: GPIOA_MODER)
 ^Offset(phần bù địa chỉ): 0x00
 ^Reset value: đây là giá trị sẽ đc thiết lập trên thanh ghi mode khi reset hệ thống(Power on/down reset)
  ~0xA800_0000 for port mode A
  ~0x0000_0280 for port mode B
  ~0x0000_0000 cho các port còn lại
 ^Thanh ghi 32bit[31:0], mỗi 2bit[1:0] quản lý một chân pin(16 pins),vd: bit[1:0]-> pin1....bit[31:30]->pin16
  Khi Set thông số nhị phân cho 2 bit:
  ~00: Input(reset state)
  ~01: General purposor output mode
  ~10: Alternate function mode(Chế độ chức năng thay thế)
  ~11: Analog mode(khi xử lý ADC or DAC)

*==Input configuration of a Microcontroller GPIO Pin(V58)
+ Trong Microcontroller, chúng ta có thể sử dụng GPIO Pin cho nhiều mực đích khác nhau, đây là lý do tại sao nó đc
gọi là Generral Purpose, tuy nhiên một số Pin ko thể sử dụng cho tất cả mục đích.
+ Sơ đồ:						An MCU GPIO Pin
		|-----------------|------------------------|------------------|
	 INPUT				OUTPUT					ALT.FUN				ANALOG
		|								(SPI CLock,ITC siggnal,...)
	 INTERRUPT
+ Nếu Pin đc cấu hình ở chế độ INPUT thì lúc này Pin cũng có thể đc cấu hình là một INTERRUPT mode cho processor
+ Tài liệu: RM407 -> 8.3 GPIO functional description (Sơ đồ khối mô tả Input và Output của Pin)
 ^Một Pin của GPIO thường đc chia làm 2 khối là Input Driver và Output Driver. Khi "Input Driver" đc kích hoạt thì
"Output Driver" sẽ bị tắt
 ^Trạng thái là Input của Pin thì với mỗi chu kỳ clock,data input sẽ đc đưa vào "Input Data Register" thông qua AHB1
 Bus. Đồng thời, "pull-up/pull-down register" sẽ bị vô hiệu hóa. Sau đó, bằng "code firmware" thì data từ "Input Data
 Register" sẽ đc truyền vào(read) vào processor
 ^Lưu ý: bất kỳ nhiễu nào của vđk trên board của bạn có thể ảnh hưởng đến value đc đọc vào "Input Data Register". Đây
 chính là lý do vì sao Input Mode đc kích hoạt thì "Output Drive,pull-up/pull-down register" bị vô hiệu hóa

 ^Vd_1: ta có một công tắc"Switch" đc nối vào Pin đc cấu hình Input(Với T1 là đầu nối với Pin, T2 là đầu nói với GND)
  ~Khi công tắc đc nhấn, xung clock có xu hướng là truyền từ pin->T1->T2->GND => Đây đc xác định là trạng thái '0'
  Thông qua AHB1 Bus thì trạng thái '0' đc cập nhật vào "Input Data Register" rồi vào Processor
  => Bất kỳ khi nào nút nhấn đc nhấn mà chân pin đc kết nối với GND thì trạng thái đọc đc là '0'

  ~Khi công tắc KHÔNG đc nhấn, thì lúc này Pin sẽ bị để hở(ko kết nối với GND), điều này dẫn đến là có sự biến động
  về điện áp ở chân Pin IO này. Dẫn đến, khi xung clock đc truyền đến thì trạng thái đọc đc sẽ ko ổn định(lúc trạng
  thái là '1', lúc trạng thái là '0')-> đây chính là việc bị nhiễu(noise) tín hiệu(dội phím)
  Vì vậy, trong quá trình code cho công tắc sẽ có thêm một hàm code dùng để xử lý nhiễu này(chống dội phím)
  $$Cách giải quyết nhiễu: khi công tắc để hở(ko nhấn) thì pull-up(internal/external) đc kích hoạt để điện áp tại
  Pin I/O luôn ở mức cao(high) và khi có xung clock đến thì trạng thái lúc đó đc đọc là '1'

 ^Vd_2: ta có một công tắc"Switch" đc nối vào Pin đc cấu hình Input(Với T1 là đầu nối với Pin, T2 là đầu nói với VCC)
  ~Khi công tắc đc nhấn, xung clock có xu hướng là truyền từ pin->T1->T2->VCC => Đây đc xác định là trạng thái '1'
  Thông qua AHB1 Bus thì trạng thái '1' đc cập nhật vào "Input Data Register" rồi vào Processor
  => Bất kỳ khi nào nút nhấn đc nhấn mà chân pin đc kết nối với GND thì trạng thái đọc đc là '1'

  ~Khi công tắc KHÔNG đc nhấn, thì lúc này Pin sẽ bị để hở(ko kết nối với VCC), điều này dẫn đến là có sự biến động
  về điện áp ở chân Pin IO này. Dẫn đến, khi xung clock đc truyền đến thì trạng thái đọc đc sẽ ko ổn định(lúc trạng
  thái là '1', lúc trạng thái là '0')-> đây chính là việc bị nhiễu(noise) tín hiệu(dội phím)
  Vì vậy, trong quá trình code cho công tắc sẽ có thêm một hàm code dùng để xử lý nhiễu này(chống dội phím)
  $$Cách giải quyết nhiễu: khi công tắc để hở(ko nhấn) thì pull-down(internal/external) đc kích hoạt để điện áp tại
  Pin I/O luôn ở mức thấp(low) và khi có xung clock đến thì trạng thái lúc đó đc đọc là '0'

 ^Khi nào nên dùng pull-up/pull-down internal và khi nào dùng pull-up/pull-down external?
  Trả lời: khi bạn tiến hành vừa code vừa test các tính năng cho các chức năng thì sử dụng "internal", vì lúc này mục
  tiêu về code chức năng là quan trọng. Tuy nhiên khi chức năng đã hoàn thành và bắt đầu Test tổng, thì lúc này bạn
  cần phải phối hợp với đội phần cứng để thiết kế mạch phù hợp cho sản phẩm của bạn thì lúc này ta nên dùng "external"
  => Tùy vào tiến độ, giai đoạn phát triển sản phẩm mà ta sẽ tiến hành code cho phù hợp với Project.

*==Output Configuaration of a GPIO Pin in Push-Pull Mode(V59)
+ Tài liệu: RM407 -> 8.3 GPIO functional description (Sơ đồ khối mô tả Input và Output của Pin)
+ Khối OUTPUT Driver đc kích hoạt
+ Sơ đồ kết nối: Processor -> Output data Register -> Logic_Đảo -> transistor(P-Mos_0/N-Mos_1) -> Pin-> Led-> Rx->GND
 ^Giả sử sử dụng khối "Output Control- Cổng Logic_Đảo", tín hiệu output từ processor là '1', qua cổng Logic_Đảo ta có
 trạng thái là '0' đồng nghĩa là sử dụng transistor "P-Mos(VCC)".
 Chân pin Output sẽ kết nối với VCC(P-Mos)->Pin->LED -> Rx -> GND => Đèn led sẽ tắt khi có xung(PUSH-Đẩy)

 ^Giả sử sử dụng khối "Output Control- Cổng Logic_Đảo", tín hiệu output từ processor là '0', qua cổng Logic_Đảo ta có
 trạng thái là '1' đồng nghĩa là sử dụng transistor "N-Mos(GND)".
 Chân pin Output sẽ kết nối với GND(N-MOS)->LED -> Rx -> GND => Đèn led sẽ tắt khi có xung(PULL-Kéo)

 ^Đây chính là phương pháp output điều khiển led sáng tắt bằng Push-Pull(Xem chi tiết ở các bài trước)

+==Output Configuaration of a GPIO Pin in Open Drain Mode(V60)
+ Tài liệu: RM407 -> 8.3 GPIO functional description (Sơ đồ khối mô tả Input và Output của Pin)
+ Khối OUTPUT Driver đc kích hoạt
+ Sơ đồ kết nối 1: Processor->Output data Register->Logic_Đảo->Transistor(P-Mos_0/N-Mos_1)->Pin->Led->Rx->VCC
 ^Tín hiệu output từ processor và Output data Register là '0', qua cổng Logic_Đảo ta có trạng thái là '1' đồng nghĩa
 là sử dụng transistor "N-Mos(GND)".
 Chân pin Output sẽ kết nối với GND(P-Mos)->Pin->LED -> Rx -> VCC => Đèn led sẽ bật

 ^Tín hiệu output từ processor và Output data Register là '1', qua cổng Logic_Đảo ta có trạng thái là '0' đồng nghĩa
 là mạch bị hở tại điểm chung Floating(điểm chung P-Mos và N-MOS, transistor N-MOS lúc này bị vô hiệu hóa)
 Chân pin Output sẽ kết nối với Floating(Open Drain)->Pin->LED->Rx->VCC => Mạch hở (Nhiễu mạch xảy ra)
 =>KHÔNG thể sử dụng sơ đồ 1 cho điều khiển led bằng pp Open Drain, Vì nếu dùng pull-down thì led vẫn sáng mà dùng
 pull-up thì sẽ có 2 nguồn VCC,VDD có thể đoản mạch.

+ Sơ đồ kết nối 2: Processor->Output data Register ->Logic_Đảo->Transistor(P-Mos_0/N-Mos_1)->Pin->Led->Rx->GND
 ^Giải pháp: ta cần kích hoạt một internal/external pull-up/pull-down để xử lý vấn đề floating
 ^Tín hiệu output từ processor và Output data Register là '0', qua cổng Logic_Đảo ta có trạng thái là '1' đồng nghĩa
 là mạch bị hở tại điểm chung Floating(điểm chung P-Mos và N-MOS,transistor N-MOS và P-MOS lúc này đều bị vô hiệu hóa)
  ~(KO cần cấu hình Internal)Chân pin Output sẽ kết nối với GND(N-Mos)->Pin->LED->Rx->GND => Đèn led sẽ tăt
  ~External: Chân pin Output sẽ kết nối với VCC(external)->GND(N-Mos) => Đèn led sẽ tắt(dòng điện ko qua led)

 ^Tín hiệu output từ processor và Output data Register là '1', qua cổng Logic_Đảo ta có trạng thái là '0', thì ta kích
 hoạt đc N-MOS hoạt động
  ~Tiến hành cấu hình internal pull-up cho mạch:
  Chân pin Output sẽ kết nối với VDD(pull-up)->Pin->LED->Rx->GND => Đèn led sẽ bật

  ~Tiến hành cấu hình external pull-up cho mạch:
  Chân pin Output sẽ kết nối với VDD(pull-up)->Pin->LED->Rx->GND => Đèn led sẽ bật

 => Xem thêm về Floating(Open Drain) ở các chương trước

+ Lưu ý: Rx trong các mạch internal pull-up/pull-down thường có giá trị 40kOhm(cố định theo nhà sản xuất chip)
  và Rx trong các mạch external pull-up/pull-down thường có giá trị 1kOhm(thay đổi đc tùy vào việc thiết kế mạch)
+ Sử dụng PP Push-Pull trong việc điều khiển mà bạn cần rõ hai trạng thái cao thấp như Led(sáng-tắt), động cơ(bật-tắt),...
+ Sử dụng Open Drain khi ứng dụng yêu cầu output là ở mức HIGH như I2C,...

*==Input stage of a GPIO pin during output configuration - Trạng thái Input của GPIO trong khi đang cấu hình Output(V61)
+ Khi cấu hình GPIO pin là Output(Driver Output đc bật). Tuy nhiên, lúc này Driver Input vẫn sẽ đc bật. Điều này đồng
nghĩa bạn có thể đọc trạng thái I/O stage của pin I/O đó thông qua thanh ghi "Input data Register"

*=="Alternate functionality" Configuaration of a GPIO pin - Chức năng "Alternate functionality" khi cấu hình chân GPIO(V62)
+ Khi ta thiết lập GPIO pin với "MODERRx[1:0] = 11" thì tính năng "Alternate functionality - chức năng thay thế" đc
kích hoạt trên GPIO pin
+ "Alternate functionality" có thể là là UART-Tx,UART-Rx, TIMER, ADC input channel,clock of the SPI, data line of I2C,...
Nói cách khác, chân pin GPIO lúc này đc sử dụng bởi các "peripheral-ngoại vi" mà bạn đã chỉ định.
+ Tài liệu: RM407 -> 8.3 GPIO functional description (Sơ đồ khối mô tả Input và Output của Pin)
 ^Trong sơ đồ ta sẽ thấy hai khối gọi là "Alternate function output" và "Alternate function input", và
 ^ Khi Pin GPIO thiết lập "Alternate functionality", thì khối "Alternate function output" sẽ điều khiển khối "Output Control"
 (Có nghĩa là lúc này "Output data Register" sẽ ko còn điều khiển, kiểm soát khối "Output Control") và khối "Alternate function
 input" sẽ điều khiển khối "Input Control" (Có nghĩa là lúc này "Input data Register" sẽ ko còn điều khiển, kiểm soát khối
 "Input Control")
 ^"Output Control" lúc này đc điều khiển bởi "Alternate functionality", có thể là I2C_SDA,I2C_CLK,UART-Tx,...
 ^"Input Control" lúc này đc điều khiển bởi "Alternate functionality", có thể là UART-Rx,ADC_IN,TIMER_CHx,CAN_RX...

*==GPIO port Output Type Register - Tìm hiểu về thanh ghi lựa chọn loại đầu ra GPIO port(V63)
+ Khi một GPIO pin đc kích hoạt ở chế độ "Output mode", thanh ghi này đc sử dụng để chọn loại output đc sử dụng trên pin
+ Các loại đầu ra "Output Type" có thể là: "Push-Pull" hoặc "Open-Drain"
+ Thông tin cấu hình thanh ghi: (Tài liệu: 8.4 GPIO register -> 8.4.2 GPIO port output type register)
 ^Name: GPIO port output type (GPIOx_OTYPER), x: có thể là từ A,...I/J/K
 ^Address offset: 0x04
 ^Reset value : 0x0000_0000
 ^Thanh ghi 32 bit[31:0], tuy nhiên từ bit[31:16] KHÔNG đc sử dụng => Bit đc sử dụng[15:0] => từ OT0->...->OT15
 ^Từ bit[15:0] thì mỗi bit quản lý một pin của một port(Một port GPIOx có 16 pin)
 ^Cấu hình nhị phân: 0: Output push-pull(reset stage) ||	1: Output Open-Drain
*/

