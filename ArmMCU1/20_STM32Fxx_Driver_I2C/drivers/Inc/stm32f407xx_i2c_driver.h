/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Minh
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"	//Thư viện lớp Driver của vđk

/**
 * Configuration structure for I2Cx peripheral
 * Thông số do người dùng cấu hình input khi khởi tạo I2C peripheral
 */
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t  I2C_DeviceAddress;
	uint8_t  I2C_AckControl;
	uint8_t  I2C_FmDutyCycle;
}I2C_Config_t;

/**
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t *pI2Cx;		//Cấu hình thanh ghi để processor sử dụng xử lý chương trình
	I2C_Config_t I2C_Config;	//Cấu hình ban đầu do user đưa vào khi khởi tạo ban đầu
}I2C_Handle_t;

/**
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM		100000			/* Standard Mode 100KHz */
#define I2C_SCL_SPEED_FM2K		200000			/* Fast Mode 200KHz */
#define I2C_SCL_SPEED_FM4K		400000			/* Fast Mode 400KHz */

/**
 * @I2C_AckControl (tham khảo thanh ghi I2C_CR1(27.6.1) bit 10 ACK )
 */
#define I2C_ACK_ENABLE			1
#define I2C_ACK_DISABLE			0				/* Mặc định biến sẽ đc tắt */

/**
 * @I2C_FmDutyCycle (tham khảo thanh ghi I2C_CCR(27.6.8) bit 14 DUTY)
 */
#define I2C_FM_DUTY_2			0
#define I2C_FM_DUTY_16_9		1

/**
 * @STOP Condition use or not use
 */
#define I2C_DISABLE_SR		RESET
#define I2C_ENABLE_SR		SET

/*
 * I2C related status flags definition
 * Thiết lập các định nghĩa khi các trạng thái của cờ_flag trong thanh ghi SR(Status register)
 * Mục đích: sử dụng công thức mặt nạ bit kiểm tra value bit tại đó có đang đc set 1 hay không
 */
#define I2C_FLAG_SB				(1 << I2C_SR1_SB)		//Dịch 1 đến vị trí bit I2C_SR1_SB of I2C_SR1 SB[0]
#define I2C_FLAG_RXNE			(1 << I2C_SR1_RxNE)		//Dịch 1 đến vị trí bit I2C_SR1_RxNE of I2C_SR1 RXNE[6]
#define I2C_FLAG_TXE			(1 << I2C_SR1_TxE)		//Dịch 1 đến vị trí bit I2C_SR1_TxE of I2C_SR1 TXE[7]
#define I2C_FLAG_OVR			(1 << I2C_SR1_OVR)		//I2C_SR1 OVR[11]
#define I2C_FLAG_AF				(1 << I2C_SR1_AF)		//I2C_SR1 AF[10]
#define I2C_FLAG_ARLO			(1 << I2C_SR1_ARLO)		//I2C_SR1 ARLO[9]
#define I2C_FLAG_BERR			(1 << I2C_SR1_BERR)		//I2C_SR1 BERR[8]
#define I2C_FLAG_STOPF			(1 << I2C_SR1_STOPF)	//I2C_SR1 OVR[4]
#define I2C_FLAG_ADD10			(1 << I2C_SR1_ADD10)	//I2C_SR1 ADD10[3]
#define I2C_FLAG_BTF			(1 << I2C_SR1_BTF)		//I2C_SR1 OVR[2]
#define I2C_FLAG_ADDR			(1 << I2C_SR1_ADDR)		//I2C_SR1 ADDR[1]
#define I2C_FLAG_TIMEOUT		(1 << I2C_SR1_TIMEOUT)	//I2C_SR1 OVR[14]


/****************************************************************************************
 *							APIs supported by this driver								*
 *			For more information about the APIs check the function definition			*
 ****************************************************************************************/
/*
 * Peripheral Clock Setup for I2C
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);				//APIs Peripheral Clock

/*
 * Init and De-Init
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);									//APIs I2C initialization
void I2C_DeInit(I2C_RegDef_t *pI2Cx);										//APIs Reset I2C

/*
 * Data Send and Receive for I2C
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr);

/*
 * IRQ Configuration and ISR Handling for I2C
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);				//APIs Interrupt config(cấu hình) - Yêu cầu ngắt
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);		//APIs Interrupt Priority

/*
 * Other Peripheral Control APIs for I2C
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);			//Enable or Disable I2C Peripheral hoạt động
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);			//Kiểm tra trạng thái các flag trong thanh ghi
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/*
 * Application callback
 */
__weak void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);


#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */
