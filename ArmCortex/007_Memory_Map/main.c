/*
 * main.c *
 *  Created on: 2023/05/25
 *      Author: Minh
 */

/**
 * S9: Memory Map and Bus Interfaces
 * ----------------------------------
 * 1.Memory map of processor - Bản đồ bộ nhớ của bộ xử lý (V35)
 * .Bản đồ bộ nhớ giải thích việc ánh xạ các thanh ghi và bộ nhớ ngoại vi khác nhau trong phạm vi vị trí bộ nhớ có thể
 * định địa chỉ của bộ xử lý.
 * .Bộ xử lý, phạm vi vị trí bộ nhớ có thể định địa chỉ, phụ thuộc vào kích thước của bus địa chỉ.
 * .Việc ánh xạ các vùng khác nhau trong phạm vi vị trí bộ nhớ có thể định địa chỉ được gọi là 'bản đồ bộ nhớ'
 *
 * Ta có một bộ vi xử lý ARM Cortex M4. Sau đó ta sẽ có một kết nối nội bộ bên trong bộ vi điều khiển(interconnect)
 * hoặc ta gọi đơn giản nó là đường kết nối(line bus). System Bus giúp kết nối bộ vi xử lý với các thiết bị ngoại vi khác nhau
 * (GPIO,TIMER, ADC, I2C) và bộ nhớ(Data_Memory-nơi lưu dữ liệu tạm thời chương trình, Code_Memory-nơi lưu các hướng dẫn
 * chương trình) bên trong bộ vi điều khiển.
 *
 * System Bus(ta xem lệnh từ vxl nó như là 1 chiếc xe bus, SystemBus(AHB) như là con đường cho xe bus và địa chỉ chính là
 * các trạm xe bus) có 32bit address channels và có 32bit data chanels.
 *
 * Cách giao tiếp và truyền dữ liệu (vxl <=> các thiết bị ngoại vi <=> bộ nhớ) thông qua system bus:
 * Ví dụ: Ta muốn vi xử lý truyền dữ liệu ở ADC lưu vào trong Data Memory(bộ nhớ của vi điều khiển)
 * Dữ liệu của ADC sau khi đc xử lý sẽ đc lưu trên thanh ghi của ADC. Bộ vi xử lý sẽ tìm cách để truy cập vào dữ liệu đó.
 * Vì vậy vxl sẽ đăng ký 1 địa chỉ trên System bus(ABH), khi địa chỉ đó khớp với địa chỉ thanh ghi mà ADC dùng làm lưu dữ
 * liệu thì thanh ghi sẽ đc mở khóa và dữ liệu đc lưu trên thanh ghi đó sẽ đc đọc trở thành data bus. Sau khi trở thành
 * data bus thì dữ liệu đó sẽ đc di chuyển và lưu vào 1 trong các thanh ghi nội bộ Core Register. Sau đó dữ liệu từ thanh ghi
 * nội bộ sẽ đc di chuyển và lưu vào bên trong thanh ghi của Data Memory.
 * Lưu ý: tất cả các bước ở trên sẽ thông qua code lập trình hoặc code Assembly để xử lý
 * VD: việc đăng ký địa chỉ chính là việc ta code các địa chỉ đc hướng dẫn trong datasheet
 *
 * Vì sử dụng 32bit nên Memory MAp của ARM Cortex Mx thường có địa chỉ từ 0x00000000 -> 0xFFFFFFFF
 * Ta gọi vùng địa chỉ đó là phạm vi bộ nhớ (memory range). Và bên trong vùng đó cũng đc thiết kế với các phân vùng nhỏ hơn theo
 * các chức năng đc thiết kế cố định.  Bạn có thể thay đổi nhưng không nên vì chúng đã đc thiết kế gần như tối ưu.
 * Ví dụ: Khi vxl tạo địa chỉ 0 trên System_Bus có nghĩa là bộ vxl muốn giao tiếp với vùng chứa Code.
 *
 * Phân vùng: Vendor-specific memory -> Code Memory
 * + Code Memory 0.5GB :
 * + SRRAM 0.5GB: Ram của vi điều khiển
 * + Peripheral 0.5GB: nơi đặt tất cả địa chỉ của thanh ghi các thiết bị ngoại vi.
 * + External RAM(ram nội bộ) 1.0GB:
 * + Môt số vung khác: Vendor-specific memory 511MB, Private peripheral bus 1.0MB, External device 1.0GB
 *
 * +Tìm hiểu về phân vùng CODE memory: (0x00000000 -> 0x1FFFFFFF 521MB)
 * Đây là khu vực mà các nhà cung cấp MCU nên kết nối CODE memory
 * Các loại CODE memory khác nhau là: Flash nhúng, ROM, OTP, EEPROM, v.v.
 * Bộ xử lý theo mặc định tìm nạp thông tin bảng vectơ từ vùng này ngay sau khi đặt lại
 *
 * +Tìm hiểu về phân vùng SRAM : (0x20000000 -> 0x3FFFFFFF 521MB)
 * Đây là vùng nằm phía trước vùng Code Memory.
 * Chủ yếu đc sử dụng để kết nối với SRAM (SRAM là 1 data memory, nơi lưu dữ liệu tạm thời của một cái gì đó)
 * 1MB đầu tiền của vùng SRAM là bit addressable(hoặc bit banding)
 *
 * +Tìm hiểu về phân vùng Peripherals : (0x40000000 -> 0x5FFFFFFF 521MB)
 * Nơi đặt tất cả địa chỉ của thanh ghi các thiết bị ngoại vi: RTC, ADC,TIMER,...
 * Đây là vùng sử dụng để giao tiếp giữa các thiết bị ngoại vi và vi điều khiển.
 * Các địa chỉ của thanh ghi nội bộ(Core register) KHÔNG nằm trong vùng này
 * 1MB đầu tiền của vùng Peripherals là bit addressable(hoặc bit banding)
 * Đây là khu vực eXecute Never: ko thể code(lập trình) từ khu vực, nếu cố gắng sẽ gây ra lỗi ngoại lệ, mục đích là để ngăn chặn
 * các cuộc tấn công mã độc thông qua các thiết bị ngoại vi.
 *
 * +Tìm hiểu về phân vùng External RAM : (0x60000000 -> 0x9FFFFFFF 1GB)
 * Khi bạn cần thêm RAM cho dự án của mình, Ví dụ liên quan đến đồ họa, liên quan đến tạo thêm bộ đệm(buffer), thêm nội dung
 * video, thêm ân thanh,...lúc này bạn có thể sử dụng(kết nối) với External RAM.
 * Sử dụng External RAM thông qua các thanh ghi địa chỉ của nó.
 *
 * +Tìm hiểu về phân vùng External Device : (0xA0000000 -> 0xDFFFFFFF 1GB)
 * Đây là vùng dành cho các thiết bị bên ngoài hoặc bộ nhớ chia sẻ.
 * Đây là khu vực eXecute Never.
 *
 * + Tìm hiểu về phân vùng Private Peripheral Bus : (0xE0000000 -> 0xE00FFFFF  1.0MB)
 * Đây là khu vực xử lý các thanh ghi ngoại vi như thanh ghi NVIC Systick Timer hoặc các thanh ghi liên quan đến điều khiển khác.
 * Đây là khu vực eXecute Never.
 */



/**
 * Memory map of processor
.Memory map explains mapping of different peripheral registers and memories in the processor addressable memory location range.
.The processor, addressable memory location range, depends upon the size of the address bus.
.The mapping of differrent regions in the addressable memory location range is called 'memory map'
 */
