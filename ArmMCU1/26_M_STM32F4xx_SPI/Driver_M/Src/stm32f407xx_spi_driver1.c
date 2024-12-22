/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: Dec 1, 2024
 *      Author: Minh
 *
 *  Lưu ý: Trạng thái Enable(1) và Disable(0) sẽ phụ thuộc vào datasheet để quyết định cấu hình bit.
 *  VD: có trường hợp bit là Disable(0) nhưng là cấu hình cho phép chức năng hoạt động
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
void SPI_PeriClockControl(SPI_Regdef_t *pSPIx, uint8_t EnorDi)
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
//		else if(pSPIx == SPI4)	//Không cấu hình
//		{
//			SPI4_PCLK_EN();
//		}
	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();		//hàm đã đc define trong stm32f407xx.h(lớp Driver)
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
	}
}

/*********************************************************************************
 * @fn			- SPI_Init
 *
 * @brief		- APIs SPI Peripheral initialization(Khởi tạo)
 *
 * @param[in]	- pSPIHandle: con trỏ là SPIx muốn xử lý công việc
 *
 * @return		- none
 *
 * @Note 		- none
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//Khai báo biến tạm lưu giá trị bit[31:0] để cấu hình thanh ghi CR1 của SPIx
	//Ta sẽ dựa vào thông số SPIConfig do người dùng cài đặt để thiết lập giá trị cho biến
	uint32_t tempreg = 0;

	//0.Enable Clock for Peripheral SPIx - Kích hoạt xung clock cho SPIx
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//1.Configure(cấu hình) device mode(Master or Slave), bit MSTR[bit2]
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;	//STM32 board là Master, bit MSTR = 0(tempreg=0)

	//2.Configure the bus config, cấu hình chuẩn bus giao tiếp, BIDIMODE[bit15] và RXONLY[bit10]
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);						//Full-Duplex
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDIMODE[bit15] should be set
		tempreg |= (1 << SPI_CR1_BIDIMODE);							//Half-Duplex

	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDIMODE[bit15] should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDIMODE);
																	//Simplex
		//RX_ONLY[bit10] must be set
		tempreg |= (1 << SPI_CR1_RXONLY);
	}

	//3.Configure the SPI serial clock (baud rate), BR[2:0], mặc định là bit[2:0] = 0 = Div2
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;	//Dịch tới vị trí bit và set bit theo value của SPI_SclkSpeed(Config)

	//4.Configure the DFF, DFF[11]
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;		//Dịch tới vị trí bit và set bit theo value của SPI_DFF(Config)

	//5.Configure the CPOL, CPOL[1]
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;		//Dịch tới vị trí bit và set bit theo value của SPI_CPOL(Config)

	//6.Configure the CPHA, CPHA[1]
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;		//Dịch tới vị trí bit và set bit theo value của SPI_CPHA(Config)

	//Sau bước 6, bạn sẽ có cấu hình (theo cấu hình value Config) thanh ghi CR1 là value biến tempreg

	//7. Lưu thông tin đã cấu hình biến tempreg vào CR1 của pSPIx muốn triển khai
	pSPIHandle->pSPIx->CR1 = tempreg;
}

/*********************************************************************************
 * @fn			- SPI_DeInit
 *
 * @brief		- Reset base address SPI Peripheral
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển cấu hình
 *
 * @return		- none
 *
 * @Note 		- Ngắt xung clock cấp cho SPIx
 */
void SPI_DeInit(SPI_Regdef_t *pSPIx)
{
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
}

/*********************************************************************************
 * @fn			- SPI_PeripheralControl
 *
 * @brief		- Cho phép SPIx có đc hoạt động hay không, bit SPE[6] của SPI_CR1
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển cấu hình
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấu hình
 *
 * @return		- none
 *
 * @Note 		- SPE: SPI enable - Cấp xung clock cho SPI Peripheral
 */
void SPI_PeripheralControl(SPI_Regdef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);				//1= Enable SPI
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);				//0= Disable SPI
	}
}

/*********************************************************************************
 * @fn			- SPI_SSMConfig
 *
 * @brief		- Thiết lập STM32 là Slave hay không, bit SSI[8] của SPI_CR1
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển cấu hình
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấu hình
 *
 * @return		- none
 *
 * @Note 		-  Software slave management (Trường hợp STM32 là Slave)
 */
void SPI_SSMConfig(SPI_Regdef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSM); 		//1: Software slave management enabled
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSM); 		//0: Software slave management disabled
	}
}

/*********************************************************************************
 * @fn			- SPI_SSIConfig
 *
 * @brief		- Thiết lập cấu hình pin NSS , bit SSI[8] của SPI_CR1
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn điều khiển cấu hình
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấu hình
 *
 * @return		- none
 *
 * @Note 		- Internal slave select (Chỉ hoạt động khi bit SSM=1)
 */
void SPI_SSIConfig(SPI_Regdef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI); 		//Enable NSS->VDD(High) -> Không cho phép kết nối Slave(STM32) với Master
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI); 		//Enable NSS->GND(Low) -> Cho phép kết nối Slave(STM32) với Master
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
 * @Note 		- SS output enable (Trường hợp STM32 là Master)
 */
void SPI_SSOEConfig(SPI_Regdef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE); 			//Enable SSOE=1(điều kiện) -> NSS=Low
	}
	else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE); 		//Disable SSOE=0(điều kiện) -> NSS=High
	}
}

/*********************************************************************************
 * @fn			- SPI_GetFlagStatus
 *
 * @brief		- Kiểm tra cờ bit TXE[1](1=empty ; 0=not empty) của thanh ghi SPI_SR
 *
 * @param[in]	- pSPIx: con trỏ là địa chỉ SPIx muốn kiểm tra bit
 * @param[in]	- FlagName : trạng thái hiện tại của vị trí flag_bit thuộc SPI_CR muốn kiểm tra
 * 				FlagName: vị trí kiểm tra sẽ đc định nghĩa(define)
 *
 * @return		- Trạng thái đã empty(Set) hay not empty(reset)
 *
 * @Note 		- none
 */
uint8_t SPI_GetFlagStatus(SPI_Regdef_t *pSPIx, uint32_t FlagName)
{
	//Kiểm tra trạng thái bit trong pSPIx->SR có đang đc SET tại vị trí bit theo value biến FlagName hay không
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;			//Trạng thái bit đang đc SET, có sự kiện xảy ra
	}
	return FLAG_RESET;				//Trạng thái bit đang đc RESET, chờ có sự kiện xảy ra hoặc đã xử lý xong
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
 * @Note 		- Chú ý: Nên thiết lập thời gian lập để tránh trạng thái vòng lặp tuần hoàn
 */
void SPI_SendData(SPI_Regdef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	//Vòng lặp kiểm tra cho đến khi gửi hết data đã đưa vào pTxBuffer
	while(Len > 0)
	{
		//1. Wait until TXE[1] of SPI_SR register is empty (0: TxBuffer not empty - 1: TxBuffer empty)
		//while(!(pSPIx->SR & (1 << 1) ));												//C1
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);						//C2

		//2. Check the DFF bit use in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))				//Dịch bit và kiểm tra bit11
		{
			//DFF[11] = 1 -> 16 bit
			//3. Load the data in to the DR(SPI Data Register), đẩy data từ TxBuffer -> DR
			pSPIx->DR = *((uint16_t *)pTxBuffer);		//Ép kiểu uint16 vì param đầu vào chỉ 8 bit
			Len--; Len--;								//Trừ 2 đơn vị độ dài vì đã gửi 2 bit lần
			(uint16_t *)pTxBuffer++;					//Tăng địa chỉ bộ đệm chuẩn bị data gửi tiếp theo
		}
		else
		{
			//DFF[11] = 0 -> 8 bit
			//3. Load the data in to the DR(SPI Data Register), đẩy data từ TxBuffer -> DR
			pSPIx->DR = *pTxBuffer;						//Đẩy value(8 bit) của địa chỉ pTxBuffer vào DR
			Len--; 										//Trừ 1 đơn vị độ dài vì đã gửi 1 bit lần
			pTxBuffer++;								//Tăng địa chỉ bộ đệm chuẩn bị data gửi tiếp theo
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
 * @Note 		- Chú ý: Nên thiết lập thời gian lập để tránh trạng thái vòng lặp tuần hoàn
 */
void SPI_ReceiveData(SPI_Regdef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
		{
		//1. Wait until RXNE[0] of SPI_SR register is empty(1: TxBuffer not empty - 0: TxBuffer empty)
		//while((pSPIx->SR & (0 << 1) ));												//C1
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_SET);						//C2


		//2. Check the DFF bit use in CR1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)) 	//dịch bit đến vị trí 11 và And với thanh ghi CR1 để kiểm tra value DFF bit
		{
			//DFF[11] = 1 -> 16 bit
			//3. Load the data from the DR(SPI Data Register) to RxBuffer, đẩy data từ  DR -> RxBuffer
			*((uint16_t*)pRxBuffer) = pSPIx->DR ;			//Ép kiểu vì param pRxBuffer đầu vào chỉ 8bit
			Len--;	Len--;									//Trừ 2 đơn vị độ dài vì đã nhận 2 bit
			(uint16_t*)pRxBuffer++;							//Tăng địa chỉ bộ đệm chuẩn bị địa chỉ đc nhận tiếp theo
		}
		else
		{
			//DFF[11] = 0 -> 8 bit
			//3. load the data from the DR(SPI Data Register) to RxBuffer, đẩy data từ  DR -> RxBuffer
			*(pRxBuffer) = pSPIx->DR ;						//Đẩy value(8 bit) của địa chỉ pTxBuffer vào DR
			Len--;
			pRxBuffer++;									//Tăng địa chỉ bộ đệm chuẩn bị địa chỉ đc nhận tiếp theo
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

