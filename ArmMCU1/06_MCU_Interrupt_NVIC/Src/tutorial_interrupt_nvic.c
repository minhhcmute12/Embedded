/*
 * tutorial_interrupt_nvic.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Minh
 */
/**
*===MCU interrupt Design, NVIC, Interrupt Handling(S12)
*==Part1(V43)
+ Ví dụ: Làm một ví dụ về cách GPIO PIN làm "interrupt-ngắt" to the processor
 ^Trả lời 2 câu hỏi: thiết kế như thế nào? và Làm cách nào gửi một ngắt đến vi xử lý?
 ^Tài liệu: DM00031020 -> 12 Interrupt and events -> 12.2.5 External interrupt/event line mapping
  Đây đc gọi là bộ điều khiển ngắt ngoài EXTIx (Xem thêm: 12.2.2 EXTI block diagram)
  ~Theo Vector Table thì EXTI0 đến EXTI4 nằm lần lượt trên các IRQ6->IRQ10, các EXTI9_5 thì đều cùng nằm trên
  một ngắt IRQ23; các EXTI15_10 thì đều nằm trên một ngắt IRQ40;...
  ~Tổng cộng có 23 ngắt EXTIx trong Vector Table(bao gồm cả các ngoại vi mượn EXTIx đến kết nối NVIC)
  Lưu ý: một số ngoại vi như là RTC,... đc thiết kế như là một EXTIx. Có nghĩa là thay vì một số ngoại vi kết
  nối trực tiếp đến NVIC(Nested Vector Interrupt Controller) mà chúng sẽ thông qua các EXTIx để kết nối vs NVIC.

*==Part2(V44)
+ Theo Part1 thì một số thiết bị ngoại vi(vd: RTC,...) cung cấp ngắt của chúng tới NVIC qua đường EXTIx và cũng có
một số thiết bị ngoại vi cung cấp ngắt trực tiếp EXTx cho NVIC. Đây là thiết kế của hãng ST.
+ Sử dụng 12.2.5 External interrupt/event line mapping
 ^Qua sơ đồ map bạn có thể thấy rằng tất cả "Pin 0" của các cổng-port GPIOx đều kết nối với line EXTI0 trên khối
 hình bộ ghép kênh. Và qua khối thì ta cũng kết luận là bất kỳ thời điểm nào cũng chỉ có một chân Pin0 của một GPIOx
 đc thiết lập làm EXTI0. Thanh ghi quản lý vấn đề lựa chọn này là SYSCFG_EXTICR1 register EXTI0[3:0]
 ^Tương tự ta cũng có các khối ghép tên tiếp theo là: "Pin 1" của các cổng-port GPIOx đều kết nối với line EXTI1
 Thanh ghi quản lý vấn đề lựa chọn này là SYSCFG_EXTICR1 register EXTI1[3:0](4 bit tiếp theo)
 ........"Pin 15" của các cổng-port GPIOx đều kết nối với line EXTI15 Thanh ghi quản lý vấn đề lựa chọn này
 là SYSCFG_EXTICR4 register EXTI15[3:0]

 ^Nếu ta có một button đc thiết kế nối vào PA0, và thiết lập PA0 trên SYSCFG_EXTICR1 register, thì khi nhấn nút thì
 ngắt đc thực hiện thông qua EXTI0, nghĩa là EXTI0 Handler run để xử lý nút nhấn người dùng bấm

+ Các bước thiết lập nút nhấn là một ngắt-interrupt của vi xử lý STM32:
 1. Button đó đc kết nối với một pin GPIO của vi điều khiển microcontroller
 2. Pin GPIO đó phải đc thiết lập cấu hình là một input mode để nhận ngắt-interrupt
 3. Liên kết liên quan giữa GPIO port và EXTIx(có tương ứng GPIO đó) phải được thiết lập(established) sử dụng thông
 qua thanh ghi SYSCFG_EXTICRx Register
 4. Định cấu hình phát hiện kích hoạt (falling-giảm/rising-tăng/both-cả hai) cho dòng EXTIx có liên quan (Điều này
 được thực hiện thông qua thanh ghi bộ điều khiển EXTI - EXTIx controller Register)
5. Triển khai trình xử lý để phục vụ ngắt

*==Part3(V45) - Tìm hiểu một số thanh ghi của EXTIx controller Register
+ Quan sát bộ điều khiển ngắt ngoài EXTIx (12.2.2 EXTI block diagram)
 ^ Ta có 5 thanh ghi thường đc dùng điều khiển các EXTIx
  ~Pending request register(Thanh ghi yêu cầu đang chờ xử lý)
  ~Interrupt mask register (Thanh ghi mặt nạ ngắt)
  ~Software interrupt event register (Thanh ghi sự kiện ngắt bằng phần mềm)
  ~Rising trigger selection register (Thanh ghi lựa chọn kích hoạt tăng(kéo lên-pull_up))
  ~Falling trigger selection register (Thanh ghi lựa chọn kích hoạt xuống(kéo xuống- pull_down))

 ^EXTI register(tài liệu 12.3 EXTI register )
  ~Interrupt mask register(12.3.1): thanh ghi có 32bit nhưng chỉ sử dụng [23:0] bit. Lý do vì EXTIx hỗ trợ 23 line.
  Có nghĩa bit0 thanh ghi EXTI_IMR sẽ liên kết với EXTI0, tương tự các bit khác của thanh ghi
  Với: 0_ Interrupt request from line x is mask
       1_ Interrupt request from line x  NOT is mask

  ~Rising trigger selection register(12.3.3): thanh ghi có 32bit nhưng chỉ sử dụng [23:0] bit. Lý do vì EXTIx hỗ trợ 23 line.
  Có nghĩa bit0 thanh ghi EXTI_RTSR sẽ liên kết với EXTI0, tương tự các bit khác của thanh ghi
  Với: 0_ Rising trigger Disable for input line
       1_ Rising trigger Enable for input line
  ~Falling trigger selection register(12.3.4): thanh ghi có 32bit nhưng chỉ sử dụng [23:0] bit. Lý do vì EXTIx hỗ trợ 23 line.
  Có nghĩa bit0 thanh ghi EXTI_FTSR sẽ liên kết với EXTI0, tương tự các bit khác của thanh ghi
  Với: 0_ Falling trigger Disable for input line
       1_ Falling trigger Enable for input line

  ~Pending register(12.3.6): thanh ghi có 32bit nhưng chỉ sử dụng [23:0] bit. Lý do vì EXTIx hỗ trợ 23 line.
  Có nghĩa bit0 thanh ghi EXTI_PR sẽ liên kết với EXTI0, tương tự các bit khác của thanh ghi
  Với: 0_ No trigger
       1_ Selected trigger request occurred(Đc lựa chọn kích hoạt yêu cầu xảy ra)

+ Mô hình hóa button ngắt GPIO:
 Button--GPIO pin(PA0)----> "EXTI controller" --- EXTI lines(EXTI0)-------> Cổng OR --6--> NVIC <---> CPU
					EXTI Pending register-----------------------------️-----↗

 ~Giả sử cho nút nhấn đc nhấn(Từ Low->High), bộ EXTI controller sẽ phát hiện việc nhấn đó và xuất nó ra EXTI0 line set 1 , khi
 đã phát hiện thị bit tương ứng của EXTI0 line bên trong thanh ghi Pending Register cũng sẽ đc tăng cao(bit0 set 1).
 Như vậy lúc này 2 ngõ vào  cổng OR đều là 1 thì theo phép tính toán tử cổng OR thì đầu ra output cũng là 1. Và khi đầu ra
 là 1 thì mới đc phép kết nối tới NVIC, nghĩa là NVIC nhận đc tín hiệu kích hoạt truyền đến CPU, và CPU chạy nhiệm vụ ngắt

 ~Khi việc nhấn kết thúc thì đầu ra của EXTI0 line cũng sẽ set 0, tuy nhiên lúc này bit tương ứng của EXTI0 line bên trong
 thanh ghi Pending Register vẫn giữ set1(nghĩa là ko tự động set về 0; lập trình viên phải chủ động code set về 0) nghĩa là
 lúc này đầu ra của Pending Register vẫn là 1. Ta có 2 ngõ vào cổng OR là 0(EXTI0 line) và 1(Pending) thì Output vẫn là 1,
 nghĩa là lúc này interrupt hoạt động vô hạn liên tục. Vì vậy lập trình viên cần chủ xóa bit(set0) trên Pending register.

+ Code ví dụ về button interrupt
*/

/**
 * Ngắt "Interrupt" trong hệ thống nhúng: (AI Gemini)
 Ngắt (Interrupt) là cơ chế cho phép CPU tạm dừng thực thi chương trình hiện tại để xử lý một sự kiện khẩn cấp. Khi xảy ra ngắt,
 CPU sẽ lưu trạng thái hiện tại của chương trình và chuyển sang thực thi một chương trình khác gọi là trình phục vụ ngắt (ISR).
 Sau khi ISR hoàn tất, CPU sẽ quay lại thực thi chương trình ban đầu.

	^Ưu điểm:
	Tăng khả năng phản hồi của hệ thống đối với các sự kiện bên ngoài.
	Cho phép thực hiện đồng thời nhiều nhiệm vụ.
	Tiết kiệm năng lượng bằng cách chỉ thực hiện các nhiệm vụ khi cần thiết.

	^Phân loại:
	Ngắt phần cứng: Do các thiết bị phần cứng bên ngoài tạo ra, ví dụ như nhấn nút, ngắt timer.
	Ngắt phần mềm: Do chương trình tạo ra, ví dụ như chia cho 0.
 */

/*
 * Ví dụ: Sử dụng ngắt để xử lý sự kiện nhấn nút. Khi người dùng nhấn nút, CPU sẽ tạm dừng chương trình hiện tại để thực thi ISR,
 * ISR sẽ xử lý sự kiện nhấn nút, ví dụ như bật đèn LED.
 *
 * Bài tập sử dụng ngắt khi lập trình hệ thống nhúng sử dụng board mạch stm32f407 Discovery:
	^ Mục tiêu: Bật LED khi người dùng nhấn nút.
	^ Thực hiện:
		1.Cài đặt bộ xử lý ngắt cho nút nhấn.
		2.Viết ISR để xử lý sự kiện nhấn nút.
		3.Bật LED trong ISR.

 		1.Cài đặt bộ xử lý ngắt:
			+Sử dụng hàm HAL_NVIC_SetPriority() để đặt mức ưu tiên cho ngắt nút nhấn.
			+Sử dụng hàm HAL_NVIC_EnableIRQ() để kích hoạt ngắt nút nhấn.

		2.Viết ISR:
			+ISR được gọi khi có ngắt từ nút nhấn.
			+ISR cần thực hiện các hành động cần thiết để xử lý sự kiện nhấn nút, ví dụ như bật LED.

		3.Bật LED: Sử dụng hàm HAL_GPIO_WritePin() để bật LED.

	^Mã nguồn: Mã nguồn này chỉ là ví dụ minh họa cho việc sử dụng ngắt, Cần chỉnh sửa mã nguồn cho phù hợp với board mạch
	và phần cứng cụ thể.

	#include "stm32f4xx_hal.h"

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	  if (GPIO_Pin == GPIO_PIN_13) {
		// Bật LED
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	  }
	}

	int main(void) {
	  // Cài đặt bộ xử lý ngắt
	  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	  // Khởi tạo LED
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	  while (1) {
		// ...
	  }
	}
 */
