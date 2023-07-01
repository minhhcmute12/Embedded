/*
 * main.c *
 *  Created on: 2023/05/28
 *      Author: Minh
 */
/**
 * S10: Stack Memory - Bộ nhớ ngăn xếp
 * ------------------------------------- *
 * Bộ nhớ ngăn xếp là một phần của bộ nhớ chính (RAM trong hoặc RAM ngoài) dành riêng cho việc lưu trữ dữ liệu tạm thời.
 * Chủ yếu được sử dụng trong function, xử lý ngắt/ngoại lệ.
 * Bộ nhớ ngăn xếp được truy cập theo dạng Last In First Out (LIFO)
 * Ngăn xếp có thể được truy cập bằng cách sử dụng lệnh PUSH và POP hoặc sử dụng bất kỳ hướng dẫn điều khiển bộ nhớ nào (LD,STR)
 * Ngăn xếp được theo dõi bằng thanh ghi con trỏ ngăn xếp (SP). Lệnh PUSH và POP tác động (giảm hoặc tăng) thanh ghi con trỏ
 * ngăn xếp (SP,R13). Có nghĩa là với PUSH khi ta đẩy data vào stack sẽ đồng thời tăng SP và POP là dùng để đọc lùi data vị trí SP
 *
 * Tại sao sử dụng Stack memory?
 * .Lưu trữ tạm thời các giá trị của thanh ghi vi xử lý
 * .Lưu trữ tạm thời các biến cục bộ của hàm
 * .Trong trường hợp ngoại lệ(exception) hoặc ngắt hệ thống(interrupt), bộ nhớ ngăn xếp sẽ được sử dụng để lưu ngữ cảnh (một số
 * thanh ghi mục đích chung, thanh ghi trạng thái bộ xử lý, địa chỉ trả về) của mã hiện đang thực thi.
 *
 * Ví dụ: Trong Stm32F407 thì ta có SRAM (128 KB) trên vi điều khiển, bên trong SRAM được chia làm 3 phần(thứ tự từ START->END)
 * . Phần RAM dành cho "Global Data" : Sẽ được tận dụng khi chương trình chứa dữ liệu toàn cục và biến cục bộ tĩnh. Thậm chí bạn
 * có thể lưu trữ hướng dẫn ở đây và thực hiện nó.
 * . Một phần RAM dành riêng cho "Heap" : Sẽ được sử dụng trong quá trình cấp phát bộ nhớ động.
 * . Một phần RAM dành riêng cho "Stack" : Sẽ được sử dụng trong khi gọi hàm để lưu dữ liệu tạm thời, lưu trữ tạm thời các
 * biến cục bộ của hàm, lưu trữ tạm thời các khung ngăn xếp trong các ngắt và ngoại lệ.
 * ..Các phần ranh giới giữa các phần sẽ đc quyết định bằng cách thực hiện các cài đặt hoặc dùng công cụ hoặc là dùng tập lệnh
 * liên kết(linked cript) vì thế nó sẽ phụ thuộc vào dự án mà bạn muốn làm.
 *
 * Stack operation model- mô hình hoạt động của Stack
 * Mô hình ngăn xếp trong vi điều khiển ARM Cortex Mx thuộc loại Full Descending(FD-giảm dần đều)
 *
 * Phân biệt các mô hình hoạt động của Stack (V40)
 * .Full Ascending Stack (FA) - ngăn xếp tăng dần đều
 * Ta có định dạng địa chỉ từ thấp -> cao. Stack Poiter-SP ban đầu sẽ ở vị trí thấp nhất +1 . Vị trí phía trước dành cho Dữ liệu
 * đã đẩy trước đó (Previously pushed data). Khi có data mới đc thêm vào Stack thì nó sẽ đc tự động lưu vào vị trí cao hơn
 * (tăng đều), đồng thời SP sẽ đc di chuyển đến vị trí của data mới đó.
 *
 * .Full Descending Stack(FD) - ngăn xếp tăng giảm đều -> dùng cho ARM Cortex Mx
 * Định nghĩa ngược lại với FA, có định dạng địa chỉ từ cao -> thấp,Stack Poiter-SP ban đầu sẽ ở vị trí cao nhất -1. Vị trí cao
 * trước đó dành cho Dữ liệu đã đẩy trước đó (Previously pushed data). Khi có data mới đc thêm vào Stack thì nó sẽ đc tự động
 * lưu vào vị trí thấp hơn(giảm đều),đồng thời SP sẽ đc di chuyển đến vị trí của data mới đó.
 *
 * .Empty Ascending Stack(EA) - ngăn xếp trống tăng dần
 * Ta có định dạng địa chỉ từ thấp -> cao. Stack Poiter-SP ban đầu sẽ ở vị trí thấp nhất +1. Vị trí phía trước dành cho Dữ liệu
 * đã đẩy trước đó (Previously pushed data). Khi có data mới đc thêm vào Stack thì nó sẽ đc tự động lưu vào vị trí cao hơn
 * (tăng đều), đồng thời SP sẽ đc di chuyển đến vị trí phía tiếp sau của data mới đó (là ô trống nên đc gọi là Empty).
 *
 * .Empty Descending Stack(ED) - ngăn xếp trống tăng dần
 * Ta có định dạng địa chỉ từ cao -> thấp. Stack Poiter-SP ban đầu sẽ ở vị trí thấp nhấp -1. Vị trí phía trước dành cho Dữ liệu
 * đã đẩy trước đó (Previously pushed data). Khi có data mới đc thêm vào Stack thì nó sẽ đc tự động lưu vào vị trí thấp hơn
 * (giảm đều), đồng thời SP sẽ đc di chuyển đến vị trí phía tiếp trước của data mới đó.(là ô trống nên đc gọi là Empty).
 *
 * Stack Placement(V41)
 * Khái niệm một phần RAM và việc sử dụng để lưu trữ dữ liệu tạm thời-> có nghĩa là bạn chọn phần RAM nào để làm Stack
 * Ta có phần RAM có địa chỉ chạy từ RAM_START -> RAM_END. Ta có 2 mô hình phần bổ Stack là:
 * Mô hình 1: (RAM_START) Data -> Heap -> Stack -> Phần Ram ko dùng (RAM_END)
 *
 * Mô hình 2: (RAM_START) Data -> Heap -> Phần Ram ko dùng -> Stack (RAM_END)
 *
 * Lưu ý: trong cả 2 mô hình, phần Stack đều dùng mô hình Full Descending Stack(FD)
 * có nghĩa hướng hoạt động là:  Stack_START(RAM_END) -> Stack_END(RAM_START)
 * Còn 2 phần Heap và Data thì hướng hoạt động trùng với hướng RAM_START -> RAM_END
 *
 * Linker Cript:(tập lệnh liên kết): là tập lệnh quyết định các ranh giới như là nơi nào nên bắt đầu địa chỉ phần bộ nhớ
 * DATA trong RAM, và nó cũng ghi lại vị trí cuối cùng của phần DATA. Tiếp theo, quyết định vị trí khởi đầu phần HEAP, kích
 * thước của HEAP(vs bạn muốn bao nhiêu khi thiết lập bộ nhớ động),... Và cuối cùng là định vị trí bắt đầu của phần STACk
 * là theo mô hình 1 hay mô hình 2, kích thước bao nhiêu, hướng hoạt động theo mô hình nào(FA hay FD)
 * Vị trí của SP không bao giờ đc phép vượt qua khỏi kích thước mà nó đã đc cung cấp bởi Linker Cript. Khi bị vượt quá thì
 * ta gọi đó là lỗi tràn ngăn xếp (overflow of the Stack).
 *
 */

 /**
  * Stack memory is part of the main memory (Internal RAM or external RAM) reserved for the temporary storage of data
Mainly used during function, interrupt / exception handling.
Stack memory is accessed in last in firt out fashion(LIFO)
The stack can be accessed using PUSH and POP instructions or using any memory mainipulation instructions(LD,STR)
The stack is traced using a stack pointer(SP) register. PUSH and POP instructions affect (decrement or increment) stack pointer register (SP,R13)

  * .The temporary storage of processor register values
.The temporary storage of local variables of the function
.During system exception or interrupt, stack memory will be used to save the context (some general-purpose register, processor status register, return address)
 of the currently executing code.

 * Part of RAM reserved for "global data" : Will be utilized when the program contains global data and static local variables. Even you can store instructions here and execute it.
Part of RAM reserved for "Heap" : Will be utilized during dynamic memory allocation.
Part of RAM reserved for "Stack" : Will be utilized during function call to save temporary data, Temporary storage of local variables ofthe function, temporary storage of stack frames during interrupts and exceptions.
  */
