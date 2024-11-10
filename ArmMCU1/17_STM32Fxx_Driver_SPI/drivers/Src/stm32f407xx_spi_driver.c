/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Nov 26, 2023
 *      Author: Minh
 */
#include "stm32f407xx_spi_driver.h"

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
