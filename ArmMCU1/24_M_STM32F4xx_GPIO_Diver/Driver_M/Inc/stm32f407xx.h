/*
 * stm32f407xx.h
 *
 *  Created on: Nov 10, 2024
 *      Author: Minh
 *  File header dùng để Macro các thông tin quan trọng của MCUs
 */
/******************************************Driver Layer*************************************************/
#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include "stdint.h"		//Khai báo thư viện có chứa các kiểu biến như uint32_t

#define __vo volatile	//Định nghĩa tư khóa volatile

/**
 * 1_ Base address of Flash and SRAM memories
 * FLASH : bộ nhớ lưu trữ fireware của vi điều khiển, không mất dữ liệu khi mất điện
 * SRAM : một loại bộ nhớ đặc biệt của RAM, cho phép ghi dữ liệu miễn là còn điện cung cấp
 * 'U' : thông báo cho trình biên dịch đây là kiểu uint32_t
 */
#define FLASH_BASEADDR 		0x08000000U		//RM407->2.4->Table3 Memory mapping
#define SRAM1_BASEADDR 		0x20000000U		//RM407->2.3.1 hoặc RM407->2.4->Table3 Memory mapping
#define SRAM2_BASEADDR 		0x2001C000U		//RM407->2.4->Table3 Memory mapping
#define SRAM 				SRAM1_BASEADDR
#define ROM_BASEADDR 		0x1FFF0000U  	//System memory address: RM407->2.4->Table3 Memory mapping

/**
 * 2_ AHBx and APBx Bus Peripheral ("RM407->2.4->Table3 Memory mapping" and "Datasheet_stm32f407->4->Table10 (DS407)")
 * Xác định địa chỉ cở sở các thanh ghi bus từ đó sẽ dùng để xác định địa chỉ thanh ghi các ngoại vi
 * Qua kiểm tra các hình thì xác định địa chỉ bus bắt đầu Periph_Base là thanh ghi APB1 bus
 * Từ địa chỉ cở sở dùng bảng đã ghi sẵn địa chỉ để xác đinh các bus tiếp theo
 */
#define PERIPH_BASE			0x40000000U
#define APB1PERIPH_BASE		PERIPH_BASE
#define APB2PERIPH_BASE		0x40010000U 	//DSf407->4->Table10
#define AHB1PERIPH_BASE		0x40020000U		//DSf407->4->Table10
#define AHB2PERIPH_BASE		0x50050000U		//DSf407->4->Table10

/**
 * 3_ Base address of peripherals which are hanging on AHB1 Bus
 * TODO: Complete for all another peripherals (use Memory Map) -> using RM407->2.3->Table1
 * Có thể dùng địa chỉ trực tiếp tra từ bảng hoặc có thể dùng macro và offset để xác định địa chỉ
 * Lưu ý nếu dùng địa chỉ trực tiếp thì sau mỗi địa chỉ ta cần có ký tự 'U'
 */
#define GPIOA_BASEADDR		(AHB1PERIPH_BASE)			//Địa chỉ bắt đầu của GPIOA cũng là địa chỉ của AHB1PERIPH_BASE
#define GPIOB_BASEADDR		(AHB1PERIPH_BASE + 0x0400)	//0x0400 là phần offset: 0x40020400(GPIOB)-0x40020000(GPIOA) = 0x0400
#define GPIOC_BASEADDR		(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR		(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR		(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR		(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR		(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR		(AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR		(AHB1PERIPH_BASE + 0x2000)
#define GPIOJ_BASEADDR		(AHB1PERIPH_BASE + 0x2400)
#define GPIOK_BASEADDR		(AHB1PERIPH_BASE + 0x2800)
#define RCC_BASEADDR		(AHB1PERIPH_BASE + 0x3800)	//Địa chỉ cơ sở của Reset Clock Control

/**
 * 4_ Base address of peripherals which are hanging on APB1 Bus
 * TODO: Complete for all another peripherals (use Memory Map) -> using RM407->2.3->Table1
 */
#define I2C1_BASEADDR 		(APB1PERIPH_BASE + 0x5400)
#define I2C2_BASEADDR 		(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR 		(APB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR		(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR		(APB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR		(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR		(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR		(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR		(APB1PERIPH_BASE + 0x5000)

/**
 * 5_Base address of peripherals which are hanging on APB2 Bus
 * TODO: Complete for all another peripherals(use Memory Map)
 */
#define USART1_BASEADDR		(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR		(APB2PERIPH_BASE + 0x1400)
#define SPI1_BASEADDR		(APB2PERIPH_BASE + 0x3000)
#define SYSCFG_BASEADDR		(APB2PERIPH_BASE + 0x3800)
#define EXTI_BASEADDR 		(APB2PERIPH_BASE + 0x3C00)				//Interrupt


/**********************6_Peripheral register definition structure********************************/
/**
 * Note: Register of a peripheral are specific(riêng biệt) to MCU
 * Tiến hành định nghĩa cấu trúc các thanh ghi bên trong các ngoại vi(peripheral) đc cung cấp
 * e.g: Number of Registers of SPI peripheral of STM32F4x family of MCUs may be different (more or less)
 * Compared to number of registers of SPI peripheral of STM32Lx or STM32F0x family of MCUs
 * Please check your Device RMx
 * Ví dụ: Số lượng thanh ghi của thiết bị ngoại vi SPI của họ MCU STM32F4x có thể khác (ít hoặc nhiều) so với số lượng
 * thanh ghi của thiết bị ngoại vi SPI của họ MCU STM32Lx hoặc STM32F0x
 * Vui lòng kiểm tra RMx thiết bị của bạn - Sử dụng Datasheet của mỗi chip để xác định
 */
//Cấu trúc Struct chung cho một GPIOx register
typedef struct
{
	__vo uint32_t MODER;			//GPIO port mode register	 			- Address offset: 0x00
	__vo uint32_t OTYPER;			//GPIO port output type register 		- Address offset: 0x04
	__vo uint32_t OSPEEDR;			//GPIO port output speed register 	    - Address offset: 0x08
	__vo uint32_t PUPDR;			//GPIO port pull-up/pull-down register  - Address offset: 0x0C
	__vo uint32_t IDR;				//GPIO port input data register         - Address offset: 0x10
	__vo uint32_t ODR;				//GPIO port output data register        - Address offset: 0x14
	__vo uint32_t BSRR;				//BSRR GPIO port bit set/reset register - Address offset: 0x18
	__vo uint32_t LCKR;				//GPIO port configuration lock register - Address offset: 0x1C
	__vo uint32_t AFR[2];													//Address offset: 0x20-0x24
	//AFR[0]: GPIO alternate function low register - AFR[1]: GPIO alternate function high register
}GPIO_RegDef_t;

/**
 * 7_Peripheral definitions (Peripheral base address type_casted to xxx_RegDef_t)
 * Tiến hành thiết lập struct của GPIOx vào địa chỉ cơ sở từng GPIOx
 * Việc ép cấu trúc giúp ta tự động có thể thiết lập địa chỉ thanh ghi chi tiết bên trong từng GPIOx
 */
#define GPIOA 	((GPIO_RegDef_t*)GPIOA_BASEADDR)		//GPIOA là kiểu con trỏ GPIO_RegDef_t ở địa chỉ GPIOA_BASEADDR
#define GPIOB 	((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 	((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 	((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE 	((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF 	((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG 	((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH 	((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI 	((GPIO_RegDef_t*)GPIOI_BASEADDR)
#define GPIOJ 	((GPIO_RegDef_t*)GPIOJ_BASEADDR)
#define GPIOK 	((GPIO_RegDef_t*)GPIOK_BASEADDR)

/*
 * 7_x Bạn có thể tiến hành lập trình cho các Peripheral khác như I2C, USART,...
 */


/**
 * 8_Peripheral Register definition structure for RCC (Reset and clock control)
 * Tiến hành định nghĩa cấu trúc các thanh ghi bên trong Ngoại vi cấp xung Clock
 */
typedef struct
{
	__vo uint32_t CR;					/* Address offset: 0x00*/
	__vo uint32_t PLLCFGR;				/* Address offset: 0x04*/
	__vo uint32_t CFGR;					/* Address offset: 0x08*/
	__vo uint32_t CIR;					/* Address offset: 0x0C*/
	__vo uint32_t AHB1RSTR;				/* Address offset: 0x10*/
	__vo uint32_t AHB2RSTR;				/* Address offset: 0x14*/
	__vo uint32_t AHB3RSTR;				/* Address offset: 0x18*/
		 uint32_t RESERVED0;			/* Reserved - Address offset: 0x1C*/
	__vo uint32_t APB1RSTR;				/* Address offset: 0x20*/
	__vo uint32_t APB2RSTR;				/* Address offset: 0x24*/
		 uint32_t RESERVED1[2];			/* Reserved - Address offset: 0x28 - 0x2C*/
	__vo uint32_t AHB1ENR;				/* Address offset: 0x30*/
	__vo uint32_t AHB2ENR;				/* Address offset: 0x34*/
	__vo uint32_t AHB3ENR;				/* Address offset: 0x38*/
		 uint32_t RESERVED2;			/* Reserved - Address offset: 0x3C*/
	__vo uint32_t APB1ENR;				/* Address offset: 0x40*/
	__vo uint32_t APB2ENR;				/* Address offset: 0x44*/
		 uint32_t RESERVED3[2];			/* Reserved - Address offset: 0x48 - 0x4C*/
	__vo uint32_t AHB1LPENR;			/* Address offset: 0x50*/
	__vo uint32_t AHB2LPENR;			/* Address offset: 0x54*/
	__vo uint32_t AHB3LPENR;			/* Address offset: 0x58*/
		 uint32_t RESERVED4;			/* Reserved - Address offset: 0x5C*/
	__vo uint32_t APB1LPENR;			/* Address offset: 0x60*/
	__vo uint32_t APB2LPENR;			/* Address offset: 0x64*/
		 uint32_t RESERVED5[2];			/* Reserved - Address offset: 0x68 - 0x6C*/
	__vo uint32_t BDCR;					/* Address offset: 0x70*/
	__vo uint32_t CSR;					/* Address offset: 0x74*/
		 uint32_t RESERVED6[2];			/* Reserved - Address offset: 0x78 - 0x7C*/
	__vo uint32_t SSCGR;				/* Address offset: 0x80*/
	__vo uint32_t PLLI2SCFGR;			/* Address offset: 0x84*/

}RCC_Regdef_t;

/**
 * 9_RCC definitions (RCC base address type_casted to xxx_RegDef_t)
 */
#define RCC		((RCC_Regdef_t*)RCC_BASEADDR)		//RCC là kiểu con trỏ RCC_RegDef_t ở địa chỉ RCC_BASEADDR

/**********************Peripheral register enable_set function********************************/
/**
 * 10_Clock Enable Macros for GPIOx peripherals
 * AHB1 Bus là thanh ghi đc quản lý các GPIOx, RCC_AHB1ENR là thanh ghi cho phép clock hoạt động
 * Cần xem datasheet thanh ghi RCC_AHB1ENR để bit vị trí các GPIOx để enable cho chính xác
 * EN=ENABLE ; PCLK = PERI_CLOCK ; GPIOx_PCLOCK_EN(): là các hàm macros
 */
#define GPIOA_PCLK_EN()		(RCC->AHB1ENR |= (1 << 0))		//cho bit0(GPIOA) của thanh ghi AHB1ENR lên 1 (enable clock)
#define GPIOB_PCLK_EN()		(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()		(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()		(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()		(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()		(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()		(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()		(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()		(RCC->AHB1ENR |= (1 << 8))

/**
 * 11_Clock Enable Macros for I2Cx peripherals
 * APB1 Bus là thanh ghi đc quản lý các I2Cx, RCC_APB1ENR là thanh ghi cho phép clock hoạt động
 * Cần xem datasheet thanh ghi RCC_APB1ENR để bit vị trí các I2Cx để enable cho chính xác
 */
#define I2C1_PCLK_EN()		(RCC->APB1ENR |= (1 << 21))		//cho bit21(I2C1) của thanh ghi APB1ENR lên 1 (enable clock)
#define I2C2_PCLK_EN()		(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()		(RCC->APB1ENR |= (1 << 23))

/**
 * 12_Clock Enable Macros for SPIx peripherals
 * APB2 Bus là thanh ghi đc quản lý các SPI1, APB1 Bus là thanh ghi đc quản lý các SPIx(2,3)
 * Cần xem datasheet thanh ghi RCC_APB1ENR và RCC_APB2ENR để bit vị trí các SPIx để enable cho chính xác
 */
#define SPI1_PCLK_EN()		(RCC->APB2ENR |= (1 << 12))		//cho bit12(SPI1) của thanh ghi APB2ENR lên 1 (enable clock)
#define SPI2_PCLK_EN()		(RCC->APB1ENR |= (1 << 14))		//cho bit14(SPI1) của thanh ghi APB1ENR lên 1 (enable clock)
#define SPI3_PCLK_EN()		(RCC->APB1ENR |= (1 << 15))

/**
 * 13_Clock Enable Macros for USARTx peripherals
 * APB1 và APB2 Bus là thanh ghi đc quản lý các USARTx
 * Cần xem datasheet thanh ghi RCC_APB1ENR và RCC_APB2ENR để bit vị trí các UARTx để enable cho chính xác
 */
#define USART1_PCLK_EN()	(RCC->APB2ENR |= (1 << 4))	//cho bit4(USART1) của thanh ghi APB2ENR lên 1 (enable clock)
#define USART2_PCLK_EN()	(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()	(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()		(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()		(RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()	(RCC->APB2ENR |= (1 << 5))

/**
 * 14_Clock Enable Macros for SYSCFG peripherals - Cấu hình nguồn xung, vd: HSE,HSI,LSE,LSI,PLL,...
 * APB2 Bus là thanh ghi đc quản lý các SYSCFG - System Configuration Generator
 * Cần xem datasheet thanh ghi RCC_APB2ENR để bit vị trí các SYSCFG để enable cho chính xác
 */
#define SYSCFG_PCLK_EN() 	(RCC->APB2ENR |= (1 << 14))

/**
 * 15_Clock Disable(tắt) Macros for GPIOx peripherals
 * AHB1 Bus là thanh ghi đc quản lý các GPIOx
 * DI=DISABLE(tắt xung clock) ; PCLOCK = PERI_CLOCK ; GPIOx_PCLOCK_DI(): là các hàm macros
 */
#define GPIOA_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 0))		//cho bit0(GPIOA) của thanh ghi AHB1ENR về 0 (disable clock)
#define GPIOB_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()		(RCC->AHB1ENR &= ~(1 << 8))

/**
 * 16_Macros to reset GPIO Peripheral(V98)
 * AHB1 Bus là thanh ghi đc quản lý các GPIOx
 * Cần xem datasheet thanh ghi RCC_AHB1RSTR để bit vị trí các GPIOx để enable cho chính xác
 */
#define GPIOA_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 0)) ; (RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 1)) ; (RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 2)) ; (RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 3)) ; (RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 4)) ; (RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 5)) ; (RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 6)) ; (RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 7)) ; (RCC->AHB1RSTR &= ~(1 << 7)); } while(0)
#define GPIOI_REG_RESET()	do { (RCC->AHB1RSTR |= (1 << 8)) ; (RCC->AHB1RSTR &= ~(1 << 8)); } while(0)

/*1. Giải thích: phần phía sau có 2 mệnh đề, mệnh đề thứ 1 "(RCC->AHB1RSTR |= (1 << x))" dùng để set 1 để yêu cầu
  thực hiện việc reset value các register liên quan đén GPIOx, mệnh đề thứ 2"(RCC->AHB1RSTR &= ~(1 << 0))"
  sau khi reset hoàn thành dùng để quay trở lại trạng thái ko kích hoạt reset '0'. Vì nếu vẫn để set 1 thì trình
  biên dịch vẫn sẽ luôn làm hành động reset register mỗi khi có xung clock.

  2. Do..While: cách để thiết lập 2 mệnh đề để macro với 1 biến, với while(0) thì chỉ cần thực hiện do{} 1 lần.

  3. So sánh GPIOx_PCLK_DI() và GPIOx_REG_RESET():
   + GPIORST (GPIO Reset Register): Thanh ghi này được sử dụng để đặt lại các chân GPIO về trạng thái mặc định,
  thường là mức thấp (0).
   Sử dụng khi khởi tạo vi điều khiển: Đảm bảo tất cả các chân GPIO đều ở trạng thái đã biết trước khi bắt đầu cấu hình.
  và khi muốn khôi phục lại trạng thái mặc định của các chân trong quá trình hoạt động.

   + GPIOENR (GPIO Enable Register): Thanh ghi này được sử dụng để kích hoạt đồng hồ cho các chân GPIO,
  cho phép chúng hoạt động.
   Luôn được thiết lập trước khi cấu hình các chế độ làm việc cho chân GPIO (input, output, interrupt, ...).
  và khi cần kích hoạt lại đồng hồ cho các chân sau khi đã tắt (ví dụ: trong chế độ tiết kiệm năng lượng).
*/

/**
 * Some Generic Marcos - Masros Trạng thái(Mục đích: thống nhất value trạng thái )
 */
#define ENABLE 				1
#define DISABLE 			0
#define SET					ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET 		SET
#define GPIO_PIN_RESET 		RESET

#endif /* INC_STM32F407XX_H_ */
