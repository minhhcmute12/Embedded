/*
 * tutorial_vector_table.c
 *
 *  Created on: Oct 21, 2023
 *      Author: Minh
 */
/**
*==MCU Vector table(S11)
+ Vector: là một cái gì đó để làm hướng(direction) và chúng ta thường gọi nó là con trỏ(pointer) hoặc address
Có nghĩa là có thể nói "Vector Table" là "Pointer or Address Table"
+ Addresses(Các địa chỉ) ở đây là các "address of exception handler(system exception + interrupt)"
+ Vector Table lưu trữ địa chỉ của các trình xử lý ngoại lệ(exception handler) khác nhau: 15 system exception
và 240 interrupts(external exception)
+ Ví dụ: Ta sẽ tiến hành phân tích Table 61. Vector table for STM32F405xx/07xx (DM00031020- Tr.372)
 ^Cột Position:
  ~Trong bảng thì cột này đc để trống cho tất cả các "system exception", nguyên nhân vì tất cả chúng
 đều đã đc cố định, và các nhà cung cấp vi điều khiển ko có quyền kiểm soát các vị trí của các ngoại lệ hệ thống.
 Ko có quyền vì việc này đc thực hiện bởi người thiết kế processor(core ARM). Các hãng sản xuất vi điều khiển như
 là ST ko có quyền quyết định các vị trí của "system exception"
  ~Các exception liên quan đến "NVIC-Nested Vector Interrupt Control" và IRQx đc đánh số thứ tự. số thứ tự "0" đc
  đánh dấu cho "WWDG"->......

 ^Cột Priority: cho biết mức độ ưu tiên của exception
 ^Cột Type Priority: cho biết exception đó đc thiết lập là ưu tiên cố định(fixed-ko thể thay đổi mức ưu tiên) hay
 là ưu tiên biến đổi(settable-có thể định cấu hình mức ưu tiên bằng cách code chương trình)
 Lưu ý: số mức ưu tiên càng thấp thì mức ưu tiên trong trình xử lý càng cao

 ^Cột Acronym: cho biết tên viết tắt của các exception
 ^Cột Description: cung cấp các miêu tả ngắn gọn về exception
 ^Cột Address: cung cấp địa chỉ của các exception trong memory map
 Lưu ý: bạn phải giữ các exception handler tương ứng với các địa chỉ đc cung cấp trong bảng.
 Vd: Với địa chỉ 0x0000_0004 thì bạn phải giữ địa chỉ này cho trình xử lý "Reset Handler". Nếu bạn để trình xử lý
 khác đc vào địa chỉ chỉ định 0x0000_0004 thì sẽ dẫn tới exception xảy ra. Điều này sẽ tương tự cho tất cả các
 exception đc cung cấp bên trong Vector Table
  ~Với địa chỉ 0x0000_0000 (vị trí đầu tiên): bạn nên lưu địa chỉ của con trỏ ngăn xếp ban đầu(initial stack pointer
  address) ở vị trí này. Để bất kỳ khi nào bộ xử lý Reset thì bộ xử lý sẽ đọc nội dung của vị trí này đầu tiên
  từ đó ta có đc value của MSP(Main Stack Pointer) để bắt đầu lại các chương trình đc code lưu trong Memory.
  Và sau đó bộ xử lý sẽ tiến hành đọc tuần tự các địa chỉ tiếp theo trong bảng Vector Table

+ Tìm hiểu về một số thông tin đc lưu trong file startup_stm32xxx
 ^Đây là tệp khởi động đc tạo bởi STM32CubeIDE dưới dạng file .s; Viết chủ yếu bằng Assembly language(hoặc một chút C)
 ^Các điểm cần chú ý:
  ~Reset Handler : hướng dẫn(instruction-lệnh hoặc có thể gọi là một hàm) đầu tiên về dự án của bạn, và theo bảng Vector
  Table thì địa chỉ 0x0000_0004 sẽ đc lưu địa chỉ của hàm Reset Handler
  ~The STM32F407VGtxx Vector Table: bạn có thể xem nó như là một "Array-Mảng", chứa các phần tử là các Handler mô phỏng
  lại bảng Vector Table đc code theo ngôn ngữ Assembly. Vị trí đầu tiên của mảng là value của "initial stack pointer address".
  Lưu ý: bạn có thể xem thêm về video cách viết file startup_stm32xxx mà ko sử dụng IDE: https://www.youtube.com/playlist?list=PLERTijJOmYrDiiWd10iRHY0VRHdJwUH4g
  Lưu ý: về các .section trong bộ nhớ(đc miêu tả liên kết trong Linker Cript)
  Lưu ý: Value địa chỉ các exception đc lưu trong địa chỉ Vector Table đc lưu bên trong bộ nhớ FLASH Memory
  ~Ta có thể Run and Debug để quan sát vi xử lý xử lý file startup_stm32xxx
  Lưu ý: các value địa chỉ Reset Handler đc lưu trong địa chỉ 0x0000_0004 thường sẽ đc +1 để biểu đạt Tbit=1(Thumb Mode)
  ~Provide weak aliases: chứa các thông tin của "dummy implementations", đây là lệnh dùng để thông báo hàm, và sau đó các
  nội dung hàm sẽ đc code bởi lập trình viên trong file code và sau đó ghi đè lên các hàm "dummy implementations" này khi
  chương trình đc chạy. Các "dummy implementations" sẽ thông báo trước cho vi xử lý là có sự xuất hiện của các hàm xử lý
  exception mà ko cần phải khai báo hàm trong các thư viện .h; điều này giúp ta trả lời đc câu hỏi tại sao các hàm bình
  thường thường phải khai báo trước hàm main hoặc trong file thư viên .h mà trong khi các hàm xử lý exception-interrupt
  lại ko cần phải làm như vậy, vì chúng đã đc khai báo trước trong file startup_stm32xxx
*/

