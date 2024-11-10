/*
 * stm32f407xx_usart_driver.c
 *
 *  Created on: Jan 24, 2024
 *      Author: Minh
 */

#include "stm32f407xx_usart_driver.h"
#include "stm32f407xx_rcc_driver.h"

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
 * @Note              - none

 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{

	//Variable to hold the APBx clock , giá trị tần số theo USARTx kết nối với APBx Bus
	uint32_t PCLKx;

	uint32_t usartdiv;		//value usartdiv để cấu hình vào USART_BRR

	//variables to hold "Mantissa" and "Fraction" values
	uint32_t M_part,F_part;

	uint32_t tempreg=0;

	//Get the value of APBx bus clock in to the variable PCLKx
	if(pUSARTx == USART1 || pUSARTx == USART6)
	{
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
	}
	else
	{
		//USART2 and USART3 are hanging on APB1 bus
	   PCLKx = RCC_GetPCLK1Value();
	}

	//Check for "OVER8" configuration bit
	if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
	{
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	}
	else
	{
	   //over sampling by 16
		usartdiv = ((25 * PCLKx) / (4 *BaudRate));
	}

	//Calculate the Mantissa part
	M_part = usartdiv/100;

	//Place the Mantissa part in appropriate bit position . refer USART_BRR - Đặt phần Mantissa ở vị trí bit thích hợp. tham khảo USART_BRR
	tempreg |= M_part << 4;  //phần Mantissa là 4bit đầu tiên [3:0] của biến 32 bit tempreg

	//Extract the "fraction" part
	F_part = (usartdiv - (M_part * 100));

	//Calculate the final "fractional"
	if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
	{
		//OVER8 = 1 , over sampling by 8
		F_part = ((( F_part * 8)+ 50) / 100) & ((uint8_t)0x07);
	}
	else
	{
		//over sampling by 16
		F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);
	}

	//Place the fractional part in appropriate bit position . refer USART_BRR
	tempreg |= F_part;

	//copy the value of tempreg in to BRR register
	pUSARTx->BRR = tempreg;

}

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
	uint32_t tempreg=0;

/******************************** 1. Configuration of CR1******************************************/

	//Implement the code to enable the "Clock" for given USART peripheral - Kích hoạt xung Clock
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	//Biện luận để Kích hoạt USART Tx và Rx engines theo mục cấu hình USART_Mode
	//Trạng thái dùng để so sánh là sử dụng thông số "USART_Config" do User cài đặt ban đầu
	if ( pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)	//Trạng thái nhận
	{
		//Implement the code to enable the Receiver bit field - Triển khai mã để kích hoạt trường bit Receiver
		tempreg|= (1 << USART_CR1_RE);
	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the "Transmitter" bit field
		tempreg |= ( 1 << USART_CR1_TE );

	}
	else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields - Cả hai RX và TX Mode
		tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
	}

    //Implement the code to configure the Word length configuration item
	tempreg |= pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M ;


    //Configuration of parity control bit fields - Cấu hình các trường bit kiểm soát tính Parity
	if ( pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		//Implement the code to enable the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//Implement the code to enable EVEN parity - Triển khai mã để kích hoạt EVEN Parity
		//Not required because by default EVEN parity will be selected once you enable the parity control
		//Không bắt buộc code vì theo mặc định USART_CR1_PS=0=EVEN, EVEN sẽ được chọn sau khi bạn bật kiểm soát Parity

	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		//Implement the code to enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    //Implement the code to enable ODD parity - kích hoạt bit field USART_CR1_PS lên 1 = Odd
	    tempreg |= ( 1 << USART_CR1_PS);

	}

   //Program the CR1 register - Cấu hình thông số đã cài đặt tempreg cho CR1 Register
	pUSARTHandle->pUSARTx->CR1 = tempreg;


/******************************** 2. Configuration of CR2******************************************/

	tempreg=0;

	//Implement the code to configure the number of "Stop Bits" inserted during USART frame transmission
	//Triển khai mã để định cấu hình số lượng "Stop Bits" được chèn trong quá trình truyền khung USART
	tempreg |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP; //Thông báo có stop bit đc sử dụng

	//Program the CR2 register
	pUSARTHandle->pUSARTx->CR2 = tempreg;

/******************************** 3. Configuration of CR3******************************************/

	tempreg=0;

	//Biện luận thông số "USART_HWFlowControl" do User cài đặt ban đầu

	//Configuration of USART hardware flow control
	if ( pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//Implement the code to "Enable CTS" flow control
		tempreg |= ( 1 << USART_CR3_CTSE);


	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//Implement the code to "Enable RTS" flow control
		tempreg |= ( 1 << USART_CR3_RTSE);

	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//Implement the code to enable both "CTS and RTS" Flow control
		tempreg |= ( 1 << USART_CR3_CTSE);
		tempreg |= ( 1 << USART_CR3_RTSE);
	}


	pUSARTHandle->pUSARTx->CR3 = tempreg;

/******************************** 4. Configuration of BRR(Baudrate register)******************************************/

	//Implement the code to configure the baud rate
	//We will cover this in the lecture. No action required here

	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

/*********************************************************************
 * @fn      		  - USART_SendData
 *
 * @brief             - Hàm xử lý gửi data
 *
 * @param[in]         - pUSARTHandle: con trỏ struct là USARTx đã đc cấu hình
 * @param[in]         - pTxBuffer: Nội dung data muốn truyền, thường là mảng ký tự
 * @param[in]         - Len: Độ dài dữ liệu muốn gửi
 *
 * @return            - none
 *
 * @Note              - none

 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t *pdata;
   //Loop over until "Len" number of bytes are transferred - Lặp lại cho đến khi số byte "Len" được chuyển
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until TXE flag is set in the SR
		//Triển khai mã để đợi cho đến khi cờ TXE được đặt trong SR, nghĩa là thanh ghi TDR là empty
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_TXE));

         //Check the "USART_WordLength" item for "9BIT" or "8BIT" in a "frame"
		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)  //9bit
		{
			//if 9BIT, load the DR with "2bytes masking the bits" other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pdata & (uint16_t)0x01FF); //0x01FF = 0000 0001 1111 1111, 9bit

			//Check for "USART_ParityControl" - có sử dụng hay ko parity bit
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used in this transfer. so, 9bits of user data will be sent
				//Implement the code to increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;

				//Nếu cài đặt Không sử dụng bit Parity nào được sử dụng trong lần truyền này.
				//thì cả 9 bit dữ liệu user muốn gửi sẽ được gửi
				//Triển khai đoạn mã để tăng bộ đệm pTxBuffer 2 lần, nguyên nhân vì pTxBuffer++ là tăng địa
				//chỉ ô nhớ, vì sử dụng 9bit nên cần sử dụng 2 ô nhớ
			}
			else
			{
				//Parity bit is used in this transfer . so , 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;

				//Trường hợp có sử dụng Parity bit, 8bit data và 1bit Parity
			}
		}
		else
		{
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer  & (uint8_t)0xFF);	//1111 1111

			//Implement the code to increment the buffer address
			pTxBuffer++;
		}
	}

	//Implement the code to wait till "TC flag" is set in the "SR register", nghĩa là chờ đến khi TC=1
	while( ! USART_GetFlagStatus(pUSARTHandle->pUSARTx,USART_SR_TC));  //TC=1-> Transmission complete
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
   //Loop over until "Len" number of bytes are transferred
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

				//read only first 9 bits. so, mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->DR  & (uint16_t)0x01FF);

				//Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used, so, 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);

				 //Increment the pRxBuffer
				 pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				 *pRxBuffer = (pUSARTHandle->pUSARTx->DR  & (uint8_t)0xFF);
			}

			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

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
	uint8_t txstate = pUSARTHandle->TxBusyState;

	if(txstate != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		//Implement the code to enable interrupt for TXE
		pUSARTHandle->pUSARTx->CR1 |= ( 1 << USART_CR1_TXEIE);

		//Implement the code to enable interrupt for TC
		pUSARTHandle->pUSARTx->CR1 |= ( 1 << USART_CR1_TCIE);

	}

	return txstate;

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
	uint8_t rxstate = pUSARTHandle->RxBusyState;

	if(rxstate != USART_BUSY_IN_RX)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		(void)pUSARTHandle->pUSARTx->DR;

		//Implement the code to enable interrupt for RXNE - Triển khai đoạn mã kích hoạt ngắt cho RXNE
		pUSARTHandle->pUSARTx->CR1 |= ( 1 << USART_CR1_RXNEIE);
	}

	return rxstate;

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
 * @Note        - none
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName)
{
	if(pUSARTx->SR & FlagName)
	{
		return SET;			//High=1
	}

	return RESET;			//Low=0
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
	pUSARTx->SR &= ~( StatusFlagName);			//Reset flag các bit field của SR register
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

	uint32_t temp1 , temp2, temp3;

	uint16_t *pdata;

/*************************1. Check for TC flag ********************************************/

    //Implement the code to check the state of TC bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TC);

	 //Implement the code to check the state of TCEIE bit
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TCIE);

	if(temp1 && temp2 )
	{
		//this interrupt is because of TC

		//close transmission and call application callback if TxLen is zero
		if ( pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			//Check the TxLen . If it is zero then close the data transmission
			if(! pUSARTHandle->TxLen )
			{
				//Implement the code to clear the TC flag
				pUSARTHandle->pUSARTx->SR &= ~( 1 << USART_SR_TC);

				//Implement the code to clear the TCIE control bit

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

/*************************2. Check for TXE flag ********************************************/

	//Implement the code to check the state of TXE bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_TXE);

	//Implement the code to check the state of TXEIE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_TXEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of TXE

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

/*************************3. Check for RXNE flag ********************************************/

	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_RXNE);
	temp2 = pUSARTHandle->pUSARTx->CR1 & ( 1 << USART_CR1_RXNEIE);


	if(temp1 && temp2 )
	{
		//this interrupt is because of rxne
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


			}//if of >0

			if(! pUSARTHandle->RxLen)
			{
				//disable the rxne
				pUSARTHandle->pUSARTx->CR1 &= ~( 1 << USART_CR1_RXNEIE );
				pUSARTHandle->RxBusyState = USART_READY;
				USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_RX_CMPLT);
			}
		}
	}


/*************************4. Check for CTS flag ********************************************/
//Note : CTS feature is not applicable for UART4 and UART5

	//Implement the code to check the status of CTS bit in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & ( 1 << USART_SR_CTS);

	//Implement the code to check the state of CTSE bit in CR1
	temp2 = pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_CTSE);

	//Implement the code to check the state of CTSIE bit in CR3 (This bit is not available for UART4 & UART5.)
	temp3 = pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_CTSIE);


	if(temp1  && temp2 )
	{
		//Implement the code to clear the CTS flag in SR
		pUSARTHandle->pUSARTx->SR &=  ~( 1 << USART_SR_CTS);

		//this interrupt is because of cts
		USART_ApplicationEventCallback(pUSARTHandle,USART_EVENT_CTS);
	}

/*************************5. Check for IDLE detection flag ********************************************/

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

/*************************6. Check for Overrun detection flag ********************************************/

	//Implement the code to check the status of ORE flag  in the SR
	temp1 = pUSARTHandle->pUSARTx->SR & USART_SR_ORE;

	//Implement the code to check the status of RXNEIE  bit in the CR1
	temp2 = pUSARTHandle->pUSARTx->CR1 & USART_CR1_RXNEIE;


	if(temp1  && temp2 )
	{
		//Need not to clear the ORE flag here, instead give an api for the application to clear the ORE flag .

		//this interrupt is because of Overrun error
		USART_ApplicationEventCallback(pUSARTHandle,USART_ERR_ORE);
	}



/*************************7. Check for Error Flag ********************************************/

//Noise Flag, Overrun error and Framing Error in multibuffer communication
//We dont discuss multibuffer communication in this course. please refer to the RM
//The blow code will get executed in only if multibuffer mode is used.

	temp2 =  pUSARTHandle->pUSARTx->CR3 & ( 1 << USART_CR3_EIE) ;

	if(temp2 )
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

		if(temp1 & ( 1 << USART_SR_ORE) )
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
