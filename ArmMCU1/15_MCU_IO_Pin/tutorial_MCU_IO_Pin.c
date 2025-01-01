/*
 * tutorial_MCU_IO_Pin.c
 *
 *  Created on: Nov 23, 2023
 *      Author: Minh
 */
/**
*===MCU I/O Pin Specifications(Thông số kỹ thuật)(S30)
Trong chương này ta sẽ tiến hành tìm hiểu các thông số kỹ thuật của pin MCU
*==STM32 Pin Specifications(V118)
+ Absolute maximum ratings(AMR): Tìm hiểu Vdd và Vss
 ^Tham khảo tài liệu: "Datasheet_stm32f407x.pdf" -> 5.2 AMR -> Table 11
 ^"Vdd" là nguồn điện chính cấp cho vi điều khiển hoạt động. Được cung cấp từ nguồn bên ngoài vào chân VDD của vi điều khiển.
 Trên board STM32F407G-DISC1 có mạch ổn áp nên giúp cho việc nguồn USB-5V thành VDD cho vi điều khiển.
 ^Điện áp hoạt động chuẩn của Vdd là từ 1.8V <= Vdd <= 3.6V
 Nếu điện áp dưới 1.8V thì có thể vđk sẽ không thể hoạt động, hoặc lớn hơn 3.6V thì vđk có thể bị hư hỏng
 ^Điện áp tối đa có thể đặt vào bất kỳ chân Vdd nào của vđk là 4V. (Bạn cần phải check AMR table trong datasheet
 để kiểm tra điện áp tối đa của mạch đó để cấp nguồn cho đúng)

 ^"Vss"là tham chiếu(refernce) nối đất của vi điều khiển và phải được duy trì ở mức 0V.
 ^Giá trị tối thiểu(minimum value) có thể áp dụng cho Vss là -0.3V

+ General operating conditions(GOC) - Điều kiện hoạt động chung
 ^Tham khảo tài liệu: "Datasheet_stm32f407x.pdf" -> 5.3.1 GOC -> Table 14
 ^Các điều kiện vận hành chung thể hiện phạm vi giá trị được đảm bảo trong đó thiết bị hoạt động trong điều kiện thích hợp
 ^Các thông số cần chú ý trong Table 14: VDD, VDDA(Analog Voltage)

+ Vin: là điện áp đầu vào(input voltage) có thể áp dụng cho bất kỳ STM32pin
 ^Sơ đồ: GND -> Vin -> STM32F Pin

+ STM32 pins: Có 3 kiểu pin có thể đc áp dụng trong STM32
 ^Three-volt tolerant(abbreviated as TT) - Vin của Pin chấp thuận mức 3V(3.3V)
 ^Five-volt tolerant(abbreviated as FT)  - Vin của Pin chấp thuận mức 5V
 ^Three-volt tolerant directly connected to ADC(TTa) - Pin chấp thuận mức 3V kết nối ADC(ít đc sử dụng)

 ^Tolerant biểu thị giá trị điện áp mà GPIO có thể chấp nhận

 ^Tham khảo Datasheet_stm32f407x -> 3 Pinout and Pin decription -> Table 7
 ^Tại table 7 bạn sẽ kiểm tra đc chân pin vđk thuộc kiểu pin nào (TT hay FT hay TTa) qua cột I/O Structure
 ^Việc kiểm tra giúp bạn xác định đc Vin tối đa đc phép đưa vào pin lựa chọn

+ Three-volt tolerant pin(TT or TTa)
 ^The maximum Vin voltage is: "Vdd +0.3V" (Nghĩa là 3 + 0.3 = 3.3V)
 Điện áp thí nghiệm vượt quá 3.3V sẽ làm hỏng chân TT - TTa và có thể dẫn đến hư hỏng thiết bị
 ^Điều kiện hoạt động chung: -0.3V <= Vin(TT-TTa) <= Vdd + 0.3V(3.3V)

+ Five-volt tolerant pin(FT)
 ^Chân chịu được 5 volt có nghĩa là bảo vệ điện áp được đảm bảo (kẹp tín hiệu sử dụng điốt bảo vệ)
 cho chân khi điện áp đầu vào của nó giảm xuống -0,3V <= Vin(FT) <= 5,5V
 ^GPIO chỉ có khả năng chịu được 5 volt ở chế độ đầu vào (Tính năng dung sai 5 volt không được áp
 dụng khi gpio ở chế độ đầu ra hoặc chế độ analog)
 ^Điều kiện hoạt động chung: -0.3V <= Vin(FT) <= Vdd + 0.5V(5.5V)
 ^Đối với chân FT, bất kể điện áp nguồn, đảm bảo rằng Vin không thể vượt quá 5.5V
 ^Đối với FT_GPIO, điện áp Vin tối đa giới hạn ở Vdd +4V
 ^Khi Vdd = 0V (Thiết bị stm32 không được cấp nguồn), điện áp đầu vào trên FT GPIO không thể vượt quá 4V

*==Pin current characteristics(Đặc điểm dòng điện) (V119)
+ Datasheet: Datasheet_stm32f407x -> Table 12. Current characteristics
 ^I_VDD max is: 240mA
 ^I_VSS max is: 240mA
 ^I_IO(current sunk) and I_IO(current sourcee) max is: 25mA

+ Phân biệt "current sunk" và "current source"
Sunk current và source current là hai thuật ngữ được sử dụng để mô tả khả năng của một thiết bị điện tử để
cung cấp hoặc tiêu thụ dòng điện.
 ^Sunk current là khả năng của một thiết bị để tiêu thụ dòng điện. Khi một thiết bị tiêu thụ dòng điện,
 nó sẽ kéo dòng điện từ nguồn điện. Ví dụ, một đèn LED tiêu thụ dòng điện khi được bật.
 ^Source current là khả năng của một thiết bị để cung cấp dòng điện. Khi một thiết bị cung cấp dòng điện,
 nó sẽ đẩy dòng điện vào tải. Ví dụ, một pin cung cấp dòng điện cho thiết bị điện tử
 ^Ứng dụng:
  ~Sunk current thường được sử dụng trong các ứng dụng đòi hỏi thiết bị phải tiêu thụ nhiều dòng điện, chẳng hạn
  như đèn LED, động cơ, v.v.
  ~Source current thường được sử dụng trong các ứng dụng đòi hỏi thiết bị phải cung cấp nhiều dòng điện, chẳng hạn
  như pin, nguồn điện, external power v.v.

+ Vd: ta có một current source(Vdd) và đc dùng cho X pin I/O ở chế độ digital OUTPUT.
 ^Trong trường hợp 5 pin I/O đều ở mức1 và theo datasheet thì mỗi chân pin sẽ có mức tiêu thụ dòng điện tối đa là 25mA
 thì cường độ dòng điện tổng cung cấp ở Vdd là 125mA(25mA*5)
 ^Trong trường hợp 20 pin I/O đều ở mức1 và theo datasheet thì mỗi chân pin sẽ có mức tiêu thụ dòng điện tối đa là 25mA
 thì cường độ dòng điện tổng cần cung cấp ở Vdd là 500mA(25mA*20). Tuy nhiên, theo datasheet thì dòng điện tối đa Vdd
 có thể cung cấp đc là 240mA, vì vậy lúc này dong điện trên các pin trong trường hợp này có mức là 12mA(240mA/12)
 ->Tổng dòng điện trên các pin I/O phải nhỏ hơn hoặc bằng dòng điện tại Vdd

+ Vd về "current sourcing" và "current sinking"
 ^Current sourcing:
  ~Sơ đồ: 	Vdd+(MCU)
  			I/O pin(MCU) --> R1 --> Led ---> GND
  ~Khi bạn muốn Led sáng, bạn sẽ cung cấp một mức level High(Vdd) tại pin I/O, lúc này mạch sẽ đc thông từ Vdd->..->GND

 ^Current sinking:
  ~Sơ đồ:    Vdd+(MCU)
   			I/O pin(MCU) <-- R1 <-- Led <--- 5V
			  Vss(GND)
  ~Khi bạn muốn Led sáng, bạn sẽ cung cấp một mức level Low(0V) tại pin I/O, lúc này mạch sẽ đc thông từ 5V->..->Vss
  mà sẽ ko sử dụng đến Vdd
  ~Lưu ý: Khi sử dụng Current sinking thì tổng dòng điện ở các pin sử dung phải <=240mA

+ Injected Current(I_INJ) - Dòng điện cưỡng bức
 ^Datasheet: Datasheet_stm32f407x -> Table 12. Current characteristics
 ^Dòng điện cưỡng bức là dòng điện bị ép vào chân pin bởi điện áp đầu vào (Vin) cao hơn nguồn cung cấp dương (Vdd)
 hoặc thấp hơn Ground (Vss).
 ^Dòng điện vượt quá thông số kỹ thuật đã cho sẽ tạo ra dòng điện bên trong thiết bị và ảnh hưởng đến độ tin cậy của
 thiết bị. Ngay cả một dòng điện nhỏ vượt quá giới hạn quy định cũng không được phép
 ^Nagetive(âm)-Injection là dòng điện cảm ứng khi Vin<Vss. Nagetive-Injection tối đa được đưa vào là -5mA và mức điện áp
 Vin tối thiểu được chấp nhận trên GPIO là -0,3V đối với TT và FT GPIO
 ^0mA có nghĩa là việc Current injection có thể làm hỏng GPIO và gây ra sự cố STM32
 ^Positive(dương) Current Injectionn bị cấm đối với TT hoặc Ft GPIO được xác định là 0mA
 ^Miễn là Vin trên FT nhỏ hơn AMR(Vdd +4V) thì việc tiêm Positive(dương) Current Injectionn sẽ không xảy ra. Giới hạn trên
 0-mA chỉ ra rằng Positive(dương) Current Injectionn không bao giờ được chạy vào chân gpio, nếu không, thiết bị có thể bị hỏng.
 Nếu Vin vượt quá Vdd +4V thì việc Positive(dương) Current Injectionn sẽ xảy ra và thiết bị có thể bị hỏng nếu Positive(dương)
 Current injectionn vượt quá ngưỡng, điều này không được đề cập trong bảng datasheet

*==Logic Level - Mức Logic(V120)
Kiến thức trong video sẽ đc áp dụng cho hầu hết các dòng vi điều khiển
+ Tất cả các I/Os đều tuân thủ theo CMOS and TTL
+ Logic level compatibility(khả năng tương thích)
 ^Bất cứ khi nào bạn kết nối một peripheral device với vđk STM32 thì bạn phải cần quan tâm đến mức logic
 Nghĩa là mức điện áp đầu vào nào đc coi là mức HIGH hoặc mức LOW
 ^Mô hình:
			Vdd(3.3V)________________						Vdd(3.3V)_______________
					 | Logic level 1|								| 				|
					 ---------------- V_OH(2.9V)					| Logic level 1 |
					 | Intermediate	|		Noise Margin			-----------------V_IH(2.31V)
					 |	region		|								| Intermediate	|
					 |				|								|  region		|
					 |				|		Noise Margin			-----------------V_IL(0.99V)
					 ---------------- V_OL(0.4V)					| Logic level 0 |
					 | Logic level 0|								|				|
					 ----------------								-----------------
				STM32 Digital pin Output						  Arduino Digital pin Input

 ^Trong trường hợp trao đổi truyền thông, tín hiệu output STM32 phải tương thích với V_IL/V_IH của thiết bị thu và tín hiệu
 input STM32 phải tương thích với V_OL/V_OH của thiết bị phát như mô hình
 ^V_IL/V_IH and  V_OL/V_OH: là các điện áp ngưỡng

 ^Vd: Ta có STM32 giao tiếp với Arduino
  ~Sơ đồ:	STM32	 1| ------>---------|1  Arduino
			(Output) 2| ------>---------|2	(Input)
			  Tx							 Rx
  ~Khi tín hiệu đc truyền từ STM32(Output) sang Arduino(Input), nghĩa là tín hiệu sở STM32_1 và STM32_2 đều ở mức HIGH.
  Lúc này sẽ xuất hiện một sự sụp áp trên STM32_1 và STM32_2 gọi là V_OH. Lúc này ta có V_OH >= V_IH(Arduino) thì bo
  Arduino mới hiểu là có mức HIGH
  ~Tương tự ta có (STM32)V_OL <= V_IL(Arduino) để bo Arduino mới hiệu là có mức LOW
  ~Giải thích tương tự nếu STM32 làm Input Block
  ~Nếu điện áp nhỏ hơn V_IH(min) và lớn hơn V_IL(max) thì sẽ ko nhận dạng đc mức Logic
  ~Cần kiểm tra datasheet của board mạch sử dụng để biết V_IL/V_IH và V_OH/V_OL trước khi thiết kế sơ đồ mạch và lập trình

  ~V_IH là mức điện áp cao tối thiểu đưa vào để nó hiểu mức logic High
  ~V_IL là mức điện áp thấp tối thiểu đưa vào để nó hiểu mức logic Low

  ~V_OH là mức điện áp cao tối thiểu đầu ra để nó hiểu mức logic High
  ~V_OL là mức điện áp thấp tối thiểu đầu ra để nó hiểu mức logic Low

+ CMOS technology: V_IH(min) =~ 2/3 Vdd và V_IL(max) =~ 1/3 Vdd
  TTL technology: V_IH(min) =~ 2V và V_IL(max) =~ 0.8V

+ Vd1: Ta có STM32(Tranmitter) giao tiếp với Arduino(Receptor)
  Vdd for STM32: 3.3V và Vdd for Arduino: 5V
  ^Mô hình: 												  Vdd(5V)_______________
																	|				|
																	|				|
			Vdd(3.3V)________________								|Logic level 1	|
					 | Logic level 1| ---------Noise Margin-------- -----------------V_IH(3V)
					 ---------------- V_OH(2.9V)  -----------------	|  				|
					 | Intermediate	| 								|				|
					 |	region		|								| Intermediate	|
					 |				|								|  region		|
					 |				| ----------Noise Margin------- -----------------V_IL(1.5V)
					 ---------------- V_OL(0.4V)  -----------------	| Logic level 0 |
					 | Logic level 0|								|				|
					 ----------------			0V					-----------------
				STM32 Digital pin Output	----------->			Arduino Digital pin Input

  ^Noise Margin là vùng chênh lệch điện áp giữa V_OL/VIL và V_OH/V_IH
  Intermediate region: vùng điện áp mà các vđk ko thể xác định đc mức Logic

  ^Vì điện áp V_OL(0.4V) < V_IL(1.5V) nên phía Arduino sẽ ko gặp vấn đề gì trong việc xác định mức Logic 0(Low)
  và dù có tăng mức V_OL lên một chút thì phía Arduino vẫn xác định đc mức Low.

  ^Tuy nhiên, ở V_OH của STM32 có mức cao tối thiểu là 2.9V, nhưng mức cao tối thiểu của V_IH(Arduino) là 3V. Vì vậy,
  phía STM32 phải cấp cho điện áp V_OH >= 3V. Trong trường hợp xấu là STM32 cấp cho điện áp V_OH = 2.9V thì lúc này
  mức điện áp này sẽ rơi vào vùng Intermediate, làm cho phía Arduino ko xác định đc mức Logic

  ~Vì vậy, bạn sẽ ko thể kết nối trực tiếp từ STM32->Arduino(Vì sẽ có hiện tượng  mức độ Logic ko tương thích)

+ Vd2: Ta có STM32(Receptor) giao tiếp với Arduino(Tranmitter)
  Vdd for STM32: 3.3V và Vdd for Arduino: 5V
  ^Mô hình: 												 Vdd(5V)________________
																	| Logic level 1 |
																	|				|
																	-----------------V_IH(4.1V)
																	|				|
			Vdd(3.3V)________________								|				|
					 | Logic level 1|								| 				|
					 ---------------- V_OH(2.31V) 					|  				|
					 | Intermediate	| 							    |				|
					 |	region		|								| Intermediate	|
					 |				|								|  region		|
					 |				| ----------Noise Margin------- 				|
					 ---------------- V_OL(0.99V)  -----------------|----------------V_IL(0.8V) 				|
					 | Logic level 0|								| Logic level 0	|
					 ----------------			0V					-----------------
				STM32 Digital pin Input	  <-----------			Arduino Digital pin Output

  ^Ta có V_IL(STM32) tối đa là 0.99V > 0.8(V_OL) của Arduino nên nên phía STM32 sẽ ko gặp vấn đề gì trong việc xác
  định mức Logic 0(Low)
  ^Tuy nhiên V_OH(Arduino) có mức cao tối thiểu là 4.1V > Vdd_5V(>>V_IH 2.31V) nên phía STM32 sẽ không thể xác định mức
  Logic 1(High). Ngoài ra nếu cung cấp một mức điện áp >3.3V từ Arduino -> STM32 thì có thể làm hư hỏng STM32 hoặc làm
  hỏng pin tiếp nhận input.
  =>Vì vậy, bạn sẽ ko thể kết nối trực tiếp từ STM32->Arduino(Vì sẽ có hiện tượng  mức độ Logic ko tương thích)
  ->Việc xác định các mức điện áp ngưỡng là quan trọng.

+ Giải pháp: để giao tiếp STM32<-->Arduino thì giữa chúng sẽ có sử dụng một mạch trung gian chuyển áp tín hiệu.
Sẽ đc giới thiệu trong các chương dùng cho mục đích giao tiếp như SPI, I2C
*/

