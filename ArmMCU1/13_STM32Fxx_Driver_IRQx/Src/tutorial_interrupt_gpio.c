/*
 * tutorial_interrupt_gpio.c
 *
 *  Created on: Nov 21, 2023
 *      Author: Minh
 */
/**
*===GPIO Pin Interrupt Configuaration(S28)
*==GPIO Pin Interrupt configuration coding: Part1(V108)
+ Tài liệu: PowerPoint-> STM32f4x GPIO Pin Interrupt delivery to the Processor
 ^Tài liệu mô tả tổng quan về kết nối:
	GPIOx Pinx -> EXTI Block -> EXTIx(interrupt)-> NVIC(IRQx) <---> Processor Core
 ^Mỗi EXTIx sẽ đc kết nối với một cổng của NVIC(IRQx), vd EXTI5 đc nối với IRQ23, để lấy đc thông tin kết nối
 này bạn phải dựa vào "Vector Table"
 ^EXTI Block: dùng để phát hiện cạnh xung(FT,RT), và cung cấp việc Enable/Disable việc phân phối ngắt tới Processor
 ^Các IRQx trong NVIC sẽ bị tắt theo mặc định, vì vậy bạn phải cấu hình thanh ghi NVIC để bật/tắt tính năng ngắt trên
 các IRQx mong muốn
 ^Cổng GPIOx(mong muốn sử dụng ngắt) được quyết định bởi cấu hình thanh ghi SYSCFG_EXTICR
  Vd: định kết nối GPIOx_Pin0 sẽ kết nối với EXTI0

+ GPIO Pin Interrupt Configuaration:
 1. Pin must be input configuration(Pin phải là cấu hình đầu vào)

 2. Configure the edge trigger(RT,FT,RFT)(Định cấu hình trình kích hoạt cạnh (RT, FT, RFT))

 2.1 Configure SYSCFG_EXTICR Register

 3. Eanble interrupt delivery from peripheral to the processor(on peripheral side)
    Cho phép phân phối ngắt từ thiết bị ngoại vi đến bộ xử lý (ở phía ngoại vi)

 4. Identify the IRQx number on which the processor acccept the interrupt from that pin
    Xác định số IRQx mà bộ xử lý chấp nhận ngắt từ chân đó

 5. Configure the IRQx priority for the identified IRQ number (Processor side)
    Định cấu hình mức độ ưu tiên IRQx cho số IRQ đã xác định (Phía bộ xử lý)

 6. Enable interrupt reception on that IRQ number(Processor side)
    Cho phép nhận ngắt trên số IRQ đó (Phía bộ xử lý)

 7. Implement IRQx Handler - Triển khai Trình xử lý IRQx

*==GPIO Pin Interrupt configuration coding: Part2(V109)
+ Code:
 ^Code file "stm32f407xx.h" phần 9_1 Interrupt Peripheral Register definition structure for EXTI(12.3.7 EXTI register map)
 ^Code file "stm32f407xx.h" phần 9_1_1 Interrupt definitions

+ Code: hàm "void GPIO_Init()"

*==GPIO Pin Interrupt configuration coding: Part3(V110)
+ Code
 ^Code file "stm32f407xx.h" phần 9_2 Periperal Register definition structure for SYSCFG(9.2.8 SYSCFG register maps for STM32F405xx/07xx)

*==GPIO Pin Interrupt configuration coding: Part4(V111)
+ Tiến hành cấu hình thanh ghi SYSCFG_EXTICR register
 ^Ta sẽ chỉ định cấu hình kết nối: vd định kết nối GPIOx_Pin0 sẽ kết nối với EXTI0
  ~Datasheet: 9.2.3 SYSCFG external interrupt configuration register 1~4(SYSCFG_EXTICR1~4)
  ~Mỗi thanh ghi SYSCFG_EXTICRx sẽ quản lý 4 EXTIx => Ta có 15 EXTIx đc quản lý bởi 4 thanh ghi SYSCFG_EXTICRx
  ~Vd: Button nhấn ngắt sẽ nối với PD3 -> EXTI3, tuy nhiên mặc định EXTI3 đang nối với GPIOA nên ta phải sửa cấu hình
  kết nối bằng SYSCFG_EXTICR register. Theo datasheet thì để EXTI3 kết nối port D thì bit EXTI3[3:0] từ bit 12->bit15
  thanh ghi SYSCFG_EXTICR1 đc cấu hình là 0011
  ~Vd2: Button nhấn ngắt sẽ nối với PC13 -> EXTI13, Theo datasheet thì để EXTI13 kết nối port C thì bit EXTI13[3:0]
  từ bit4->bit7 thanh ghi SYSCFG_EXTICR4 đc cấu hình là 0010
  ~Cách tìm và cấu hình EXTIx: Ta chọn GPIO Port C pin 13(làm interrupt),
   . temp1 = 13 / 4 = 3 -> Thanh ghi SYSCFG_EXTICRx[4] cần sử dụng là SYSCFG_EXTICR4(Mảng chạy từ 0->3)
   . temp2 = 13 % 4 = 1(*4bit) -> Thanh ghi SYSCFG_EXTICR4[4] sẽ sử dụng cụm 4 bit thứ 1(EXTI13: từ bit4->bit7)
   . Vì sử dụng PortC nên cấu hình bit cho EXTI13 là 0010

 ^define biến: GPIO_BASEADDR_TO_CODE (file stm32f407xx.h)

+ Xác định vị trí position trong thanh ghi NVIC để có thể đc sử dụng làm IRQx_EXTIx
 ^Datasheet: Table 61. Vector table for STM32F405xx/07xx and STM32F415xx/17xx(12.1.3)
 ^Triển khai phần 18_IRQ(Interrupt request) Number of SRTM32F407x MCU
 
+ Thêm(Gemini):Tìm hiểu về thao tác gán và thao tác OR(còn gọi là bit-by-bit): Để hiểu rõ sự khác biệt giữa hai đoạn code trên, 
chúng ta cần nắm rõ hai thao tác cơ bản trong lập trình nhúng:
 ^Gán (=): Thao tác này sẽ ghi đè giá trị ở vế phải vào địa chỉ bộ nhớ được chỉ định ở vế trái. Toàn bộ nội dung của ô nhớ 
 sẽ được thay thế bằng giá trị mới.
 
 ^OR_bit-by-bit (|=): Thao tác này thực hiện phép OR logic trên từng bit của hai toán hạng. Kết quả của phép OR sẽ được gán 
 trở lại cho biến ở vế trái. Nghĩa là, nếu một bit nào đó của hai toán hạng có giá trị là 1, thì bit tương ứng trong kết 
 quả cũng sẽ có giá trị là 1.
 
 ^Ví dụ 1: Phân tích từng đoạn code
  ~Đoạn code 1: SYSCFG->EXTICR[3] = 0x2 << (1* 4);
  ->Ý nghĩa: Đoạn code này gán giá trị 0x2 << (1* 4) (tức là 0010) trực tiếp vào thanh ghi SYSCFG->EXTICR[3] 
  bắt đầu từ vị trí bit4 và kéo dài 4 bit. Điều này có nghĩa là tất cả các bit trong thanh ghi này sẽ được thay thế và 
  chỉ có 4 bit từ bit4->7 sẽ bằng giá trị 0x2(0010).
  ->Tác dụng: Đoạn code này sẽ ghi đè cấu hình hiện tại của ngắt bên ngoài liên quan đến thanh ghi EXTICR[3] bằng cấu 
  hình mới. Các bit khác trong thanh ghi này sẽ bị mất đi.

  ~Đoạn code 2: SYSCFG->EXTICR[3] |= 0x2 << (1* 4);
  ->Ý nghĩa: Đoạn code này thực hiện phép OR bit-by-bit giữa giá trị hiện tại của thanh ghi SYSCFG->EXTICR[3] và giá 
  trị 0x2 << (1* 4). Kết quả của phép OR sẽ được gán lại cho chính thanh ghi này.
  ->Tác dụng: Đoạn code này sẽ thiết lập các bit tương ứng của thanh ghi EXTICR[3] thành 1, trong khi giữ nguyên các 
  bit còn lại. Điều này có nghĩa là chúng ta có thể thêm một cấu hình ngắt mới vào cấu hình hiện tại mà không làm mất đi   
  các cấu hình khác.
  
 ^Khi nào nên sử dụng mỗi loại?
  ~Sử dụng =(gán) khi:
  Bạn muốn thay thế hoàn toàn cấu hình hiện tại bằng một cấu hình mới.
  Bạn chắc chắn rằng các cấu hình cũ không còn cần thiết nữa.

  ~Sử dụng |=(OR_Bit by Bit) khi:
  Bạn muốn thêm một cấu hình ngắt mới vào cấu hình hiện tại.
  Bạn muốn tránh làm mất đi các cấu hình ngắt khác đã được thiết lập trước đó.

 ^Ví dụ 2: Giả sử ban đầu thanh ghi SYSCFG->EXTICR[3] có giá trị là 0x10 (tức là bit 4 đã được thiết lập).
 Sau khi thực hiện SYSCFG->EXTICR[3] = 0x20;: Giá trị của thanh ghi sẽ là 0x20, chỉ có bit 5 được thiết lập.
 Sau khi thực hiện SYSCFG->EXTICR[3] |= 0x20;: Giá trị của thanh ghi sẽ là 0x30, cả bit 4 và bit 5 đều được thiết lập.
 Kết luận:
 Thao tác gán (=): Thay thế hoàn toàn giá trị.
 Thao tác OR (|=): Thêm giá trị mới vào giá trị hiện tại.
 ->Việc lựa chọn sử dụng toán tử nào phụ thuộc vào mục đích cụ thể của bạn. Nếu bạn muốn thay đổi hoàn toàn cấu hình, 
 hãy sử dụng toán tử gán. Nếu bạn muốn thêm một cấu hình mới mà không ảnh hưởng đến các cấu hình khác, hãy sử dụng 
 toán tử OR.

 ^Lưu ý:
  ~Cấu hình ngắt: Việc cấu hình ngắt không chỉ dừng lại ở việc thiết lập thanh ghi EXTICR. Bạn cần phải thực hiện các 
  bước cấu hình khác như kích hoạt đồng hồ cho cổng GPIO, cấu hình chế độ làm việc của chân GPIO, kích hoạt ngắt 
  trong NVIC, ...
  ~Kiểm tra datasheet: Để có thông tin chính xác và chi tiết về cấu trúc của các thanh ghi và cách thức hoạt động 
  của chúng, bạn nên tham khảo datasheet của vi điều khiển.

*==GPIO Pin Interrupt configuration coding: Part5(V112)
+ Tìm hiểu về cấu hình thanh ghi NVIC
 ^Datasheet: CortexM4 Generic user Device-> CortexM4 peripheral -> 4.2 Nested Vector Interrupt Control 
 ^NVIC là một đơn vị quản lý ngắt trong vi điều khiển STM32F4x. Nó chịu trách nhiệm nhận, ưu tiên và xử lý các yêu cầu 
 ngắt từ các nguồn khác nhau, bao gồm các thiết bị ngoại vi, ngoại vi bên trong và các sự kiện hệ thống. Chức năng:
  +Nhận yêu cầu ngắt: Khi một sự kiện ngắt xảy ra, thiết bị ngoại vi hoặc nguồn ngắt bên trong sẽ gửi một yêu cầu 
  ngắt đến NVIC.
  +Ưu tiên ngắt: NVIC có thể ưu tiên các yêu cầu ngắt khác nhau. Ngắt có mức ưu tiên cao hơn sẽ được xử lý trước.
  +Xử lý ngắt: Khi một yêu cầu ngắt được chấp nhận, NVIC sẽ tạm dừng chương trình chính và chuyển điều khiển đến hàm 
  xử lý ngắt tương ứng.
  +Quản lý trạng thái ngắt: NVIC có thể kích hoạt hoặc vô hiệu hóa các nguồn ngắt. Nó cũng có thể thiết lập các cờ ngắt
  để chỉ ra rằng một ngắt đã xảy ra.
  +Cấu trúc của NVIC: (* : là thanh ghi cần quan tâm)
   NVIC_ISER (Interrupt Set-Enable Register): Được sử dụng để kích hoạt các nguồn ngắt. (*)
   NVIC_ICER (Interrupt Clear-Enable Register): Được sử dụng để vô hiệu hóa các nguồn ngắt. (*)
   NVIC_ISPR (Interrupt Set-Pending Register): Được sử dụng để đặt cờ ngắt cho một nguồn ngắt cụ thể.
   NVIC_ICPR (Interrupt Clear-Pending Register): Được sử dụng để xóa cờ ngắt của một nguồn ngắt cụ thể.
   NVIC_IPR (Interrupt Priority Register): Được sử dụng để thiết lập mức ưu tiên cho các nguồn ngắt.
 
 ^Như đã biết mỗi EXTIx sẽ đc một position của NVIC Register quản lý(vd EXTI0 đc nối với Position 6 của NVIC) 
 ^Quan sát bảng 4-2 NVIC register summary
  ~Interrupt Set-enable Register(ISER) : cho phép ngắt trên IRQx
  ~Interrupt Clear-enable Register(ICER) : vô hiệu hóa ngắt trên IRQx
  ~Interrupt Priority Register(IPR): cài đặt mức ưu tiên cho ISER và ICER

 ^Tìm hiểu Interrupt Set-enable Register(NVIC_ISER0~7)
  ~Mỗi Thanh ghi 32 bit[31:0],
   .Với NVIC_ISER0: IRQ0(bit0), IRQ1(bit1), IRQ2(bit2), IRQ9(bit9), IRQ15(bit15), IRQ16(bit16), IRQ31(bit31)
   .Với NVIC_ISER1: IRQ32(bit0), IRQ33(bit1), IRQ34(bit2), IRQ41(bit9), IRQ47(bit15), IRQ48(bit16), IRQ63(bit31)
   .......
  ~Với: 0=no effect(không có tác dụng)  --   1= enable interrupt

 ^Tìm hiểu Interrupt CLear-enable Register(NVIC_ICER0~7)
  ~Mỗi Thanh ghi 32 bit[31:0],
   .Với NVIC_ICER0: IRQ0(bit0), IRQ1(bit1), IRQ2(bit2), IRQ9(bit9), IRQ15(bit15), IRQ16(bit16), IRQ31(bit31)
   .Với NVIC_ICER1: IRQ32(bit0), IRQ33(bit1), IRQ34(bit2), IRQ41(bit9), IRQ47(bit15), IRQ48(bit16), IRQ63(bit31)
   .......
  ~Với: 0=no effect(không có tác dụng)  --   1= disable interrupt

 ^Tìm hiểu Interrupt Priority Register(NVIC_IPR0~59)
  ~Mỗi Thanh ghi 32 bit[31:0], mỗi 8 bit quản lý IRQ -> Có IRQ0_PRI ->...->IRQ239_PRI
  
 ^Ví dụ: Cấu hình ngắt nút nhấn trên STM32F4 
 Để cấu hình ngắt nút nhấn trên STM32F4, chúng ta cần thực hiện các bước sau:
 1.Cấu hình chân GPIO:
  +Thiết lập chân GPIO làm chân nhập.
  +Cấu hình ngắt bên ngoài (EXTI) cho chân GPIO.
 2.Cấu hình NVIC:
  +Kích hoạt ngắt EXTI cho chân GPIO tương ứng.
  +Thiết lập mức ưu tiên cho ngắt.  
 3.Code
	// Cấu hình ngắt nút nhấn trên chân PA0
	void EXTI0_IRQHandler(void) {
	  // Xử lý sự kiện nút nhấn
	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); // Đảo trạng thái của chân PB5
	}

	void SystemClock_Config(void) {
	  // ... (Cấu hình xung nhịp hệ thống)
	}

	void GPIO_Init(void) {
	  // ... (Cấu hình chân PA0 làm input với pull-up)
	  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0); // Thiết lập mức ưu tiên cho ngắt EXTI0
	  HAL_NVIC_EnableIRQ(EXTI0_IRQn); // Kích hoạt ngắt EXTI0
	} 
  Trong ví dụ trên, khi nút nhấn được nhấn, một ngắt EXTI0 sẽ được tạo ra. Hàm EXTI0_IRQHandler sẽ được gọi để xử lý 
  sự kiện này.

+ Code hàm: void GPIO_IRQConfig
+ Code : "stm32f407xx.h" -> Phần "Start processor specific details"
 ^Datasheet: CortexM4 Device-> CortexM4 peripheral -> 4.2 Nested Vector Interrupt Control-> bảng 4-2 NVIC register summary
 ^Xác định địa chỉ address của từng NVIC_ISER0~7
 ^Xác định địa chỉ address của từng NVIC_ICER0~7

+ Code : hàm GPIO_IRQConfig

*==GPIO Pin Interrupt configuration coding: Part6(V113)
+ Tìm hiểu Interrupt Priority Register(NVIC_IPR0~59)
 ^Trong mỗi phần 8bit thì chỉ lấy 4bit cao làm priority, 4bit thấp ko sử dụng
  Vd: b10000000: có 4 bit cao 1000 đc thiết lập, 4 bit thấp 0000 ko đc sử dụng

+ Code: "stm32f407xx_gpio_driver.c" -> 4_3APIs Interrupt Priority(ưu tiên) - cấu hình mức ưu tiên cho ngắt

*==GPIO Pin Interrupt configuration coding: Part7(V114)
+ Việc xử lý Interrupt - ngắt thực sự phụ thuộc vào thiết bị ngoại vi. Cách bạn viết thay đổi ISR sẽ làm thay đổi
thiết bị ngoại vi kết nối. Vd: ta có peripheral I2C, I2C là một ngoại vi khá phức tạp, cách bạn xử lý ngắt với I2C
sẽ khác với cách bạn xử lý ngắt với GPIO.

+ Cách hoạt động thực tế của ngắt
 ^Giả sử: Ta có một button đc kết nối với Pin0 của GPIOA, và đc cấu hình cạnh xung là FT, sử dụng EXTI0
 ^Khi bạn thực hiện việc nhấn nút, ngắt này sẽ đc đăng ký vào thanh ghiPR(Pending Register)của khối EXTI(công cụ EXTI),
 nếu ko có ngắt nào khác đc kích hoạt thì nó sẽ kiểm tra quá trình chuyển đổi xung của ngắt có phải là quá trình cạnh
 xung FT hay không. Nếu đc xác định là xung FT thì việc phân phối ngắt đc bật(enable), ngắt sẽ đc gửi qua EXTI0 line
 đc nối tới IRQ Number 6(NVIC)
 ^Khi NVIC nhận đc ngắt, đăng ký ngắt sẽ đc cấu hình vào thanh ghi PR(Pending Register), nếu lúc này Processor đang
 không thực thi ISR nào thì nó sẽ thực thi gọi địa chỉ Vector của IRQ(Position Vector Table) liên quan (vd IRQ Number 6
 của NVIC đc liên kết với EXTI0 có địa chỉ trong Vector Table là 0x0000_0058)
 ^Bạn phải có trách nhiệm(code triển khai) giữ địa chỉ của chức năng ngắt ISR tại địa chỉ IRQ 0x0000_0058
 ^Xóa bit đang chờ ở thanh ghi PR(Pending Register) của NVIC, và ngắt sẽ đc đăng ký với Processor

 ^Thêm nữa, nếu có một ngắt tiếp tục xảy ra, thì ở NVIC sẽ tiến hành so sánh mức ưu tiên của hai ISR, nếu ISR nào có mức
 ưu tiên lớn hơn sẽ đc đẩy lên xử lý. Vd: Ta có một button khác đc kết nối với Pin4 của GPIOA, và đc cấu hình cạnh xung
 là FT, sử dụng EXTI4. GPIOA_Pin4 --> EXTI4(ISR) -> IRQ Number 10(NVIC). Ta thấy EXTI4(ISR) tại IRQ Number 10(NVIC)
 có mức ưu tiên cao hơn EXTI0(ISR) tại IRQ Number 6(NVIC), nên processor sẽ đẩy EXTI4(ISR) lên xử lý trước

 ^Ngay cả khi bạn tiến hành cấu hình vô hiệu hóa ngắt, thì ngắt vẫn sẽ đc đăng ký trong thanh ghi PR(NVIC), chỉ khác ISR
 này sẽ ko đc vi xử lý thực thi

+ Các bước triển khai ngắt
 1. Implement the ISR function
 2. Store the address of ISR at the vector address location corresponding to the IRQ Number
 Lưu trữ địa chỉ của ISR tại vị trí địa chỉ vector tương ứng với IRQ Number(Sử dụng Vector Table)

+ IRQ thường là hàm của một chức năng cụ thể nên chúng thường đc triển khai bên trong lớp User Application(vd hàm main.c)
+ Trong các file startup_stm32xxx, các hàm IRQ dành cho ngắt(ISR) nói chung và chức năng khác nói riêng thường đã đc khai báo,
khi ban muốn triển khai hàm ngắt(ISR) chỉ cần copy tên hàm ngắt đó ra và sau đó copy hàm thực hiện chức năng ngắt
 ^Vd: Ta muốn thiết lập hàm "void EXTI0_IRQHandler(void)", EXTI0_IRQHandler là hàm đã đc khai báo trong file startup_stm32xxx,
 ta tiến hành copy tên hàm ra file main.c, sau đó copy hàm "GPIO_IRQHangling(0)"(GPIO_IRQHangling hàm xử lý ngắt cho IRQ0
 đc viết trong file stm32f407xx_gpio_driver.c) vào trong nội dung hàm EXTI0_IRQHandler() 
*/
/** Gemini
+ Ngắt là gì: Trong lập trình nhúng, ngắt là một cơ chế cho phép vi điều khiển tạm dừng việc thực hiện chương trình chính để xử 
lý một sự kiện nào đó. Sự kiện này có thể là một tín hiệu từ bên ngoài (như nhấn nút, thay đổi trạng thái của một cảm biến) 
hoặc một sự kiện bên trong (như timer hết giờ, lỗi bộ nhớ).

+ Tại sao sử dụng ngắt
 ->Tăng hiệu suất: Thay vì liên tục kiểm tra trạng thái của các thiết bị ngoại vi, vi điều khiển chỉ cần xử lý khi có sự kiện 
ngắt xảy ra, giúp tiết kiệm tài nguyên CPU.
 ->Đáp ứng nhanh các sự kiện thời gian thực: Ngắt cho phép vi điều khiển phản ứng nhanh chóng với các sự kiện quan trọng, 
đảm bảo hệ thống hoạt động ổn định và chính xác.
 ->Cấu trúc chương trình rõ ràng: Việc sử dụng ngắt giúp chia nhỏ chương trình thành các phần xử lý khác nhau, làm cho code 
 dễ đọc và bảo trì hơn.
 
+ Ngắt trong STM32F407: STM32F407 hỗ trợ nhiều loại ngắt khác nhau, bao gồm:
 -Ngắt ngoại vi (EXTI): Các ngắt được kích hoạt bởi các sự kiện bên ngoài như thay đổi trạng thái của chân GPIO.
 -Ngắt timer: Các ngắt được kích hoạt khi timer hết giờ.
 -Ngắt DMA: Các ngắt được kích hoạt khi quá trình truyền dữ liệu DMA hoàn thành.
 -Ngắt ngoại lệ: Các ngắt được kích hoạt bởi các sự kiện bất thường như chia cho 0, tràn bộ đếm,...

+ Ví dụ: Ngắt nút nhấn trên STM32F407: Giả sử chúng ta có một nút nhấn được kết nối với chân PA0 và muốn thực hiện một 
hành động khi nút nhấn được nhấn. Các bước thực hiện:
B1.Cấu hình chân GPIO: Thiết lập chân PA0 làm chân nhập. Kích hoạt ngắt cho chân PA0.
Cấu hình loại cạnh kích hoạt ngắt (sườn lên, sườn xuống hoặc cả hai).

B2: Viết hàm xử lý ngắt: Hàm này sẽ được gọi tự động khi ngắt xảy ra. Trong hàm này, chúng ta sẽ thực hiện các tác vụ cần 
thiết khi nút nhấn được nhấn.

B3: Kích hoạt ngắt trong NVIC: NVIC (Nested Vectored Interrupt Controller) là một đơn vị quản lý các ngắt trong vi điều khiển. 
Chúng ta cần kích hoạt ngắt cho chân PA0 trong NVIC.

#include "stm32f4xx_hal.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
  {
    // Thực hiện hành động khi nút nhấn được nhấn
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); // Đảo trạng thái của chân PB5
  }
}
int main(void)
{
  // ... Khởi tạo các thành phần khác ...

  // Cấu hình ngắt cho chân PA0
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);

  while (1)
  {
    // Chương trình chính
  }
}

//HAL_GPIO_EXTI_Callback: Hàm này được gọi tự động khi ngắt xảy ra.
//GPIO_Pin == GPIO_PIN_0: Kiểm tra xem ngắt có phải do chân PA0 gây ra hay không.
//HAL_GPIO_TogglePin: Đảo trạng thái của chân PB5 để báo hiệu nút đã được nhấn (ví dụ: bật/tắt một LED).

*/
