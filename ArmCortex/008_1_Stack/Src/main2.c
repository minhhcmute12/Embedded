/**
* Stack activities during interrupt and exception - Hoạt động của Stack khi đang trong ngắt và ngoại lệ (V46)
* -----------------------------------------------------------------------------------------------------
* Đặt vấn đề, Theo tiêu chuẩn AAPCS thì ta có 2 hàm tiêu chuẩn là caller và callee. Mỗi hàm trong 2 hàm này đều có trách nhiệm, công việc
* riêng cần phải xử lý vd như caller phải lưu value trên các thanh ghi cho phép hay callee cũng thế. Tuy nhiên vấn đề ở đây là nếu có thiết
* lập ngắt(interrupt) hay ngoại lệ(exception)?

* Interrupt and Exception là 2 hàm đặc biệt này không có caller vì về bản chất nó là các hàm bất đồng bộ, nó có thể đến bất cứ lúc
* nào(ko xác định chính xác đc thời gian, thường là cần điều kiện ngoại vi để kích hoạt). Vì không có caller nên chính bộ vi xử lý
* sẽ là người quyết định (caller save register) việc gọi 2 hàm này. Vì vậy bạn có thể thoải mái viết chương trình ngắt hay ngoại lệ
* bằng 'C' mà không cần lo lắng về quy tắc AAPCS.

* Vd: Giả sử bạn có một thread(luồng) đang chạy và đột nhiên xuất hiện một ngắt(vd như nhấn nút) và bộ vi xử lý sẽ bắt đầu thực thi trình
* xử lý ngắt một cách tự động. Nhưng trước đó, các value thanh ghi sẽ đc lưu một cách tự động bởi phần cứng của bộ vi xử lý. Phần cứng này
* chính là Stack. Trình biên dịch sẽ ko cần phải tạo bất kỳ hướng dẫn nào (viết code) để làm việc này vì bộ vi xử lý sẽ tự động làm việc
* này. Vì vậy bạn có thể viết chương trình ngắt và ngoại lệ một cách bình thường.
* Giải thích việc lưu các value thanh ghi bởi phần cứng: Các value ở đây chính là các value như là địa chỉ của vị trí mà luồng đang phải dừng
* lại để xử lý ngắt hoặc ngoại lệ hay các value cần thiết để sau khi ngắt hay ngoại lệ xảy ra chương trình có thể tiếp tục hoạt động.
* Tất cả sẽ đc lưu ở vùng Stack và sau khi kết thúc ngắt hay ngoại lệ thì vi xử lý sẽ đọc chúng để có thể tiếp tục luồng xử lý.
* Xem thêm sơ đồ hoạt động: V46 3p10s

* Stack initialization - Khởi tạo ngăn xếp
* -----------------------------------------
* Có 2 giai đoạn: Trước và Sau khi xử lý hàm main
* + Trước khi hàm main đc đọc thì Stack phải đc khởi tạo. Và điều này thường đc thực hiện bởi chính bộ vi xử lý bằng cách đọc vị trí đầu tiên
* của bảng vector. Đó là lý do, bạn phải luôn đảm bảo rằng vị trí đầu tiên của bảng vector phải giữ giá trị hợp lệ cho thanh ghi con trỏ
* ngăn xếp. Có nghĩa là vị trí đó phải dùng làm địa chỉ khởi tạo con trỏ ngăn xếp  Điều này thường đc thực hiện bởi mã khởi động mà chúng
* ta học đc ở các bài trước.
* + Sau khi đọc xong các hàm main, bạn cũng có thể khởi tạo, khởi động lại ngăn xếp. Giả sử, sau khi đọc hàm main, bạn muốn chuyển ngăn xếp
* sang một số bộ nhớ khác(như SRAM3, external RAM). Việc chuyển này sẽ ko thể làm ở gai đoạn trước khi đọc hàm main.
*
* Một số thủ thuật tạo Stack:
* + Bạn phải đánh giá ứng dụng mục tiêu của mình. Quyết định số lượng ngăn xếp sẽ cần thiết cho thời gian xấu nhất trong thời gian chạy
* ứng dụng của bạn. Vd: số hàm ngắt hoặc ngoại lệ là bao nhiêu để cần số lượng tương ứng khởi tạo Stack.
* + Cần biết bộ xử lý Stack của bạn thuộc loại nào. Vd: FD,FA,ED,EA
* + Quyết định vị trí Stack trong RAM. Bạn muốn đặt nó ở đâu: middle(giữa), end(cuối), hay external Memory. Bạn có thể điều chỉnh vị trí
* bộ nhớ bằng cách sử dụng linker script.
* + Trong nhiều ứng dụng, Stack đc khai báo ở giai đoạn 2. Vd: Nếu bạn muốn phân bổ ngăn xếp trong external(ngoại) SDRAM thì trước tiên
* hãy bắt đầu với internal(nội) RAM. Có nghĩa là trong hàm main hoặc startup code ta khởi tạo một SDRAM và sau đó thay đổi stack pointer
* để trỏ tới SDRRAM. Vì vậy có 2 giai đoạn: đầu tiên, ứng dụng khởi động với internal RAM dưới dạng một stack, sau đó trong hàm main, bạn
* định cấu hình external SDRAM, sau đó bạn sủ dụng external SDRAM làm Stack.
* + Nếu bạn sử dụng một bộ xử lý ARM Cortex Mx, hãy chắc chắn rằng vị trí đầu tiên của bảng vector chứa địa chỉ khởi tạo Stack ban đầu.
* The starup code của project thường làm công việc này.
* + Bạn cũng có thể sử dụng "linker script-tập lệnh liên kết" để quyết định ranh giới Stack,Heap, và các vùng RAM khác. The startup code
* thường lấy thông tin ranh giới từ linker script.
* + Trong một kịch bản RTOS, kernel code(mã lõi) có thể sử dụng MSP để theo dõi ngăn xếp của chính nó và cấu hình PSP cho ngăn xếp tác vụ
* của người dùng.
*/
