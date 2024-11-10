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
		//Todo: (Dùng cho việc ngắt xung)
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
	uint32_t temp = 0; 	//temp register

	//1. configure(cấu hình) the mode of gpio pin(Datasheet 8.4.1)
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//giá trị GPIO_PinMode thuộc 1 trong 4 trường hợp bit của Mode Register
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
		//Theo datasheet thì để reset MODERx thì ta cần thiết lập value bit là 00, Ta có: 0x3 = 11(set cùng lúc 2 giá trị bit)
		//~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber): nghĩa là tiến hành đảo trạng thái 11 thành 00
		//vị trí chân Pin_Number

		//Tiến hành cấu hình 2bit(set lên 1) theo giá trị biến temp(đã đc dịch đến vị trí bit mong muốn)
		//vào trong thanh ghi MODER của biến pGPIOHandle
		pGPIOHandle->pGPIOx->MODER |= temp;
		//=> Lúc này ta đã xác định(cấu hình) đc vị trí mode(MODERx) của GPIO tùy thuộc vào pin_number bạn muốn control
	}
	else
	{
		//this is part(phần) will code later (interrupt mode) //các trường hợp khác từ 4->6
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
 * IRQNumber: ngắt số mấy, IRQPriority: mức ưu tiên, EnorDi: Enable hoặc Disable
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi)
{

}

/*
 * 4_2APIs Interrupt Hangling(xử lý) - thực hiện công việc khi ngắt xảy ra
 */
void GPIO_IRQHangling(void)
{

}
