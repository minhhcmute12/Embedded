/*
 * tutorial.c
 *
 *  Created on: Oct 6, 2023
 *      Author: Minh
 */
/**
*==Embedded Code Debugging Tips(Mẹo) and Tricks(Thủ thuật)(S6)
+ Các lựa chọn công cụ hỗ trợ khi Embedded Code Debugging
 ^Serial Wire Viewer and Data Tracing (printf style debugging)
 ^Single stepping, stepping over and stepping out - thực hiện các lệnh theo từng dòng code
 ^Breakpoints(Inserting, deleting and skipping breakpoints) - dừng tại điểm mong muốn khi code đang run
 ^Disassembly - Chuyển đổi từ ngôn ngữ cấp cao sang Assembly(ngôn ngữ máy)
 ^Call Stack
 ^Expression and variable window - Cho phép quan sát một biến và viết các biểu thức để tìm ra một value hoặc một address
 hoặc một function nào đó
 ^Memory browser - Kiểm tra các nội dung data mà bộ nhớ(FLASH hoặc SRAM) đang lưu trữ
 ^Data watch points -

+ SWV(Serial Wire Viewer) and ITM based Printf Style debugging:
 ^Nguồn theo dõi(trace source) ITM của bộ xử lý ARM Cortex M3/M4 hỗ trợ gỡ lỗi kiểu printf để theo dõi các sự kiện ứng
 dụng và hệ điều hành, đồng thời phát ra thông tin chẩn đoán cho ứng dụng của bạn. Thông tin này có thể được ghi lại trên
 máy chủ IDE để phân tích hành vi ứng dụng ngay cả khi ứng dụng đang chạy
 ^Giới thiệu chi phí không gian-thời gian rất tối thiểu

+ Breakpoints(Điểm dừng)
 ^Breakpoints là một lệnh(hay cách để nói) với bộ vi xử lý để break(bẻ gãy) hoặc dừng thực thi mã tại các địa chỉ lệnh
 đc xác định trước đó bởi lập trình viên
 ^Breakpoints đc thực hiện thông qua các bộ so sánh địa chỉ phần cứng có bên trong Processor Debug Unit
 ^Các bộ vi xử lý cơ bản(underlying Processor) phải hỗ trợ thiết lập "Breakpoints"
 ^Bằng cách sử dụng IDE, ta có thể thiết lập(set), xóa(delete),bỏ qua(skip) các điểm Breakpoints
 ^Breakpoints cũng đc gọi là các điểm dừng phần cứng(hardware breakpoints) vì những điểm này đc nhận ra thông qua bởi
 khối hardware unit của bộ xử lý
 ^Các điểm dừng "software breakpoints" đc thực hiện thông qua các lệnh "breakpoints instruction"
 ^Hướng dẫn các phím tắt sử dụng "Breakpoints" trong Debugging(V20)

+ Expression and variable window(V21) - Cửa sổ biểu thức và biến
 ^Khi bạn trỏ con trỏ vào một biến bất kỳ thì cửa sổ Expression sẽ hiển ra để hiển thị thông tin của biến đó
 ^Thông tin hiển thị thường là kiểu của biến(Type), và value của biến(Có quy đổi ra các hệ số)
 ^Cách 2: mở tab [Windown]->[Show View]->[Variable] : hiển thị thông tin của tất cả biến trong một hàm hoặc
 một file tùy theo bạn chọn(Việc chọn sẽ dựa vào cột Thread#1 bên bảng Debug khi chọn Build)
 ~Tab [Windown]->[Show View]->[Expression]: cho phép bạn thực hiện phép tính với một biến bất kỳ(đc kéo thả vào),
 giúp bạn tạo ra các biểu thức ví dụ mong muốn vs biến mà ko làm ảnh hưởng đến chu trình đang chạy

+ Memory Browser - Trình duyệt(cửa số) tab Memory(V22)
 ^Đc sử dụng để khám phá về "various memory" của vi điều khiển, hoặc để đọc nội dung của "various memory" của vi
 điều khiển như SRAM,FLASH,EEFROM,...
 ^Vd: là xem việc lưu các thành phần của một mảng
 ^Tab [Windown]->[Show View]->[Memory Browser]+[Expression]+[Breakpoints]: kết hợp 3 bảng để lấy đểm dừng, sau đó địa chỉ biến
 cần quan sát sau đó nhập địa chỉ vào ô tìm kiếm rồi Enter(chuột phải->radix->Decima: để quan sat value thập phân; column: để
 quan sát cột value; cell text: biểu diễn value dưới dạng bao nhiêu byte(1,2,4,8))

+ Call Stack(V23)
 ^Tính năng này dùng để theo dõi các "stack frame" của các biến gọi "Function Call-gọi hàm" trong chương trình
 ^Khi bạn đang sử dụng một dự án lớn hơn thường bao gồm nhiều lệnh gọi hàm khác nhau và ở một số nơi hoặc trong một số hàm
 có sự cố xảy ra thì nó dẫn đến ngoại lệ kích hoạt và code của ban ko hoạt động -> bạn sẽ sử dụng tính năng này giúp cho
 việc xác định dòng code nào hoặc instruction(lệnh) nào đang có vấn đề để gỡ lỗi
 ^Vd về cách sử dụng ở video 1p30s
 ^Nếu có lỗi thì tab [Debug] hiển thị các hàm đc gọi mà dẫn đến lỗi, ta có thể click từ dưới lên trên(từ hàm lớn -> hàm nhỏ)
 để xác định line đang bị lỗi
 ^[Fault Analyzer]: tab này cho biết lỗi exception nào đã đc kích hoạt(trigger)

+ Data Watch-Points(V24)
 ^Khi code một của dự án lớn thì có nhiều nhà phát triển cùng code các chương trình và sẽ liên quan đến nhau, vì vậy sẽ có
 trường hợp lập trình viên khác thay đổi value biến của bạn và tất nhiên bạn ko đồng ý, hoặc bất cứ lý do chồng chéo nào đó
 ^Vì vậy sử dụng Data Watch-Points để giúp bạn theo dõi vấn đề này, bạn có thể xác định ai thực sự đã thay đổi value biến
 hoặc value con trỏ của bạn
 ^Cách sử dụng: [Breakpoints]-> biểu tượng ▽ -> chọn "add Watch-Points" -> nhập tên biến muốn theo dõi -> tích vào các ô
 "read-nếu mong muốn", mặc định sẽ là "write" -> OK
 Lúc này, tất cả các lệnh gán value biến hoặc các phép tính làm thay đổi value tên biến mà bạn đã nhập, thì khi chạy đến dòng
 đó thì tự động chương trình sẽ dừng lại
 ^Cách khác: ta cũng có thể sử dụng tab [Expression] để theo dõi địa chỉ biến, từ đó theo dõi đc sự thay đổi value của biến

+ SFR Windown(dùng cho Periperal Register)(V25)
 ^SFR là các thanh ghi ánh xạ bộ nhớ(memory mappped register) của các thiết bị ngoại vi của vi điều khiển.
 ^Bên trong microcontroller có rất nhiều thanh ghi ngoại vi như là SPI,ADC,DAC,... để kiểm soát các chức năng khác nhau của
 vi điều khiển, và tất cả các thanh ghi này sẽ đc liệt kê cụ thể bên trong tab[SFRs]
 ^Lưu ý: cần phân biệt tab[SFRs] vs tab[Register], tab[Register] là liệt kê các Internal Register(Rx(1->13), Special Register),
 còn tab[SFRs] là liệt kê các thanh ghi ngoại vi(Periperal Register,vd: TIM,ADC,GPIO,...)
 ^SFR dùng để kiểm soát trạng thái(status) thanh ghi, điều khiển(control) thanh ghi, và hiển thị trạng thái thanh ghi theo một
 hàng ô vuông 32bit(MSB<--->LSB). Ngoài ra còn hiển thị một số thông tin như địa chỉ, value, size,.. của thanh ghi khi click chọn
 ^Hàng ô vuông 32bit(MSB<--->LSB), bạn có thể thay đổi trục tiếp trạng thái bit trên ô vuông từ đó điều khiển trạng thái ngõ ra

+ Một số hướng dẫn khác trên IDE(V26)
 ^Hướng dẫn một số phím tắt trên thanh công cụ của IDE
 ^Tổ hợp [left Ctrl + phím Space] -> tạo ra ko gian gợi ý code, hàm tự động
 ^Tổ hợp [Ctrl+O] -> Hiển thị thông tin các thư viện,hàm,biến nào đang có trong một file source
*/

