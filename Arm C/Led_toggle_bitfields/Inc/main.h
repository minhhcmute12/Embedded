/*
 * main.h
 *
 *  Created on: Jan 17, 2021
 *      Author: Minh
 *
 *  Đây là file thư viện cấu hình bit của các chân thanh ghi đc sử dụng trong chương trình
 */

//Thiết lập tiền xử lý
#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>

//Thiết lập chân_bit trên thanh ghi RCC_AHB1ENR dựa vào hình 7.3.10 của datasheet
typedef struct
{
	uint32_t gpioa_en   	: 1;			//:1	pin chiếm 1bit
	uint32_t gpiob_en   	: 1;
	uint32_t gpioc_en   	: 1;
	uint32_t gpiod_en   	: 1;
	uint32_t gpioe_en   	: 1;
	uint32_t gpiof_en   	: 1;
	uint32_t gpiog_en   	: 1;
	uint32_t gpioh_en   	: 1;
	uint32_t gpioi_en   	: 1;
	uint32_t reserved1   	: 3;			//:3	pin chiếm 3bit
	uint32_t crc_en     	: 1;
	uint32_t reserved2   	: 3;
	uint32_t reserved3   	: 2;
	uint32_t bkpsram_en 	: 1;
	uint32_t reserved4     	: 1;
	uint32_t ccmdataram_en 	: 1;
	uint32_t dma1_en		: 1;
	uint32_t dma2_en 		: 1;
	uint32_t reserved5 		: 2;
	uint32_t ethmac_en 		: 1;
	uint32_t ethmactxe_en	: 1;
	uint32_t ethmacrxe_en	: 1;
	uint32_t ethmacptp_en	: 1;
	uint32_t otghs_en 		: 1;
	uint32_t otghsulpi_en 	: 1;
	uint32_t reserved6 		: 1;

}RCC_AHB1ENR_t;

//Thiết lập chân_bit trên thanh ghi GPIOx_MODE_t dựa vào phần 8.4.1 của datasheet
typedef struct
{
	uint32_t pin_0			:2;
	uint32_t pin_1			:2;
	uint32_t pin_2			:2;
	uint32_t pin_3			:2;
	uint32_t pin_4			:2;
	uint32_t pin_5			:2;
	uint32_t pin_6			:2;
	uint32_t pin_7			:2;
	uint32_t pin_8			:2;
	uint32_t pin_9			:2;
	uint32_t pin_10			:2;
	uint32_t pin_11			:2;
	uint32_t pin_12			:2;
	uint32_t pin_13			:2;
	uint32_t pin_14			:2;
	uint32_t pin_15			:2;

}GPIOx_MODE_t;

//Thiết lập chân_bit trên thanh ghi GPIOx_ODR_t dựa vào hình 8.4.6 của datasheet
typedef struct
{
	uint32_t pin_0			:1;
	uint32_t pin_1			:1;
	uint32_t pin_2			:1;
	uint32_t pin_3			:1;
	uint32_t pin_4			:1;
	uint32_t pin_5			:1;
	uint32_t pin_6			:1;
	uint32_t pin_7			:1;
	uint32_t pin_8			:1;
	uint32_t pin_9			:1;
	uint32_t pin_10			:1;
	uint32_t pin_11			:1;
	uint32_t pin_12			:1;
	uint32_t pin_13			:1;
	uint32_t pin_14			:1;
	uint32_t pin_15			:1;
	uint32_t reserved 		:16;

}GPIOx_ODR_t;


#endif /* MAIN_H_ */
