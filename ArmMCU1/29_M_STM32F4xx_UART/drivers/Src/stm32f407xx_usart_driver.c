/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: Jan 24, 2024
 *      Author: Minh
 */

#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"			//Thư viện Reset Clock Control

/*********************************************************************
 * @fn      	- USART_PeriClockControl
 *
 * @brief       - Enable/Disable Clock cho USART peripheral
 *
 * @param[in]   - pUSARTx: con trỏ là địa chỉ USARTx muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return      - none
 *
 * @Note        - none
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pUSARTx == USART1)
		{
			USART1_PCLK_EN();
		}
		else if (pUSARTx == USART2)
		{
			USART2_PCLK_EN();
		}
		else if (pUSARTx == USART3)
		{
			USART3_PCLK_EN();
		}
		else if (pUSARTx == USART6)
		{
			USART6_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}
}

/*********************************************************************
 * @fn      		  - USART_Init
 *
 * @brief             - APIs USART Peripheral initialization
 *
 * @param[in]         - pUSARTHandle: con trỏ struct là USARTx muốn xử lý công việc
 *
 * @return            - none
 *
 * @Note              - none

 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
	//Temporary variable
	uint32_t tempreg = 0;						//Biến dùng để lưu cấu hình user rồi cấu hình vào các thanh ghi control của USART

	/******************************** 1. Configuration of CR1******************************************/
	//Implement the code to enable the "Clock" for given USART peripheral - Kích hoạt xung Clock cho UARTx
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	//Biện luận để Kích hoạt USART Tx và Rx engines theo mục cấu hình USART_Mode
	//Trạng thái dùng để so sánh là sử dụng thông số "USART_Config" do User cài đặt ban đầu
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)	//Đang thiết lập trạng thái nhận
	{
		//Implement the code to enable the Receiver bit field - Cấu hình bit theo mã bit field của USART_CR1_RE
		tempreg |= (1 << USART_CR1_RE);							//Dịch đến bit 2 và Cấu hình bit 2 của biến 32 bit tempreg[32:0]
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)	//Đang thiết lập trạng thái gửi
	{
		//Implement the code to enable the "Transmitter" bit field	- Cấu hình bit theo mã bit field của USART_CR1_TE
		tempreg |= (1 << USART_CR1_TE);						//Dịch đến bit 3 và Cấu hình bit 3 của biến 32 bit tempreg[32:0]
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)		//Đang thiết lập trạng thái cả gửi và nhận
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields - Cấu hình bit theo mã bit field của USART_CR1_TE và USART_CR1_RE
		tempreg |= ( (1 << USART_CR1_RE) | (1 << USART_CR1_TE) );			//Cấu hình cả bit 2 và 3 của biến 32 bit tempreg[32:0]
	}

	//Implement the code to configure the Word length configuration item - Cấu hình Data Frame theo mã bit của USART_CR1_M
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M;		//Cấu hình bit 12 của biến 32 bit tempreg[32:0]

	//Configuration of parity control bit fields - Cấu hình các trường bit kiểm soát tính Parity
	if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)			//Parity chẵn
	{
		//Implement the code to enable the parity control - Cấu hình bit theo mã bit field của USART_CR1_PCE
		tempreg |= (1 << USART_CR1_PCE);									//Cấu hình bit 10 của biến 32 bit tempreg[32:0]

		//Implement the code to enable EVEN parity - Triển khai mã để kích hoạt EVEN Parity
		//Not required because by default EVEN parity will be selected once you enable the parity control
		//Không cần code PS=0 vì theo mặc định USART_CR1_PS=0=EVEN khi set PCE=1
		//Nghĩa là khi PCE(Parity control enable) đc set thì Parity EVEN sẽ được chọn tự động

	}
	else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)		//Parity lẻ
	{
		//Implement the code to enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//Implement the code to enable ODD parity - kích hoạt bit field USART_CR1_PS lên 1 = Odd
		tempreg |= ( 1 << USART_CR1_PS);									//Dịch đến bit 9 và Cấu hình bit 9 của biến 32 bit tempreg[32:0]
	}

	//Program the CR1 register - Cấu hình thông số đã cài đặt tempreg cho CR1 Register
	pUSARTHandle->pUSARTx->CR1 = tempreg;


	/******************************** 2. Configuration of CR2******************************************/
	tempreg = 0;			//Reset value

	//Implement the code to configure the number of "Stop Bits" inserted during USART frame transmission
	//Triển khai mã để định cấu hình số lượng "Stop Bits" sử dụng theo cấu hình bit theo mã bit field của USART_CR2_STOP
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;			//Dịch đến bit 12 và Cấu hình bit 12 của biến 32 bit tempreg[32:0]

	//Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 = tempreg;

	/******************************** 3. Configuration of CR3******************************************/
	tempreg = 0;			//Reset value

	//Configuration of USART hardware flow control - Check thông số "USART_HWFlowControl" do User cài đặt ban đầu
	if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to "Enable CTS" flow control - cấu hình bit theo mã bit field của USART_CR3_CTSE
		tempreg |= (1 << USART_CR3_CTSE);					//Dịch đến bit 9 và Cấu hình bit 9 của biến 32 bit tempreg[32:0]
	}
	else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to "Enable RTS" flow control - cấu hình bit theo mã bit field của USART_CR3_RTSE
		tempreg |= (1 << USART_CR3_RTSE);					//Dịch đến bit 8 và Cấu hình bit 8 của biến 32 bit tempreg[32:0]
	}
	else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both "CTS and RTS" Flow control - cấu hình bit theo mã bit field của USART_CR3_RTSE và USART_CR3_CTSE
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}

	//Program the CR3 register
	pUSARTHandle->pUSARTx->CR3 = tempreg;

	/******************************** 4. Configuration of BRR(Baudrate register)******************************************/
	//Implement the code to configure the baud rate - We will cover this in the lecture. No action required here
	//Hàm cấu hình baud rate vào trong thanh ghi USART_BRR - Sử dụng con trỏ
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

/*********************************************************************
 * @fn      		  - USART_SetBaudRate
 *
 * @brief             - Cấu hình thông số Baud Rate
 *
 * @param[in]         - pUSARTx: Configuration USART Register Peripheral
 * @param[in]         - BaudRate: thông số BaudRate user muốn sử dụng
 *
 * @return            - none
 *
 * @Note              - USART_BRR Register
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	//Variable to hold the APBx clock - Giá trị tần số theo USARTx kết nối với APBx Bus
	uint32_t PCLKx;						//Biến lưu tần số clock từ APBx Bus
	uint32_t usartDiv;					//Value usartdiv - Bộ Chia hệ số

	//Variables to hold "DIV_Mantissa" and "DIV_Fraction" values with USART_BRR
	uint32_t M_part,F_part;

	uint32_t tempreg = 0;				//Biến sẽ lưu cấu hình cuối cùng để set baudrate cho thanh ghi USART_BRR

	//1-Get the value of APBx bus clock in to the variable PCLKx - Lấy giá trị xung clọck từ APBx mà UARTx đang kết nối
	if(pUSARTx == USART1 || pUSARTx == USART6)					//Kiểm tra user muốn chọn UARTx nào
	{
		//USART1 and USART6 are hanging on APB2 bus
		PCLKx = RCC_GetPCLK2Value();
	}
	else
	{
		//USART2 and USART3 are hanging on APB1 bus
		PCLKx = RCC_GetPCLK1Value();
	}

	//2-Check for "OVER8" configuration bit - Kiểm tra cấu hình UARTx muốn lấy mẫu ở mức 8 bit hay 16 bit
	if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
	{
		//OVER8 = 1 , over sampling by 8
		usartDiv = ((25 * PCLKx) / (2 * BaudRate));
	}
	else
	{
		//OVER8 = 0 , over sampling by 16
		usartDiv = ((25 * PCLKx) / (4 *BaudRate));
	}

	//3-Calculate USART_BRR
	//3.1-Calculate the Mantissa part of USART_BRR
	M_part = usartDiv / 100;

	//Place the Mantissa part in appropriate bit position . refer USART_BRR
	//Phần Mantissa của USART_BRR là bit field [15:4] của biến 32 bit tempreg
	tempreg |= M_part << 4;					//Dịch 4 bit sang phải và lưu value M_part vào tempreg bắt đầu từ bit thứ 4

	//3.2-Extract the "fraction" part of USART_BRR
	F_part = (usartDiv - (M_part * 100));

	//3.3-Calculate the final "fractional" of USART_BRR
	if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
	{
		//OVER8 = 1, over sampling by 8
		F_part = ((( F_part * 8)+ 50) / 100) & ((uint8_t)0x07);
	}
	else
	{
		//OVER8 = 0, over sampling by 16
		F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);
	}

	//Phần Fractional của USART_BRR là bit field [3:0] của biến 32 bit tempreg
	tempreg |= F_part;						//Đưa value vào 4 bit trống bit field [3:0]

	//4-Set value of tempreg in to BRR register
	pUSARTx->BRR = tempreg;
}

/*********************************************************************
 * @fn      		  - USART_SendData
 *
 * @brief             - Hàm xử lý gửi data USART Peripheral
 *
 * @param[in]         - pUSARTHandle: con trỏ struct là USARTx đã đc cấu hình
 * @param[in]         - pTxBuffer: Nội dung data muốn truyền, thường là mảng ký tự
 * @param[in]         - Len: Độ dài dữ liệu muốn gửi
 *
 * @return            - none
 *
 * @Note              - Chương trình có sửa một số chỗ so với chương trình gốc - Edit
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t *pData;					//Biến con trỏ 16 bit

	//Loop over until "Len" number of bytes are transferred - Lặp lại cho đến khi số byte "Len" được chuyển hết
	for(uint32_t i = 0; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR - Kiểm tra cho đến khi cờ TXE được đặt trong SR
		while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TXE));

		//Check the "USART_WordLength" item for "9BIT" or "8BIT" in a "frame" - Kiểm tra Data Fram sử dụng   -- "Edit 2025/1/1"
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)				//9 bit
		{
			//Check for "USART_ParityControl" - Có sử dụng hay ko parity bit
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)			//Not use
			{
				//if 9BIT, load the DR with "2bytes masking the bits" other than first 9 bits
				pData = (uint16_t *)pTxBuffer;			//Ép kiểu địa chỉ vì param "pTxBuffer" đầu vào chỉ có kiểu là uint8_t

				//Chuẩn hóa data cần gửi *pTxBuffer là kiểu data sử dụng 9 bit
				pUSARTHandle->pUSARTx->DR = (*pData & (uint16_t)0x01FF);			//0x01FF = 0000 0001 1111 1111 - 9bit data đc sử dụng

				//No parity is used in this transfer. 9bits of user data will be sent - Implement the code to increment pTxBuffer address twice
				pTxBuffer++;
				pTxBuffer++;
				/*
				 * Giải thích:
				 * Nếu cài đặt "Không" sử dụng bit Parity nào trong lần truyền này thì cả 9 bit data user muốn gửi sẽ được gửi
				 * Triển khai đoạn mã để tăng bộ đệm pTxBuffer 2 lần, nguyên nhân vì pTxBuffer++ là tăng địa chỉ ô nhớ, vì sử dụng 9bit nên
				 * cần sử dụng 2 ô nhớ
				 */
			}
			else																				//8 bit + Parity bit
			{
				//This is 8bit data transfer - Chuẩn hóa data cần gửi *pTxBuffer là kiểu data sử dụng 8 bit
				pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);	//0xFF = 1111 1111

				//Parity bit is used in this transfer . 8bits of user data will be sent - Trường hợp có sử dụng Parity bit, 8bit data và 1bit Parity
				//The 9th bit will be replaced by parity bit by the hardware - Bit thứ 9 sẽ được thay thế bằng bit chẵn lẻ bởi phần cứng
				pTxBuffer++;

				//Parity bit đã đc thiết lập ở hàm USART_Init()
			}
		}
		else																				//8bit
		{
			//Check for "USART_ParityControl" - Có sử dụng hay ko parity bit
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)			//Not use
			{
				//This is 8bit data transfer - Chuẩn hóa data cần gửi *pTxBuffer là kiểu data sử dụng 8 bit
				pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);	//0xFF = 1111 1111
			}
			else																				//7bit + 1bit Parity
			{
				//This is 8bit data transfer - Chuẩn hóa data cần gửi *pTxBuffer là kiểu data sử dụng 7 bit
				pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0x7F);	//0x7F = 0111 1111

				//Parity bit đã đc thiết lập ở hàm USART_Init()
			}
			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till "TC flag" is set in the "UART_SR register" - Nghĩa là chờ đến khi TC=1 của USART_SR Register
	while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_SR_TC));  			//TC=1-> Transmission complete
}

/*********************************************************************
 * @fn      		  - USART_ReceiveData
 *
 * @brief             - Nhận dữ liệu USART Peripheral
 *
 * @param[in]         - pUSARTHandle: con trỏ struct là USARTx đã đc cấu hình
 * @param[in]         - pRxBuffer: Bộ đệm chúa data nhận được, thường là mảng ký tự
 * @param[in]         - Len: Độ dài dữ liệu đc nhận
 *
 * @return            - none
 *
 * @Note              - none
 */

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	//Loop over until "Len" number of bytes are transferred - Lặp lại cho đến khi số byte "Len" được chuyển hết
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until RXNE flag is set in the SR
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_RXNE));

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//We are going to receive "9bit" data in "a frame"

			//Check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used. so, all 9bits will be of user data

				//read only first 9 bits. so, mask the DR with 0x01FF - nhận cả 9 bit đều là data
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

				//Now increment the pRxBuffer two times - Nguyên nhân vì nhận 2 byte cùng lúc
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used, so, 8bits will be of user data and 1 bit is parity
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

				//Increment the pRxBuffer - Bit thứ 9 sẽ được thêm bằng bit chẵn lẻ bởi phần cứng
				pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data - 8 bit data

				//read 8 bits from DR
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
			}
			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity - 7 bit data  + 1 bit Parity

				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F); //0111 1111
			}
			//increment the pRxBuffer
			pRxBuffer++;
		}
	}
}

/*********************************************************************
 * @fn      		  - USART_SendDataWithIT
 *
 * @brief             - Kích hoạt Send Data USART Interrupt Peripheral
 *
 * @param[in]         - pUSARTHandle: con trỏ struct là USARTx đã đc cấu hình
 * @param[in]         - pTxBuffer: Bộ đệm chứa data muốn gửi, thường là mảng ký tự
 * @param[in]         - Len: Độ dài dữ liệu muốn gửi
 *
 * @return            - Trạng thái của txstate
 *
 * @Note              - khi cờ bit USART_CR1_TXEIE và USART_CR1_TCIE đc set thì hàm "void USART2_IRQHandler(void)" đc thực hiện
 */
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	//Lưu trạng thái hiện tại của hệ thông UART Peripheral
	uint8_t txstate = pUSARTHandle->TxBusyState;

	//Kiểm tra hệ thống có đang truyền một data nào không
	if(txstate != USART_BUSY_IN_TX)
	{
		//Cấu hình các nội dung data sẽ gửi và các trạng thái vào trong biến toàn cục pUSARTHandle của USARTx nào đó
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		//Implement the code to enable interrupt for TC
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
	}
	return txstate;												//Trả về trạng thái Tx xem đã sẵn sàng để gửi bằng Interrupt hay chưa
}

/*********************************************************************
 * @fn      		  - USART_ReceiveDataIT
 *
 * @brief             - Kích hoạt Receive Data USART Interrupt Peripheral
 *
 * @param[in]         - pUSARTHandle: con trỏ struct là USARTx đã đc cấu hình
 * @param[in]         - pRxBuffer: Bộ đệm chứa data nhận được, thường là mảng ký tự
 * @param[in]         - Len: Độ dài dữ liệu nhận được
 *
 * @return            - Trạng thái của rxstate
 *
 * @Note              - khi cờ bit USART_CR1_RXNEIE đc set thì hàm "void USART2_IRQHandler(void)" đc thực hiện

 */
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
	//Lưu trạng thái hiện tại của hệ thông UART Peripheral
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	//Kiểm tra hệ thống có đang nhận một data nào không
	if(rxstate != USART_BUSY_IN_RX)
	{
		//Cấu hình các nội dung data đã nhận và các trạng thái vào trong biến toàn cục pUSARTHandle của USARTx nào đó
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		(void)pUSARTHandle->pUSARTx->DR;

		//Implement the code to enable interrupt for RXNE - Triển khai đoạn mã kích hoạt ngắt cho RXNE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
	}

	return rxstate;												//Trả về trạng thái Rx xem đã nhận bằng Interrupt hay chưa
}

/*********************************************************************
 * @fn      	- USART_PeripheralControl
 *
 * @brief       - Cho phép USARTx có đc hoạt động hay không, bit UE[0] của USART_CR1
 *
 * @param[in]   - pUSARTx: con trỏ là địa chỉ USARTx muốn điều khiển xung clock đến
 * @param[in]  	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return      - none
 *
 * @Note        - none
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	}
	else
	{
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
	}
}


/*********************************************************************
 * @fn      	- USART_GetFlagStatus
 *
 * @brief       - Kiểm tra,Truy xuất trạng thái các flag trong thanh ghi SR
 *
 * @param[in]   - pUSARTx: con trỏ là cấu hình USART Register của Processor
 * @param[in]   - FlagName: trạng thái tại vị trí flag muốn kiểm tra
 *
 * @return      - Trạng thái cờ_flag
 *
 * @Note        - USART_SR register
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName)
{
	if(pUSARTx->SR & (1 << FlagName))
	{
		return SET;						//Trạng thái bit đang là 1
	}
	return RESET;						//Trạng thái bit đang là 0

	//Lưu khi nếu không định nghĩa 1 << FlagName trước thì khi lập trình thì cần dịch bit rồi mới AND
}

/*********************************************************************
 * @fn      	- USART_ClearFlag
 *
 * @brief       - Xóa cờ flag của bit field(set các cờ về 0)
 *
 * @param[in]   - pUSARTx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - FlagName: trạng thái tại vị trí flag muốn xóa cờ
 *
 * @return      - none
 *
 * @Note        - Chỉ áp dụng cho USART_CTS_FLAG , USART_LBD_FLAG, USART_TC_FLAG, USART_TC_FLAG
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName)
{
	pUSARTx->SR &= ~(StatusFlagName);			//Reset flag các bit field của SR register
}

/*********************************************************************************
 * @fn			- USART_IRQPriorityConfig
 *
 * @brief		- Set Priority(ưu tiên) Interrupt Status for USARTx
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- IRQPriority: mức ưu tiên của ngắt mong muốn
 *
 * @return		- none
 *
 * @Note 		- none
 */
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
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
 * @fn			- USART_IRQInterruptConfig
 *
 * @brief		- Config(Cấu hình) Interrupt Status for USARTx
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc sử dụng ngắt
 *
 * @return		- none
 *
 * @Note 		- none
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

/*********************************************************************
 * @fn       	- USART_IRQHandling
 *
 * @brief       - Hàm xử lý "Event" khi kích hoạt ngắt USART Interrupt
 *
 * @param[in]   - pUSARTHandle: Con trỏ là chứa cấu hình USART peripheral

 * @return      - none
 *
 * @Note        - Chương trình sẽ kiểm tra tuần từ các Evencủa UART có thể làm Interrupt Processor

 */
void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
	uint32_t temp1, temp2, temp3;				//Các biến để lưu trứ value để set hoặc nhận trạng thái thanh ghi

	uint16_t *pdata;							//Biến tạm 16 bit

	/*************************1. Check for SR_TC(Transmission complete) flag ********************************************/

	//Implement the code to check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TC);

	 //Implement the code to check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TCIE);

	if(temp1 && temp2)									//Cả hai bit đều đang đc set
	{
		//this interrupt is because of TC

		//Close transmission and call application callback if TxLen is zero
		if (pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			//Check the TxLen. If it is zero then close the data transmission
			if(! pUSARTHandle->TxLen)
			{
				//Implement the code to clear the TC flag
				pUSARTHandle->pUSARTx->SR &= ~(1 << USART_SR_TC);

				//Implement the code to clear the TCIE control bit
				pUSARTHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TCIE);			//Edit 2025/1/1

				//Reset the application state
				pUSARTHandle->TxBusyState = USART_READY;

				//Reset Buffer address to NULL
				pUSARTHandle->pTxBuffer = NULL;

				//Reset the length to zero
				pUSARTHandle->TxLen = 0;

				//Call the application call back with event USART_EVENT_TX_CMPLT
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_TX_CMPLT);
			}
		}
	}

	/*************************2. Check for SR_TXE(Transmit data register empty) flag ********************************************/

	//Implement the code to check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TXE);

	//Implement the code to check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);

	if(temp1 && temp2)
	{
		//This interrupt is because of TXE

		//Check trạng thái có đang trong truyền data không
		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			//Keep sending data until Txlen reaches to zero
			if(pUSARTHandle->TxLen > 0)
			{
				//Check the USART_WordLength item for 9BIT or 8BIT in a frame
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//if 9BIT load the DR with 2bytes masking  the bits other than first 9 bits
					pdata = (uint16_t*) pUSARTHandle->pTxBuffer;
					pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF);

					//check for USART_ParityControl
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used in this transfer , so 9bits of user data will be sent
						//Implement the code to increment pTxBuffer twice
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=2;
					}
					else
					{
						//Parity bit is used in this transfer . so 8bits of user data will be sent
						//The 9th bit will be replaced by parity bit by the hardware
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen-=1;
					}
					/*
					 * Giải thích: Khi truyền 9bit data thì ta lưu data vào 2 byte(16 bit) và cổng tịnh tiến 2 địa chỉ
					 * Tuy nhiên ở trường hợp data 9bit(8data+1Parity) ta vẫn có thể dùng biến 16bit và chỉ cộng tịnh tiến 1 địa chỉ
					 * vì data đã đc lưu ở byte(8bit) đầu tiên, bit bị dôi thừa ra do AND ở byte tiếp theo sẽ đc ghi đè ở lần gửi tiếp theo
					 */
				}
				else
				{
					//This is 8bit data transfer
					pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer  & (uint8_t)0xFF);

					//Implement the code to increment the buffer address
					pUSARTHandle->pTxBuffer++;
					pUSARTHandle->TxLen-=1;
				}

			}
			if (pUSARTHandle->TxLen == 0 )
			{
				//TxLen is zero
				//Implement the code to clear the TXEIE bit (disable interrupt for TXE flag )
				pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_TXEIE);
			}
		}
	}

	/*************************3. Check for SR_RXNE(Read data register not empty) flag ********************************************/

	//Implement the code to check the state of RXNE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_RXNE);

	//Implement the code to check the state of RXNEIE bit in the SR
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE);

	if(temp1 && temp2)
	{
		//This interrupt is because of rxne
		if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX)
		{
			if(pUSARTHandle->RxLen > 0)
			{
				//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					//We are going to receive 9bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 9bits will be of user data

						//read only first 9 bits so mask the DR with 0x01FF
						*((uint16_t*) pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

						//Now increment the pRxBuffer two times
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->pRxBuffer++;
						pUSARTHandle->RxLen-=2;
					}
					else
					{
						//Parity is used, so 8bits will be of user data and 1 bit is parity
						 *pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
						 pUSARTHandle->pRxBuffer++;
						 pUSARTHandle->RxLen-=1;
					}
				}
				else
				{
					//We are going to receive 8bit data in a frame

					//Now, check are we using USART_ParityControl control or not
					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						//No parity is used , so all 8bits will be of user data

						//read 8 bits from DR
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
					}
					else
					{
						//Parity is used, so , 7 bits will be of user data and 1 bit is parity

						//read only 7 bits , hence mask the DR with 0X7F
						 *pUSARTHandle->pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->DR  & (uint8_t)0x7F);
					}
					//Now , increment the pRxBuffer
					pUSARTHandle->pRxBuffer++;
					pUSARTHandle->RxLen-=1;
				}
			}

			//Check complete receive
			if(! pUSARTHandle->RxLen)
			{
				//Disable the rxne bit
				pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_RXNEIE);
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}

	/*************************4. Check for SR_CTS flag ********************************************/
	//Note : CTS feature is not applicable for UART4 and UART5 - Không áp dụng cho UART4 và UART5
	//Vì trong bài không define UART4 và UART5 nên không cần biện luận

	//Implement the code to check the status of CTS bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_CTS);

	//Implement the code to check the state of CTSE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSE);

	//Implement the code to check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
	temp3 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSIE);

	if(temp1 && temp2 && temp3)				//Edit: 2024/1/1
	{
		//Implement the code to clear the CTS flag in SR
		pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_CTS);

		//This interrupt is because of cts
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

	/*************************5. Check for IDLE detection flag ********************************************/
	/*
	 * IDLE line được hiểu là trạng thái khi đường truyền không có tín hiệu, tức là đường truyền đang  ở mức logic 1 (high)
	 * trong một khoảng thời gian nhất định. Khi vi điều khiển STM32F4 phát hiện được trạng thái IDLE này kéo dài đủ lâu,
	 * flag IDLE line detected trong thanh ghi USART_SR sẽ được set lên 1
	 */

	//Implement the code to check the status of IDLE flag bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_IDLE);

	//Implement the code to check the state of IDLEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_IDLEIE);

	if(temp1 && temp2)
	{
		//Implement the code to clear the IDLE flag. Refer to the RM to understand the clear sequence
		temp1 = pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_IDLE);

		//this interrupt is because of idle
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_IDLE);
	}

	/*
	 * Từ phần 6 thì sẽ phải tự viết lại chương trình lỗi
	 * Cách làm là kiểm tra các cờ lỗi và sau đó gọi một hàm api (ví dụ như hàm USART_ApplicationEventCallback())
	 * để xử lý như là đưa ra cảnh báo lỗi, xóa các cờ bit lỗi,...
	 */
	/*************************6. Check for Overrun detection flag ********************************************/

	//Implement the code to check the status of ORE flag  in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;

	//Implement the code to check the status of RXNEIE  bit in the CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;

	if(temp1 && temp2)
	{
		//Need not to clear the ORE flag here, instead give an api for the application to clear the ORE flag .
		//Không cần xóa cờ ORE ở đây, thay vào đó hãy cung cấp một api cho ứng dụng để xóa cờ ORE.

		//this interrupt is because of Overrun error
		USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
	}

	/*************************7. Check for Error Flag ********************************************/

	//Noise Flag, Overrun error and Framing Error in multibuffer communication
	//We dont discuss multibuffer communication in this course. please refer to the RM
	//The blow code will get executed in only if multibuffer mode is used.

	temp2 =  pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_EIE) ;

	if(temp2)
	{
		temp1 = pUSARTHandle->pUSARTx->SR;
		if(temp1 & ( 1 << USART_SR_FE))
		{
			/*
				This bit is set by hardware when a de-synchronization, excessive noise or a break character
				is detected. It is cleared by a software sequence (an read to the USART_SR register
				followed by a read to the USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_FE);
		}

		if(temp1 & ( 1 << USART_SR_NF))
		{
			/*
				This bit is set by hardware when noise is detected on a received frame. It is cleared by a
				software sequence (an read to the USART_SR register followed by a read to the
				USART_DR register).
			*/
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_NE);
		}

		if(temp1 & ( 1 << USART_SR_ORE))
		{
			USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
		}
	}
}


/*********************************************************************
 * @fn      		  - USART_ApplicationEventCallback
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t event)
{

}
