/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Dec 27, 2023
 *      Author: Minh
 */

#include "stm32f407xx_i2c_driver.h"

/****************************************************************************************
 *					Static Function supported by this driver							*
 *				Hàm Static chỉ đc phép sử dụng bên trong file mà nó đang chứa			*
 *				Ngăn chặn các lệnh từ file khác có thể gọi hàm xử lý					*
 ****************************************************************************************/
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTEXInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);


//Mảng chứa bộ chia có thể đc sử dụng trong System Clock
uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint16_t APB1_PreScaler[4] = {2,4,8,16};

/*********************************************************************
 * @fn      	- I2C_PeriClockControl
 *
 * @brief       - Enable/Disable Clock cho SPI peripheral
 *
 * @param[in]   - pI2Cx: con trỏ là địa chỉ I2Cx muốn điều khiển xung clock đến
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return      - none
 *
 * @Note        - none

 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		//TODO
	}
}

/*****************************************************************************
 * @fn      	- RCC_GetPLLOutputClock
 *
 * @brief       - Tính toán xung PLL đc sử dụng từ System Clock
 *
 * @param[in]   - none
 *
 * @return      - none
 *
 * @Note        - Không sử dụng xung clock dạng PLL trong khóa học

 */
uint32_t RCC_GetPLLOutputClock(void)
{
	return 0;
}

/*****************************************************************************
 * @fn      	- RCC_GetPCLK1Value
 *
 * @brief       - Xác định xung PCLK1 đc sử dụng từ System Clock
 *
 * @param[in]   - none
 *
 * @return      - pclk1: xung clock đc sử dụng trong I2C peripheral
 *
 * @Note        - Sử dụng sơ đồ khối liên quan đến xung Clock của System Clock
 * 				- Thanh ghi CFGR thuộc RCC Register đc sử dụng
 */
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;					//biến tạm xung clock

	uint8_t clksrc,temp,ahbp,abp1p;				//biến xung clock cho khối system clock

	//Kiểm tra xem SWS[3:2] đang có giá trị bit là bao nhiêu, từ đó xác định khối
	//Oscillator đang đc sử dụng là System Clock
	clksrc = (RCC->CFGR >> 2) & 0x3;	//0x03=0011, nghĩa là đọc 2 vị trí bit liền nhau từ vị trí bit số 2


	//1. Xác định tần số xung clock tổng của System Clock
	if(clksrc == 0)    		 	//HSI Oscillator
	{
		SystemClk = 16000000;
	}
	else if(clksrc == 1)		//HSE Oscillator
	{
		SystemClk = 8000000;
	}
	else if(clksrc == 2)		//PLL Oscillator
	{
		SystemClk = RCC_GetPLLOutputClock();
	}

	//2. Xác định bộ chia của AHBP Prescaler, bit field HPRE[7:4] của CFGR register
	temp = ((RCC->CFGR >> 4) & 0xF);	//0xF=1111, nghĩa là đọc 4 vị trí bit liền nhau từ vị trí bit số 4
	if(temp < 4)
	{
		ahbp = 1;			//Không sử dụng bộ chia
	}
	else
	{
		//Có sử dụng bộ chia, nếu temp=8 thì 8-8=0, sử dụng bộ chia 2,...
		ahbp = AHB_PreScaler[temp - 8];
	}

	//3. Xác định bộ chia của APB1 PreScaler,bit ffield PPRE[12:10] của CFGR register
	temp = ((RCC->CFGR >> 10) & 0x7);	//0xF=1111, nghĩa là đọc 3 vị trí bit liền nhau từ vị trí bit số 10
	if(temp < 8)
	{
		abp1p = 1;			//Không sử dụng bộ chia
	}
	else
	{
		//Có sử dụng bộ chia, nếu temp=8 thì 8-8=0, sử dụng bộ chia 2,...
		abp1p = APB1_PreScaler[temp - 4];
	}

	//4. Xác định xung PCLK1 dùng cho I2C đc lấy từ từ System Clock
	pclk1 = (SystemClk / ahbp) / abp1p;

	return pclk1;
}

/******************************************************************************
 * @fn      	- I2C_Init
 *
 * @brief       - APIs I2C Peripheral initialization
 *
 * @param[in]   - pI2CHandle: con trỏ struct là I2Cx muốn xử lý công việc
 *
 * @return      - none
 *
 * @Note        - none

 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;		//biến tạm 32 bit temp register

	//Enable the Clock for the I2Cx peripheral - Kích hoạt xung clock cho I2Cx
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	//Đọc và Lưu giá trị của I2C_AckControl(sẽ đc set khi init) vào vị trí bit thứ 10 biến tạm tempreg
	tempreg |= pI2CHandle->I2C_Config.I2C_AckControl << 10;				//Ack control bit
	//Kích hoạt hoặc vô hiệu hóa theo yêu cầu của User

	//Thiết lập giá trị bit thứ 10 của thanh ghi CR1 với giá trị biến tempreg
	pI2CHandle->pI2Cx->CR1 = tempreg;

	/* Cấu hình bit field FREQ của CR2 register */
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	//Giải thích: vì giá trị trả về là 16MHz nên ta chia 1MHz để lấy giá trị 16

	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);  //0x3F = 0011 1111
	//Giá trị 16 của tempreg khi biên dịch sẽ đc trình biên dịch tách thành mã nhị phân
	//sau đó & với 0x3F vì bit field FREQ sử dụng 6 bit đầu của CR2 Register

	/* Lập trình định dạng địa chỉ và địa chỉ của thiết bị , OAR1 register */
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;	// ADD[7:1] : lấy giá trị từ bit thứ 2
	tempreg |= (1 << 14);								//Lệnh bắt buộc bit thứ 14 luôn là 1 khi cấu hình
	pI2CHandle->pI2Cx->OAR1 = tempreg;					//Đưa vào cấu hình thanh ghi OAR1 bằng giá trị tempreg

	/* CCR Calculations, Tính toán tần số cho Clock control register */
	uint16_t ccr_value = 0;					//giá trị tần số tạm
	tempreg = 0;

	//Kiểm tra speed đc cài đặt đầu vào ở tần số nào từ đó suy ra chế độ đang sử dụng
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Nếu ở Standard Mode
		ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= ccr_value & 0xFFF;	//0xFFF: CCR[11:0] của CCR Register (If Standard Mode)
	}
	else
	{
		//Nếu ở Fast Mode
		tempreg |= ( 1 << 15);	//Đưa bit field F/S[15] lên 1 của CCR Register, kích hoạt chế độ Fast Mode
		tempreg |= (pI2CHandle->I2C_Config.I2C_FmDutyCycle << 14); //Set value Duty đã đc user input khi init
		//vào bit field DUTY[14]

		//Kiểm tra DUTY đang ở trạng thái nào để áp dụng công thức
		if(pI2CHandle->I2C_Config.I2C_FmDutyCycle == I2C_FM_DUTY_2)
		{
			//Nếu Duty = 0
			ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			//Nếu Duty = 1
			ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}

		tempreg |= ccr_value & 0xFFF;	//0xFFF: CCR[11:0] của CCR Register (If Fast Mode)
	}
	pI2CHandle->pI2Cx->CCR |= tempreg;		//Cấu hình thanh ghi CCR cho processor

	/* TRISE Configuration TRISE register (I2C_TRISE) */
	//Kiểm tra speed đc cài đặt đầu vào ở tần số nào từ đó suy ra chế độ đang sử dụng
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Nếu ở Standard Mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;	//công thức datasheet
	}
	else
	{
		//Nếu ở Fast Mode
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1; //công thức datasheet
	}
	//0x3F: 0011 1111, TRISE[5:0] của TRISE Register
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);		//Cấu hình thanh ghi TRISE cho processor
}

/*********************************************************************
 * @fn      	- I2C_MasterSendData
 *
 * @brief       - Hàm xử lý gửi data từ Master
 *
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @param[in]  	- pTxbuffer : Nội dung data muốn truyền, thường là mảng ký tự
 * @param[in]  	- Len: Độ dài dữ liệu muốn gửi
 * @param[in]  	- SlaveAddr : Địa chỉ của Slave muốn gửi đến
 * @param[in]  	- Sr : Sử dụng hay không sử dụng Stop Condition
 *
 * @return      - none
 *
 * @Note        - none
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate the Start Condition - Tạo điều kiện START
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that START generation is complete by checking the SB flag in the SR1
	//Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	//2. Xác nhận rằng quá trình tạo START đã hoàn tất bằng cách kiểm tra cờ SB trong SR1
	//Lưu ý: Cho đến khi SB bị xóa SCL sẽ bị kéo gián đoạn (kéo về mức LOW)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the Slave with r/nw bit set to w(0) (total 8 bits)
	//3. Gửi tới địa chỉ của Slave với bit r/nw được đặt thành w(0) (tổng cộng 8 bit)
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	//4. Xác nhận rằng "address phase" đã hoàn tất bằng cách kiểm tra cờ ADDR trong SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Clear the ADDR flag according to its software sequence
	//Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	//5. Xóa cờ ADDR theo trình tự phần mềm của nó
	//Lưu ý: Cho đến khi xóa bit field ADDR, SCL sẽ bị kéo dài gián đoạn (kéo về Low)
	I2C_ClearADDRFlag(pI2CHandle);

	//6. Send the Data until Len becomes 0 - Gửi data cho đến khi Len==0
	while(Len > 0)				//Kiểm tra độ dài của Data(Đã gửi hết data trong pTxBuffer hay chưa)
	{
		//Kiểm tra cờ Txe đã đc set, cho phép đc gửi data đi
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));    //Wait till TxE is set

		//Đưa nội dung data và DR Register
		pI2CHandle->pI2Cx->DR = *pTxbuffer; 		//Cấu hình thanh ghi DR cho processor(Data in DR)
		pTxbuffer++;								//Mỗi lần gửi 8bit data. tăng đến ký tự data tiếp theo
		Len--;										//Giảm 1 đơn vị data(đã truyền 1 ký tự)
	}

	//7. When Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//Note: TXE=1, BTF=1, means that both SR and DR are empty and next transmission should begin
	//when BTF=1 SCL will be stretched(pulled to LOW)
	//7. Khi Len trở về 0, hãy đợi TXE=1 và BTF=1 trước khi tạo điều kiện STOP
	//Lưu ý: TXE=1, BTF=1, có nghĩa là cả SR và DR đều trống và quá trình truyền tiếp theo sẽ bắt đầu
	//khi BTF=1 SCL sẽ bị kéo giãn gián đoạn (kéo xuống THẤP)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate STOP condition and master need not to wait for the completion of STOP condition
	//Note: Generating STOP, automatically clears the BTF
	//8. Tạo điều kiện STOP và Master không cần đợi hoàn thành điều kiện STOP
	//Lưu ý: Tạo STOP, tự động xóa BTF
	if(Sr == I2C_DISABLE_SR)
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

/*********************************************************************
 * @fn      	- I2C_MasterReceiveData
 * @brief       - Hàm xử lý gửi data từ Master
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 *  			- pRxbuffer : Nội dung data đc nhận, thường là mảng ký tự
 *   			- Len: Độ dài dữ liệu đc nhận
 *   			- SlaveAddr : Địa chỉ của Slave gửi data đến Master
 * @return      - none
 * @Note        - pRxbuffer: vì là con trỏ nên khi nhận đc data từ Slave sẽ nhúng trực tiếp vào
 * 				địa chỉ của con trỏ nên ko cần thiết lập biến trả về
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate the START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm the start generation is completed by the checking the SB flag in the SR1
	//Note: Until SB is cleared SCL will be stretched (pulled to LOW)
	//Xác nhận việc tạo bắt đầu được hoàn thành bằng cách kiểm tra cờ SB trong SR1
	//Lưu ý: Cho đến khi SB bị xóa SCL sẽ bị kéo giãn (kéo về THẤP)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the Slave with r/nw bit set to R(1) (total 8 bits)
	//Gửi địa chỉ của Slave(gửi data đến Master) với bit r/nw được đặt thành R(1) (tổng cộng 8 bit)
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);

	//4. Wait until address phase is completed by checking the ADDR flag in the SR1
	//Đợi cho đến khi hoàn thành giai đoạn địa chỉ bằng cách kiểm tra cờ ADDR trong SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//TH1: Procedure to read only 1 byte from Slave(Slave gửi đến Master 1byte data)
	if(Len == 1)
	{
		//Disable Acking, thông báo cho Master ko còn data tiếp theo sau khi đã nhận 1 byte data
		I2C_ManageAcking(pI2CHandle->pI2Cx,I2C_ACK_DISABLE);

		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//Wait until bit field RxNE become 1
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx,I2C_FLAG_RXNE));

		//Generate STOP condition
		if(Sr == I2C_DISABLE_SR)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//Read data in to Buffer from DR Register
		*pRxbuffer = pI2CHandle->pI2Cx->DR;			//nhúng trực tiếp vào địa chỉ của param đưa vào
	}

	//TH2: Procedure to read n byte from Slave(Slave gửi đến Master nbyte data)
	if(Len > 1)
	{
		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//Read the data until(cho tới khi) Len becomes zero
		for(uint32_t i = Len; i > 0 ; i-- )
		{
			//Wait until RxNE becomes 1
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			//If last 2 byte are remaining
			if(i == 2)
			{
				//Clear the ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);

				//Generate STOP Condition
				if(Sr == I2C_DISABLE_SR)
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			//Read the Data from Data Register in to Buffer
			*pRxbuffer = pI2CHandle->pI2Cx->DR;

			//Increment(tăng) the Buffer address, đi đến byte data kế tiếp
			pRxbuffer++;
		}
	}

	//Re-enable ACKing, nếu user cho phép Slave->Master
	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		//thông báo cho phép Master tiếp tục nhận data từ Slave ở lần kết nối sau
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}
}

/*********************************************************************
 * @fn      	- I2C_GenerateStartCondition
 *
 * @brief       - Khởi tạo START Condition
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C peripheral của Processor
 *
 * @return      - none
 *
 * @Note        - I2C_CR1, bit field START[8]
 */
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	//Set bit field START[8] lên high
	pI2Cx->CR1 |= (1 << I2C_CR1_START);		//Cấu hình thanh ghi CR1 cho processor
}

/*********************************************************************
 * @fn      	- I2C_GetFlagStatus
 *
 * @brief       - Kiểm tra trạng thái các flag trong thanh ghi SR
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - FlagName: trạng thái tại vị trí flag muốn kiểm tra
 *
 * @return      - Trạng thái cờ_flag
 *
 * @Note        - none
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)  //Kiểm tra value của pI2Cx->SR1 and với FlagName
	{
		//Khi pI2Cx->SR1 & FlagName == 1 thì thực hiện if
		return FLAG_SET;		//Có cờ flag tại vị trí đó đang hoạt động 1
	}
	return FLAG_RESET;		//Nếu ko có cờ flag thì trả về 0
}

/*********************************************************************
 * @fn      	- I2C_ExecuteAddressPhaseWrite
 *
 * @brief       - Giai đoạn thực thi xác nhận địa chỉ của Slave(Master->Slave)
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - SlaveAddr: địa chỉ Slave mà Master muốn gửi Data đến
 *
 * @return      - none
 *
 * @Note        - SlaveAddr : sẽ đc cài đặt ở phía phần cứng làm Slave
 */
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	//Tiến hành dịch biến SlaveAddr lên 1 bit vì bit0 sẽ chứa giá trị thực hiện việc read hoặc write,
	SlaveAddr = SlaveAddr << 1;			//và bit1->7chứa data địa chỉ(ko làm mất data addr khi set bit0)
	SlaveAddr &= ~(1); 					//thiết lập bit0 = 0 = write(gửi data từ master->slave)
										//SlaveAddr: Slave Addrress + r/nw bit[0]=0

	pI2Cx->DR = SlaveAddr;				//Cấu hình thanh ghi DR cho processor DR[7:0]
}

/*********************************************************************
 * @fn      	- I2C_ExecuteAddressPhaseRead
 *
 * @brief       - Giai đoạn thực thi xác nhận địa chỉ của Slave(Slave->Master)
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - SlaveAddr: địa chỉ Slave muôn gửi Data đến Master
 *
 * @return      - none
 *
 * @Note        - SlaveAddr : sẽ đc cài đặt ở phía phần cứng làm Slave
 */
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	//Tiến hành dịch biến SlaveAddr lên 1 bit vì bit0 sẽ chứa giá trị thực hiện việc read hoặc write,
	SlaveAddr = SlaveAddr << 1;			//và bit1->7chứa data địa chỉ(ko làm mất data addr khi set bit0)
	SlaveAddr |= 1; 					//thiết lập bit0 = 1 = read(gửi data từ slave->master)
										//SlaveAddr: Slave Addrress + r/nw bit[0]=1

	pI2Cx->DR = SlaveAddr;				//Cấu hình thanh ghi DR cho processor DR[7:0]
}

/*********************************************************************
 * @fn      	- I2C_ClearADDRFlag
 *
 * @brief       - Xóa cờ flag của bit field ADDR của SR1,SR2 Register
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 *
 * @return      - none
 *
 * @Note        - Đạt đc EVx Condition
 */
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummyRead;							//biến tạm lưu value
//	uint32_t dummyRead = pI2Cx->SR1;			//biến tạm lưu value SR1 Register 32bit
//	dummyRead = pI2Cx->SR2;						//biến tạm lưu value SR2 Register 32bit
//	(void)dummyRead;							//Tránh trình biên dịch tối ưu hóa vì biến đc sử dụng

	//Việc đọc(gán) giá trị của thanh ghi SR1 và SR2 sẽ làm xóa cờ ADDR.
	//Việc bỏ qua giá trị của biến tạm là để tránh lãng phí tài nguyên bộ nhớ.

	//New(V224)
	//Check for device mode
	if(pI2CHandle->pI2Cx->SR2 && (1 << I2C_SR2_MSL))
	{
		//device is in Master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxLen == 1)
			{
				//first disable the ack - vô hiệu hóa ack
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
				//vô hiệu hóa vì ở hàm I2C_MasterReceiveData thì ở TH1 len==1 ta đã vô hiệu hóa
				//nhưng lúc đó có thể ko thuộc I2C_BUSY_IN_RX nên cần làm lại

				//clear the ADDR flag (read SR1, read SR2)
				dummyRead = pI2CHandle->pI2Cx->SR1;			//biến tạm lưu value SR1 Register 32bit
				dummyRead = pI2CHandle->pI2Cx->SR2;			//biến tạm lưu value SR2 Register 32bit
				(void)dummyRead;				//Tránh trình biên dịch tối ưu hóa vì biến đã đc sử dụng
			}
		}
		else
		{
			//clear the ADDR flag (read SR1, read SR2)
			dummyRead = pI2CHandle->pI2Cx->SR1;			//biến tạm lưu value SR1 Register 32bit
			dummyRead = pI2CHandle->pI2Cx->SR2;			//biến tạm lưu value SR2 Register 32bit
			(void)dummyRead;				//Tránh trình biên dịch tối ưu hóa vì biến đã đc sử dụng
		}
	}
	else
	{
		//device is in Slave mode
		//clear the ADDR flag (read SR1, read SR2)
		dummyRead = pI2CHandle->pI2Cx->SR1;			//biến tạm lưu value SR1 Register 32bit
		dummyRead = pI2CHandle->pI2Cx->SR2;			//biến tạm lưu value SR2 Register 32bit
		(void)dummyRead;				//Tránh trình biên dịch tối ưu hóa vì biến đã đc sử dụng
	}

}

/*********************************************************************
 * @fn      	- I2C_GenerateStopCondition
 *
 * @brief       - Khởi tạo STOP Condition
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 *
 * @return      - none
 *
 * @Note        - I2C_CR1, bit field STOP[9]
 */
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	//Set bit field STOP[9] lên high
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);		//Cấu hình thanh ghi CR1 cho processor
}

/*********************************************************************
 * @fn      	- I2C_ManageAcking
 *
 * @brief       - Quản lý cho phép cấp bit Ack từ Master(Processor)
 *
 * @param[in]   - pI2Cx: con trỏ là địa chỉ I2Cx muốn điều khiển
 * @param[in]  	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp Ack
 *
 * @return      - none
 *
 * @Note        - Việc Enable hoặc Disable sẽ do user cài đặt
 * 0: No acknowledge returned
   1: Acknowledge returned after a byte is received (matched address or data)
   Mặc định khi Slave->Master từ Ack=1
 */
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//Enable the Ack(set bit)
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);	//Cấu hình thanh ghi CR1[ACK] cho processor
	}
	else
	{
		//Disable the Ack(Clear bit)
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);	//Cấu hình thanh ghi CR1[ACK] cho processor
	}
}

/*********************************************************************
 * @fn      	- I2C_PeripheralControl
 *
 * @brief       - Cho phép I2Cx có đc hoạt động hay không, bit PE[0] của I2C_CR1
 *
 * @param[in]   - pI2Cx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]  	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return      - none
 *
 * @Note        - none
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
		//pI2cBaseAddress->CR1 |= I2C_CR1_PE_Bit_Mask;
	}else
	{
		pI2Cx->CR1 &= ~(1 << 0);
	}
}

/*********************************************************************
 * @fn      	- I2C_MasterSendDataIT
 * @brief       - Hàm xử lý gửi data từ Master for Interrupt
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 *  			- pRxbuffer : Nội dung data đc nhận, thường là mảng ký tự
 *   			- Len: Độ dài dữ liệu đc nhận
 *   			- SlaveAddr: Địa chỉ của Slave gửi data đến Master
 *   			- Sr : Lưu trữ giá trị bắt đầu lặp lại
 * @return      -
 * @Note        - Hàm cấu hình thanh ghi khi muốn yêu cầu ngắt
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len,
		uint8_t SlaveAddr,uint8_t Sr)
{
	//Đọc hành động thực hiện của I2C Peripheral thông qua biến
	uint8_t busystate = pI2CHandle->TxRxState;

	//Kiểm tra trạng thái muốn hành động có nằm trong hai trường hợp mong muốn
	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		//Cấu hình trạng thái gửi trong trường hợp sử dụng ngắt
		pI2CHandle->pTxBuffer = pTxbuffer;			//Nội dung data cần gửi bằng Interrupt
		pI2CHandle->TxLen = Len;					//Độ dài nội dung cần gửi bằng Interrupt
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;		//Xác định trạng thái busy là TX bằng interrupt
		pI2CHandle->DevAddr = SlaveAddr;			//Địa chỉ Slave muốn gửi data tới bằng Interrput
		pI2CHandle->Sr = Sr;						//Lưu trữ giá trị bắt đầu lặp lại bằng Interrupt

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		/*Cấu hình các bit cần thiết cho việc I2C Interrupt*/
		//Implement the code to enable ITBUFEN(Buffer interrupt enable) Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);		//Set bit ITBUFEN của thanh ghi CR2

		//Implement the code to enable ITEVTEN(Event interrupt enable) Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);		//Set bit ITEVTEN của thanh ghi CR2


		//Implement the code to enable ITERREN(Error interrupt enable) Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);		//Set bit ITERREN của thanh ghi CR2
	}

	return busystate;							//Trả về trạng thái thực hiện hành động
}

/*********************************************************************
 * @fn      	- I2C_MasterReceiveDataIT
 * @brief       - Hàm xử lý gửi data từ Master for Interrupt
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 *  			- pRxbuffer : Nội dung data đc nhận, thường là mảng ký tự
 *   			- Len: Độ dài dữ liệu đc nhận
 *   			- SlaveAddr: Địa chỉ của Slave gửi data đến Master
 *   			- Sr: Lưu trữ giá trị bắt đầu lặp lại
 * @return      -
 * @Note        - pRxbuffer: vì là con trỏ nên khi nhận đc data từ Slave sẽ nhúng trực tiếp vào
 * 				địa chỉ của con trỏ nên ko cần thiết lập biến trả về
 * 				- Hàm cấu hình thanh ghi khi muốn yêu cầu ngắt
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer,
		uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{
	//Đọc hành động thực hiện của I2C Peripheral thông qua biến
	uint8_t busystate = pI2CHandle->TxRxState;

	//Kiểm tra trạng thái muốn hành động có nằm trong hai trường hợp mong muốn
	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		//Cấu hình trạng thái nhận trong trường hợp sử dụng ngắt
		pI2CHandle->pTxBuffer = pRxbuffer;			//Nội dung data sẽ nhận bằng Interrupt
		pI2CHandle->TxLen = Len;					//Độ dài nội dung sẽ nhận bằng Interrupt
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;		//Xác định trạng thái busy là RX bằng interrupt
		pI2CHandle->DevAddr = SlaveAddr;			//Địa chỉ Slave gửi data đến Master tới bằng Interrput
		pI2CHandle->Sr = Sr;						//Lưu trữ giá trị bắt đầu lặp lại bằng Interrupt

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		/*Cấu hình các bit cần thiết cho việc I2C Interrupt*/
		//Implement the code to enable ITBUFEN((Buffer interrupt enable)) Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);		//Set bit ITBUFEN của thanh ghi CR2

		//Implement the code to enable ITEVTEN( Event interrupt enable) Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);		//Set bit ITEVTEN của thanh ghi CR2


		//Implement the code to enable ITERREN(Error interrupt enable) Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);		//Set bit ITERREN của thanh ghi CR2
	}

	return busystate;							//Trả về trạng thái thực hiện hành động
}

/*********************************************************************************
 * @fn			- I2C_IRQInterruptConfig
 *
 * @brief		- Config(Cấu hình) Interrupt Status for I2Cx
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- EnorDi : trạng thái Enable hoặc Disable cho việc sử dụng ngắt
 *
 * @return		- none
 *
 * @Note 		- none
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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
 * @fn			- I2C_IRQPriorityConfig
 *
 * @brief		- Set Priority(ưu tiên) Interrupt Status for I2Cx
 *
 * @param[in]	- IRQNumber: ngắt số mấy(Datasheet và define in stm32f407xx.h)
 * @param[in]	- IRQPriority: mức ưu tiên của ngắt mong muốn
 *
 * @return		- none
 *
 * @Note 		- none
 */
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
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

/*********************************************************************
 * @fn      	- I2C_EV_IRQHangling
 * @brief       - Hàm xử lý "Event" khi kích hoạt ngắt I2C Interrupt
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @return      - none
 * @Note        - Hàm thực thi Xử lý ngắt cho các sự kiện I2C gửi hoặc nhận (tham khảo SR1)
 */
void I2C_EV_IRQHangling(I2C_Handle_t *pI2CHandle)
{
	//Note: Interrupt handling for both master and slave mode of a device
	//Chú ý: Xử lý ngắt cho cả Master mode và Slave mode của một thiết bị đc chọn
	//Vd: phải thiết lập hành động ngắt cho STM32F407 ở cả hai chế độ Master và Slave

	//Kiểm tra các điều kiện tiên quyết là các "Enable Control Bit" đại diện cho Event hoặc Error có đc set
	//Đồng thời kiểm tra các cấu hình bit I2C Interrupt có đang đc set hay không
	//Nếu các điều kiện đủ thì Processor sẽ cho phép I2C Interrupt for Event hoặc Error đc kích hoạt
	//ITEVTEN(Event interrupt enable) là bit "Enable Control Bit" cho hành động Event(hàm này kiểm tra cho hành động Event)
	//ITBUFEN(Buffer interrupt enable) là bit "Enable Control Bit" cho hành động quản lý buffer có trống hay không

	int32_t temp1,temp2,temp3;									//Biến tạm chứa các trạng thái của cấu hình I2C
	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);	//Đọc và lưu trạng thái bit ITEVTEN vào biến
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);	//Đọc và lưu trạng thái bit ITBUFEN vào biến

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);			//Đọc và lưu trạng thái bit SB (Start bit-Master mode)

	//1. Handle For interrupt generated by SB event - Note : SB flag is only applicable in Master mode
	//Xử lý ngắt do sự kiện SB tạo ra - Lưu ý: Cờ SB chỉ áp dụng được ở chế độ Master
	if(temp1 && temp3)	//Nếu hai value này đều đang đc set lên 1
	{
		//SB flag is set
		//The Interrupt is generated because of SB event
		//This blcok will not be executed in Slave mode because for Slave Mode then bit SB is always zero
		//In this block lets executed the address phase
		//Ngắt được tạo ra do sự kiện SB
		//Khối "If" này sẽ không được thực thi ở chế độ Slave vì đối với Slave thì bit SB luôn bằng 0
		//Trong khối này cho phép thực thi "Address Phase"

		//Kiểm tra đang thực hiện Tx hay Rx để kích hoạt "ExecuteAddressPhase"
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			//Tx
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			//Rx
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);	//Đọc và lưu trạng thái bit ADDR(Address sent (master mode)/matched (slave mode))
	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	//Xử lý ngắt được tạo bởi sự kiện ADDR
	//Lưu ý: Khi ở chế độ Master: Địa chỉ được gửi
	// 		 Khi ở chế độ Slave: Địa chỉ khớp với địa chỉ của chính nó
	if(temp1 && temp3)	//Nếu hai value này đều đang đc set lên 1
	{
		//ADDR flag is set - ADDR: Làm cho Processor ở Master mode hoặc Slave Mode bị stretched
		//Xóa việc stretched để cho phép ngắt hành động
		I2C_ClearADDRFlag(pI2CHandle);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);	//Đọc và lưu trạng thái bit BTF(Byte transfer finished)
	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	//Xử lý ngắt được tạo bởi sự kiện BTF(Byte Transfer Finished)
	//Nghĩa là cứ mỗi lần nhận hoặc truyền 1 byte(1 ký tự) thì một hành động EV_IRQ sẽ đc kích hoạt
	//cho đến khi hoàn thành chuỗi data muốn thực hiện
	if(temp1 && temp3)	//Nếu hai value này đều đang đc set lên 1
	{
		//BTF flag is set - Xác nhận byte data cuối cùng đã đc truyền đi
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			//Tx - Make sure that TxE is also set
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE))
			{
				//BTF,TxE = 1 -> Close Data Tranfer Communication
				if(pI2CHandle->TxLen == 0)
				{
					//1.Generate the STOP Condition - Cài đặt ko tiếp tục sử dụng StartCondition từ User
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					//2.Reset all the member elements of the handle Structure
					//Đặt lại tất cả các phần tử thành viên của "handle Structure" of I2C
					I2C_CloseSendData(pI2CHandle);

					//3.Notify the application about transmission complete - Thông báo cho ứng dụng về việc truyền hoàn tất
					I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_TX_CMPLT);
				}
			}
		}
		//BTF flag is set - Xác nhận byte data cuối cùng đã đc nhận (2024/12/31)
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			//Rx - Make sure that RxNE is also reset
			/*if(! (pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RxNE)))
			{
				//BTF,RxE = 1 -> Close Data Tranfer Communication
				if(pI2CHandle->RxLen == 0)
				{
					//1.Generate the STOP Condition - Cài đặt ko tiếp tục sử dụng StartCondition từ User
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					//2.Reset all the member elements of the handle Structure
					//Đặt lại tất cả các phần tử thành viên của "handle Structure" of I2C
					I2C_CloseReceiveData(pI2CHandle);

					//3.Notify the application about transmission complete - Thông báo cho ứng dụng về việc nhận hoàn tất
					I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_RX_CMPLT);
				}
			}*/
			//Lưu ý: Trong bài này việc kiểm tra nhận data xong hay chưa phụ thuộc vào hàm nhận_ngắt
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);	//Đọc và lưu trạng thái bit STOPF(Stop detection (slave mode))
	//4. Handle For interrupt generated by STOPF event
	//Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	//Xử lý ngắt được tạo bởi sự kiện STOPF
	//Lưu ý: Cờ phát hiện dừng chỉ được áp dụng ở chế độ Slave. Đối với Master, cờ này sẽ không bao giờ được đặt
	if(temp1 && temp3)	//Nếu hai value này đều đang đc set lên 1
	{
		//STOPF flag is set in Slave mode
		//Clear the STOPF (B1:read SR1(đã làm ở trên) -> B2:Write to CR1)
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//Notify the application that STOP is detected(đc phát hiện)
		I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_STOP);

	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE);	//Đọc và lưu trạng thái bit TxE Event
	//5. Handle For interrupt generated by TXE event
	if(temp1 && temp2 && temp3)	//Nếu ba value này đều đang đc set lên 1
	{
		//MCU thực hiện gửi data khi nó đang ở Master Mode
		//Kiểm tra MCU đang ở chế độ Master hay Slave bằng cách kiểm tra bit MSL của I2C_SR2
		//Check device mode
		if(pI2CHandle->pI2Cx->SR2 && (1 << I2C_SR2_MSL))		//STM32 is Master mode
		{
			//TXE flag is set - We have to do the data transmission
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTEXInterrupt(pI2CHandle);
			}
		}
		else	//STM32 is Slave Mode
		{
			//Make sure that the Slave is really in transmitter mode
			if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
			{
				//Thông báo cho user là đang gửi data STM32 -> Master
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_REQ);
			}
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RxNE);	//Đọc và lưu trạng thái bit RxNE Event
	//6. Handle For interrupt generated by RXNE event
	if(temp1 && temp2 && temp3)	//Nếu ba value này đều đang đc set lên 1
	{
		//MCU thực hiện gửi data khi nó đang ở Master Mode
		//Kiểm tra MCU đang ở chế độ Master hay Slave bằng cách kiểm tra bit MSL của I2C_SR2
		//Check device mode
		if(pI2CHandle->pI2Cx->SR2 && (1 << I2C_SR2_MSL))	//STM32 is Master mode
		{
			//RxNE flag is reset - We have to do the data reception
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}
		else	//STM32 is Slave mode
		{
			//Make sure that the Slave is really in transmitter mode
			if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
			{
				//Thông báo cho user là đang nhận data Master->STM32
				I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_RCV);
			}
		}
	}

	//Lưu ý: điều kiện tiên trước để ngắt RxNE và TxE xảy ra(bit field TxE Event và RxNE event) là
	//các "Enable Control Bit" của cả "ITEVFEN và ITRRREN" đều đang đc set
}

/*********************************************************************
 * @fn      	- I2C_ER_IRQHangling
 * @brief       - Hàm xử lý "Error" khi kích hoạt ngắt I2C Interrupt(có lỗi khi sử dụng I2C peripheral)
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @return      - none
 * @Note        - Dựa vào các lỗi có thể có khi sử dụng I2C peripheral để code
 * 				- Sử dụng thanh ghi SR1 để kiểm tra trạng thái bit của các lỗi đó
 * 				- Hàm thực thi xử lý lỗi khi trong quá trình truyền bằng I2C có xăỷ ra lỗi
 */
void I2C_ER_IRQHangling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1,temp2;			//Biến tạm lưu các giá trị bit đc đọc

	//Know the status of  ITERREN control bit in the CR2
	//Đọc trạng thái của "Enable control bit" ITERREN trong CR2
	temp2 = (pI2CHandle->pI2Cx->CR2) & ( 1 << I2C_CR2_ITERREN);				//Lưu trạng thái even có đang bị lỗi xảy ra hay không

	//Lập trình theo các trường hợp lỗi có thể xuất hiện trong việc sử dụng I2C Peripheral

	/***********************1.Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag -Thực hiện mã để xóa cờ lỗi bus error
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

	/***********************2.Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_ARLO);
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);

	}

	/***********************3.Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

		//Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
	}

	/***********************4.Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

		//Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
	}

	/***********************5.Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

		//Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
	}
}

/*********************************************************************
 * @fn      	- I2C_MasterHandleTEXInterrupt
 * @brief       - Hàm hỗ trợ xử lý sụ kiện TXE trong hàm I2C_EV_IRQHangling
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @return      - none
 * @Note        - none
 */
static void I2C_MasterHandleTEXInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen > 0)	//nếu có từ 1byte trở lên
	{
		//1.Load the data in to DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);	//value của địa chỉ pTxBuffer

		//2.Decrement the TxLen
		pI2CHandle->TxLen--;

		//3.Increment the buffer address
		pI2CHandle->pTxBuffer++;
	}
}

/*********************************************************************
 * @fn      	- I2C_MasterHandleRXNEInterrupt
 * @brief       - Hàm hỡ trợ xử lý sụ kiện RXNE trong hàm I2C_EV_IRQHangling
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @return      - none
 * @Note        - none
 */
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->RxSize == 1)
	{
		//1.Load the data from DR->address RxBuffer
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;

		//2.Decrement the address RxLen
		pI2CHandle->RxLen--;
	}

	if(pI2CHandle->RxSize > 1)
	{
		if(pI2CHandle->RxLen == 2)
		{
			//clear the ack bit
			I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
		}
		//1.Load the data from DR->address RxBuffer
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;

		//2.Increment the buffer address
		pI2CHandle->pRxBuffer++;

		//3.Decrement the address RxLen
		pI2CHandle->RxLen--;
	}

	//Đã nhận hết data từ Slave
	if(pI2CHandle->RxSize == 0)
	{
		//Close the I2C data reception and notify the application

		//1.generate the stop condition
		if(pI2CHandle->Sr == I2C_DISABLE_SR)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//2.close the I2C Rx
		I2C_CloseReceiveData(pI2CHandle);

		//3.Notify the application
		I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_TX_CMPLT);
	}
}

/*********************************************************************
 * @fn      	- I2C_CloseReceiveData
 * @brief       - Hàm xử lý sự kiện đóng các thành phần I2C Interrupt
 * 				khi hoàn thành nhận Data
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @return      - none
 * @Note        - none
 */
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVFEN control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	//Cấu hình lại(giải phóng) I2C Handle Structure
	pI2CHandle->TxRxState = I2C_READY;			//Chuyển trạng thái về Bus empty
	pI2CHandle->pRxBuffer = NULL;				//Giải phóng RxBuffer
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize = 0;

	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
	}
}

/*********************************************************************
 * @fn      	- I2C_CloseSendData
 * @brief       - Hàm xử lý sự kiện đóng các thành phần I2C Interrupt
 * 				khi hoàn thành gửi Data
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 * @return      - none
 * @Note        - none
 */
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable IT control Bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	//Cấu hình lại(giải phóng) I2C Handle Structure - sẵn sàng cho event kế tiếp
	pI2CHandle->TxRxState = I2C_READY;			//Chuyển trạng thái về Bus empty
	pI2CHandle->pTxBuffer = NULL;				//Giải phóng xBuffer
	pI2CHandle->TxLen = 0;
}

/*********************************************************************
 * @fn      	- I2C_SLaveSendData
 * @brief       - Hàm xử lý sự kiện gửi data đến Master
 * @param[in]   - pI2Cx: con trỏ là địa chỉ I2Cx chứa cấu hình I2C Register
 * @param[in]	- data: nội dung dữ liệu muốn gửi
 * @return      - none
 * @Note        - Trường hợp STM32 đc cài đặt là Slave
 */
void I2C_SLaveSendData(I2C_RegDef_t *pI2C, uint8_t data)
{
	pI2C->DR = data;			//đưa nội dung data muốn gửi vào DR Register
}

/*********************************************************************
 * @fn      	- I2C_CloseSendData
 * @brief       - Hàm xử lý sự nhận data từ Master
 * @param[in]   - pI2Cx: con trỏ là địa chỉ I2Cx chứa cấu hình I2C Registe
 * @return      - Nội dung data nhận đc
 * @Note        - Trường hợp STM32 đc cài đặt là Slave
 */
uint8_t I2C_SlaveReceiveData(I2C_RegDef_t *pI2C)
{
	return (uint8_t)pI2C->DR;	//Nội dung data nhận đc từ Master vào DR Register
}

/*********************************************************************
 * @fn      	- I2C_SlaveEnableDisableCallBackEvents
 *
 * @brief       - Cho phép các "Enable Control bit" trong I2C_CR1 của I2Cx có đc
 * 				hoạt động hay không
 *
 * @param[in]   - pI2Cx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]  	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp xung
 *
 * @return      - none
 *
 * @Note        - none
 */
void I2C_SlaveEnableDisableCallBackEvents(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}
	else
	{
		pI2Cx->CR2 &= ~ (1 << I2C_CR2_ITEVTEN);
		pI2Cx->CR2 &= ~ (1 << I2C_CR2_ITBUFEN);
		pI2Cx->CR2 &= ~ (1 << I2C_CR2_ITERREN);
	}
}
