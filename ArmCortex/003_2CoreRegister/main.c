/*
 * main.c *
 *  Created on: May 14, 2023
 *      Author: Minh
 */

//Tham khảo tài liệu: CortexM4 generic user guide -> The Cortex M4 processor -> 2.1.3 Core Register
/*
 * Core Registers: Thanh ghi vi xử lý
 * ------------------------------------------
 *
 * Tiến hành phân tích các thành phần bên trong thanh ghi của vi xử lý Cortex-M4
 * 1) Thanh ghi từ R0 -> R12 (13 register) được dùng cho mục đích chung như là: hoạt động lưu trữ dữ liệu (store data),
 * hoạt động lưu trữ địa chỉ(store addresses),hoạt động thao tác dữ liệu(data manipulation), hoạt động thực thi
 * dữ liệu(data operation),...
 *
 * 2) Tất cả các Core registers đều có độ rộng là 32bit
 *
 * 3) Thanh ghi R13 được gọi là Stack Pointer(SP-Con trỏ ngăn xếp). Được thường xuyên sử dụng để theo dõi
 * Stack Memory(bộ nhớ ngăn xếp). Ngoài ra, sẽ có thêm 2 thanh ghi tương tự là Processor Stack Pointer(PSP-bộ xử lý con trỏ)
 * và Main Stack Pointer(MSP-Con trỏ ngăn xếp chính).
 *
 * 4) Thanh ghi R14 được gọi là Link Register(LR-Thanh ghi liên kết). Nó sẽ lưu trữ các thông tin trả lại(return) cho các
 * chương trình con(subroutines), các hàm được gọi(function call), và ngoại lệ(exception). Và khi ta tiến hành
 * reset, vi xử lý sẽ thiết lập giá trị địa chỉ cho thanh ghi LR là 0XFFFFFFFF.
 * Ví dụ về hoạt động của LR: Viết hai chương trình
 * 		Caller											Callee
 * 	void fun1(void)									Void fun2(void)
 * 	{												{
 * 		fun1_ins_1;										fun2_ins_1;
 * 		fun2();	//*_1									fun2_ins_2;
 * 		fun1_ins_2;	//*_2							}
 * 	}
 * Chương trình bắt đầu ở "Caller", Khi chương trình chạy đến dòng '*_1' thì lập tức hàm "Callee" được gọi. Đồng thời lúc đó
 * vi xử lý còn làm 1 hành động nữa là lưu địa chỉ của hướng dẫn tiếp theo, tức là lưu địa chỉ dòng '*_2' vào tròn thanh ghi LR.
 * Mục đích là sau khi hàm 'Callee' hoàn thành thì chương trình phải quay về hàm "Caller" để xử lý tiếp. Và tại thời điểm đó thì
 * địa chỉ đã đc lưu trước đó trong thanh ghi LR sẽ đc sao chép vào PC, đó là cách PC nhảy trở lại để tiếp tục xử lý hàm fun1.
 *
 * 5) Thanh ghi R15 được gọi là Program Counter(PC-Bộ đếm chương trình). Nó chứa địa chỉ của chương trình hiện tại hoặc là chứa
 * các hướng dẫn hiện tại sẽ được thực thi. Khi reset thì vi xử lý sẽ thiết lập PC cùng với giá trị của 'Reset Vector' có địa
 * chỉ là 0X00000004(bit0=0). Vì Bit 0 của giá trị đc tải vào EPSR T-bit khi reset và phải là 1. Nên nếu bit0=0 thì trước khi
 * chuyển bitO vào T-bit, trình biên dịch sẽ thông qua thanh ghi này tiến hành lệnh cộng 1 bit để lên 1.
 * EPSR T-bit: thanh ghi đặc biệt, đc giải thích ở bên dưới.
 *
 * **Các thanh ghi đặc biệt (Special Register)
 * 6) Thanh ghi Đăng ký trạng thái chương trình (PSR-Program Status Register),nó cho biết nó đang giữ trạng thái nào của việc thực
 * hiện chương trình hiện tại. Có độ rộng 32 bit. Nó được tập hợp từ 3 thanh ghi phụ khác nhau:
 * + 6.1)Application Program Status Register(APSR-thanh ghi đăng ký trạng thái ứng dụng), chiếm 5 bit(từ bit 31->bit 27)
 * Chứa tất cả flag(cờ) có điều kiện
 * Vd: Bit[31]_N_Negatice flag: Nếu thực thi lệnh hiện tại dẫn đến bất kỳ giá trị âm nào trong ALU thì cờ này đc kích hoạt.
 * Bit[30]_Z_Zero flag: Nếu thực thi lệnh hiện tại dẫn đến bất kỳ giá trị 0 nào trong ALU thì cờ này đc kích hoạt.
 * Đọc thêm datasheet để biết về 3 bit còn lại
 *
 * + 6.2)Interrupt Program Status Register(IPSR-Thanh ghi đăng ký trạng thái ngắt), chiếm 9 bit (từ bit 0 -> bit 8)
 * Chứa các kiểu ngoại lệ hiện tại của ngắt(interrupt)
 *
 * + 6.3)Execution Program Status Register(EPSR- Thanh ghi đăng ký trạng tháo thực thi), chiếm 18 bit (từ bit 26->bit 9)
 * Trong thanh ghi này chứa thành phần T-bit(bit 24 trong 32 bit) quan trọng
 * Chức năng quan trọng của T-bit:
 * - Nếu T-bit của EPSR được đặt thành 1, bộ xử lý nghĩ rằng hướng dẫn tiếp theo mà nó sắp thực hiện là từ Thumb ISA
 * Thumb ISA: là một kiến trúc tập lệnh được viết dành riêng cho bộ vi xử lý Cortex MX ARM.
 *
 * -Nếu T-bit của EPSR được đặt thành 0, bộ xử lý nghĩ rằng hướng dẫn tiếp theo mà nó sắp thực hiện là từ ARM ISA
 *
 * => Luôn luôn để mặc địch T-bit ở trạng thái 1, vì khi T-bit bằng 0 sẽ dẫn đến lỗi bộ vi xử lý.
 *
 * 7) Ngoài ra, còn có một số thanh ghi đặc biệt khác như: Tổ hợp 3 thanh ghi Exception Mask Register(Primask, Faulmask,Baseprri);
 * Thanh ghi Control Register
 *
 * 8) Một số lưu ý:
 * + Tất cả các thanh ghi ở trên được gọi là thanh ghi bản đồ phi bộ nhớ (Non-memory mapped register) vì những thanh ghi này không có
 * địa chỉ duy nhất để truy cập chúng. Chúng không phải là một phần của memory-map. Tất cả các mục đích như là Stack pointer,
 * program counter, link register, special register,... đều không là memory-map. Chúng là nội tại xử lý của lõi bộ xử lý mà không
 * có bất kỳ địa chỉ nào để truy cập chúng từ chương trình C. Có nghĩa là bạn không thể dùng kỹ thuật thao tác địa chỉ của chương
 * trình C của mình để truy cập vào các thanh ghi này.
 * Nếu bạn muốn truy cập các thanh ghi này thì bạn phải sử dụng các chỉ dẫn assembly. Có nghĩa là đầu tiên bại phải truy cập các
 * thanh ghi mục đích chung từ code C, sau đó bạn phải sử dụng các hướng dẫn chuyển động như 'MOV' hoặc 'LOAD' để thực thi yêu cầu.
 *
 * + Thanh ghi bản đồ bộ nhớ (Memory mapped registers): là một phần thuộc bộ nhớ của vi xử lý. Có nghĩa là mọi thanh ghi đều có
 * địa chỉ, và bằng cách sử dụng địa chỉ đó trong chương trình, bạn có thể xóa(dereference), read, write dữ liệu các thanh ghi này.
 * Bạn có thể truy cập các thanh ghi này trong chương tình C mà không phải sử dụng các assembly instruction.
 * Trong Memory mapped registers : gồm 2 loại
 *  ++ Registers of the processor specific peripherals(Thanh ghi vi xử lý các thiết bị ngoại vi):
 *   vd: .Nested vector interrupt controller(bộ điều khiển ngắt vector lồng nhau),
 *        .The system control block(khối điều khiển hệ thống),
 *        . The memory protection unit (đơn vị bảo vệ bộ nhớ),
 *        . Debug unit (bộ thực thi lệnh)
 *        . Periphearals of the Processor (Bộ xử lý các thiết bị ngoại vi),...
 *
 *  ++ Registers of the Microcontroler specific peripherals(Thanh ghi vi điều khiển các thiết bị ngoại vi đặc biệt)
 *   vd: RTC, I2C, Timer, CAN, USB,...
 *
 *
  */
