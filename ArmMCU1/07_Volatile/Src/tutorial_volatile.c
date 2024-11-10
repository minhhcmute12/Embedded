/*
 * tutorial_volatile.c
 *
 *  Created on: Oct 22, 2023
 *      Author: Minh
 */
/**
*==="Volatile" Keyword(S13)
*==Ôn lại kiến thức khóa cũ(ArmC)
+ Tối ưu hóa-Optimization: là hình thức thu gọn kích thước file source giúp tăng hiệu suất xử lý chương trình.
Tối ưu hóa thực hiện bằng cách sẽ bỏ qua hoặc ko thực hiện các dòng lệnh(hoặc là việc đọc bộ nhớ nhiều lần)
đc trình biên dịch cho là ko cần thiết.
Ở mức tối ưu hóa càng cao thì việc này việc bỏ qua ko thực hiện càng cao. Chính vì thế có thể xảy là tình trạng
bỏ qua ko thực hiện một số lệnh cần thiết mà chỉ có thể thực hiện khi run(Vd: chỉ có thể đọc trạng thái nút nhấn
đã đc nhấn hay chưa khi run chương trình) => Từ khóa "Volatile" ra đời để giải quyết tình trạng vừa muốn tối ưu
hóa ở một mức độ để tăng hiệu suất chương trình vừa ko gây ra lỗi do ko thực hiện đc lệnh code trong quá trình
Run chương trình. (Xem thêm ở bài "Read_Led_Pin_Optimization" ở khóa ArmC)

+ Volatile Keyword:
* Định nghĩa:
* Là một kiểu từ khóa hạn định, giới hạn(qualifier) để đặc tả các biến trong ngôn ngữ C.
* Vai trò sử dụng giống với từ khóa 'const' nhưng cách sử dụng khác nhau.
* Được dùng để hướng dẫn trình biên dịch không được gọi bất kỳ tối ưu nào(optimization) trên hoạt động của biến
* (Hoạt động của biến là hoạt động đọc và ghi)
* Thông báo cho trình biên dịch rằng giá trị biến có thể thay đổi bất cứ lúc nào mà không phải từ
* lập trình viên thay đổi. Vì vậy, trình biên dịch sẽ tắt tối ưu hóa các hoạt động read-write trên
* các biến được khai báo bằng cách khai báo sử dụng từ khóa 'volotile'.
* Từ khóa 'volotile' rất hữu ích trong mã hệ thống nhúng để giảm lỗi.
*
+ Cách sử dụng:
* Sử　dụng từ khóa 'volatile' khi biến có khả năng thay đổi bất ngờ về giá trị biến.
* Vd: giá trị biến đọc chân pin PA0 thường thay đổi bất ngờ, người dùng có thể thay đổi
* giá trị vào của biến đọc chân pin bất cứ lúc nào
* Một số trường hợp cụ thể:
* ++ Khi bạn làm việc với bộ nhớ được ánh xạ tới các thanh ghi ngoại vi của các bộ vi điều khiển.
* ++ Khi bạn phải làm nhiều task vụ như truy cập các biến global trong một ứng dụng đa luồng RTOS.
* ++ Khi một biến global được sử dụng để chia sẻ dữ liệu giữa main code và mã ISR.
* ISR_Interrupt Service Routine(ngắt)
*
+ Cách khai báo:
* ++Cách 1: volatile data (dùng cho biến)
*     kiểu_biến volatile tên_biến;    hoặc    volatile kiểu_biến tên_biến
* vd: uint8_t volatile my_data;       hoặc    volatile uint8_t my_data;
*
* ++Cách 2: non-volatile pointer to volatile data (dùng cho con trỏ-hay sử dụng trong embedded)
*     kiểu_biến volatiole *tên_con_trỏ
* vd: uint8_t volatile *pStatusReg
* Cách này thường được sử dụng trong trường hợp 1(cần ánh xạ đến các thanh ghi ngoại vi)

*==Part1+Part2(V46+V47)
+ Viết ví dụ và giải thích về khi "Không hoặc Có" volatile ở các mức ưu tiên level tăng dần từ 0
*/

