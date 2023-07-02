/*
 * main1.c
 *
 *  Created on: Jun 25, 2023
 *      Author: Minh
 */
/**
 * Function call and AAPCS Standard - Tiêu chuẩn AAPCS và việc gọi hàm (S10-V45)
 * ----------------------------------------------------------------------------
 * Ví dụ:
 * //hàm 1
 * void fun_x(void){
 * 		int ret;
 * 		ret = fun_y(1,2,4,5);
 * }
 *
 * //hàm 2
 * void fun_y(int a, int b, int c, int d){
 * 	return(a+b+c+d);
 * }
 *
 * Ta có ở hàm x ta sẽ gọi hàm y, x đc gọi là hàm Caller và y đc gọi là hàm callee
 *
 * Tiêu chuẩn AAPCS mô tả tiêu chuẩn gọi thủ tục đc sử dụng bởi binary interface cho kiến trúc ARM.
 * + AAPCS xác định cách các chương trình con có thể đc viết tách biệt, biên dịch riêng để làm việc cùng nhau.
 * + Nó mô tả một hợp đồng giữa một quy trình gọi và một quy trình đc gọi để xác định. Bổn phận trên caller là tạo ra một
 * trạng thái chương trình mà trong đó quy trình đc gọi(callee) có thể bắt đầu thực thi.
 * + Bổn phận của hàm đc gọi(callee) là duy trì trạng thái của caller trong suốt quá trình gọi hàm.
 * + Quyền của hàm đc gọi callee là có thể thay đổi trạng thái chương trình của hàm gọi caller.
 * + Tiêu chuẩn này hoạt động như là một quy tắc hoặc đặc điểm kỹ thuật khi nhiều người đang cùng phát triển quy trình code và
 * tất cả các quy trình đó cuối cùng sẽ đi đến một dự án duy nhất, sau đó tất cả phải tuân theo một đặc điểm kỹ thuật này.
 *
 * Lưu ý: + Khi trình biên dịch C biên dịch code cho kiến trúc ARM, nó phải tuân theo đặc điểm kỹ thuật AAPCS để tạo code liên
 * quan đến caller và callee.
 *
 * + Theo tiêu chuẩn này, gọi hàm 'C' có thể sửa đổi các thanh ghi R0,R1,R2,R3,R14,PSR và nó cũng không có trách nhiệm lưu các value
 * thanh ghi trên trước khi có bất kỳ sửa đổi nào.
 * Vd: ở hàm callee ta có code thay đổi các thanh ghi R0,R1,R2. Và trong caller ta có một số data quan trọng như R0,R1,R2.
 * Ta có hàm y đang sửa đổi thanh ghi và sau đó trả về cho hàm x caller để làm các công việc khác. Vì vậy ta nói rằng hàm y
 * chỉ có chức năng sửa đổi nội dung còn chức năng lưu nội dung thì do caller đảm nhận.
 * Trong caller, việc gọi hàm là PUSH và việc nhận các thay đổi đăng ký nó vào các ngăn xếp(nhận return) đc gọi là POP.
 *
 * + Nếu một hàm muốn sử dụng các thanh ghi R4 đến R11, thì nó phải có trách nhiệm lưu nội dung trước đó của các thanh ghi này
 * trước khi có sự sửa đổi thanh ghi đó và lấy lại nó trước khi thoát khỏi chức năng của hàm đc gọi.
 * Vd: Hàm callee muốn sửa đổi thanh ghi R7, thì đầu tiên nó phải PUSH R7 (lưu value R7 ở Stack) sau đó tiến hành sửa đổi R7
 * để làm một việc gì đó gián tiếp, và trước khi thoát hỏi callee thì phải trả lại giá tri ban đầu trước thay đổi của R7(POP)
 * + Lập trình viên ko cần lo lắng việc này khi code vì trình biên dịch sẽ tự động tuân thủ quy chuẩn này khi biên dịch.
 */

/**
 * Procedure Call Standard for the Arm Architecture - Thủ tục gọi tiêu chuẩn cho kiến trúc ARM
 * -------------------------------------------------------------------------------------------
 * + Các thanh ghi R0,R1,R2,R3,R14 đc gọi là các "caller save register", có trách nhiệm lưu các value thanh ghi này trên stack trước khi
 * gọi hàm called nếu những value đó cần thiết sau khi gọi hàm, và sẽ lấy lại các value đó sau khi hàm called return. Ngoài ra, các giá
 * trị thanh ghi không bắt buộc phải có thì không cần phải đc lưu trước.
 * + Các thanh ghi từ R4->R11 đc gọi là các thanh ghi "calee saved register".
 * + Theo tiêu chuẩn này, hàm caller sử dụng các thanh ghi R0,R1,R2,R3 để gửi các đối số đầu vào (các argrument) đến hàm callee.
 * + Hàm callee sử dụng các thanh ghi R0 và R1 để gửi kết quả return trở lại hàm caller.
 *
 * Ví dụ: Trong xử l hàm fun_x thì ta có dòng code gọi một hàm fun_y có 4 đối số đầu vào. Khi ta cho đối số cần thiết vào fun_y ở caller
 * thì nhiệm vụ của hàm fun_x là lưu các value này trên các thanh ghi R0,R1,R2,R3 vào sau đó khi gọi hàm fun?_y callee thì nhiệm vụ của hàm
 * này là chuyển chúng vào theo thứ tự làm các tham số hàm. Các value này sẽ trở thành các biến cục bộ của hàm y. Nói cách khác các thanh ghi
 * này đc sử dụng dể chuyển các số từ hàm này qua hàm khác.
 * Sau khi hoàn thành và return kết quả thì sẽ sử dụng thanh ghi R0(32bit) để lưu kết quả trả về, và nếu kết quả là 64bit thì nó sẽ sử dụng
 * hai thanh ghi R0(32bit cao),R1(32bit thấp) để lưu kết quả trả về đó.
 * Câu hỏi: vậy khi có nhiều hơn 4 argrument-parameter thì lấy đâu ra thanh ghi để lưu các value thứ 5,6,..?
 * Trong trường hợp này, các giá trị 5,6,7,... sẽ đc chuyển từ register qua bộ nhớ "Stack". Có nghĩa là ta có R0,R1,R2,R3,Stack(số ô nhớ cần thiết)
 * Nguyên nhân vì theo tiêu chuẩn AAPCS thì các argrument sẽ chỉ đc lưu trên R0,R1,R2,R3 (các đối số tiếp theo lưu trên Stack), không đc lưu các
 * argrument trên R4->R12, va sử dụng thanh ghi R0(hoặc R0 và R1) cho kết quả trả về.
 * Bạn có thể đọc thêm phần này trong tài liệu: Procedure Call Standard for the Arm Architecture (phần 6)
 * Xem thêm cách giải thích hoạt động của chương trình bằng mã assembly (đoạn 10p00s)
 */
