/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Nov 17, 2024
 *      Author: Minh
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

/**
 * 1_This is a Configuration for a GPIO pin - cấu hình một pin của GPIO muốn sử dụng
 * Sử dụng uint8_t = 2bye = 16bit, vì mỗi GPIO port chứa 16 pin
 */
typedef struct
{
	uint8_t GPIO_PinNumber;				/*!<Possible value from @GPIO_PIN_NUMBER>*/
	uint8_t GPIO_PinMode;				/*!<Possible value from @GPIO_PIN_MODE>*/
	uint8_t GPIO_PinSpeed;				/*!<Possible value from @GPIO_PIN_SPEED>*/
	uint8_t GPIO_PinPuPdControl;		/*!<Possible value from @GPIO_PIN_PUPDCONTROL>*/
	uint8_t GPIO_PinOPType;				/*!<Possible value from @GPIO_PIN_OPTYPE>*/
	uint8_t GPIO_PinAltFunMode;			/*!<Possible value from @GPIO_PIN_ALTFUNMODE>*/

}GPIO_PinConfig_t;

/**
 * 2_This is a Handle(xử lý) structure for a GPIO pin
 */
typedef struct
{
	GPIO_RegDef_t *pGPIOx;				//Con trỏ giữ địa chỉ cơ sở của GPIx mà ta muốn sử dụng chân pin
	GPIO_PinConfig_t GPIO_PinConfig;	//Cấu hình cài đặt chân pin thuộc GPIOx đã sẽ địa chỉ ở trên

	//Biến GPIO_PinConfig không phải con trỏ về đây là biến chứa các giá trị có thể thay đổi theo
	//chương trình điều khiển của lập trình viên

}GPIOx_Handle_t;

/**
 * 3_GPIO Register Macros - Tiến hành định nghĩa(Macros) trạng thái bit cho các register liên quan đến GPIO
 * Phần này cầm phải tham khảo Datasheet
 */
/*
 * @GPIO_PIN_MODES
 * GPIO pin possible(có thể) mode (Tham khảo MODER Register(8.4.1))
 */
#define GPIO_MODE_IN 			0			//00
#define GPIO_MODE_OUT			1			//01
#define GPIO_MODE_ALTFN			2			//10
#define GPIO_MODE_ANALOG		3			//11

//Các mode khác có thể định nghĩa (thường dùng cho Interrupt)
#define GPIO_MODE_IT_FT			4			//Xung cạnh giảm đầu vào(Flow Trigger)
#define GPIO_MODE_IT_RT			5			//Xung cạnh tăng đầu vào(Rising Trigger)
#define GPIO_MODE_IT_RFT		6			//Xung cạnh tăng giảm (Rising Flow Trigger)

/*
 * @GPIO_PIN_OPTYPE
 * GPIO pin possible(có thể) output type (Tham khảo OUTPUT Type Register(8.4.2))
 */
#define GPIO_OP_TYPE_PP			0			//Push-pull
#define GPIO_OP_TYPE_OD			1			//Open-Drain

/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible(có thể) output speed (Tham khảo OUTPUT SPEED Register(8.4.3))
 */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPIO_SPEED_HIGH			3

/*
 * @GPIO_PIN_PUPD
 * GPIO pin possible(có thể) pullup-pulldown (Tham khảo PUPDR Register(8.4.4))
 */
#define GPIO_NO_PUPD			0
#define GPIO_PIN_PU				1
#define GPIO_PIN_PD				2

/*
 * @GPIO_PIN_NUMBER
 * GPIO port có 16 pin, tiến hành định nghĩa từng pin
 */
#define GPIO_PIN_NO_0			0
#define GPIO_PIN_NO_1 			1
#define GPIO_PIN_NO_2			2
#define GPIO_PIN_NO_3			3
#define GPIO_PIN_NO_4			4
#define GPIO_PIN_NO_5			5
#define GPIO_PIN_NO_6			6
#define GPIO_PIN_NO_7			7
#define GPIO_PIN_NO_8			8
#define GPIO_PIN_NO_9			9
#define GPIO_PIN_NO_10			10
#define GPIO_PIN_NO_11			11
#define GPIO_PIN_NO_12			12
#define GPIO_PIN_NO_13			13
#define GPIO_PIN_NO_14			14
#define GPIO_PIN_NO_15			15

/*Tiến hành viết các APIs(các hàm) để phục vụ công việc lập trình GPIO peripheral*/
/*********************************************************************************
 * 		4_APIs supported by this driver(API được trình điều khiển này hỗ trợ)
 * For more information about the APIs check the function definition
 * (Để biết thêm thông tin về các API, hãy kiểm tra định nghĩa hàm)
 *********************************************************************************/

/*
 * 4_1_Peripheral Clock setup
 * Bật/tắt Clock cho GPIO peripheral
 * pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển xung clock đến
 * EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);			//APIs Peripheral Clock

/*
 * 4_2_Initialization And De-Ini
 * pGPIOHandle: chọn con trỏ là GPIO port x muốn xử lý công việc
 */
void GPIO_Init(GPIOx_Handle_t *pGPIOHandle);									//APIs GPIO initialization
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);										//APIs Reset GPIO Port

/*
 * 4_3_Data Read and Write
 * pGPIOx: con trỏ là địa chỉ GPIO port x muốn điều khiển
 * PinNumber: chân Pin muốn điều khiển(đọc,viết) trạng thái
 * value: trạng thái muốn điều khiển(port có 16pin nên khi khai báo là uint16_t)
 * Các hàm Read_Port và Read_Pin sẽ cần có giá trị trả về return value
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);				//APIs Read Input Pin
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);									//APIs Read Input Port
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t value);	//APIs Write Output Pin
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx,uint16_t value);						//APIs Write Output Port
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);					//APIs Toggle(chuyển đổi) Status Pin

/*
 * 4_4_IRQ Configuration and ISR Hangling
 * IRQNumber: ngắt số mấy, IRQPriority: mức ưu tiên, EnorDi: Enable hoặc Disable
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);						//APIs Interrupt config(cấu hình) - Yêu cầu ngắt
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);					//APIs Interrupt Priority
void GPIO_IRQHangling(uint8_t PinNumber);				//APIs Interrupt Hangling(xử lý) - thực hiện công việc khi ngắt xảy ra


#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
