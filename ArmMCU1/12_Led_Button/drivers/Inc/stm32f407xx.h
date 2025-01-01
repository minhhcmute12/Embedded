/*
 * stm32f407xx.h
 *
 *  Created on: Nov 10, 2023
 *      Author: Minh
 *  File Header này dùng để Macros các thông tin quan trong của MCUs
 */
/****************************************DRIVER Layer**************************************/
#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include "stdint.h"			//Khai báo thư viện có chứa các kiểu biến như uint32_t


#define __vo volatile		//Định nghĩa từ khóa volatile

/**
 * 1_Base address of Flash and SRAM memories
 */
#define FLASH_BASEADDR		0x08000000U		//Table5(Phần 3, RM407)
#define SRAM1_BASEADDR		0x20000000U		//'U': thông báo cho trình biên dịch đây là số nguyên kiểu int, uint32_t
#define SRAM2_BASEADDR		0x2001C000U
//Cách tính địa chỉ SRAM2: ta có SRAM(SRAM1+SRAM2) và dung lượng của SRAM1 là 112KB, suy ta vị trí từ
//0x20000000U + 111kb là vị trí cuối của SRAM1
//Vị trí của SRAM2 là 112*1024 = 1C000(Hex) -> Địa chỉ: 0x20000000U + 1C000 = 0x2001C000

#define ROM_BASEADDR		0x1FFF0000U		////Table5(Phần 3, RM407) -> System Memory
#define SRAM 				SRAM1_BASEADDR

/*
 * Thêm: Ta có thể viết là
 *  + 		#define HAL_FLASH_BASEADDR		0x08000000U
 *  + hoặc 	#define DRV_FLASH_BASEADDR		0x08000000U
 * HAL hoặc DRV : có nghĩa là biến macro này đang thuộc "HAL layer header file" hoặc "Driver layer header file",
 * thường đc dùng để ký hiệu phân biết biến sử dụng đang thuộc file lập trình nào,thuộc lớp nào, nằm ở đâu
 */

/**
 * 2_AHBx and APBx Bus Peripheral address
 */
#define PERIPH_BASE			0x40000000U					//Địa chỉ cở sở Bus
#define APB1PERIPH_BASE		PERIPH_BASE
#define APB2PERIPH_BASE		0x40010000U
#define AHB1PERIPH_BASE		0x40020000U
#define AHB2PERIPH_BASE		0x50000000U

/**
 * 3_Base address of peripherals which are hanging on AHB1 Bus
 * TODO: Complete for all another peripherals(use Memory Map)
 */
#define GPIOA_BASEADDR		(AHB1PERIPH_BASE)				//Địa chỉ bắt đầu của AHB1PERIPH_BASE cũng là địa chỉ GPIOA
#define GPIOB_BASEADDR		(AHB1PERIPH_BASE + 0x0400) 		//0x0400 là phần offset
#define GPIOC_BASEADDR		(AHB1PERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR		(AHB1PERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR		(AHB1PERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR		(AHB1PERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR		(AHB1PERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR		(AHB1PERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR		(AHB1PERIPH_BASE + 0x2000)
#define RCC_BASEADDR		(AHB1PERIPH_BASE + 0x3800)		//Địa chỉ cở sở của RCC

/**
 * 4_Base address of peripherals which are hanging on APB1 Bus
 * TODO: Complete for all another peripherals(use Memory Map)
 */
#define I2C1_BASEADDR 		(APB1PERIPH_BASE + 0x5400) 		//0x5400 là phần offset
#define I2C2_BASEADDR 		(APB1PERIPH_BASE + 0x5800)
#define I2C3_BASEADDR 		(APB1PERIPH_BASE + 0x5C00)

#define SPI2_BASEADDR 		(APB1PERIPH_BASE + 0x3800)
#define SPI3_BASEADDR 		(APB1PERIPH_BASE + 0x3C00)

#define USART2_BASEADDR 	(APB1PERIPH_BASE + 0x4400)
#define USART3_BASEADDR 	(APB1PERIPH_BASE + 0x4800)
#define UART4_BASEADDR 		(APB1PERIPH_BASE + 0x4C00)
#define UART5_BASEADDR 		(APB1PERIPH_BASE + 0x5000)

/**
 * 5_Base address of peripherals which are hanging on APB2 Bus
 * TODO: Complete for all another peripherals(use Memory Map)
 */
#define EXTI_BASEADDR		(APB2PERIPH_BASE + 0x3C00)		//0x3C00 là phần offset
#define SPI1_BASEADDR		(APB2PERIPH_BASE + 0x3000)
#define SYSCFG_BASEADDR		(APB2PERIPH_BASE + 0x3800)
#define USART1_BASEADDR		(APB2PERIPH_BASE + 0x1000)
#define USART6_BASEADDR		(APB2PERIPH_BASE + 0x1400)


/**********************6_Peripheral register definition structure********************************/
/**
 * Note: Register of a peripheral are specific(riêng biệt) to MCU
 * e.g: Number of Registers of SPI peripheral of STM32F4x family of MCUs may be different (more or less)
 * Compared to number of registers of SPI peripheral of STM32Lx or STM32F0x family of MCUs
 * Please check your Device RMx
 * Ví dụ: Số lượng thanh ghi của thiết bị ngoại vi SPI của họ MCU STM32F4x có thể khác (ít hoặc nhiều) so với số lượng
 * thanh ghi của thiết bị ngoại vi SPI của họ MCU STM32Lx hoặc STM32F0x
 * Vui lòng kiểm tra RMx thiết bị của bạn
 */
//Cấu trúc Struct chung cho một GPIOx register
typedef struct
{
	__vo uint32_t MODER;   	//GPIO port mode register 			    - Address offset: 0x00 - __vo: volatile
	__vo uint32_t OTYPER;		//GPIO port output type register 	    - Address offset: 0x04
	__vo uint32_t OSPEEDR;		//GPIO port output speed register 	    - Address offset: 0x08
	__vo uint32_t PUPDR;		//GPIO port pull-up/pull-down register  - Address offset: 0x0C
	__vo uint32_t IDR;			//GPIO port input data register         - Address offset: 0x10
	__vo uint32_t ODR;			//GPIO port output data register        - Address offset: 0x14
	__vo uint32_t BSRR;			//BSRR GPIO port bit set/reset register - Address offset: 0x18
	__vo uint32_t LCKR;			//GPIO port configuration lock register - Address offset: 0x1C
	__vo uint32_t AFR[2];		//Address offset: 0x20-0x24
	//AFR[0]: GPIO alternate function low register - AFR[1]: GPIO alternate function high register
}GPIO_RegDef_t;

/**
 * 7_Peripheral definitions (Peripheral base address type_casted to xxx_RegDef_t)
 */
#define GPIOA		((GPIO_RegDef_t*)GPIOA_BASEADDR)   //GPIOA là kiểu con trỏ GPIO_RegDef_t ở địa chỉ GPIOA_BASEADDR
#define GPIOB		((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC		((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD		((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE		((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF		((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG		((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH		((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOI		((GPIO_RegDef_t*)GPIOI_BASEADDR)

/*
 * 7_x Bạn có thể tiến hành lập trình cho các Peripheral khác như I2C, USART,...
 */

/**
 * 8_Peripheral Register definition structure for RCC
 */
typedef struct
{
	__vo uint32_t CR; 				/* Address offset: 0x00*/
	__vo uint32_t PLLCFGR; 			/* Address offset: 0x04*/
	__vo uint32_t CFGR; 			/* Address offset: 0x08*/
	__vo uint32_t CIR; 				/* Address offset: 0x0C*/
	__vo uint32_t AHB1RSTR; 		/* Address offset: 0x10*/
	__vo uint32_t AHB2RSTR; 		/* Address offset: 0x14*/
	__vo uint32_t AHB3RSTR; 		/* Address offset: 0x18*/
		 uint32_t RESERVED0; 		/* Reserved - Address offset: 0x1C*/
	__vo uint32_t APB1RSTR; 		/* Address offset: 0x20*/
	__vo uint32_t APB2RSTR; 		/* Address offset: 0x24*/
		 uint32_t RESERVED1[2]; 	/* Reserved - Address offset: 0x28 ~ 0x2C*/
	__vo uint32_t AHB1ENR; 			/* Address offset: 0x30*/
	__vo uint32_t AHB2ENR; 			/* Address offset: 0x34*/
	__vo uint32_t AHB3ENR; 			/* Address offset: 0x38*/
		 uint32_t RESERVED2; 		/* Reserved - Address offset: 0x3C*/
	__vo uint32_t APB1ENR; 			/* Address offset: 0x40*/
	__vo uint32_t APB2ENR; 			/* Address offset: 0x44*/
		 uint32_t RESERVED3[2]; 	/* Reserved - Address offset: 0x48 ~ 0x4C*/
	__vo uint32_t AHB1LPENR; 		/* Address offset: 0x50*/
	__vo uint32_t AHB2LPENR; 		/* Address offset: 0x54*/
	__vo uint32_t AHB3LPENR; 		/* Address offset: 0x58*/
		 uint32_t RESERVED4; 		/* Reserved - Address offset: 0x5C*/
	__vo uint32_t APB1LPENR; 		/* Address offset: 0x60*/
	__vo uint32_t APB2LPENR; 		/* Address offset: 0x64*/
		 uint32_t RESERVED5[2]; 	/* Reserved - Address offset: 0x68 ~ 0x6C*/
	__vo uint32_t RCC_BDCR; 		/* Address offset: 0x70*/
	__vo uint32_t RCC_CSR; 			/* Address offset: 0x74*/
		 uint32_t RESERVED6[2]; 	/* Reserved - Address offset: 0x78 ~ 0x7C*/
	__vo uint32_t RCC_SSCGR; 		/* Address offset: 0x80*/
	__vo uint32_t RCC_PLLI2SCFGR; 	/* Address offset: 0x84*/
}RCC_RegDef_t;

/**
 * 9_RCC definitions (RCC base address type_casted to xxx_RegDef_t)
 */
#define RCC			((RCC_RegDef_t*)RCC_BASEADDR)	//RCC là kiểu con trỏ RCC_RegDef_t ở địa chỉ RCC_BASEADDR

/**
 * 10_Clock Enable Macros for GPIOx peripherals
 * AHB1 Bus là thanh ghi đc treo(quản lý) các GPIOx
 * EN=ENABLE ; PCLK = PERI_CLOCK ; GPIOx_PCLOCK_EN(): là các hàm macros
 */
#define GPIOA_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 0))  	//Cho bit0(GPIOA) thanh ghi AHB1ENR lên 1
#define GPIOB_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 1))		//Cho bit1(GPIOB) thanh ghi AHB1ENR lên 1
#define GPIOC_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 2))		//Cho bit1(GPIOB) thanh ghi AHB1ENR lên 1
#define GPIOD_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN() 		(RCC->AHB1ENR |= (1 << 8))

/**
 * 11_Clock Enable Macros for I2Cx peripherals
 * APB1 Bus là thanh ghi đc treo(quản lý) các I2Cx
 */
#define I2C1_PCLK_EN() 		(RCC->APB1ENR |= (1 << 21))  	//Cho bit21(I2C1) thanh ghi APB1ENR lên 1
#define I2C2_PCLK_EN() 		(RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN() 		(RCC->APB1ENR |= (1 << 23))

/**
 * 12_Clock Enable Macros for SPIx peripherals
 * APB2 Bus là thanh ghi đc treo(quản lý) các SPI1, APB1 Bus là thanh ghi đc treo(quản lý) các SPIx(2,3)
 */
#define SPI1_PCLK_EN() 		(RCC->APB2ENR |= (1 << 12))  	//Cho bit12(SPI1) thanh ghi APB2ENR lên 1
#define SPI2_PCLK_EN() 		(RCC->APB1ENR |= (1 << 14))		//Cho bit14(SPI2) thanh ghi APB1ENR lên 1
#define SPI3_PCLK_EN() 		(RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN() 		(RCC->APB2ENR |= (1 << 13))

/**
 * 13_Clock Enable Macros for USARTx peripherals
 * APB1 và APB2 Bus là thanh ghi đc treo(quản lý) các USARTx
 */
#define USART1_PCLK_EN() 		(RCC->APB2ENR |= (1 << 4))	//Cho bit4(USART) thanh ghi APB2ENR lên 1
#define USART2_PCLK_EN() 		(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN() 		(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN() 		(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN() 		(RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN() 		(RCC->APB2ENR |= (1 << 5))

/**
 * 14_Clock Enable Macros for SYSCFG peripherals
 * APB2 Bus là thanh ghi đc treo(quản lý) các SYSCFG
 */
#define SYSCFG_PCLK_EN() 		(RCC->APB2ENR |= (1 << 14))

/**
 * 15_Clock Disable Macros for GPIOx peripherals
 * AHB1 Bus là thanh ghi đc treo(quản lý) các GPIOx
 * DI=DISABLE ; PCLOCK = PERI_CLOCK ; GPIOx_PCLOCK_DI(): là các hàm macros
 */
#define GPIOA_PCLOCK_DI() 		(RCC->AHB1ENR &= ~(1 << 0))  	//Cho bit0(GPIOA) thanh ghi AHB1ENR lên 0(reset)
#define GPIOB_PCLOCK_DI() 		(RCC->AHB1ENR &= ~(1 << 1))  	//Cho bit1(GPIOB) thanh ghi AHB1ENR lên 0(reset)
#define GPIOC_PCLOCK_DI() 		(RCC->AHB1ENR &= ~(1 << 2))  	//Cho bit2(GPIOC) thanh ghi AHB1ENR lên 0(reset)

/**
 * 16_Macros to reset GPIO Peripheral(V98)
 * AHB1 Bus là thanh ghi đc treo(quản lý) các GPIOx
 */
#define GPIOA_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 0)); 	(RCC->AHB1RSTR &= ~(1 << 0)); } while(0)
#define GPIOB_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 1));	(RCC->AHB1RSTR &= ~(1 << 1)); } while(0)
#define GPIOC_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 2));	(RCC->AHB1RSTR &= ~(1 << 2)); } while(0)
#define GPIOD_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 3)); 	(RCC->AHB1RSTR &= ~(1 << 3)); } while(0)
#define GPIOE_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 4));	(RCC->AHB1RSTR &= ~(1 << 4)); } while(0)
#define GPIOF_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 5));	(RCC->AHB1RSTR &= ~(1 << 5)); } while(0)
#define GPIOG_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 6));	(RCC->AHB1RSTR &= ~(1 << 6)); } while(0)
#define GPIOH_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 7));	(RCC->AHB1RSTR &= ~(1 << 7)); } while(0)
#define GPIOI_REG_RESET()		do{ (RCC->AHB1RSTR |= (1 << 8));	(RCC->AHB1RSTR &= ~(1 << 8)); } while(0)

//Giải thích: phần phía sau có 2 mệnh đề, mệnh đề thứ 1 "(RCC->AHB1RSTR |= (1 << x))" dùng để set 1 để yêu cầu
//thực hiện việc reset value các register liên quan đén GPIOx, mệnh đề thứ 2"(RCC->AHB1RSTR &= ~(1 << 0))"
//sau khi reset hoàn thành dùng để quay trở lại trạng thái ko kích hoạt reset '0'. Vì nếu vẫn để set 1 thì trình
//biên dịch vẫn sẽ luôn làm hành động reset register mỗi khi có xung clock.
//Do..While: cách để thiết lập 2 mệnh đề để macro với 1 biến, với while(0) thì chỉ cần thực hiện do{} 1 lần
/**
 * Some Generic Marcos - Masros Trạng thái(Mục đích: thống nhất value trạng thái )
 */
#define ENABLE 				1
#define DISABLE 			0
#define SET					ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET 		SET
#define GPIO_PIN_RESET 		RESET

//#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
//Phải khai báo ở dưới vì phải thiết lập xong các define của file "stm32f407xx.h" chung mới đến lượt thiết lập gpio

#endif /* INC_STM32F407XX_H_ */
