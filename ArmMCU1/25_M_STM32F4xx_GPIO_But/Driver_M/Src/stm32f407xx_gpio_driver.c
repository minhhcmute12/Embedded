/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Nov 17, 2024
 *      Author: Minh
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
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDI)
{
	if(EnorDI == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();			//Hàm đã đc define trong file stm32f407xx.h
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
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();			//Hàm đã đc define trong file stm32f407xx.h
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
		else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}
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
	uint32_t temp = 0;		//value temp of register

	//1. Configure(cấu hình) the mode of gpio pin (RM407 -> 8.4.1)
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//--->Đây là trường hợp giá trị GPIO_PinMode thuộc 0 trong 3 trường hợp bit của Mode Register
		//Và cũng là trường hợp cho dạng non-interrupt mode

		//1.1_Xác định vị trí và gán value MODERx cần điều khiển dựa vào số chân Pin_Number và lưu vào biến temp
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

		//Lưu ý: Có nghĩa là trước khi thực hiện khởi tạo ta sẽ xác định vị trí sử dụng pin_number nào
		//sau đó sẽ lưu value vào pGPIOHandle->pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber

		//Dịch 2 bit vì theo datasheet, mỗi 2 bit quản lý một pin_number của GPIO port mode register
		//vd: GPIO_PinNumber=Pin0 thì sẽ kích hoạt MODER0(2*0=0 -> bắt đầu từ bit 0)
		//vd: GPIO_PinNumber=Pin1 thì sẽ kích hoạt MODER0(2*1=0 -> bắt đầu từ bit 2)

		//Vd ta chọn PinMode là Output=1 thì và Pin_Number=0 ta sẽ có biểu thức:
		//temp = (1 << 0) => Set bit 0 lên 1 => Hai bit 0 và 1 có cấu hình là 01 -> Cấu hình pin0 là output mode

		//1.2_Xóa(clear) value bit tại vị trí 2 bit muốn thiết lập từ cấu hình PinConfig
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		/* Theo datasheet thì để reset MODERx thì ta cần thiết lập value bit là 00, ta có: 0x3 = 11(set cùng lúc 2 giá trị bit)
		 * ~(0x3<<pGPIOHandle->GPIO_PinConfig->GPIO_PinNumber): nghĩa là tiến hành đảo trạng thái xx thành 00 theo giá
		 * trị của chân PinNumber đc thiết lập
		 */

		//1.3_Tiến hành cấu hình 2bit theo giá trị biến temp(đã đc thiết lập giá trị bit theo vị trí mong muốn)
		//vào trong thanh ghi MODER của biến pGPIOHandle
		pGPIOHandle->pGPIOx->MODER |= temp;

		//Giải thích: ta sẽ xác định giá trị temp dựa vào các giá trị cấu hình do người dùng nhập vào
		//Sau đó dùng gán giá trị temp(đc xác định) vào trong thanh ghi điều khiển MODERx
		//Thanh ghi MODERx của pGPIOHandle->pGPIOx đã đc cấu hình địa chỉ theo datasheet của chip stm32f407xx
		//Sử dụng cách tương tự ở trên cho các phần setup tiếp theo thuộc pGPIOHandle->pGPIOx

		/* pGPIOHandle->GPIO_PinConfig : cấu hình thanh ghi do người dùng lựa chọn
		 * pGPIOHandle->pGPIOx : cấu hình thanh ghi cho chip hoạt động
		 * => Nghĩa là điều khiển hoạt động của chip(cấu hình thanh ghi pGPIOHandle->pGPIOx) dựa trên các thông số
		 * cấu hình đc cài đặt bởi người dùng(cấu hình thanh ghi pGPIOHandle->GPIO_PinConfig)
		 */
	}
	else{
		//--->Phần này sẽ đc sử dụng cho Interrupt mode từ 4 đến 6
		/* Chú ý: Nếu thiết lập Interrup thì phần GPIOx->MODER sẽ không đc thiết lập
		* Việc quản lý ngắt sẽ do EXTIx Register và NVIC Register thực hiện            */

		//1.1 Kiểm tra xem xung ngắt có phải là đc cấu hình cạnh lên, cạnh xuống hay cả hai
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1.1.1 Set cấu hình đã chọn vào thanh ghi FTSR(Falling(xuống) Trigger Select Register)
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			//1.1.2 Clear the corresponding(tương ứng) RTSR, việc này phòng ngừa cho tình huống sử dụng RTSR trước đó
			//và có thể gây xung đột cho lần cấu hình hiện tại
			EXTI->RTSR &= ~(1 <<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1.1.1 Set cấu hình đã chọn vào thanh ghi RTSR(Rising(lên) Trigger Select Register)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			//1.1.2 Clear the corresponding(tương ứng) FTSR, việc này phòng ngừa cho tình huống sử dụng FTSR trước đó
			//và có thể gây xung đột cho lần cấu hình hiện tại
			EXTI->FTSR &= ~(1 <<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//1.1.1 Set cấu hình đã chọn vào thanh ghi RTSR(Rising(lên) Trigger Select Register)
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			//1.1.2 Set cấu hình đã chọn vào thanh ghi FTSR(Falling(xuống) Trigger Select Register)
			EXTI->FTSR |= (1 <<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//1.2. Configure the GPIO port selection in SYSCFG_EXTICR - Cấu hình cấp nguồn xung khi có Interrupt
		//1.2.1 Tính toán thanh ghi SYSCFG_EXTICRx(x từ 1->4) và nhóm 4bit nào đc sử dụng
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;

		//1.2.2 Xác định GPIO port nào đc sử dụng để làm ngắt
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		//1.2.3 Kích hoạt xung clock cho Interrupt
		SYSCFG_PCLK_EN();

		//1.2.4 Cấu hình thanh ghi SYSCFG_EXTICRx với x là thanh ghi số temp1 và nhóm bit temp2
		SYSCFG->EXTICR[temp1] = portcode << (temp2 *4);
		/* Giải thích: Ví dụ ngắt trên pin 1 GPIOC -> portcode = 2 = 0010
		 * -> SYSCFG->EXTICR[0] = 0010 << (1 *4);
		 * -> Cấu hình tại thanh ghi SYSCFG->EXTICR[0] bắt đầu từ bit 4 với value nhóm bit [7:4] = 0010
		 */

		//1.2.5 Eanble the EXTI interrupt delivery(chuyển, sử dụng) by using IMR(Interrupt Masking Register)
		//Thanh ghi có chức năng kích hoạt hoặc vô hiệu hóa ngắt
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	}

	//Reset biến temp để sử dụng cho các lệnh khác trong hàm

	//2. Configure the speed (RM407->8.4.3)
	//2.1_Xác định vị trí và gán value OSPEEDx cần điều khiển dựa vào số chân Pin_Number và lưu vào biến temp
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	//2.2_Xóa bit thanh ghi chip
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	//2.3_Set bit thanh ghi chip
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	temp = 0;
	//3. Configure the pullup-pulldown settings (RM407 -> 8.4.4)
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;
	//4. Configure the optype (RM407 -> 8.4.2)
	//Trong thanh ghi OPTYPE thì mỗi một bit chỉ quản lý một pin nghĩa là chỉ sử dụng 16 bit
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER = temp;
	temp = 0;

	//5. Configure the alt functionality mode (RM407 -> 8.4.9 and 8.4.10)
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//Chế độ này đc thực thi khi và chỉ khi bit MODERx đc thiết lập GPIO_MODE_ALTFN
		//Trong thanh ghi AFRx thì mỗi 4 bit quản lý 1 AFx

		uint32_t temp1,temp2;

		//Xác định thanh ghi cần sử dụng là GPIOx_AFRH[1] hay GPIOx_AFRL[0] dựa vào số chân Pin_Number
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;

		//Xác định vị trí cụm 4 bit cần sử dụng ở vị trí nào trong thanh ghi dựa vào số chân Pin_Number
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;

		//Xác định value AFx cần điều khiển dựa vào số chân Pin_Number và lưu vào biến temp
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));

		//Clearing bit field of chip register - 0xF=1111(sử dụng 4bit cùng lúc)
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << (4 * temp2));	//đảo xxxx->0000 tại các bit bắt đầu từ 4*temp2

		//Thiết lập cấu hình AFRx vào trong thanh ghi của chip điều khiển
		pGPIOHandle->pGPIOx->AFR[temp1] |= temp;

		//Lưu ý: Mỗi chân Pin thuộc GPIOx chỉ có(hoặc không) mặc định 1 chức năng khác khi thiết lập ALT
		//Nghĩa là khi thiết lập Pin là ALT thì chương trình cũng sẽ tự động chuyển từ GPIOx sang ngoại vi khác
		//Vd: chân PA1(pin1) có ALT mode là ngoại vi TIM2_CH2. Xem thêm: Datasheet_stm32f407x -> Table 9
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
		GPIOA_REG_RESET();
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
 * @Note 		- Đọc giá trị tại một pin input
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;			//biến lưu value đọc đc từ pin
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;

	/* (pGPIOx->IDR >> PinNumber): Dịch chuyển giá trị IDR sang phải của của vị trí PinNumber đầu vào,
	 * và căn chỉnh giá trị đầu vào của chân mong muốn về bit ngoài cùng bên phải (bit thứ 0).
	 * & 0x00000001 : thao tác AND theo bit này che giấu tất cả ngoại trừ bit ngoài cùng bên phải(bit thứ 0)
	 * Mục đích: chỉ trích xuất giá trị đầu vào của mã pin đc chỉ định (các giá trị từ pin khác ko đc đọc)
	 */
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
 * @Note 		- Đc sử dụng khi cần đọc cùng lúc nhiều input trên cùng một port
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;			//biến 16bit lưu value đọc đc từ portX
	value = pGPIOx->IDR;
	return value;

	//Chú ý: Sau khi có giá trị InputPort thì ta sẽ dùng các pp xử lý bit để lấy data input tại các pin mong muốn
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
 * @Note 		- pGPIOx: đã đc cấu hình trước đó bởi hàm GPIO_Init()
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	if(value == GPIO_PIN_SET)
	{
		//Set tại vị trí bit thanh ghi ODR lên 1 tương ứng với chân pin number
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else{
		//Set tại vị trí bit thanh ghi ODR về 0 tương ứng với chân pin number
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
	pGPIOx->ODR = value;		//Giá trị value là một giá trị có 16 bit
}

/*
 * 3_5APIs Toggle(chuyển đổi) Status Pin
 * pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển
 * PinNumber: chân Pin muốn điều khiển(đọc,viết) trạng thái
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
	pGPIOx->ODR ^= (1 << PinNumber);

	/* Cách viết đày đủ: pGPIOx->ODR = pGPIOx->ODR ^ (1 << PinNumber);
	 * Nghĩa là: Đảo trạng thái bit (toán tử XOR) tại vị trí PinNumber của pGPIOx->ODR
	 * '^':Hoạt động XOR đảo ngược bit ở pin đc chỉ định, chuyển đổi trạng thái đầu ra của nó
	 * sau đó sẽ gán value trỏ vào thanh ghi pGPIOx->ODR
	 */
}

/*
 * 4_IRQ Configuration and ISR Hangling
 */
/*
 * 4_1APIs Interrupt config(cấu hình) - Yêu cầu ngắt
 * IRQNumber: ngắt số mấy, IRQPriority: mức ưu tiên, EnorDi: Enable hoặc Disable
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
	if(EnorDi == ENABLE)		//Cho phép kích hoạt interrupt
	{
		if(IRQNumber <= 31)
		{
			//Set ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//Set ISER1 register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);		//Vd: IRQNumber=33 % 32 = 1 -> Set bit1 của ISER1 lên 1
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//Set ISER2 register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);		//Vd: IRQNumber=65 % 64 = 1 -> Set bit1 của ISER2 lên 1
		}

		/* Giải thích sao chỉ cấu hình cho NVIC_ISER0,NVIC_ISER1,NVIC_ISER2:
		 * Nguyên tắc là mỗi bit thuộc NVIC_ISERx sẽ quản lý một Position NVIC(IRQNumber)
		 * Theo datasheet(RM407->Table61) thì STM32F407x thì NVIC chỉ có 81 Position
		 * => chỉ cần sử dụng số bit đến thanh ghi NVIC_ISER2(32*3=96) là có thể cấu hình NVIC
		 */
	}
	else
	{	//Vô hiệu hóa Interrupt
		if(IRQNumber <= 31)
		{
			//Set ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//Set ICER1 register
			*NVIC_ICER1 |= (1 << IRQNumber % 32);		//Vd: IRQNumber=33 % 32 = 1 -> Set bit1 của ICER1 lên 1
		}
		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//Set ICER2 register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);		//Vd: IRQNumber=65 % 64 = 1 -> Set bit1 của ICER2 lên 1
		}
	}
}

/*
 * 4_2 APIs Interrupt Priority(ưu tiên) - cấu hình mức ưu tiên cho ngắt
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
 * @Note 		- Cấu hình mức ưu tiên cho mỗi ngắt
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;			//Xác định sử dụng thanh ghi nào
	uint8_t iprx_section = IRQNumber % 4;	//Xác định phần 8bit nào đc sử dụng(mỗi thanh ghi có 4 phần 8 bit)

	//2. Mỗi phần 8bit(PRI_x) thì chỉ lấy 4bit cao làm priority, 4bit thấp ko sử dụng
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + (iprx * 4)) |= (IRQPriority << shift_amount);
	//iprx * 4 : địa chỉ thanh ghi iprx tiếp theo phải + 4 do mỗi thanh ghi iprx đều 32 bit

	/* Giải thích: Theo datasheet CM4->Cortex M4 peripheral -> 4.2.7
	 * Ta sẽ có các thanh ghi từ IPR0 đến IPR59. Mỗi thanh ghi quản lý 4 mức ưu tiên(mỗi mức chiếm 8 bit)
	 * Mỗi ngắt có một mức ưu tiên: Mỗi ngắt được gán một giá trị ưu tiên cụ thể trong thanh ghi NVIC_IPRx.
	 * Giá trị ưu tiên: Giá trị ưu tiên càng nhỏ thì mức ưu tiên càng cao. Nghĩa là phần PRI0[7:0] sẽ lớn hơn phần PRI1[15:8]
	 * trong thanh ghi IPR0 và sẽ lớn hơn phần bit PRI_236[7:0] của thanh ghi IPR59
	 * So sánh ưu tiên: Khi có nhiều ngắt xảy ra cùng lúc, NVIC sẽ so sánh giá trị ưu tiên của các ngắt đó và chọn ngắt có giá trị
	 * ưu tiên nhỏ nhất (cao nhất) để xử lý trước.
	 * Xử lý ngắt: Sau khi xử lý xong ngắt có mức ưu tiên cao nhất, NVIC sẽ tiếp tục xử lý các ngắt còn lại theo thứ tự ưu tiên giảm dần.
	 *
	 * Lưu ý: mức ưu tiên đã được quy định theo bảng trong datasheet RM407->12->Table61
	 * Tuy nhiên ta có thể dùng hàm này để set mức ưu tiên của interrupt
	 */
}

/*
 * 4_3 APIs Interrupt Hangling(xử lý) - thực hiện công việc khi ngắt xảy ra
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
	//Clear the exti PR register corresponding(tương ứng) to the pin number
	if(EXTI->PR & (1 << PinNumber))		//kiểm tra bit có đc set 1 hay không
	{
		EXTI->PR |= (1 << PinNumber); 		//Clear bit trong PR(Pending Register EXTI)
	}

	//Giải thích: Khi ngắt đc phát hiện thì bit tương ứng trong thanh ghi PR(Peding Register)thuộc EXTIx sẽ đc tự động set 1
	//Xóa bit đang chờ ở thanh ghi PR(Pending Register) của NVIC, và ngắt sẽ đc đăng ký với Processor
	//Theo RM407->12.3.6: bit đc xóa khi đc set 1(This bit is cleared by programming it to ‘1’)
	//=>Cần nghiên cứu kỹ datasheet vì ko phải thanh ghi nào cũng set=1(hoạt động) và clear=0(dừng)
}


