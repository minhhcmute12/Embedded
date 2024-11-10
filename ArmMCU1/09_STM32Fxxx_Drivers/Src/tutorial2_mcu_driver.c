/*
 * tutorial2_mcu_driver.c
 *
 *  Created on: Nov 17, 2023
 *      Author: Minh
 */
/**
*===GPIO driver API requirements and handle structure - Yêu cầu API trình điều khiển GPIO và cấu trúc xử lý(S23)
+Trong một chương trình nhúng ta sẽ code 3 lớp chính là:
 ^Lớp Driver: Chủ yếu chứa macros define address của các thanh ghi và thành phần làm việc với vđk
 ^Lớp APIx: Sử dụng các biến đã đc masros là các thanh ghi ở lớp Driver để Viết các hàm cụ thể cho từng tính năng
  Nói cách khác đây là lớp giúp liên kết giữa lớp Driver và lớp Application
 ~Lớp Application: kết hợp các chức năng (đã đc viết ở lớp Driver và APIx) của peripheral,vv... để tạo thành một
 hệ thống nhúng hoàn chỉnh

 ==> Mô hình lập trình hệ thống nhúng: Driver Layer <==> APIs Layer <==> Application Layer

+ Trong chương trình ta sẽ tiến hành viết lớp APIx và cụ thể là viết APIs cho GPIOs
 Lưu ý: mỗi peripheral thì ta nên viết riêng cho từng chức năng gồm: peripheral_x.h và peripheral_x.c

*==Creating GPIO Driver header and source file - Tạo tệp nguồn và Header file GPIO Driver(V88)
+ Tạo file source stm32f407xx_gpio_driver.c và header file stm32f407xx_gpio_driver.h

*==Defining GPIO handle and configuration(cấu hình) structure(V89)
+ GPIOx peripheral : GPIO Port Name, GPIO Pin Number, GPIO Mode, GPIO Speed, GPIO OutputType,
					 GPIO Pullup-Pulldown, GPIO Alt.function mode

+ Các mục trên dùng có thể cấu hình cho ứng dụng người dùng(chức năng mong muốn của hệ thống)

*==Driver API requirements(Yêu cầu) and adding(Thêm) API prototypes(hàm mẫu) (V90)
+ Sơ đồ: 						  -->	GPIO initialization
								  -->	Enable/Disable GPIO port Clock
			GPIO Driver <--> APIs -->	Read From a GPIO Pin
								  -->	Write to GPIO Pin
								  -->	Configure alternate functionality
								  -->	Interrupt Handling
+ Các GPIO Driver sẽ cung cấp tất cả các APIs(hàm-function) ở trên cho user application(Lớp Application)
+ Ta tiến hành lập trìn các APIs này(tiến hành khai báo và triển khai lập trình hàm)

*==Driver API input parameter and return types(V91)
+ Trong bài này ta sẽ tìm hiểu về tham số đầu vào(parameter) và kiểu trả về của Driver API
+ Để viết hàm void GPIO_DeInit() thì cần phải tìm hiểu về "7.3.5 RCC AHB1 peripheral reset register(RCC_AHB1RSTR)"

*==Driver empty API implementations(Triển khai code) and documentation(V92)
+ Tiến hành triển khai code các hàm đã khai báo trong driver_gpio.h sang driver_gpio.c
+ Mẫu comment một hàm trong chương trình:

	* @fn			-(tên hàm)
	*
	* @brief		-(mô tả ngắn gọn về hàm)
	*
	* @param[in]	-(biến đưa vào)
	* @param[in]	-
	* @param[in]	-
	*
	* @return		-(giá trị trả về)
	*
	* @Note			-(Chú ý)
	*
*/

/**
*===GPIO driver API Implemetation: CLOCK Control(S24)
*==Implemetation of GPIO peripheral clock control API(V93)
+ Trong bài và chương này ta sẽ tiến hành code triển khai hàm:
 "void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)"
*/

/**
*===GPIO driver API Implemetation: CLOCK init and de-init(S25)
*==Writting user configurable macros - Viết macro có thể định cấu hình của người dùng(V94)
*==Implemetation of GPIO init API(V95+V96)
+ Lưu ý: Có nghĩa là trước khi thực hiện khởi tạo ta sẽ xác định vị trí sử dụng pin_number nào sau đó sẽ lưu value
vào pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber
+ Trước khi thực hiện việc tác động vào trường bit của thanh ghi bất kì nào thì ta cần tiến hành reset trường bit
tại vị trí đó để khi ta cấu hình thì đc cấu hinh bit chính xác

+ Nếu ta code "pGPIOHandle->pGPIOx->MODER = temp;" thì điều đó ta đã yêu cầu tác động vào toàn bộ bit của thanh
ghi MODER. Điều này là ko cần thiết và nguy hiểm bởi vì nó có thể làm ảnh hưởng đến bit khác(vd bit khác trong
MODER register đang quản lý việc gì đó, việc thay đổi này dẫn đến ta vô tình thay đổi bit đó dẫn đến lỗi)
+ Vi vậy trong trường hợp này ta nên sử dụng toán tử OR để chỉ tác động tới vị trí bit mà ta mong muốn theo vị trí
pin_number muốn điều khiển:  "pGPIOHandle->pGPIOx->MODER |= temp;"

+ Trong lập trình nhúng, ban nên hạn chế sử dụng các phép gán "=" mà hãy sử dụng các toán tử BIT=WISE(OR hoặc AND)

*==Configure Alternate function Register(V97)
+ Theo datasheet ta có 2 thanh ghi quản lý Alternate là
 ^GPIO Alternate function high register (GPIOx_AFRH ->AFR[1]) với mỗi 4 bit quản lý một pin (PIN8->Pin15 of GPIO)
 ^GPIO Alternate function low register (GPIOx_AFRL ->AFR[0]) với mỗi 4 bit quản lý một pin (PIN0->Pin7 of GPIO)
+ Giả sử 1: người dùng sử dụng PIN9, lấy 9/8 = 1 dư xxx, lưu vào biến temp1, vì vậy bạn sẽ sử dụng AFR[1], có nghĩa
thanh ghi cần sử dụng để cấu hình là GPIOx_AFRH. Tiếp: temp2 = 9%8=1(lấy cụm 4bit thứ 2(tính từ 0) cho pin9)
 ->AFRH[temp1] = value << (4*temp2) = value << (4*1) -> Pin9 lấy từ bit thứ 4 của AFRH

+ Giả sử 2: người dùng sử dụng PIN6, lấy 6/8 = 0 dư xxx, lưu vào biến temp1, vì vậy bạn sẽ sử dụng AFR[0], có nghĩa
thanh ghi cần sử dụng để cấu hình là GPIOx_AFRL. Tiếp: temp2 = 6%8=6(lấy cụm 4bit bit thứ 6(tính từ 0) cho pin6)
 ->AFRL[temp1] = value << (4*temp2) = value << (4*6) -> Pin9 lấy từ bit thứ 24 của AFRH

*==GPIO De-Init API implementations(triển khai) (V98)
+ Triển khai code hàm "void GPIO_DeInit()"
+ Datasheet: 7.3.5 RCC AHB1 peripheral reset register(RCC_AHB1RSTR)
+ Mẹo: /Do..While: cách để thiết lập 2 mệnh đề  để macro với 1 biến, với while(0) thì chỉ cần thực hiện do{} 1 lần
 ^Vd: do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
*/

/**
*===GPIO driver API Implemetation: GPIO Data Read and Write(S26)
*==Implemetation of GPIO input port read and input pin read APIs(V99)
+ Lập trình các hàm APIs để đọc trạng thái ngõ vào GPIO port
+ Datasheet: 8.4.5 GPIO port input data register(GPIOx_IDR) (IDR0->IDR15)
+ Tại sao phải sử dụng phương pháp dịch bit trong câu lệnh mà không đọc trực tiếp trạng thái của pin number?
 ^Thanh ghi IDR của cổng GPIO chứa các bit đại diện cho trạng thái đầu vào của tất cả các chân trong cổng.
 Để đọc trạng thái của một pin cụ thể, chúng ta cần xác định vị trí của bit tương ứng trong thanh ghi IDR.
 Cách đơn giản nhất để làm điều này là sử dụng phương pháp dịch bit. Bằng cách dịch thanh ghi IDR sang phải
 theo số lượng vị trí bằng với số thứ tự của pin, chúng ta có thể sắp xếp bit tương ứng sang vị trí phải nhất.
 Sau đó, chúng ta có thể sử dụng phép AND bitwise để trích xuất bit này.
 => Đây chính là tác dụng của bộ so sánh trong nhân của chip, trích xuất value status từ thanh ghi
 ^Vd: value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

+ So sánh với phương pháp đọc trực tiếp trạng thái của pin number?
 ^Một phương pháp khác để đọc trạng thái của một pin cụ thể là sử dụng phương pháp truy cập trực tiếp vào thanh ghi IDR.
  Phương pháp này có thể được thực hiện như sau:
	uint8_t value;
	value = GPIOA->IDR & (1 << PinNumber);
 ^Câu lệnh này sẽ sử dụng phép AND bitwise để trích xuất bit ở vị trí PinNumbert của thanh ghi IDR. Bit này chính
 là trạng thái đầu vào của pin GPIOAx.
 Phương pháp này cũng có thể hoạt động, nhưng nó kém hiệu quả hơn phương pháp sử dụng dịch bit. Lý do là phương pháp
 này yêu cầu chúng ta phải thực hiện phép AND bitwise trên tất cả các bit trong thanh ghi IDR, ngay cả khi chỉ có một
 bit duy nhất mà chúng ta quan tâm.
 Trong trường hợp này, phương pháp sử dụng dịch bit chỉ cần thực hiện một phép dịch bit và một phép AND bitwise. Điều này
 khiến nó hiệu quả hơn đáng kể về mặt thời gian và bộ nhớ

*==Implemetation of GPIO output port write and output pin write APIs(V100)

*==Implemetation of GPIO pin toggle API(101)
+ Code: pGPIOx->ODR = pGPIOx->ODR ^ (1 << PinNumber);  or pGPIOx->ODR ^= (1 << PinNumber);
+ Phần này thực hiện thao tác OR (XOR) độc quyền theo bit giữa giá trị ODR hiện tại và mặt nạ bit với một bit duy nhất
được đặt tại vị trí PinNumber. Hoạt động XOR đảo ngược bit ở chân được chỉ định, chuyển đổi trạng thái đầu ra của nó.
*/
