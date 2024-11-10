/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Nov 26, 2023
 *      Author: Minh
 */
#include "stm32f407xx_spi_driver.h"

/*
 * Static function (interrupt support)
 * Vì là hàm static nên ko cần khai báo Prototype
 */
static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle);
static void SPI_RXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle);
static void SPI_OVR_ERR_Interrupt_Handle(SPI_Handle_t *pSPIHandle);

/*********************************************************************************
 * @fn			- SPI_PeriClockControl
 *
 * @brief		- Enable/Disable Clock cho SPI peripheral
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();		//hàm đã đc define trong stm32f407xx.h(lớp Driver)
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
//		else if(pSPIx == SPI4)
//		{
//			SPI4_PCLK_EN();
//		}
	}
	else
	{
		//Todo: (Disable Status)

	}
}


/*********************************************************************************
 * @fn			- SPI_Init
 *
 * @brief		- APIs SPI Peripheral initialization
 *
 * @param[in]	- pSPIHandle: con trỏ là SPIx muốn xử lý công việc
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//First lets configure the SPI_CR1 register, biến tạm đại diện vị trí bit muốn cấu hình CR1 Register
	uint32_t tempreg = 0;

	//0.Enable CLock for Peripheral SPI
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//1.Configure the device mode , MSTR[bit2]
	tempreg |=  pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;  //Chỉ dịch tới vị trí bit ko mà set bit
	//Ban đầu sẽ cấu hình device là Slave và vì tempreg đã bằng 0 nên chỉ cần dịch 2 bit

	//2. Configure the bus config , cấu hình mode theo chuẩn giao tiếp
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDIMODE[bit15] should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDIMODE[bit15] should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDIMODE[bit15] should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);

		//RX_ONLY[bit10] must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	//3. Configure the SPI serial clock (baud rate), BR[bit2:0], mặc định Div2, bit[2:0] = 0
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;  		//Chỉ dịch tới vị trí bit mà ko set bit

	//4. Configure the DFF, DFF[bit11]
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;			//Chỉ dịch tới vị trí bit ko mà set bit

	//5. Configure the CPOL, CPOL[bit1]
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;			//Chỉ dịch tới vị trí bit ko mà set bit

	//6. Configure the CPHA, CPHA[bit0]
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;			//Chỉ dịch tới vị trí bit ko mà set bit

	//Sau bước 6, bạn sẽ có cấu hình tạm(theo ý muốn lập trình viên) của thanh ghi CR1 là value biến tempreg

	//7. Lưu thông tin đã cấu hình biến tempreg vào CR1 của pSPIx muốn triển khai
	pSPIHandle->pSPIx->CR1 = tempreg;
}

/*********************************************************************************
 * @fn			- SPI_DeInit
 *
 * @brief		- Reset base address SPI Peripheral
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	//lập trình giống GPIO De_Init
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
//	else if(pSPIx == SPI4)
//	{
//
//	}
}

/*********************************************************************************
 * @fn			- SPI_GetFlagStatus
 *
 * @brief		- Kiểm tra cờ bit TXE[1](1=empty ; 0=not empty) của thanh ghi SPI_SR
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn kiểm tra bit
 * @param[in]	- FlagName : trạng thái hiện tại của flag_bit thuộc SPI_CR muốn kiểm tra
 * 				FlagName: vị trí kiểm tra sẽ đc định nghĩa(define)
 *
 * @return		- Trạng thái đã empty(Set) hay not empty(reset)
 *
 * @Note 		- none
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)  //Kiểm tra value của pSPIx->SR and với FlagName
	{
		return FLAG_SET; //TH1:nếu kết quả là 1 thì TXE[1]==1 -> trả về SET, thoát vòng lặp
						 //TH2: kiểm tra BSY có đang busy truyền hay không
	}
	return FLAG_RESET;		//nếu kết quả là 0 thì tiếp tục vòng lặp
}

/*********************************************************************************
 * @fn			- SPI_SendData
 *
 * @brief		- Send Data use SPI peripheral
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]	- pTxBuffer : Nội dung data muốn truyền, thường là mảng ký tự
 * @param[in]	- Len : độ dài Data
 *
 * @return		- none
 *
 * @Note 		- This is Blocking Call
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait until TXE[1] of SPI_SR register is set
		//while(! (pSPIx->SR & (1 << 1) ));												//C1
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);					//C2


		//2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)) //dịch bit đến vị trí 11 và And với thanh ghi CR1 để kiểm tra value DFF bit
		{
			//16 bit DFF
			//3. load the data in to the DR(SPI Data Register), đẩy data từ TxBuffer -> DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);	//ép kiểu vì param pTxBuffer đầu vào chỉ 8bit
			Len--;									//Trừ 2 đơn vị độ dài vì đã gửi 2 bit
			Len--;
			(uint16_t*)pTxBuffer++;			//tăng địa chỉ bộ đệm chuẩn bị lần gửi tiếp theo
		}
		else
		{
			//8 bit DFF
			//3. load the data in to the DR(SPI Data Register), đẩy data từ TxBuffer -> DR
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;					//tăng địa chỉ bộ đệm chuẩn bị lần gửi tiếp theo
		}
	}
}

/*********************************************************************************
 * @fn			- SPI_ReceiveData
 *
 * @brief		- Receive Data use SPI peripheral
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]	- pRxBuffer : biến sẽ lưu dữ liệu nhận đc
 * @param[in]	- Len : độ dài Data
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
	while(Len > 0)
	{
		//1. Wait until RXNE[0] of SPI_SR register is set
		//while(! (pSPIx->SR & (1 << 1) ));												//C1
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);					//C2


		//2. Check the DFF bit in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)) //dịch bit đến vị trí 11 và And với thanh ghi CR1 để kiểm tra value DFF bit
		{
			//16 bit DFF
			//3. load the data from the DR(SPI Data Register) to RxBuffer, đẩy data từ  DR -> RxBuffer
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;	//ép kiểu vì param pRxBuffer đầu vào chỉ 8bit
			Len--;									//Trừ 2 đơn vị độ dài vì đã nhận 2 bit
			Len--;
			(uint16_t*)pRxBuffer++;			//tăng địa chỉ bộ đệm chuẩn bị lần gửi tiếp theo
		}
		else
		{
			//8 bit DFF
			//3. load the data from the DR(SPI Data Register) to RxBuffer, đẩy data từ  DR -> RxBuffer
			*(pRxBuffer) = pSPIx->DR ;
			Len--;
			pRxBuffer++;					//tăng địa chỉ bộ đệm chuẩn bị lần gửi tiếp theo
		}
	}
}

/*********************************************************************************
 * @fn			- SPI_IRQInterruptConfig
 *
 * @brief		- Config(Cấu hình) Interrupt Status for SPIx
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc sử dụng ngắt
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

/*********************************************************************************
 * @fn			- SPI_IRQPriorityConfig
 *
 * @brief		- Set Priority(ưu tiên) Interrupt Status for SPIx
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- IRQPriority: mức ưu tiên của ngắt mong muốn
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	//APIs Interrupt Priority
	//1. first lets find out the ipr register
	uint8_t iprx = IRQNumber / 4;			//Xác định sử dụng thanh ghi nào
	uint8_t iprx_section = IRQNumber % 4;	//Xác định phần 8bit nào đc sử dụng(mỗi thanh ghi có 4 phần 8 bit)

	//Mỗi phần 8bit thì chỉ lấy 4bit cao làm priority, 4bit thấp ko sử dụng
	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	//*(NVIC_PR_BASEADDR + (iprx * 4 ) |= (IRQPriority << shift_amount);	//lỗi
	//lỗi công thức xác định địa chỉ iprx chưa chính xác

	*(NVIC_PR_BASEADDR + iprx ) |= (IRQPriority << shift_amount);
	//iprx : địa chỉ thanh ghi iprx

	//Vd: IRQNumber =  23(IRQ_NO_EXTI9_5)-> 23 / 4 = 5 -> thanh ghi IPR5
	//									 -> 23 % 4 = 3 -> phần section thứ tư[3:0]
}

/*********************************************************************************
 * @fn			- SPI_IRQHangling
 *
 * @brief		- Hangling(thực hiện) Interrupt Status of SPIx
 *
 * @param[in]	- pHandle: SPIx đã cài đặt trạng thái ngắt
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_IRQHangling(SPI_Handle_t *pHandle)
{
	uint8_t temp1, temp2;

	//First lets check for TXE, kiểm tra App có đang trong trạng thái truyền hay không
	//Nếu đang là 0 thì & vs 1 sẽ là 0, nếu đang là 1 thì & 1 là 1
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)		//nếu cả hai đều là 1 thì cho phép thực hiện ngắt SPI TXE
	{
		//Handle TXE
		SPI_TXE_Interrupt_Handle(pHandle);		//hàm thực hiện ngắt SPI TXE
	}

	//Second Lets check RXNE
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2)		//nếu cả hai đều là 1 thì cho phép thực hiện ngắt SPI RXE
	{
		//Handle RXE
		SPI_RXE_Interrupt_Handle(pHandle);		//hàm thực hiện ngắt SPI RXE
	}

	//Third Lets check OVR Error
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)		//nếu cả hai đều là 1 thì cho phép thực hiện xử lý lỗi OVR
	{
		//Handle RXE
		SPI_OVR_ERR_Interrupt_Handle(pHandle);		//hàm thực hiện xử lý lỗi OVR
	}
}

/*********************************************************************************
 * @fn			- SPI_TXE_Interrupt_Handle
 *
 * @brief		- Hàm thực hiện ngắt SPI TXE
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 *
 * @return		- none
 *
 * @Note 		- none
 */
static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	//Check the DFF bit in CR1
	if((pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF) ) )
	{
		//16 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}else
	{
		//8 bit DFF
		//1. load the data in to the DR
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	//Check Len=0?
	if(! pSPIHandle->TxLen)
	{
		//TxLen is zero, so close the spi communication and inform the application that TX is over

		//This presents interrupt from setting up of TXR flag- Điều này thể hiện sự gián đoạn khi thiết lập cờ TXR
//		pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
//		pSPIHandle->pTxBuffer = NULL;		//Làm sạch TxBuffer(ko lưu bất cứ data nào)
//		pSPIHandle->TxLen = 0;				//Ko có data
//		pSPIHandle->TxState = SPI_READY;	//Trạng thái sẵn sàng cho lần truyền kế tiếp
		SPI_CloseTransmisson(pSPIHandle);

		//Thông báo cho App đang thực hiện sự kiện ngắt SPI TXE
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);

	}

}

/*********************************************************************************
 * @fn			- SPI_RXE_Interrupt_Handle
 *
 * @brief		- Hàm thực hiện ngắt SPI RXE
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 *
 * @return		- none
 *
 * @Note 		- none
 */
static void SPI_RXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	//do rxing as per the dff
	if(pSPIHandle->pSPIx->CR1 & ( 1 << 11))
	{
		//16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;
		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;

	}else
	{
		//8 bit
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	//Check Len=0?
	if(! pSPIHandle->RxLen)
	{
		//reception is complete
//		pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_RXNEIE);
//		pSPIHandle->pRxBuffer = NULL;
//		pSPIHandle->RxLen = 0;
//		pSPIHandle->RxState = SPI_READY;
		SPI_CloseReception(pSPIHandle);

		//Thông báo cho App đang thực hiện sự kiện ngắt SPI RXE
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}

/*********************************************************************************
 * @fn			- SPI_OVR_ERR_Interrupt_Handle
 *
 * @brief		- Hàm thực hiện việc xử lý nếu có lỗi OVR
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 *
 * @return		- none
 *
 * @Note 		- none
 */
static void SPI_OVR_ERR_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	//uint8_t temp;
	//1. clear the ovr flag
//	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
//	{
//		//Lấy thông tin value của các thanh ghi
//		temp = pSPIHandle->pSPIx->DR;
//		temp = pSPIHandle->pSPIx->SR;
//	}
//	(void)temp;
	SPI_ClearOVRFlag(pSPIHandle->pSPIx);

	//2. inform(thông báo) the application, đang xử lý lỗi OVR xảy ra
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}

/*********************************************************************************
 * @fn			- SPI_ApplicationEventCallback
 *
 * @brief		- Gửi dòng thông báo đến App sự kiện xảy ra là gì
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 * @param[in]	- AppEv : Sự kiện mong muốn đc thực hiện
 *
 * @return		- none
 *
 * @Note 		- none
 */
__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{
	//Đây là cách triển khai weak. ứng dụng người dùng có thể ghi đè chức năng này.
}

/*********************************************************************************
 * @fn			- SPI_CloseTransmisson
 *
 * @brief		- Đóng công việc interupt truyền, xóa các cở trên thanh ghi
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

/*********************************************************************************
 * @fn			- SPI_CloseReception
 *
 * @brief		- Đóng công việc interupt nhận, xóa các cở trên thanh ghi
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

/*********************************************************************************
 * @fn			- SPI_ClearOVRFlag
 *
 * @brief		- Xóa cờ bit OVR trên thanh ghi -> cho phép TX-RX tiếp theo hoạt động
 *
 * @param[in]	- SPI_RegDef_t : Handle structure for SPIx register(support to Interrupt)
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

/*********************************************************************************
 * @fn			- SPI_SendData_IT
 *
 * @brief		- Send Data when use Interrupt
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 * @param[in]	- pTxBuffer : Nội dung data muốn truyền, thường là mảng ký tự
 * @param[in]	- Len : độ dài Data
 *
 * @return		- none
 *
 * @Note 		- none
 */
uint8_t SPI_SendData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	//Kiểm tra App có đang trong trnạg thái nhận data hay không, nếu có thì KHÔNG thực hiện hàm
	if(state != SPI_BUSY_IN_RX)
	{
		//1. Save the Tx Buffer address and Len information in some global variables
		//1. Lưu địa chỉ bộ đệm Tx và thông tin Len trong một số biến toàn cục
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		//2. Mark the SPI state as busy in tranmission so that
		//no other code can take over same SPI peripheral until transmission is over
		//2. Đánh dấu trạng thái SPI là đang bận truyền tải(busy) để không có mã nào khác có thể
		//tiếp quản cùng một thiết bị ngoại vi SPI cho đến khi quá trình truyền kết thúc
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		//3. Cho phép bit điều khiển TXEIE nhận ngắt bất cứ khi nào cờ TXE được đặt trong SR
		//Datasheet: RM407-> 28.5.2 SPI_CR2 -> TXEIE[7]
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

	}

	//4. Data Transmission will be handled by the ISR code(will implement later)
	//4. Việc truyền dữ liệu sẽ được xử lý bằng mã ISR (sẽ triển khai sau)


	return state;
}

/*********************************************************************************
 * @fn			- SPI_ReceiveData_IT
 *
 * @brief		- Receive Data when use Interrupt
 *
 * @param[in]	- pSPIHandle : Handle structure for SPIx peripheral(support to Interrupt)
 * @param[in]	- pRxBuffer :  Biến chứa nội dung nhận đc
 * @param[in]	- Len : độ dài Data
 *
 * @return		- none
 *
 * @Note 		- none
 */
uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHangle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHangle->RxState;

	//Kiểm tra App có đang trong trnạg thái truyền data hay không, nếu có thì KHÔNG thực hiện hàm
	if(state != SPI_BUSY_IN_TX)
	{
		//1. Save the Tx Buffer address and Len information in some global variables
		//1. Lưu địa chỉ bộ đệm Tx và thông tin Len trong một số biến toàn cục
		pSPIHangle->pRxBuffer = pRxBuffer;
		pSPIHangle->RxLen = Len;

		//2. Mark the SPI state as busy in tranmission so that
		//no other code can take over same SPI peripheral until receve is over
		//2. Đánh dấu trạng thái SPI là đang bận truyền tải(busy) để không có mã nào khác có thể
		//tiếp quản cùng một thiết bị ngoại vi SPI cho đến khi quá trình nhận kết thúc
		pSPIHangle->RxState = SPI_BUSY_IN_RX;

		//3. Enable the RXNEIE control bit to get interrupt whenever RXE flag is set in SR
		//3. Cho phép bit điều khiển RXNEIE nhận ngắt bất cứ khi nào cờ RXE được đặt trong SR
		//Datasheet: RM407-> 28.5.2 SPI_CR2 -> RXNEIE[6]
		pSPIHangle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

	}

	//4. Data Transmission will be handled by the ISR code(will implement later)
	//4. Việc truyền dữ liệu sẽ được xử lý bằng mã ISR (sẽ triển khai sau)


	return state;
}

/*********************************************************************************
 * @fn			- SPI_PeripheralControl
 *
 * @brief		- Cho phép SPIx có đc hoạt động hay không, bit SPE[6] của SPI_CR1
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE); 		//1= Enable SPI
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE); 		//0= Disable SPI
	}
}

/*********************************************************************************
 * @fn			- SPI_SSIConfig
 *
 * @brief		- Tín hiệu NSS pin ở mức cao, bit SSI[8] của SPI_CR1
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI); 		//Enable NSS->VDD
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI); 		//Disable NSS->VDD
	}
}

/*********************************************************************************
 * @fn			- SPI_SSOEConfig
 *
 * @brief		- Cấu hình bit SSOE để cho phép Tín hiệu NSS pin ở mức low khi truyền data
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE); 		//Enable SSOE=1(điều kiện) -> NSS=Low
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE); 		//Disable SSOE=0(điều kiện) -> NSS=High
	}
}
