/*
 * tutorial.c
 *
 *  Created on: Oct 6, 2023
 *      Author: Minh
 */

/**
*==Giới thiệu về phần cứng của khóa học(S2)
+ Trong khóa học sẽ sử dụng board STM32F407VGT6 làm board Target
+ STM32F407VGT6: sử dụng vi điều khiển 32bit ARM Cortex M4 with FPU, Tần số thạch anh 8MHz,
IO pins sẽ hỗ trợ các kết nối với LCS,keypads,leds... đc gọi là external interface headers,
trên bo mạch đã đc tích hợp sẵn debugger nên có thể load code trực tiếp lên mạch mà ko cần
một bo mạch chuyển đổi nào khác.
+ Thông tin board mạch: https://www.st.com/en/evaluation-tools/stm32f4discovery.html
+ IDE: Sử dụng STM32CubeIDE
*/

/**
*==Tóm tắt nội dung khóa học(S1)
+ Khóa học đc chia làm 3 nội dung chính:
 ^GĐ1: Tìm hiểu về cách hoạt động của hệ thống ẩn phía sau, Vd: khi kết nối vs các ngoại vi như GPIO, SPI,I2C,USART.
  ~Bạn sẽ tìm hiểu cơ bản hoạt động của từng ngoại vi(peripheral). Các ngoại vi này sẽ điều khiển(control) và
  cấu hình(configuration) các thanh ghi(register).
  ~Tiến hành việc lập trình để cấu hình đơn giản các Periperal Clocks, IRQs, Interrupt,... để chuẩn bị học GĐ2

 ^GĐ2: Tìm hiểu đầy đủ các thông tin về hoạt động của các peripheral đã đc học ở GĐ1
  ~Cấu hình, code điều khiển(driver) cho các peripheral -> Periperal Driver Development
  ~Viết các "Macros" để mô tả các thiết bị ngoại vi
  ~Triển khai(Implemeting) API trình điều khiển ngoại vi.(Giơi thiệu các bước để thực hiện việc triển khai API trong
  một source code)

 ^GĐ3: Testing and Debugging
  1.Ta sẽ viết một ứng dụng đơn giản cho Periperal mà sử dụng Driver Exposed APIs
  2.Tiến hành Testing và Debugging sử dụng công cụ Logic Analyzer Hardware và Software
  Logic Analyzer: là công cụ phân tích xung điện

+ Source code khóa học: https://github.com/niekiran/MasteringMCU.git

*/

/**
*==Các linh kiện điện tử sử dụng trong khóa học(S3)
Đc cập nhật trong file .pdf
*/

/**
*==Hướng dẫn Tải và cài đặt IDE trên các môi trường OS(S4)
*/
