/*
 * tutorial1_gpio_knowles.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Minh
 */

/**
*===GPIO Knowles(Kiến thức về GPIO)(S14)
*==GPIO pin and GPIO port(V48)
+ GPIO là tên viết tắt của từ "General Purpose Input Output"
+ GPIO thường đc sử dụng cho các mục đích:
 ^ Reading digital signal(Đọc tín hiệu kỹ thuật số)
 ^ Generating triggers for external components (Tạo trình kích hoạt cho các thành phần bên ngoài)
 ^ Insuing Interrupt (Thực hiện tiến hành ngắt)
 ^ Waking up the processor (Kích hoạt vi xử lý)
 ^ ....
+ GPIO port là một tập hợp một số chân IO cố định, số chân(pin) của port tùy thuộc vào thiết kế của nhà sản xuất.
Vd: ta có GPIO portA gồm 16 I/O pin[15:0], ta nói rằng port GPIOA có độ rộng là 16 bit và mỗi pin tương đương với
một bit tương ứng của GPIOA port Register.
+ GPIO port của các bộ vi điều khiển khác nhau thì có thể có số lượng chân Pin khác nhau
Vd: Intel 8051 có GPIO port 8 I/O pin; NXP LPC214x có GPIO port 32 I/O pin; STM32 có GPIO port 16 I/O pin

*==GPIO behind the scene - Tìm hiểu hoạt động đằng sau của GPIO(V49)
+ Câu hỏi: Làm thế nào mà một pin GPIO đc triển khai bên trong vi điều khiển?
 ^ Bên trong bộ vi điều khiển sẽ nối với pin thông qua 2 buffer là "Input buffer" line và "Output buffer" line và
 một dòng đc gọi là "Enable Line" đc nối với ngõ vào "Output buffer". Khi "Enable line" bằng 0, bộ "Output buffer"
 sẽ đc kích hoạt và "Input buffer" sẽ bị vô hiệu hóa(deactivated) và ngược lại.

 ^ Bộ Output Buffer đc cấu tạo bởi 2 cổng(gate) song song MOS transistor (PMOS_T1 và NMOS_T2).
  ~Với T1 một đầu nối với Vcc và một đầu nối với ngõ ra(drain) nối với chân Pin
  ~Với T2 một đầu nối với GND và một đầu nối với ngõ ra(drain) nối với chân Pin
  ~Phía trước đầu vào gate 2 cổng thì có một "cổng logic đảo" để nếu từ vi điều khiển input=1 thì output=0 và ngược lại
  ~Khi "Enable line" bằng 0 truyền ra từ vi điều khiển, bộ "Output buffer" sẽ đc kích hoạt
   &Nếu như đầu vào thứ 2 của "Output buffer" là 1 thì khi đi qua "cổng logic đảo" thì nó trở thành 0, lúc này cổng
  PMOS_T1 đc sử dụng. Và vì nó nối với Vcc nên lúc này ta đc đầu ra của "Output buffer" là một xung cạnh lên-pull_up
  cho chân Pin đc kết nối
  Sơ đồ: 					   +VCC
					 	 	 	S|
						   P_Transistor_T1---
					  	  	  	 |	  	    |
				      	  	  	 |			|
   VĐK---Logic_đảo--->---Drain---|          G -->-----Pin---->----GND
			          	  	  	 |			|
			    		   P_Transistor_T2---
					 	 	 	S|
					 	 	 	GND

   &Nếu như đầu vào thứ 2 của "Output buffer" là 0 thì khi đi qua "cổng logic đảo" thì nó trở thành 1, lúc này cổng
  NMOS_T2 đc sử dụng. Và vì nó nối với Gnd nên lúc này ta đc đầu ra của "Output buffer" là một xung cạnh xuống-pull_down
  cho chân Pin đc kết nối
  Sơ đồ: 					   +VCC
					 	 	 	S|
						   P_Transistor_T1---
					  	  	  	 |	  	    |
				      	  	  	 |			|
   VĐK---Logic_đảo--->---Drain---|          G -->-----Pin---->----VCC
			          	  	  	 |			|
			    		   P_Transistor_T2---
					 	 	 	S|
					 	 	 	GND

 ^ Bộ Input Buffer đc cấu tạo bởi 2 cổng(gate) song song MOS transistor (PMOS_T1 và NMOS_T2).
  ~Với T1 một đầu nối với Vcc và một đầu nối với ngõ ra(drain) nối vào vi điều khiển
  ~Với T2 một đầu nối với GND và một đầu nối với ngõ ra(drain) nối vào vi điều khiển
  ~Ở giữa chân Pin và gate 2 cổng thì có một "cổng logic đảo" để nếu nếu từ Pin input=1 thì output=0 và ngược lại
  ~Khi "Enable line" bằng 1 truyền ra từ vi điều khiển thì bộ "Input buffer" sẽ đc kích hoạt
   &Nếu như chân Pin nối với 1_Vcc đi vào Pin(logic 1) thì khi đi qua "cổng logic đảo" thì nó trở thành 0, lúc này
   cổng PMOS_T1 đc sử dụng. Và vì nó nối với Vcc nên lúc này ta đc đầu ra của "Input buffer" là một xung cạnh lên-pull_up
   cho input vào vi điều khiển. Túc là vi điều khiển nhận mức input=1 làm mức trạng thái thay đổi từ chân Pin
   Vd: khi nhấn nút thì điều kiện để vi điều khiển nhận ra có sự nhấn nút là mức logic 1 (Nối button và Vcc)
    Sơ đồ: 			+VCC
					 S|
				P_Transistor_T1---
					  |	  	     |
				      |			 |
   VĐK----<---Drain---|          G --<--Logic_đảo---Pin----<---- 1
			          |			 |
			    P_Transistor_T2---
					 S|
					 GND

   &Nếu như chân Pin nối với 0_Gnd đi vào Pin(logic 0) thì khi đi qua "cổng logic đảo" thì nó trở thành 1, lúc này
   cổng NMOS_T2 đc sử dụng. Và vì nó nối với Gnd nên lúc này ta đc đầu ra của "Input buffer" là một xung cạnh xuống-pull_down
   cho input vào vi điều khiển. Túc là vi điều khiển nhận mức input=0 làm mức trạng thái thay đổi từ chân Pin
   Vd: khi nhấn nút thì điều kiện để vi điều khiển nhận ra có sự nhấn nút là mức logic 0 (Nối button và Gnd)
   Sơ đồ: 			+VCC
					 S|
				P_Transistor_T1---
					  |	  	     |
				      |			 |
   VĐK----<---Drain---|          G --<--Logic_đảo---Pin----<---- 0
			          |			 |
			    P_Transistor_T2---
					 S|
					 GND
+ Đây chính là ví dụ về hoạt động pull_down và pull_up trong điều khiển GPIO của Embedded.
  Mô hình phần cứng:
             +VCC										+VCC
			  |											  |-------|
			  Ri_điệntrở(Pull-up) 					    Button   Tụ(0.01uF)
			  |											  |-------|
  VĐK---Pin---|D1				              VĐK---Pin---|D2
			  |--------|								  |
	        Button    Tụ(0.01uF)				        Re_điệntrở (Pull-down)
			  |--------|						          |
			 GND							             GND
  --->Dùng R Pull-up: Khi Button chưa đc nhấn thì điện áp tại D1 đc xác định là mức cao, và khi có sự nhấn nút thì dòng
  điện sẽ chạy thẳng từ Vcc-Gnd dẫn đến tại D1 đc xác định ở mức thấp.
  --->Dùng R Pull-down: Khi Button chưa đc nhấn thì điện áp tại D1 đc xác định là mức thấp, và khi có sự nhấn nút thì dòng
  điện sẽ chạy thẳng từ Vcc-Gnd dẫn đến tại D1 đc xác định ở mức cao.

*==GPIO input mode with high impedance state(Đầu vào GPIO ở mức trở kháng cao)(V50)
+ "High impedance state - Mức trở kháng cao": gọi tắt là Hi-Z
+ Đối với "Input Mode" thì sau khi bạn cấp nguồn cho MCU, theo mặc định, tất cả các chân GPIO sẽ ở chế độ đầu vào với trạng
thái HIGH-Z hoặc trạng thái thả nổi"floating state"(ko xác định đc mức logic, bị dội, bị nhiễu). Điều này đúng với hầu hết
các bộ vi điều khiển. Việc giữ pin ở trạng thái nổi "floating state" có thể dẫn đến rò rỉ dòng điện, dẫn đến tiêu thụ điện
năng cao hơn. Vì vậy bạn ko nên kết nối điểm "Floating" của pin của bạn với Hi-Z hoặc với GND. Điều này là do pin nối rất
dễ bị nhiễu và có thể dẫn đến rò rỉ dòng điện.

*==GPIO input mode with pull-up and pull_down state - Đầu vào GPIO với trạng thái kéo lên và kéo xuống(V51)
+ Thay vì sử dụng các Vcc và Gnd từ bên ngoài thì để tạo pull-up và pull_down thì ta sử dụng Vcc và Gnd bên trong chính
bộ vi điều khiển mà ta đang lập trình
+ (VCC + Ri_điện trở) + Pin_input ---> Input buffer : gọi là internal pull-up
+ (GND + Re_điện trở) + Pin_input ---> Input buffer : gọi là internal pull-down

*==GPIO output mode with open drain state - Đầu ra GPIO với trạng thái ngõ ra hở(V52)
+ GPIO ở chế độ đầu ra, cấu hình "Open Drain - cực hở" nghĩa là bóng bán dẫn PMOS_T1 đã bị hủy kích hoạt. Nó chỉ đơn
giản là không có mặt trong "Output buffer", và chỉ là một bóng bán dẫn NMOS_T2 duy nhất. Khi Transistor được BẬT, một chân T2
được kéo xuống GND. Và khi Transistor này TẮT thì cực máng(điểm đầu ra nối với pin) của Transistor sẽ floating hoặc open-hở.
Đó là lý do nó được gọi là "open drain- cực hở". Vì vậy, kết quả là cấu hình đầu ra "Open Drain" chỉ có thể kéo chân xuống-
pull-down chứ không có khả năng kéo lên pull-up. Điều đó có nghĩa là nó có 2 trạng thái, nối đất hoặc floating. Hai trạng thái
này hoàn toàn vô dụng.
+ Open Drain là vô dụng cho đến khi bạn cung cấp khả năng kéo lên-pull-up. Bằng cách sử dụng điện trở kéo lên bằng cách kích
hoạt điện trở kéo lên bên trong(internal pull-up) hoặc điện trở kéo lên bên ngoài(external pull-up). Vì vậy, nếu không áp
dụng điện trở kéo lên(pull-up) để mở cống thì chế độ Open Drain hoàn toàn vô dụng.
Mô phỏng: 	+VCC										+VCC
			  |											  |
			  Ri_điệntrở nội					         Re_điệntrở ngoại
			  |											  |
			  Drain---Pin---				Drain---Pin---|---Button---GND
			  |								  |
	VĐK----Transistor				VĐK----Transistor
			  |								  |
			 GND							 GND
(Open Drain with internal pull-up)		(Open Drain with external pull-up)

+ Ngày nay, hầu hết MCU đều hỗ trợ điện trở kéo lên(internal pull-up register) cho mỗi pin của GPIO. Vì vậy bạn chỉ cần sử
dụng "thanh ghi cấu hình GPIO - GPIO configuration register" để kích hoạt hoặt hủy kích hoạt pull-up. Nếu MCU của bạn ko hỗ
trở internal pull-up thì bạn nên cân nhắc tạo mạch external pull-up.

+Vd: Làm cách nào để điều khiển đèn LED từ GPIO ở chế độ "Open Drain"
 ^Sơ đồ: 			+VCC		(Ở sơ đồ này ta sẽ dùng một internal pull-up)
					  |
			    Ri_(điệntrở nội)
				      |
			        Drain---Pin---LED----R_led----GND
			          |
VĐK--Logic_đảo---G_Transistor
					 S|
					 GND
 ^Khi bạn muốn bật đèn LED, bạn code đầu ra từ "VĐK" có logic là '1'. Khi qua "Logic_đảo" thì mức logic trở thành '0', thì lúc
 này "Transistor" sẽ bị vô hiệu hóa(Disable) hoặc turn_off(Nhánh từ Transistor xuống GND bị off). LED sẽ đc điều khiển đến nguồn
 VCC(nói cách khác là LED sẽ đc kết nối với nguồn VCC và thông mạch) bằng "Register pull-up" và nó sẽ làm sáng đèn LED.

 ^Khi bạn muốn tắt đèn LED, bạn code đầu ra từ "VĐK" có logic là '0'. Khi qua "Logic_đảo" thì mức logic trở thành '1', thì lúc
 này "Transistor" sẽ đc kích hoạt(Enable) hoặc turn_on(Nhánh từ Transistor xuống GND được on - thông mạch). Nghĩa là lúc này
 dòng điện sẽ từ VCC chảy thằng xuống S_GND đc nối với Transistor mà ko chạy qua mạch LED, điều này làm cho mạch LED ko còn nguồn
 điện duy trì dẫn tới đèn LED tắt.
 ^Điện trỏ R pull-up phải có giá trị cố định(từ 10kOM đến 50kOM)
 ^Thêm: bạn cũng có thể điều khiển trạng thái của LED bằng cách sử dụng I2C Bus
 ^Drain(thuộc Transistor) trong vd này như là chiếc cần gạt công tắc chuyển hướng dòng điện

*==GPIO output mode with Push-Pull State(Trạng thái kéo đẩy)(V53)
+ Sơ đồ: 			+VCC		(Ở sơ đồ này ta sẽ dùng một internal pull-up)
					 S|
				---P_Transistor_T1
				|	  |
				|     |
VĐK--Logic_đảo--|	  |---Drain---Pin---
			    |     |
			    ---P_Transistor_T2
					 S|
					 GND
+ Khi bạn cấu hình chân_pin ở chế độ output_mode, hãy nhớ "enable" một GPIO port theo mặc định
+ Cấu hình đầu ra này gọi là push pull, vì đầu ra sẽ được kéo chủ động giữa mức cao và mức thấp bằng cách sử dụng 2 transistor T1
và T2 không giống nhau. Các cấu hình khác như cấu hình open drain, trong đó đầu ra chỉ có thể được kéo xuống mức thấp như bạn đã
thấy trong video trước. Tuy nhiên, trong cấu hình kéo đẩy, bạn không cần bất kỳ điện trở kéo lên hoặc kéo xuống nào!
+ Đầu ra kéo đẩy thực sự sử dụng hai bóng bán dẫn. Mỗi Transistor sẽ được bật để điều khiển đầu ra đến mức thích hợp. Bóng bán dẫn
trên cùng(T1) sẽ BẬT khi đầu ra phải được điều khiển CAO! và bóng bán dẫn phía dưới(T2) sẽ BẬT khi đầu ra phải xuống mức thấp.

+ Tìm hiểu cách bạn có thể điều khiển đèn LED từ cấu hình đầu ra kéo đẩy- push-pull GPIO pin.
 ^Sơ đồ: 			+VCC		(Ở sơ đồ này ta sẽ dùng một internal pull-up)
					 S|
				---P_Transistor_T1
				|	  |
				|     |
VĐK--Logic_đảo--|	  |---Drain---Pin----R_led---LED----GND
			    |     |
			    ---P_Transistor_T2
					 S|
					 GND
 ^Khi bạn muốn bật đèn LED, bạn code đầu ra từ "VĐK" có logic là '1'. Khi qua "Logic_đảo" thì mức logic trở thành '0', thì lúc
 này "Transistor_T1" trên sẽ Enable hoặc turn_on(Transistor_T2 bị off). LED sẽ đc điều khiển push lên mức HIGH tức là kết nối
 đến nguồn VCC(nói cách khác là LED sẽ đc kết nối với nguồn VCC và thông mạch) nó sẽ làm sáng đèn LED.
 ^Khi bạn muốn bật đèn LED, bạn code đầu ra từ "VĐK" có logic là '0'. Khi qua "Logic_đảo" thì mức logic trở thành '1', thì lúc
 này "Transistor_T2" dưới sẽ Enable hoặc turn_on(Transistor_T1 bị off). LED sẽ đc điều khiển pull xuống mức LOW tức là kết nối
 đến GND(nói cách khác là LED sẽ ko còn nguồn duy trì) nó sẽ làm tắt đèn LED.
 ^S(Transistor) trong vd này như là chiếc cần gạt công tắc chuyển hướng dòng điện

*==Optimizing IO Power Consumption	- Tối ưu hóa mức tiêu thụ năng lượng IO(V54)
+Sơ đồ: 			+VCC
					 S|
				P_Transistor_T1---
					  |	  	     |
				      |			 |
   VĐK----<---Drain---|          G --<--Logic_đảo---Pin----<----(Floating)
			          |			 |
			    P_Transistor_T2---
					 S|
					 GND
+ Nếu một pin làm input mode mà để "floating-ko kết nối vs ngoại vi nào" thì có thể dẫn đến dòng rò từ VCC, làm mất hoặc hao
hụt khoảng 30% VCC.
+ Việc hao hụt điện áp dẫn đến tình trạng điện áp nằm đâu đó ở giữa(mức điện áp mà ko thể xác định là logic 1 hay logic 0).
Dẫn đến trường hợp là cả hai Transistor T1 và T2 đều đc kích hoạt. Kết quả là sẽ có một lượng nhỏ dòng điện sẽ chảy theo đường
từ VCC(Transistor_T1) xuống GND(Transistor_T2), làm thất thoát, tiêu lãng phí năng lượng.
*/
