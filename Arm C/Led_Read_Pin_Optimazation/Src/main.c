/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
/*
Address of clock control register(AHB1ENR)
0x40023800+0x30 => 	0x40023830

Address of the GPIO mode register (used to control mode)
0x40020C00 + 0x00 	=>	0x40020C00

Address of the GPIO output data register (used to write)
0x40020C00 + 0x14  =>  0x40020C14
*/

/*
 * Thiết lập phần cứng:
 * Nối chân PA0 với VDD: input = 1;
 * Nối chân PA0 với VDD: input = 0;
 * */

/*
 * Optimazation: Tối ưu hóa trong mã code
 * Tối ưu hóa là một loạt các hành động được thực hiện bởi trình biên dịch trên mã chương trình của bạn
 * Mục đích:
 * + Giảm một số hướng dẫn(instruction) trên code của bạn
 * + Giảm thời gian truy cập bộ nhớ
 * + Giảm mức tiêu thụ điện
 * Nếu thời gian truy cập bộ nhớ và một số hướng dẫn giảm thì bạn có thể nhận được mức tiêu thụ năng lượng
 * tốt hơn.
 *
 * Theo mặc định, trình biên dịch không gọi bất kỳ tối ưu hóa nào trong chương trình của bạn.
 * Bạn có thể bật tối ưu hóa bằng cờ biên dịch
 *
 * Cờ biên dịch(compiler flags) gồm các mức tối ưu hóa: O0,O1,O2,O3
 * +O0: đây là mức tối ưu hóa bằng 0.
 * Khi bạn có giới hạn về code và giới hạn bộ nhớ RAM thì hầu như không có tối ưu hóa nào được thực hiện
 * trên mã của bạn. Trình biên dịch cũng sẽ không gọi bất kỳ thuật toán tối ưu hóa nào nên tốc độ biên
 * dịch của nó cũng rất nhanh.
 * Đây là mức gỡ lỗi thân thiện, có nghĩa là khi bạn đang cố gắng gỡ lỗi mã của mình,sẽ có một mapping
 * (bản đồ) giữa hướng dẫn(instruction) và source nguồn đc viết. Các mapping sẽ giúp lập trình viên kiểm
 * tra được các lệnh,các lỗi xảy ra,.. trong quá trình biên dịch.Nó được sử dụng trong quá trình phát triển.
 * Lưu ý: Các mã đã hoạt động với tối ưu hóa O0 thì có thể KHÔNG hoạt động với các tối ưu O1,O2,O3. Vì vậy,
 * code cần được xác minh lại.
 *
 * +O1: Đây là mức tối ưu hóa vừa phải
 * Dùng để giảm thời gian truy cập bộ nhớ và không gian code.
 *
 * +O2: Đây là mức tối ưu hóa hoàn toàn
 * Dẫn đến thời gian biên dịch chương trình chậm. Vì lúc này trình biên dịch sẽ phải gọi các thuật toán tối
 * ưu hóa khác nhau để xử lý code của bạn
 * Đây không phải là mức dùng để gỡ lỗi(cả mức tối ưu hóa O3) vì nó sẽ không tạo ra mapping giữa instruction
 * và source code
 *
 * +O3: Đây là mức tối ưu hóa hoàn toàn O2+ (thêm một số mức tối ưu hóa cao hơn O2)
 * Dẫn đến nó cũng gay ra tốc độ biên dịch chậm nhất
 * Có thể gây ra lỗi chương trình nếu không được tối ưu hóa đúng cách.
 * Vì có thể gây ra lỗi nên nếu bạn bật mức tối ưu hóa này thì ứng dụng của bạn có thể không hoạt động
 * đúng. vì vậy cso thể bạn sẽ phải viết lại code chương trình. (Cần chú ý khi sử dụng mức O3)
 *
 * =>Tùy vào chương trình mà bạn nên chọn mức tối ưu hóa phù hợp
 * =>Sau khi chọn mức tối ưu hóa bắt buộc phải run lại chương trình để kiểm tra xem chương trình
 * ở mức tối ưu hóa đó có hoạt động hay không hoặc hoạt động có ổn định hay không.
 *
 * Cách chọn tối ưu hóa: Chuột phải -> properties -> C/C++ Build -> Settings -> Tool Settings
 * -> MCU GCC Compiler -> Optimization -> Chọn mức tối ưu hóa (Optimization level)
 * */

/*
 * Sử dụng từ khóa 'volatile' kết hợp với việc tối ưu hóa:
 * Trường hợp 1: Ban đầu khi biến *pPortAInReg khi chưa đc thêm từ khóa 'volatile' thì khi run thì trình
 * biên dịch sẽ bỏ qua lệnh đọc giá trị chân biến này(do nhận thấy đây ko phải là biến đc sử dụng)
 * và chỉ tiếp tục các lệnh if_else, nhưng biến *pPortAInReg sẽ đọc giá trị thay đổi của người dùng
 * để thực hiện nhiệm vụ sáng tắt theo yêu cầu, vì vậy tối ưu hóa lúc này sẽ ko chạy chính xác nhiệm vụ.
 *
 * Trường hợp 2: Thêm từ khóa 'volatile' vào trong khai báo biến
 * Kết quả ta thấy dù có thiết lập mức tối ưu hóa nào thì trình biên dịch sẽ thực hiện các lệnh
 * của các biến có thiết lập từ khóa 'volatile'.
 * Dòng text từ file .elf được tạo giảm từ 820->764
 * */

#include <stdint.h>

int main(void)
{
	//Trường hợp 1: Khai báo biến không sử dụng volatile
//    uint32_t *pClkCtrlReg = (uint32_t*)0x40023830; 	//Khai báo cấu hình địa chỉ xung clock
//
//    uint32_t *pPortDmodeReg = (uint32_t*)0x40020C00; //Khai báo cấu hình các địa chỉ cần thiết thiết lập port D
//    uint32_t *pPortDoutReg = (uint32_t*)0x40020C14;  //thiết lập địa chỉ làm output port D
//
//    uint32_t *pPortAmodeReg = (uint32_t*)0x40020000 ;  //GPIOA ///Khai báo cấu hình các địa chỉ cần thiết thiết lập port A

//    uint32_t *pPortAInReg =   (uint32_t*)0x40020010 ;  //GPIOA 0x40020000 + 0x01(address offset)
    //Đây là địa chỉ cấu hình để thiết lập đọc trạng thái của các pin port A

    //Trường hợp 2: tối ưu hóa kết hợp từ khóa 'volatile'
    uint32_t volatile *pClkCtrlReg = (uint32_t*)0x40023830; 	//Khai báo cấu hình địa chỉ xung clock

    uint32_t volatile *pPortDmodeReg = (uint32_t*)0x40020C00; //Khai báo cấu hình các địa chỉ cần thiết thiết lập port D
    uint32_t volatile *pPortDoutReg = (uint32_t*)0x40020C14;  //thiết lập địa chỉ làm output port D

    uint32_t volatile *pPortAmodeReg = (uint32_t*)0x40020000 ;  //GPIOA ///Khai báo cấu hình các địa chỉ cần thiết thiết lập port A

    uint32_t volatile *pPortAInReg =   (uint32_t*)0x40020010 ;

    //II. Su dung phuong phap dich bit
    //1.Bat dong ho cho thiet bi ngoai vi GPIOD, GPOIA o AHB1ENR
    *pClkCtrlReg |= ( 1 << 3 );			//GPIOD
    *pClkCtrlReg |= ( 1 << 0 );			//GPIOA

    //2.Cau hinh che do chan IO lam dau ra
    //a.clear the bit 24 and 25  PD12 là output
    *pPortDmodeReg &= ~ ( 3 << 24 ) ;    //3 = 11 : set bit 24,25 ve 0
    //b. set bit 24 len 1
    *pPortDmodeReg |= ( 1 << 24 );		//dich bit cho chan 24 len 1

    //thiet lap PA0 input (GPIO MODE REGISTER)
    *pPortAmodeReg &= ~( 3 << 0);		//3 = 11 : set bit 0,11 về 00
    // vừa là trạng thái reset vừa là trạng thái input (xem datasheet phần GPIO port mode register)

    //Gợi ý: &(AND) : thường sẽ thay đổi tất cả trạng thái của tất cả bit
    //		 |(NOT) : chỉ thay đổi bit mà ta muốn thay đổi

    while(1)
    {
    	//doc trang thai chan PA0
    	uint8_t pinStatus = (uint8_t)(*pPortAInReg & 0x1);  //set tat ca bit khac la 0 tru bit 0 là 1
    	//lúc này việc set bit 0 lên 1 có nghĩa là yêu cầu chương trình đọc giá trị đang có tại chân PA01
    	//pPortAInReg : chứa địa chỉ mà ta đã cấu hình ban đầu là cho phép đọc chân pin của port A

		if(pinStatus)
		{
			*pPortDoutReg |= ( 1 << 12 );
		}
		else{
			*pPortDoutReg &= ~ ( 1 << 12 );
		}

    //3. Set chan 12 GPIO len 1 cho LED sang
    //*pPortDoutReg |= ( 1 << 12 );		//dich bit cho chan 12 len 1
    }
}


