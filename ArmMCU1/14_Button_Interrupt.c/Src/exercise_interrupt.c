/*
 * exercise_interrupt.c
 *
 *  Created on: Nov 22, 2023
 *      Author: Minh
 */

/**
*===Exercise: GPIO Interrupt(S29)
*==Exercise: External button interrupt implementation(V115)
+ Đề: Connect an external button to PD5 pin and toggle the led whenever interrupt is triggered by the button press.
Interrupt should be triggered during falling edge of button press. Kết nối nút bên ngoài với chân PD5 và bật tắt
đèn LED bất cứ khi nào ngắt được kích hoạt bằng cách nhấn nút. Ngắt sẽ được kích hoạt trong khi nhấn nút xuống(FT).
 ^Thiết lập cấu hình cho led và nút nhấn(cấu hình nhấn thực hiện interrupt)
 ^Thiết lập cấu hình cho ngắt(thiết lập interrupt number and interrupt priority level)
 ^Lấy hàm ngắt(xác định từ interrupt number) đã đc định danh trong file startup_stm32xxx:
 Vd: Ta sử dụng ngắt IRQ_NO_EXTI9_5 nên cần tìm hàm liên quan tới EXTI9_5 -> gọi hàm "EXTI9_5_IRQHandler"
 ^Thực hiện hàm ngắt trên vị trí pin input thiết lập ngắt

+ Chạy lần 1: sẽ bị lỗi trong quá trình thiết lập thanh ghi(ko phải lỗi cấu trúc code) làm đèn ko sáng khi nhấn.
Tiến hành debig và gỡ lỗi:
 ^Kiểm tra quá trình nhấn có thực hiện hàm ngắt hay ko
 ^Kiển tra cửa sổ I/O Register -> Kiểm tra các thanh ghi liên quan đến thiết lập led và button

 Nguyên nhân lỗi: vì ta đã sử dụng 2 lần hàm khởi tạo GPIO_Init trên cùng một thanh ghi port là GPIOD, dẫn tới ở
 lần khởi tạo GPIO_Init thứ 2 thì việc khởi tạo input(button) đã làm thay đổi thông số khởi tạo output(led) ở lần 1.
 Nghĩa là value của các thành phần trong struct GpioBtn đã ảnh hưởng đến value của các thành phần struct GpioLed.
 Nguyên nhân của ảnh hưởng này là mặc dù GpioLed và GpioBtn đều là các biến cục bộ, và ko liên quan đến nhau trong
 quá trình code, tuy nhiên thành phần của struct hai biến này đều hướng đến cùng một thanh ghi nếu thiết lập trên
 cùng một port. Dẫn đến việc trình biên dịch tạo địa chỉ giống nhau cho các các thành phần giống nhau của hai struct
 khi thiết lập thanh ghi. Dẫn đến khả năng value có thể bị ghi đè khi khởi tạo
 Vd: khi GPIO_Init của led đc khởi tạo thì thanh ghi OPType đc thiết lập value. tuy nhiên khi GPIO_Init của button
 đc khởi tạo thì do ko đc set value thành phân nên trình biên dịch đã vô tình làm thay đổi thanh ghi OPType của led.
 Dẫn đến việc thiết lập cấu hình led là có các sai số xuất hiện làm led ko hoạt động.
 ==>Khi thực hiện các chức năng trên cùng một port cần chú ý đến việc khi khởi tạo các chức năng có thể gây ảnh hưởng
 lẫn nhau do việc ghi thông số cấu hình lên các thanh ghi.

 Giải pháp: Khởi tạo 2 biến cấu trúc GpioLed và GpioBtn với mỗi thành viên của  2 cấu trúc này đều thành 0. Có nghĩa
 là yêu cầu tách hai struct thành hai biến cục bộ có địa chỉ khác nhau. Địa chỉ của các thành phần trong struct sẽ ko
 ảnh hưởng đến địa chỉ của các thành phần struct khác có độ giống nhau về thành phần.
 Cách làm: dùng hàm "memset()"

+ Hàm memset() trong hệ thống nhúng được sử dụng để sao chép một giá trị nhất định vào một vùng bộ nhớ nhất định.
Hàm này có thể được sử dụng để khởi tạo các biến, mảng hoặc các vùng bộ nhớ khác.
 Cú pháp: void *memset(void *s, int c, size_t n);
 Trong đó: 		s: Con trỏ đến vùng bộ nhớ cần sao chép
				c: Giá trị cần sao chép
				n: Số lượng byte cần sao chép

+ Việc interrupt khi nhấn nút cũng phải đc "chống dội", vì khi bị dội thì có khả năng thực hiện ngắt nhiều lần

*==Exercise: Debugging the application Part1+2(V116+V117)
+ Hãy dành thời gian để tiến hành debug dể có thể thực sự nắm vững kỹ năng lập trình nhúng. Vì trong lập trình nhúng
có rất nhiều lỗi mà tìm kiếm trên internet sẽ ko có mà phải tự debug.
+ Hầu hết mọi trường hợp, việc bạn cần làm khi debug là kiểm tra các thanh ghi register cấu hình của peripheral device,
nghĩa là kiểm tra chéo xem tất cả các bit có đc cấu hình đúng hay không.
+ Hoặc là tùy theo chức năng mà bạn đang làm, thì việc cấu hình các thanh ghi liên quan đến chức năng đó có đúng hay không.
+ Bạn cũng có thể xem lại debug cách chạy của các bài đã chạy thành công

+ Mẹo: sử dụng breakpoints tại vị trí các hàm dùng để thiết lập value cho các thanh ghi, vd như vị trí các hàm GPIO_Init().
Sau đó tiến hành chương trình debug -> vào showView -> I/O register(quan sát các value input/output trên các thanh ghi)
hoặc các cửa sổ khác mà bạn mong muốn kiểm tra

+ Sửa một số lỗi đến hàm GPIO_IRQPriorityConfig() => File driver chuẩn GPIO Interrupt
*/
