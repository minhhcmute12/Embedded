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

#include <stdint.h>			//Khai báo thư viện có chứa các kiểu biến như uint32_t
#include <stddef.h>			//Khai báo thư viện cho giá trị 'NULL'


#define __vo 	volatile		//Định nghĩa từ khóa volatile
#define __weak 	__attribute__((weak))

/*****************************19_START: Processor Specific Details*************************/
/**
 * ARM Cortex Mx Processor NVIC ISERx register Address(112)
 */
#define NVIC_ISER0			((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1			((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2			((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3			((__vo uint32_t*)0xE000E10C)
#define NVIC_ISER4			((__vo uint32_t*)0xE000E110)
#define NVIC_ISER5			((__vo uint32_t*)0xE000E114)
#define NVIC_ISER6			((__vo uint32_t*)0xE000E118)
#define NVIC_ISER7			((__vo uint32_t*)0xE000E11C)

/**
 * ARM Cortex Mx Processor NVIC ICERx register Address(112)
 */
#define NVIC_ICER0			((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1			((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2			((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3			((__vo uint32_t*)0xE000E18C)
#define NVIC_ICER4			((__vo uint32_t*)0xE000E190)
#define NVIC_ICER5			((__vo uint32_t*)0xE000E194)
#define NVIC_ICER6			((__vo uint32_t*)0xE000E198)
#define NVIC_ICER7			((__vo uint32_t*)0xE000E19C)

/**
 * ARM Cortex Mx Processor NVIC IPR register Address(113)
 */
#define NVIC_PR_BASEADDR	((__vo uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED		4		/*Số bit ko sử dụng trong cụm 8bit set priority*/

/**
 * 1_Base address of Flash and SRAM memories
 */
#define FLASH_BASEADDR		0x08000000U		//Table5(Phần 3, RM407)
#define SRAM1_BASEADDR		0x20000000U		//'U': thông báo cho trình biên dịch đây là số nguyên kiểu int, uint32_t
#define SRAM2_BASEADDR		0x2001C000U
//Cách tính địa chỉ SRAM2: ta có SRAM(SRAM1+SRAM2) và dung lượng của SRAM1 là 112KB, suy ta vị trí từ
//0x20000000U + 111kb là vị trí cuối của SRAM1
//Vị trí của SRAM2 là 112*1024 = 1C000(Hex) -> Địa chỉ: 0x20000000U + 1C000 = 0x2001C000

#define ROM_BASEADDR		0x1FFF0000U		//Table5(Phần 3, RM407) -> System Memory
#define SRAM 				SRAM1_BASEADDR	//RAM=SRAM1+SRAM2

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

	//__vo uint32_t RCC_DCKCFGR; 	/* Address offset: 0x8C*/
	//__vo uint32_t RCC_CKGATENR; 	/* Address offset: 0x90*/
	//__vo uint32_t RCC_DCKCFGR2; 	/* Address offset: 0x94*/
}RCC_RegDef_t;

/**
 * 9_RCC definitions (RCC base address type_casted to xxx_RegDef_t)
 */
#define RCC			((RCC_RegDef_t*)RCC_BASEADDR)	//RCC là kiểu con trỏ RCC_RegDef_t ở địa chỉ RCC_BASEADDR

/**
 * 9_1 Interrupt Peripheral Register definition structure for EXTI(12.3.7 EXTI register map)
 */
typedef struct
{
	__vo uint32_t IMR;   		//Interrupt mask register 			    - Address offset: 0x00 - __vo: volatile
	__vo uint32_t EMR;			//Event mask register 	    			- Address offset: 0x04
	__vo uint32_t RTSR;			//Rising trigger selection register 	- Address offset: 0x08 - xung cạnh lên
	__vo uint32_t FTSR;			//Falling trigger selection register  	- Address offset: 0x0C - xung cạnh xuống
	__vo uint32_t SWIER;		//Software interrupt event register     - Address offset: 0x10
	__vo uint32_t PR;			//Pending register 	       				- Address offset: 0x14
}EXTI_RegDef_t;

/*
 * 9_1_1 Interrupt definitions (Peripheral base address type_casted to xxx_RegDef_t)
 */
#define EXTI		((EXTI_RegDef_t*)EXTI_BASEADDR)

/**
 * 9_2 Peripheral Register definition structure for SYSCF6(9.2.8 SYSCFG register maps)
 * SYSCFG:  Dùng để quản lý cho phép GPIO port nào đc sử dụng ngắt
 * EXTICR: là thanh ghi quan trọng nhất dùng để điều khiển ngắt trên GPIO port
 */
typedef struct
{
	__vo uint32_t MEMRMP;   		//Memory remap register 			    		- Address offset: 0x00 - __vo: volatile
	__vo uint32_t PMC;				//Peripheral mode configuration register 	    - Address offset: 0x04
	__vo uint32_t EXTICR[4];		//External interrupt configuration 4 register 	- Address offset: 0x08 ~ 0x14
		 uint32_t RESERVED1[2]; 	/* Reserved - Address offset: 0x18 ~ 0x1C*/
	__vo uint32_t CMPCR;			//Compensation cell control register  			- Address offset: 0x20
		 uint32_t RESERVED2[2]; 	/* Reserved - Address offset: 0x24 ~ 0x28*/
	__vo uint32_t CFGR;				//Todo:  										- Address offset: 0x2C

}SYSCFG_RegDef_t;

/*
 * 9_2_1 SYSCFG definitions (Peripheral base address type_casted to xxx_RegDef_t)
 */
#define SYSCFG		((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

/*
 * 9_3 Peripheral Register definition structure for SPI
 * 28.5.10 SPI register map
 */
typedef struct
{
	__vo uint32_t CR1;			//SPI control register 1 			    	- Address offset: 0x00 - __vo: volatile
	__vo uint32_t CR2;			//SPI control register 2			    	- Address offset: 0x04
	__vo uint32_t SR;			//SPI status register 			    		- Address offset: 0x08
	__vo uint32_t DR;			//SPI data register			   				- Address offset: 0x0C
	__vo uint32_t CRCPR;		//SPI CRC polynomial register			   	- Address offset: 0x10
	__vo uint32_t RXCRCR;		//SPI RX CRC register 			   			- Address offset: 0x14
	__vo uint32_t TXCRCR;		//SPI TX CRC register 			   			- Address offset: 0x18
	__vo uint32_t I2SCFGR;		//SPI_I2S configuration register 		    - Address offset: 0x1C
	__vo uint32_t I2SPR;		//SPI_I2S prescaler register  		    	- Address offset: 0x20
}SPI_RegDef_t;

#define SPI1		((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2		((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3		((SPI_RegDef_t*)SPI3_BASEADDR)

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
 * APB2 Bus là thanh ghi đc treo(quản lý) các SYSCFG *
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
 * 16_Macros function to reset GPIO Peripheral(V98)
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
 * 17_Return port code for given GPIOx base address(V111)
 * Mã nhị phân dùng để xác định GPIO port trong thanh ghi SYSCFG_EXTICRx(Note: PI[15:12] are not used)
 */
#define GPIO_BASEADDR_TO_CODE(x)	(	(x == GPIOA) ? 0 : \
										(x == GPIOB) ? 1 : \
										(x == GPIOC) ? 2 : \
										(x == GPIOD) ? 3 : \
										(x == GPIOE) ? 4 : \
										(x == GPIOF) ? 5 : \
										(x == GPIOG) ? 6 : \
										(x == GPIOH) ? 7 : 0	)
//Sử dụng toán tử 3 ngôi để thiết lập mệnh đề define với x là đầu vào param là GPIOx
//'\' trong define tương đương với mệnh đề OR

/**
 * 18_IRQ(Interrupt request) Number of SRTM32F407x MCU(V111)
 * Note: Update these macros with valid values according to your MCU
 * TODO: You may complete this list for other peripheral with NVIC register(Vector table)
 * Dựa vào Vector table xác định các ngắt có thể sử dụng
 * Datasheet: RM407 -> 12 Interrupt and Event -> Table 61. Vector table for STM32F405xx/07xx and STM32F415xx/17xx
 */
//GPIO Interrupt
#define IRQ_NO_EXTI0		6
#define IRQ_NO_EXTI1		7
#define IRQ_NO_EXTI2		8
#define IRQ_NO_EXTI3		9
#define IRQ_NO_EXTI4		10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40

//SPI Interrupt
#define IRQ_NO_SPI1			35
#define IRQ_NO_SPI2         36
#define IRQ_NO_SPI3         51
#define IRQ_NO_SPI4

/**
 * 19_IRQ(Interrupt request) Priority(mức ưu tiên) level of SRTM32F407x MCU(V115)
 * Note: Update these macros with valid values according to your MCU
 * TODO: You may complete this list for other peripheral with NVIC register(Vector table)
 * Mức ưu tiên sẽ nằm trong khoảng từ 0->15
 */
#define NVIC_IRQ_PRI0		0
#define NVIC_IRQ_PRI1		1
#define NVIC_IRQ_PRI2		2
#define NVIC_IRQ_PRI3		3
#define NVIC_IRQ_PRI4		4
#define NVIC_IRQ_PRI5		5
#define NVIC_IRQ_PRI6		6
#define NVIC_IRQ_PRI7		7
#define NVIC_IRQ_PRI8		8
#define NVIC_IRQ_PRI9		9
#define NVIC_IRQ_PRI10		10
#define NVIC_IRQ_PRI11		11
#define NVIC_IRQ_PRI12		12
#define NVIC_IRQ_PRI13		13
#define NVIC_IRQ_PRI14		14
#define NVIC_IRQ_PRI15		15

/**
 * 20_Bit Position definition of SPI Peripheral(V140)
 * Định nghĩa các vị trí bit sẽ đc sử dụng của các thanh ghi SPI Peripherals
 */
/*
 * Bit position definitions SPI_CR1
 */
#define SPI_CR1_CPHA 		0			/* Clock phase */
#define SPI_CR1_CPOL 		1			/* Clock polarity */
#define SPI_CR1_MSTR 		2			/* Master selection */
#define SPI_CR1_BR 			3			/* Baud rate control */
#define SPI_CR1_SPE			6			/* SPI enable */
#define SPI_CR1_LSBFIRST	7			/* Frame format */
#define SPI_CR1_SSI			8			/* Internal slave select */
#define SPI_CR1_SSM			9			/* Software slave management */
#define SPI_CR1_RXONLY		10			/* Receive only */
#define SPI_CR1_DFF 		11			/* Data frame format */
#define SPI_CR1_CRCNEXT		12			/* CRC transfer next */
#define SPI_CR1_CRCEN		13			/* Hardware CRC calculation enable */
#define SPI_CR1_BIDIOE		14			/* Output enable in bidirectional mode */
#define SPI_CR1_BIDIMODE	15			/* Bidirectional data mode enable */

/*
 * Bit position definitions SPI_CR2
 */
#define SPI_CR2_RXDMAEN 	0			/* Rx buffer DMA enable */
#define SPI_CR2_TXDMAEN 	1			/* Tx buffer DMA enable */
#define SPI_CR2_SSOE	 	2			/* SS output enable */
#define SPI_CR2_FRF 		4			/* Frame format */
#define SPI_CR2_ERRIE	 	5			/* Error interrupt enable */
#define SPI_CR2_RXNEIE	 	6			/* RX buffer not empty interrupt enable */
#define SPI_CR2_TXEIE	 	7			/* Tx buffer empty interrupt enable */

/*
 * Bit position definitions SPI_SR
 */
#define SPI_SR_RXNE		 	0			/* Receive buffer not empty */
#define SPI_SR_TXE		 	1			/* Transmit buffer empty */
#define SPI_SR_CHSIDE		2			/* Channel side */
#define SPI_SR_UDR		 	3			/* Underrun flag */
#define SPI_SR_CRCERR		4			/* CRC error flag */
#define SPI_SR_MODF		 	5			/* Mode fault */
#define SPI_SR_OVR		 	6			/* Overrun flag */
#define SPI_SR_BSY		 	7			/* Busy flag */
#define SPI_SR_FRE		 	8			/* Frame format error */

/**
 * 20_1_Macros function to reset SPI Peripheral
 * ABP1(SPI2,SPI3) và APB2(SPI1) Bus là thanh ghi đc treo(quản lý) các SPIx
 */
#define SPI1_REG_RESET()		do{ (RCC->APB2RSTR |= (1 << 12)); 	(RCC->APB2RSTR &= ~(1 << 12)); } while(0)
#define SPI2_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 14)); 	(RCC->APB1RSTR &= ~(1 << 14)); } while(0)
#define SPI3_REG_RESET()		do{ (RCC->APB1RSTR |= (1 << 15)); 	(RCC->APB1RSTR &= ~(1 << 15)); } while(0)


/**
 * Some Generic Marcos - Masros Trạng thái(Mục đích: thống nhất value trạng thái )
 */
#define ENABLE 				1
#define DISABLE 			0
#define SET					ENABLE
#define RESET 				DISABLE
#define GPIO_PIN_SET 		SET
#define GPIO_PIN_RESET 		RESET
#define FLAG_SET			SET			//SPI
#define FLAG_RESET			RESET		//SPI

//#include "stm32f407xx_gpio_driver.h"	//Khai báo thư viện xử lý GPIO peripheral
//Phải khai báo ở dưới vì phải thiết lập xong các define của file "stm32f407xx.h" chung mới đến lượt thiết lập gpio

#endif /* INC_STM32F407XX_H_ */
