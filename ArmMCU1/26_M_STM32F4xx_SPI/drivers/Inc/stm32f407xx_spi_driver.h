/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Minh
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"	//Thư viện lớp Driver của vđk

/*
 * Configuration structure for SPIx peripheral
 * Các thành phần biến cần thiết cho việc cấu hình sử dụng SPI Peripheral
 * Các thành phần biến này thực chất là các bit của các thanh ghi SPI Peripheral sẽ đc sử dụng để cấu hình SPI
 * Các thanh ghi SPI Peripheral đã đc định nghĩa "SPI_RegDef_t" trong file stm32f407xx.h
 */
typedef struct
{
	uint8_t SPI_DeviceMode;		/*Master or Slave*/
	uint8_t SPI_BusConfig;		/*Full-duplex,Haft-duplex,Simplex*/
	uint8_t SPI_SclkSpeed;		/*f_PCLK -> DIVx*/
	uint8_t SPI_DFF;			/*8bit or 16bit*/
	uint8_t SPI_CPOL;			/*Xung cạnh lên or Xung cạnh xuống*/
	uint8_t SPI_CPHA;			/*Phần Chu kỳ nửa đầu or Phần chu kỳ nửa sau*/
	uint8_t SPI_SSM;			/*Slave Hardware or Slave Software*/
}SPI_Config_t;

/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
	SPI_RegDef_t 	*pSPIx; 		/* This holds the base address of SPIx(x:0,1,2) peripheral */
	SPI_Config_t	SPIConfig;
}SPI_Handle_t;

/****************************************************************************************
 *		Definition For more information about the SPI_Config_t of variable 				*
 ****************************************************************************************/
/*
 * @SPI_DeviceMode
 * Định nghĩa thiết bị là Master hay Slave
 * Dựa vào Datasheet phần "Bit 2 MSTR: Master selection" của thanh ghi CR1 of SPI_RegDef_t
 */
#define SPI_DEVICE_MODE_MASTER		1
#define SPI_DEVICE_MODE_SLAVE		0

/*
 * @SPI_BusConfig
 * Định nghĩa lựa chọn chuẩn giao tiếp muốn dùng, là full-duplex(FD), half-duplex(HD), simplex(S)
 * Dựa vào Datasheet phần "Bit 15 BIDIMODE" và "Bit 14 BIDIOE" và "Bit 10 RXONLY" của thanh ghi CR1 of SPI_RegDef_t
 *
 * KO sử dụng SPI_BUS_CONFIG_SIMPLEX_TXONLY vì giống với trạng thái Half-duplex với TX Only(bit14=1)
 */
#define SPI_BUS_CONFIG_FD					1		/*Full-duplex		-> bit15 = 0, bit10=0				*/
#define SPI_BUS_CONFIG_HD					2		/*Half-duplex		-> bit15 = 1, bit14=0 or bit14=1	*/
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY		3		/*Simplex-RX_Only	-> bit15 = 1, bit14=0, bit10=1		*/
//#define SPI_BUS_CONFIG_SIMPLEX_TXONLY		3		/*Simplex-TX_Only	-> bit15 = 1, bit14=1				*/
/*
 * @SPI_SclkSpeed
 * Định nghĩa bộ chia cho SPI -> Tốc độ f_PCLK cho SPI
 * Dựa vào Datasheet phần "Bits 5:3 BR[2:0]: Baud rate control" của thanh ghi CR1 of SPI_RegDef_t
 */
#define SPI_SCLK_SPEED_DIV2			0		/*Mặc định ban đầu sử dụng bộ chia 2*/
#define SPI_SCLK_SPEED_DIV4			1
#define SPI_SCLK_SPEED_DIV8			2
#define SPI_SCLK_SPEED_DIV16		3
#define SPI_SCLK_SPEED_DIV32		4
#define SPI_SCLK_SPEED_DIV64		5
#define SPI_SCLK_SPEED_DIV128		6
#define SPI_SCLK_SPEED_DIV256		7

/*
 * @SPI_DFF
 * Định nghĩa Data frame format, định dạng data mong muốn 8bit hay 16bit
 * Dựa vào Datasheet phần "Bit 11 DFF" của thanh ghi CR1 of SPI_RegDef_t
 */
#define SPI_DFF_8BIT				0
#define SPI_DFF_16BIT 				1

/*
 * @SPI_CPOL
 * Định nghĩa lúc cạnh(lên_high hay xuống_low) xung clock sử dụng để lấy mẫu
 * Dựa vào Datasheet phần "Bit1 CPOL" của thanh ghi CR1 of SPI_RegDef_t
 *
 */
#define SPI_CPOL_HIGH				1
#define SPI_CPOL_LOW				0

/*
 * @SPI_CPHA
 * Định nghĩa phần chu kỳ sử dụng clock để lấy mẫu(đầu_low, sau_high)
 * Dựa vào Datasheet phần "Bit 0 CPHA" của thanh ghi CR1 of SPI_RegDef_t
 */
#define SPI_CPHA_HIGH				1
#define SPI_CPHA_LOW				0

/*
 * @SPI_SSM
 * Định nghĩa dạng Slave là có sử dụng Software Slave hay không (SSM_DS -> Hardware Slave)
 * Dựa vào Datasheet phần "Bit 9 SSM" của thanh ghi CR1 of SPI_RegDef_t
 */
#define SPI_SSM_EN					1
#define SPI_SSM_DS					0	/*Mặc định ban đầu tắt sử dụng Software Slave*/

/*
 * SPI related status flags definition
 * Thiết lập các định nghĩa khi các cờ trạng thái muốn sử dụng đc set thuộc thanh ghi SR
 * Mục đích: sử dụng công thức mặt nạ bit kiểm tra value bit tại đó có đang đc set 1 hay không
 */
#define SPI_TXE_FLAG				(1 << SPI_SR_TXE)		//Dịch 1 đến vị trí bit SPI_SR_TXE of SPI_SR TXE[1]
#define SPI_RXNE_FLAG				(1 << SPI_SR_RXNE)		//Dịch 1 đến vị trí bit SPI_SR_TXE of SPI_SR RXNE[0]

//Kiểm tra BSY có đang busy SPI truyền hay ko
#define SPI_BUSY_FLAG				(1 << SPI_SR_BSY)		//Dịch 1 đến vị trí bit SPI_SR_TXE of SPI_SR BSY[7]

/****************************************************************************************
 *							APIs supported by this driver								*
 *			For more information about the APIs check the function definition			*
 ****************************************************************************************/
/*
 * Peripheral Clock Setup for SPI
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);				//APIs Peripheral Clock

/*
 * Init and De-Init
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);									//APIs SPI initialization
void SPI_DeInit(SPI_RegDef_t *pSPIx);										//APIs Reset SPI

/*
 * Data Send and Receive for SPI
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR Handling for SPI
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);				//APIs Interrupt config(cấu hình) - Yêu cầu ngắt
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);		//APIs Interrupt Priority
void SPI_IRQHangling(SPI_Handle_t *pHandle);				//APIs Interrupt Hangling(xử lý) - thực hiện công việc khi ngắt xảy ra

/*
 * Other Peripheral Control APIs for SPI
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);			//Enable or Disable SPI Peripheral hoạt động
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);					//Cấu hình bit SSI để nối NSS lên VDD
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);					//Cấu hình bit SSOE để NSS sẽ ở mức Low
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);			//Kiểm tra flag BSY có đang busy truyền ko

#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
