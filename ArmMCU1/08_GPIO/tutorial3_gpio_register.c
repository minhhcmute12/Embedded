/*
 * tutorial3_gpio_register.c
 *
 *  Created on: Nov 7, 2023
 *      Author: Minh
 */
/**
*===GPIO Register: SPEED, PULL-UP/DOWN, IDR and ODR(S16)
*==GPIO output speed register and its applicabillity - Tìm hiểu thanh ghi và khả năng ứng dụng của nó(V64)
+ Thanh ghi này đc sử dụng để chọn tốc độ output đc sử dụng trên pin
+ Chỉ có thể sử dụng đc thanh ghi này ở chế độ "Output Mode"
+ "Speed Control" là việc chuyển đổi từ H to L hoặc L to H; nói cách khác là cần kiểm soát "slew rate-tốc độ xoay/quay"
của pin đó bằng cách sử dụng GPIO output speed register
+ Sơ đồ: ---	   -------	     ----
			\     /       \     /
			 -----		   -----
 ^Ta có một sơ đồ mô ta cạnh xung lên và xuống (L and H)
 ^Thời gian mà chuyển từ H xuống L đc gọi là "T_rise" và thời gian mà chuyển từ L lên H đc gọi là "T_fall"
 ^Nếu bạn muốn điều khiển "Slew rate"(Cách nói khác "T_rise và T_fall") thì bạn phải sử dụng "GPIO output speed Register"

+ Thông tin cấu hình thanh ghi: (Tài liệu: 8.4 GPIO register -> 8.4.3 GPIO port output speed register)
 ^Name: GPIO port output speed Register (GPIOx_OSPEEDR), x: có thể là từ A,...I/J/K
 ^Address offset: 0x08
 ^Reset value : 0x0C00_0000 for port A; 0x0000_00C0 for port B; 0x0000_0000 for other ports
 ^Thanh ghi 32 bit[31:0]
 ^Mỗi 2bit[1:0] quản lý một chân pin(16 pins) của port => Từ OSPEEDR0->...-> OSPEEDR15
 ^Cấu hình nhị phân:
  ~00: Low Speed
  ~01: Medium Speed
  ~10: High Speed
  ~11: Very High Speed

+ Nếu chọn "00-Slow" thì "Slew rate"(Cách nói khác "T_rise và T_fall") sẽ nhỏ và tăng dần ở các mức Speed cao hơn
+ Bạn cần tham khảo datasheet của chip sử dụng (thường là các bảng I/O AC characteristics) với từ khóa "ospeed"
đế xác định mức tốc độ mà bạn cần sử dụng.
+ Việc lựa chọn tốc độ sẽ giúp ta xác định đc phần cứng cần điều khiển, Vd: khi ta sử dụng tốc độ LOW thì ta có thể chọn
các mức điện áp VDD=2.7V cùng với tụ C=10pF. Tuy nhiên khi chọn mức tốc độ HIGH thì mức điện áp cần thiết VDD>2.7V và mức
tụ C>10pF.
+ Việc lựa chọn speed, VDD, C(tụ) dựa vào datasheet sẽ giúp ta xác định đc tần số làm việc của hệ thống mà ta mong muốn
+ Việc áp dụng "Speed" chỉ áp dụng với pin ở "Output mode" chứ ko áp dụng cho "Input mode"

*== GPIO Pull-Up and Pull-Down Register(V65)
+ Thanh ghi đc sử dụng khi muốn kích hoạt Internal Pull-Up/Pull-Down
+ Thông tin cấu hình thanh ghi: (Tài liệu: 8.4 GPIO register -> 8.4.4 GPIO port pull-up/pull-down register)
 ^Name: GPIO pull-up/pull-down Register (GPIOx_PUPDR), x: có thể là từ A,...I/J/K
 ^Address offset: 0x0C
 ^Reset value : 0x6400_0000 for port A; 0x0000_0100 for port B; 0x0000_0000 for other ports
 ^Thanh ghi 32 bit[31:0]
 ^Mỗi 2bit[1:0] quản lý một chân pin(16 pins) của port => Từ PUPDR0->...-> PUPDR15
 ^Cấu hình nhị phân:
  ~00: No pull-up/pull-down(vô hiệu hóa cả hai)
  ~01: Pull-Up
  ~10: Pull-Down
  ~11: Reserved(Ko sử dụng)

*== GPIO Port Input Data Register(V66)
+ Thanh ghi đc sử dụng để đọc trạng thái I/O của một Pin của GPIO port
+ Đây chính là khối "Input Data Register" trong sơ đồ khối mạch I/O
+ Thông tin cấu hình thanh ghi: (Tài liệu: 8.4 GPIO register -> 8.4.5 GPIO port input data register
 ^Name: GPIO port input data Register (GPIOx_IDR), x: có thể là từ A,...I/J/K
 ^Address offset: 0x10
 ^Reset value : 0x0000_XXXX (Với X là số ko xác định)
 ^Thanh ghi 32 bit[31:0], tuy nhiên từ bit[31:16] KHÔNG đc sử dụng => Bit đc sử dụng[15:0] => từ IDR0->...->IDR15
 ^Từ bit[15:0] thì mỗi bit quản lý một pin của một port(Một port GPIOx có 16 pin)
 ^Cấu hình nhị phân:
  ~0: NO acccess input(Không read)
  ~1: Input (Read_Only)

*==GPIO Port Output Data Register(V67)
+ Thanh ghi đc sử dụng để đọc trạng thái I/O của một Pin của GPIO port
+ Đây chính là khối "Output Data Register" trong sơ đồ khối mạch I/O
+ Thông tin cấu hình thanh ghi: (Tài liệu: 8.4 GPIO register -> 8.4.6 GPIO port output data register
 ^Name: GPIO port output data Register (GPIOx_ODR), x: có thể là từ A,...I/J/K
 ^Address offset: 0x14
 ^Reset value : 0x0000_0000
 ^Thanh ghi 32 bit[31:0], tuy nhiên từ bit[31:16] KHÔNG đc sử dụng => Bit đc sử dụng[15:0] => từ ODR0->...->ODR15
 ^Từ bit[15:0] thì mỗi bit quản lý một pin của một port(Một port GPIOx có 16 pin)
 ^Cấu hình nhị phân:
  ~0: NO acccess output
  ~1: Access việc có thể read and write data

+ Tổng hợp các tính năng có thể thiết lập trên một pin của GPIOx
 ^Input floating
 ^Input pull-up
 ^Input pull-down
 ^Analog
 ^Output open-drain with pull-up or pull-down capability(khả năng, tích hợp)
 ^Output push-pull with pull-up or pull-down capability
 ^Alternate function push-pull with pull-up or pull-down capability
 ^Alternate function open-drain with pull-up or pull-down capability
**/

/**
*===GPIO Alternate functionality register and example of usage(S17)
*==Alternate functionality settings of a GPIO pin with example(V68+V69)
+ "Alternate functionality - chức năng thay thế" là một trong 4 chức năng có thể set trên một chân pin của GPIOx
+ Có thể thiết lập 16 chức năng thay thế là:
 ^AF0(system)
 ^AF1(TIM1/TIM2)
 ^AF2(TIM3..5)
 ^AF3(TIM8..11,CEC)
 ^AF4(I2C1..4,CEC)
 ^AF5(SPI1/2/3/4)
 ^AF6(SPI2/3/4,SAI1)
 ^AF7(SPI2/3,USART1..3,USART5,SPDIF-IN)
 ^AF8(SPI2/3,USART1..3,USART5,SPDIF-IN)
 ^AF9(CAN1/2,TIM12..14,QUADSPI)
 ^AF10(SAI2,QUADSPI,OTG_HS,OTG_FS)
 ^AF11
 ^AF12(FMC,SDIO,OTG_HS)
 ^AF13(DCMI)
 ^AF14
 ^AF15(EVENTOUT)

 Lưu ý: ko phải tất cả các pin của tất cả các port đều có thể thiết lập hết 16 chức năng thay thế, mà tùy vào nhà
sản xuất vđk mà chân nào của port nào sẽ đc sử dụng "chức năng thay thế" khác nhau. Vì vậy trước khi sử dụng tính
năng thay thế, thì ta cần xác định sử dụng chức năng thay thế nào, rồi dựa vào "datasheet" để xác định chân và port
đc phép sử dụng tính năng thay thế đó.
--->Tham khảo tài liệu sau để biết chân GPIOx nào có thể dùng Alternate:
    Datasheet_stm32f407x -> 3 Pinouts and pin description -> Table 9. Alternate function mapping

+ Bài tập 1: Hãy thiết lập tất cả "chức năng thay thế" có thể thiết lập trên GPIO port A ở chân số 8(GPIOA.8)
 ^Tham khỏa Detasheet: stm32f407vg.pdf -> 3.Pinouts and pin description -> Table9: Alternate function mapping
 ^Dựa vào bảng 9 này ta có thể xác định được trên pin nào của port nào đc phép sử dụng "tính năng thay thế" nào
 Vd: Chân PA8(PortA.8) có thể thiết lập AF0(MCO1_Micrôcntroller clock), AF1(TIM1_CH1), AF4(I2C3_SCL),AF7(USART_CK)....
 Nói cách khác là nó chân PA8 ko sử dụng đc tính năng thay thế AF2,AF3,...
 ^Từ đó: ta có thể lập bảng tổng hợp "tính năng thay thế" trên chân PA8
  ~AF0(MCO1), AF1(TIM1_CH1), AF2(Not Supported), AF3(Not Supported),AF4(I2C3_SCL), AF5(Not Supported)
  ~AF6(Not Supported), AF7(USART1_CK)

+ Bài tập 2: Hãy thiết lập tất cả "chức năng thay thế" có thể thiết lập trên GPIO port C ở chân số 6(GPIOC.6)
 ^Ta có thể lập bảng tổng hợp "tính năng thay thế" trên chân PC6
  ~AF0(Not Supported), AF1(Not Supported), AF2(TIM3_CH1), AF3(TIM8_CH1),AF4(Not Supported), AF5(I2S2_MCK)
  ~AF6(Not Supported), AF7(Not Supported),AF8(USART6_TK),AF9(Not Supported),AF10(Not Supported),
  ~AF11(Not Supported),AF12(SDIO_D6),AF13(DCMI_D0),AF14(Not Supported),AF15(EVENTOUT)

+ GPIO Alternate functionality Low/HIGH Register
 ^Cả 2 Thanh ghi đc sử dụng khi muốn kích hoạt Alternate functionality
 ^Thông tin cấu hình thanh ghi: (Tài liệu: 8.4 GPIO register -> 8.4.9/10 GPIO Alternate functionality Low/High register
 ^Name: GPIO alternate function low/high Register (GPIOx_AFRL and GPIOx_AFRH), x: có thể là từ A,...I/J/K
 ^Address offset: 0x20(GPIOx_AFRL) và 0x24(GPIOx_AFRH)
 ^Reset value :  0x0000_0000 for every ports
 ^Thanh ghi 32 bit[31:0]
 ^Mỗi 4bit[3:0] quản lý một chân pin của port => Thanh ghi 32bit thì quản lý đc 8 chân port(2 thanh ghi 16 pins)
  ~Thanh ghi GPIOx_AFRL quản lý 8 pin đầu của Portx từ AFRL0->...->AFRL7
  ~Thanh ghi GPIOx_AFRH quản lý 8 pin sau của Portx từ AFRH8->...->AFRH15
 ^Cấu hình nhị phân:
  ~0000:AF0		~0001:AF1	~0010:AF2	~0011:AF3	~0100:AF4	~0101:AF5	~0110:AF6	~0111:AF7
  ~1000:AF8		~1001:AF9	~1010:AF10	~1011:AF11	~1100:AF12	~1101:AF13	~1110:AF14	~1111:AF15

+ Bài tập 3: Thiết lập cấu hình thanh ghi để PA0 as UART4_TX, PA1 as UART4_RX, PA10 as TIM1_CH3
 ^PA0 as UART4-TX:
  ~Đầu tiên vào bảng Table9: Alternate function mapping để xác định chân PA0 thiết lập UART4-TX nằm ở AFx nào
  ~Xác định đc: PA0 as UART4-TX in AF8=1000
  ~Vì sử dụng PA0 nên thanh ghi cần cấu hình là GPIO Alternate functionality Low Register với 4 bit AFRL0[3:0]

 ^PA1 as UART4-RX:
  ~Đầu tiên vào bảng Table9: Alternate function mapping để xác định chân PA0 thiết lập UART4-RX nằm ở AFx nào
  ~Xác định đc: PA1 as UART4-RX in AF8=1000
  ~Vì sử dụng PA1 nên thanh ghi cần cấu hình là GPIO Alternate functionality Low Register với 4 bit AFRL1[3:0]

 ^PA10 as TIM3_CH3:
  ~Đầu tiên vào bảng Table9: Alternate function mapping để xác định chân PA0 thiết lập TIM3_CH3 nằm ở AFx nào
  ~Xác định đc: PA10 as TIM3_CH3 in AF1=0001
  ~Vì sử dụng PA10 nên thanh ghi cần cấu hình là GPIO Alternate functionality High Register với 4 bit AFRH10[3:0]
*/

/**
*===GPIO peripheral clock control - Điều khiển xung clock ngoại vi GPIO(S18)
*==Enabling(Cho phép) and Disabling(Vô hiệu hóa) GPIO peripheral clock(V71)
+ Sơ đồ: RCC Peri. Clock Enable Register ----> RCC_AHB1ENR
									|--------> RCC_AHB2ENR
									|--------> RCC_AHB3ENR
									|--------> RCC_APB1ENR
									|--------> RCC_APB2ENR
+ RCC Register như là một động cơ điều khiển các xung clock cho tất cả thành phần vđk, vd: AHB, APB, processor,memory,...
+ Các GPIO kết nối với processor thông qua bus AHB1(168MHz)	-> Để kích hoạt xung clock cho GPIO thì cần tìm RCC register
đang điều khiển AHB1 Bus -> Thanh ghi RCC AHB1 peripheral enable register(RCC_AHB1ENR)
+ Tài liệu: RM407 -> 7.3 RCC register ->  7.3.10 RCC AHB1 peripheral enable register(RCC_AHB1ENR)
 ^Name: RCC AHB1 peripheral enable register(RCC_AHB1ENR)
 ^Address offset: 0x30 (của thanh ghi liên quan đến RCC clock control)
 ^Reset value :  0x0010_0000 for every ports
 ^Thanh ghi 32 bit[31:0]
 ^Mỗi bit của thanh ghi này sẽ enable/disable xung clock một GPIOx hoặc có thêm các chức năng khác
  ~Bit0:GPIOAEN		~Bit1:GPIOBEN	~Bit2:GPIOCEN .....
  ~Ngoài ra trong thanh ghi còn điều khiển xung clock cho các chức năng khác GPIO
  =>Cần tham khảo Datasheet để xác định rõ chân cần điều khiển với port nào
 ^Cấu hình nhị phân(cho GPIO): 1:enable 	and 	0:disable
*/
