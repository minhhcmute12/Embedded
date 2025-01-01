/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Nov 14, 2023
 *      Author: Minh
 *  File Source này dùng để lập trình GPIO Drivers
 */
/*************************************APIs Layer for GPIO peripheral***********************************/

#include "stm32f407xx_gpio_driver.h"

/*
 * 1_Peripheral Clock setup - APIs Peripheral Clock
 */
/****************************************************************
 * @fn			- GPIO_PeriClockControl
 *
 * @brief		- Bật/tắt Clock cho GPIO peripheral
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();		//hàm đã đc define trong stm32f407xx.h(lớp Driver)
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}
	}
	else
	{
		//Todo: (Interrupt Status)

	}
}

/*
 * 2_1Initialization - APIs GPIO initialization
 */
/****************************************************************
 * @fn			- GPIO_Init
 *
 * @brief		- Khởi tạo GPIO port và GPIO pin(mode, speed, out/in, pullup/pulldown,...)
 *
 * @param[in]	- GPIOx_Handle_t: con trỏ là GPIO port x muốn xử lý công việc
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_Init(GPIOx_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0; 	//temp. registerr

	//1. configure(cấu hình) the mode of gpio pin(Datasheet 8.4.1)
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//Giá trị GPIO_PinMode thuộc 1 trong 4 trường hợp bit của Mode Register
		//this is part non-interrupt mode

		//Xác định lấy vị trí MODERx cần điều khiển dựa vào số chân Pin_Number và lưu vào biến temp
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

		//Lưu ý: Có nghĩa là trước khi thực hiện khởi tạo ta sẽ xác định vị trí sử dụng pin_number nào
		//sau đó sẽ lưu value vào pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber

		//Dịch 2 bit vì theo datasheet, mỗi 2 bit quản lý một pin number của GPIO port mode register
		//vd: GPIO_PinNumber=Pin0 thì sẽ kích hoạt MODER0(2*0=0->bắt đầu từ bit0)
		//GPIO_PinNumber=Pin1 thì sẽ kích hoạt MODER1(2*1=2->bắt đầu từ bit2)

		//Xóa(clear) value bit tại vị trí 2 bit muốn cấu hình
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		//Theo datasheet thì để reset MODERx thì ta cần thiết lập value bit là 00, Ta có: 0x3 = 11
		//~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); nghĩa là tiến hành đảo trạng thái 11 thành 00
		//vị trí chân Pin_Number

		//Lưu value temp vào trong thanh ghi MODER của biến pGPIOHandle
		pGPIOHandle->pGPIOx->MODER |= temp;
		//=> Lúc này ta đã xác định(cấu hình) đc vị trí mode(MODERx) của GPIO tùy thuộc vào pin_number bạn muốn control
	}
	else
	{
		//This is part(phần) will code later (Interrupt mode) //các trường hợp khác từ 4->6

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1. configure the FTSR(Falling(xuống) Trigger Select Register)
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			//1.2 Clear the corresponding(tương ứng) RTSR, việc này phòng ngừa cho tình huống sử dụng RTSR trước đó
			//và có thể gây xung đột cho lần cấu hình hiện tại
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1. configure the RTSR(Rising(lên) Trigger Select Register)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			//1.2 Clear the corresponding(tương ứng) FTSR, việc này phòng ngừa cho tình huống sử dụng FTSR trước đó
			//và có thể gây xung đột cho lần cấu hình hiện tại
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//1. configure the FTSR and RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. Configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;		//xác định SYSCFG_EXTICR_temp1 register
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;		//xác định 4bit EXTI_temp2 trong SYSCFG_EXTICR_temp1
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);		//xác định mã nhị phân xxxx để xác định GPIO port sử dụng
		SYSCFG_PCLK_EN();													//Kích hoạt xung clock để cấu hình
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);					//Cấu hình thanh ghi SYSCFG_EXTICR_temp1
		
		/* Kiến thức: Phân biệt [SYSCFG->EXTICR[3] = 0x2<< (1* 4);] Và [SYSCFG->EXTICR[3]  |= 0x2<< (1* 4);]
		+Đoạn code 1: SYSCFG->EXTICR[3] = 0x2 << (1* 4);
		->Ý nghĩa: Đoạn code này gán giá trị 0x2 << (1* 4) (tức là 0010) trực tiếp vào thanh ghi SYSCFG->EXTICR[3] 
		bắt đầu từ vị trí bit4 và kéo dài 4 bit. Điều này có nghĩa là tất cả các bit trong thanh ghi này sẽ được thay thế 
		và chỉ có 4 bit từ bit4->7 sẽ bằng giá trị 0x2(0010).
		->Tác dụng: Đoạn code này sẽ ghi đè cấu hình hiện tại của ngắt bên ngoài liên quan đến thanh ghi EXTICR[3] bằng 
		cấu hình mới. Các bit khác trong thanh ghi này sẽ bị mất đi.
		
		+Đoạn code 2: SYSCFG->EXTICR[3] |= 0x2 << (1* 4);
		->Ý nghĩa: Đoạn code này thực hiện phép OR bit-by-bit giữa giá trị hiện tại của thanh ghi SYSCFG->EXTICR[3] và 
		giá trị 0x2 << (1* 4). Kết quả của phép OR sẽ được gán lại cho chính thanh ghi này.
		->Tác dụng: Đoạn code này sẽ thiết lập các bit tương ứng của thanh ghi EXTICR[3] thành 1, trong khi giữ nguyên 
		các bit còn lại. Điều này có nghĩa là chúng ta có thể thêm một cấu hình ngắt mới vào cấu hình hiện tại mà không 
		làm mất đi các cấu hình khác.
		*/

		//3. Eanble the exti interrupt delivery(chuyển, sử dụng) by using IMR(Interrupt Masking Register)
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}
	//Reset biến temp để sử dụng cho các lệnh khác trong hàm
	temp = 0;

	//2. configure the speed (Datasheet 8.4.3)
	//Xác định lấy vị trí OSPEEDRx cần điều khiển dựa vào số chân Pin_Number và lưu vào biến temp
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &=  ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);	//Clearing bit field
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	//=> Lúc này ta đã xác định(cấu hình) đc speed(OSPEEDRx) của GPIO tùy thuộc vào pin_number bạn muốn control
	temp = 0;

	//3. configure the pullup-pulldown settings (Datasheet 8.4.4)
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &=  ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	//4. configure the optype (Datasheet 8.4.2)
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	//Dịch 1 bit vì theo datasheet, mỗi 1 bit quản lý một pin number của GPIO port mode register
	pGPIOHandle->pGPIOx->OTYPER &=  ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //Clearing bit field
	//~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); nghĩa là tiến hành đảo trạng thái 1 thành 0
	//vị trí chân Pin_Number
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	//5. configure the alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//Chế độ này đc thực thi khi và chỉ khi bit MODERx đc thiết lập GPIO_MODE_ALTFN

		uint32_t temp1,temp2;

		//Xác định thanh ghi cần sử dụng là GPIOx_AFRH ->AFR[1] hay GPIOx_AFRL ->AFR[0]
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;

		//Xác định vị trí cụm 4bit cần sử dụng ở vị trí nào trong thanh ghi
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;

		//Clearing bit field
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4* temp2));   //0xF = 1111, đảo 1111->0000 tại vị trí 4*temp2

		//Lưu cấu hình AFR vào trong con trỏ pGPIOHandle
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4* temp2));
	}
}

/*
 * 2_2 De-Ini - APIs Reset GPIO Port
 */
/****************************************************************
 * @fn			- GPIO_DeInit
 *
 * @brief		- Reset base address GPIO port và GPIO pin(mode, speed, out/in, pullup/pulldown,...)
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển đến
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();		//hàm đã đc define trong stm32f407xx.h(lớp Driver)
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
	else if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}
}

/*
 * 3_Data Read and Write
 * pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển
 * PinNumber: chân Pin muốn điều khiển(đọc,viết) trạng thái
 * value: trạng thái muốn điều khiển(port có 16pin nên khi khai báo là uint16_t)
 */

/*
 * 3_1APIs Read Input Pin
 */
/****************************************************************
 * @fn			- GPIO_ReadFromInputPin
 *
 * @brief		- Read Status Input of GPIO pin
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển đến
 * @param[in]	- PinNumber: chân_pin muốn read input
 *
 * @return		- value of read in pin_number (0 or 1)
 *
 * @Note 		- none
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;			//biến lưu value đọc đc từ pin
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;

	//((pGPIOx->IDR >> PinNumber): Phần này dịch chuyển giá trị IDR sang phải của vị trí PinNumber đầu vào,
	//căn chỉnh giá trị đầu vào của chân mong muốn về bit ngoài cùng bên phải(bit0).
	//& 0x00000001: Thao tác AND theo bit này che giấu tất cả ngoại trừ bit ngoài cùng bên phải(bit0),
	//trích xuất hiệu quả giá trị đầu vào của mã pin được chỉ định.
}

/*
 * 3_2APIs Read Input Port
 */
/****************************************************************
 * @fn			- GPIO_ReadFromInputPort
 *
 * @brief		- Read Status Input of GPIO port(Đọc tất cả status pin của port)
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển đến
 *
 * @return		- value of read in GPIOx Port (0 or 1)
 *
 * @Note 		- none
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;			//biến lưu value đọc đc từ port
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

/*
 * 3_3APIs Write Output Pin
 */
/****************************************************************
 * @fn			- GPIO_WriteToOutputPin
 *
 * @brief		- Write Status Output of GPIO pin
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển đến
 * @param[in]	- PinNumber: chân_pin muốn write output
 * @param[in]	- value: trạng thái muốn điều khiển(1 or 0)
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	if(value == GPIO_PIN_SET)
	{
		//Write 1 to output data register at the bit field corresponding tho the pin_number
		//Viết 1 vào thanh ghi dữ liệu đầu ra tại trường bit tương ứng với số pin
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
		//Write 0 to output data register at the corresponding pin_number
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

/*
 *  3_4Write Output Port
 */
/****************************************************************
 * @fn			- GPIO_WriteToOutputPort
 *
 * @brief		- Write Status Output of GPIO port
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển đến
 * @param[in]	- value: trạng thái muốn điều khiển(1 or 0)(port có 16pin nên khi khai báo là uint16_t)
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t value)
{
	pGPIOx->ODR = value;		//Ghi value vào toàn bộ bit của port
}

/*
 * 3_5APIs Toggle(chuyển đổi) Status Pin
 */
/****************************************************************
 * @fn			- GPIO_ToggleOutputPin
 *
 * @brief		- Toggle(Đảo) Status Output of GPIO pin
 *
 * @param[in]	- pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển đến
 * @param[in]	- PinNumber: chân_pin muốn write toggle output
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	//pGPIOx->ODR = pGPIOx->ODR ^ (1 << PinNumber);   //Đảo trạng thái bit(toán tử XOR) tại vị trí pin number của pGPIOx->ODR
	//'^' : Hoạt động XOR đảo ngược bit ở chân được chỉ định, chuyển đổi trạng thái đầu ra của nó
	//Sau đó sẽ gán trỏ vào thanh ghi "pGPIOx->ODR"

	pGPIOx->ODR ^= (1 << PinNumber);
}

/*
 * 4_IRQ Configuration and ISR Hangling
 */
/*
 * 4_1APIs Interrupt config(cấu hình) - Yêu cầu ngắt
 */
/****************************************************************
 * @fn			- GPIO_IRQInterruptConfig
 *
 * @brief		- Config(Cấu hình) Interrupt Status of GPIO pin
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- EnorDi: Enable or Disable
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)	//cho phép kích hoạt interrupt
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//program ISER1 register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);   //vd 32%32=0, lấy bit 0 của ISER1
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ISER2 register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//program ICER1 register
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//program ICER2 register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
	}
}

/*
 * 4_3APIs Interrupt Priority(ưu tiên) - cấu hình mức ưu tiên cho ngắt
 */
/****************************************************************
 * @fn			- GPIO_IRQPriorityConfig
 *
 * @brief		- Set Priority(ưu tiên) Interrupt Status of GPIO pin
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- IRQPriority: mức ưu tiên của ngắt mong muốn
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;			//Xác định sử dụng thanh ghi nào
	uint8_t iprx_section = IRQNumber % 4;	//Xác định phần 8bit nào đc sử dụng(mỗi thanh ghi có 4 phần 8 bit)

	//Mỗi phần 8bit thì chỉ lấy 4bit cao làm priority, 4bit thấp ko sử dụng
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + (iprx * 4)) |= (IRQPriority << shift_amount);
	//iprx * 4 : địa chỉ thanh ghi iprx tiếp theo phải + 4 do mỗi thanh ghi iprx đều 32 bit
}

/*
 * 4_2APIs Interrupt Hangling(xử lý) - thực hiện công việc khi ngắt xảy ra
 */
/****************************************************************
 * @fn			- GPIO_IRQHangling
 *
 * @brief		- Hangling(thực hiện) Interrupt Status of GPIO pin
 *
 * @return		- none
 *
 * @Note 		- none
 */
void GPIO_IRQHangling(uint8_t PinNumber)
{
	//Clear the exti pr register corresponding(tương ứng) to the pin number
	if(EXTI->PR & (1 << PinNumber))
	{
		EXTI->PR &= ~(1 << PinNumber); 		//Clear bit trong PR(Pending Register EXTI)
	}
}
