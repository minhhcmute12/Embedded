/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Dec 22, 2024
 *      Author: Minh
 */

#include "stm32f407xx_i2c_driver.h"

/****************************************************************************************
 *					Static Function supported by this driver							*
 *				Hàm Static chỉ đc phép sử dụng bên trong file mà nó đang chứa			*
 *				Ngăn chặn các lệnh từ file khác có thể gọi hàm xử lý					*
 ****************************************************************************************/
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);						//Tạo điều kiện START
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);	//Thực hiện việc truyền data
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr);		//Thực hiện việc đọc data
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);									//Xóa cờ bit ADDR thuộc I2C_SR1 Register
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);							//Tạo điều kiện STOP

/*
 * Khai báo Mảng chứa bộ chia có thể đc sử dụng trong System Clock
 */
uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint16_t APB1_PreScaler[4] = {2,4,8,16};

/*********************************************************************
 * @fn      	- I2C_PeriClockControl
 *
 * @brief       - Enable/Disable Clock cho I2C peripheral
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
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}
	}
	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}
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
 * @brief       - Xác định xung PCLK1 đc sử dụng từ System Clock cho I2C Peripheral
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
	uint32_t pclk1, SystemClk;						//biến lưu thông số xung clock
	uint8_t clksrc, temp;							//biến lưu xung clock cho khối system clock
	uint8_t ahbp, apb1p;							//biến lưu số chia xung clock

	//Kiểm tra xem bit SWS[3:2](System clock switch status) thuộc CFGR Register đang có giá trị bit là bao nhiêu
	clksrc = (RCC->CFGR >> 2) & 0x3;	//0x03=0011, nghĩa là đọc 2 vị trí bit liền nhau từ vị trí bit số 2

	//Từ thông số "clksrc", biện luận để xác định khối Oscillator nào đang đc sử dụng là System Clock(RM407->6.6.3)
	//1. Xác định tần số xung clock của System Clock đang đc sử dụng
	if(clksrc == 0)
	{
		SystemClk = 16000000;						//00 : HSI Oscillator
	}
	else if(clksrc == 1)
	{
		SystemClk = 8000000;						//01: HSE Oscillator
	}
	else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();		//10: PLL Oscillator
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
	temp = ((RCC->CFGR >> 10) & 0x7);	//0x7=0111, nghĩa là đọc 3 vị trí bit liền nhau từ vị trí bit số 10
	if(temp < 8)
	{
		apb1p = 1;			//Không sử dụng bộ chia
	}
	else
	{
		//Có sử dụng bộ chia, nếu temp=8 thì 8-8=0, sử dụng bộ chia 2,...
		apb1p = APB1_PreScaler[temp - 4];
	}

	//4. Xác định xung PCLK1 dùng cho I2C đc lấy từ System Clock (Kiểm tra sơ đồ khối về xung Clock)
	//SystemClk -> AHBP Prescaler -> APB1 PreScaler(Quản lý I2C Peripheral)
	pclk1 = (SystemClk / ahbp) / apb1p;

	return pclk1;
}

/******************************************************************************
 * @fn      	- I2C_Init
 *
 * @brief       - APIs I2C Peripheral initialization
 *
 * @param[in]   - pI2CHandle: cấu hình con trỏ I2Cx muốn xử lý công việc
 *
 * @return      - none
 *
 * @Note        - Từ thông số I2C_Config_t sẽ lưu trữ vào I2C_RegDef_t
 */
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;		//biến tạm 32 bit lưu thông số sẽ setup vào I2C_RegDef_t

	//1. Enable the Clock for the I2Cx peripheral - Kích hoạt xung clock cho I2Cx
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	//2. Lưu giá trị của I2C_AckControl(sẽ đc set khi init) vào vị trí bit thứ 10 của biến tempreg
	tempreg |= pI2CHandle->I2C_Config.I2C_AckControl << 10;					//Ack control bit

	//Cấu hình thanh ghi I2C_CR1 bit ACK[10]
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//3. Cấu hình bit field FREQ(Peripheral clock frequency) của I2C_CR2 register
	tempreg = 0;															//reset value biến
	tempreg |= RCC_GetPCLK1Value() / 1000000U;								//Lấy giá trị tần số của APB1 Bus
	/*
	 * Giải thích: Theo datasheet thì bit FREQ[5:0] có giá trị tần số sử dụng sẽ từ 2MHz ~ 50MHz
	 * Vd: giá trị trả về là 16MHz nên ta chia 1MHz để lấy giá trị 16, chương trình sẽ chuyển đổi 16 sang mã nhị phân
	 * và lưu vào biến tempreg[5:0]
	 */
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);								//0x3F=0011 1111
	/*
	 * Giải thích: tempreg & 0x3F dùng để lọc chỉ lấy 6 bit đầu của biến tempreg sau đó mới lưu vào thanh ghi
	 */

	//4. Cấu hình địa chỉ của thiết bị sử dụng OAR1 register với ADD[7:1] và bit[14]
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;				//Lưu địa chỉ từ bit thứ 2 của biến tempreg
	tempreg |= (1 << 14);													//Theo datasheet bắt buộc bit14 luôn là 1
	pI2CHandle->pI2Cx->OAR1 = tempreg;										//Cấu hình OAR1 register

	//5. CCR Calculations - Tính toán tần số cho Clock control register
	tempreg = 0;
	uint16_t ccr_value = 0;													//Biến lưu tạm tần số cấu hình

	//Kiểm tra SclkSpeep người dùng muốn sử dụng từ đó cấu hình clock sử dụng vào I2C_CCR register
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Lựa chọn Standard Mode
		ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		tempreg |= ccr_value & 0xFFF;										//0xFFF -> I2C_CCR[11:0]
	}
	else
	{
		//Lựa chọn Fast Mode
		tempreg |= (1 << 15);		//Cấu hình bit15 của tempreg để có thể set I2C_CCR bit F/S[15] lên 1 để cấu hình FS
		tempreg |= pI2CHandle->I2C_Config.I2C_FmDutyCycle << 14;			//Set value Duty ở bit14 để set I2C_CCR bit DUTY[14]

		//Kiểm tra giá trị DUTY để tính toán tần số
		if(pI2CHandle->I2C_Config.I2C_FmDutyCycle == I2C_FM_DUTY_2)
		{
			//Nếu DUTY = 0
			ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			//Nếu DUTY = 1
			ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}

		tempreg |= ccr_value & 0xFFF;										//0xFFF -> I2C_CCR[11:0]

	}
	pI2CHandle->pI2Cx->CCR = tempreg;										//Cấu hình thanh ghi I2C_CCR

	//6. TRISE Configuration TRISE register (I2C_TRISE bit TRISE[5:0]) - Kiểm tra speed đc cài đặt đầu vào từ đó tính toán thời gian rise time
	tempreg = 0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		//Nếu ở Standard Mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;						//Công thức datasheet
	}
	else
	{
		//Nếu ở Fast Mode
		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1; 			//Công thức datasheet
	}
	//0x3F: 0011 1111, TRISE[5:0] của TRISE Register, các bit còn lại Reserved nên ko cấu hình
	pI2CHandle->pI2Cx->TRISE = (tempreg & 0x3F);		//Cấu hình thanh ghi TRISE bit TRISE[5:0]
}

/*********************************************************************
 * @fn      	- I2C_MasterSendData
 *
 * @brief       - Hàm xử lý gửi data từ Master -> Slave
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
	//Xác nhận set bit START đã hoàn tất bằng cách kiểm tra cờ SB trong SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the Slave with r/nw bit set to w(0) (total 8 bits)
	//Gửi tín hiệu data bao gồm địa chỉ Slave muốn kết nối và bit r/nw -> tổng cộng 8 bit gửi
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);				//Vì gửi nên cần set bit r/nw = 0 = write(send)

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	//Xác nhận việc kết nối đc "matched" bằng cách kiểm tra ADDR flag của I2C_SR1 Register
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ADDR));

	//5. Clear the ADDR flag according to its software sequence - //Note: Until ADDR is cleared SCL will be stretched (pulled to LOW)
	//Xóa ADDR flag về 0 - Nếu không xóa thì SCL sẽ kéo dài ở mức Low và ko thể tiếp tục công việc gửi
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. Send the Data until Len become 0 - Gửi data cho đến khi hết độ dài data
	while(Len > 0)
	{
		//Kiểm tra SR1_TxE flag đã đc set hay chưa (SR1_TxE flag: khi đc set sẽ cho phép gửi data)
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TxE));		//Wait till TxE flag is set

		//Đưa nội dung data muốn gửi vào DR Register - Gửi từng byte(mỗi byte là 1 chữ cái hoặc ký tự đc gửi)
		pI2CHandle->pI2Cx->DR = *pTxbuffer;						//Cấu hình DR register với từng byte *pTxbuffer
		pTxbuffer++;											//Tăng địa chỉ byte tiếp theo đc gửi
		Len--;													//Giảm độ dài của Data muốn gửi (đã truyền xong 1 ký tự)
	}

	//7. When Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//Note1: TXE=1, BTF=1(Byte transfer finished), means that both SR and DR are empty and next transmission should begin
	//Note2: When BTF=1 SCL will be stretched(pulled to LOW)
	//Khi Len trở về 0, hãy đợi TXE=1 và BTF=1 trước khi tạo điều kiện STOP
	//Lưu ý 1: TXE=1, BTF=1, có nghĩa là cả SR và DR đều trống và quá trình truyền tiếp theo có thể bắt đầu
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_TxE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_BTF));		//BTF - Byte transfer finished

	//8. Generate STOP condition and master need not to wait for the completion of STOP condition
	//Tạo điều kiện STOP và Master sẽ không cần đợi hoàn thành điều kiện STOP
	//Lưu ý: Khi thiết lập STOP condition thì SR1_BTF flag sẽ tự động đc xóa.
	if(Sr == I2C_DISABLE_SR)
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

	/*
	 * Lưu ý : Khi BTF=1 thì SCL sẽ bị kéo giãn gián đoạn (kéo xuống THẤP) -> Có 2 trường hợp xảy ra:
	 * 1. Tạo điều kiện STOP -> Lúc này I2C_BTF flag sẽ đc tự động xóa
	 * 2. Tiếp tục kết nối truyền data
	 * => Các cách xóa BTF flag:
	 *  ^ Read hoặc Write khi đưa data vào DR Register
	 *  ^ Sau khi thiết lập START hoặc STOP condition
	 *  ^ PE(Peripheral I2C enable)=0=disable
	 */
}

/*********************************************************************
 * @fn      	- I2C_MasterReceiveData
 *
 * @brief       - Hàm xử lý nhận data từ Slave -> Master
 *
 * @param[in]   - pI2CHandle: Con trỏ là chứa cấu hình I2C peripheral
 *  			- pRxbuffer : Nội dung data đc nhận, thường là mảng ký tự
 *   			- Len: Độ dài dữ liệu đc nhận
 *   			- SlaveAddr : Địa chỉ của Slave gửi data đến Master
 *
 * @return      - none
 *
 * @Note        - pRxbuffer: vì là con trỏ nên khi nhận đc data từ Slave sẽ lưu value trực tiếp vào
 * 				địa chỉ của con trỏ nên ko cần thiết lập biến trả về
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxbuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{
	//1. Generate the START Condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that START generation is complete by checking the SB flag in the I2C_SR1
	//Xác nhận set bit START đã hoàn tất bằng cách kiểm tra cờ SB trong I2C_SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the Slave with r/nw bit set to r(1) (total 8 bits)
	//Gửi địa chỉ của Slave(gửi data đến Master) với bit r/nw được đặt thành r(1) -> tổng cộng 8 bit gửi
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);				//Vì gửi nên cần set bit r/nw = 1 = read(receive)

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	//Xác nhận việc kết nối đc "matched" bằng cách kiểm tra ADDR flag của I2C_SR1 Register
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SR1_ADDR));

	//5. Receive the Data
	//TH1: Procedure to read only 1 byte from Slave - Slave gửi đến Master chỉ 1byte data
	if(Len == 1)
	{
		//Disable Acking, thông báo cho Master ko còn data tiếp theo sau khi đã nhận 1 byte data
		I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);				//Muốn ngắt kết nối Slave->Master

		//Clear the ADDR flag - Xóa địa chỉ Slave đc kết nối
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//Wait until bit field RxNE become 1
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));		//1: Data register not empty -> Cho phép tiếp tục nhận data

		//Generate STOP condition
		if(Sr == I2C_DISABLE_SR)
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//Read data in to Buffer from DR Register - Lưu data nhận đc từ Slave vào trong con trỏ
		*pRxbuffer = pI2CHandle->pI2Cx->DR;
	}

	//TH2: Procedure to read n byte from Slave(Slave gửi đến Master nhiều hơn 1 byte data)
	if(Len > 1)
	{
		//Clear the ADDR flag - Xóa địa chỉ Slave đc kết nối
		I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

		//Read the data until(cho tới khi) Len becomes zero
		for(uint32_t i = Len; i > 0 ; i-- )
		{
			//Wait until RxNE becomes 1 - Cho phép tiếp tục nhận data
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

			//Read the Data from Data Register in to Buffer - Lưu data nhận đc từ Slave vào trong con trỏ
			*pRxbuffer = pI2CHandle->pI2Cx->DR;

			//Increment(tăng) the Buffer address - Tăng địa chỉ đi đến byte data đc nhận kế tiếp
			pRxbuffer++;
		}
	}

	//Re-enable ACKing- Thiết lập lại bit ACKing để chuẩn bị cho phép Slave->Master ở lần tiếp theo
	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		//Thông báo cho phép Master tiếp tục nhận data từ Slave ở lần kết nối sau
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
	//Set bit START[8] của I2C_CR1
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

/*********************************************************************
 * @fn      	- I2C_GetFlagStatus
 *
 * @brief       - Kiểm tra trạng thái các flag trong thanh ghi SR1
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - FlagName: trạng thái tại vị trí bit_flag muốn kiểm tra
 *
 * @return      - Trạng thái cờ_flag
 *
 * @Note        - none
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	//Kiểm tra value của pI2Cx->SR1 and với FlagName
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;			//Có cờ flag tại vị trí đó là 1
	}
	return FLAG_RESET;				//Có cờ flag tại vị trí đó là 0

	/*
	 * Lưu ý: I2C trong STM32F4 có đến 2 thanh ghi trạng thái là SR1 và SR2
	 * Vì vậy nếu cần kiểm tra trạng thái cờ bit thuộc SR2 thì ta nên viết thêm một hàm kiểm tra SR2
	 */
}

/*********************************************************************
 * @fn      	- I2C_GetFlagStatusSR2
 *
 * @brief       - Kiểm tra trạng thái các flag trong thanh ghi SR2
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - FlagName: trạng thái tại vị trí bit_flag muốn kiểm tra
 *
 * @return      - Trạng thái cờ_flag
 *
 * @Note        - none
 */
uint8_t I2C_GetFlagStatusSR2(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	//Kiểm tra value của pI2Cx->SR1 and với FlagName
	if(pI2Cx->SR2 & FlagName)
	{
		return FLAG_SET;			//Có cờ flag tại vị trí đó là 1
	}
	return FLAG_RESET;				//Có cờ flag tại vị trí đó là 0
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
	//Tiến hành dịch trái biến SlaveAddr 1 đơn vị vì bit0 sẽ đc cấu hình cho yêu cầu write(send) hoặc read(receiver)
	SlaveAddr = SlaveAddr << 1;					//bit field addr[7:1] -> sẽ chứa địa chỉ SlaveAddr
	SlaveAddr &= ~(1);							//hoặc: SlaveAddr &= ~(1 << 0);
	/*
	 * Giải thích: thiết lập bit0 = 0 = write(gửi data từ master->slave)
	 * Data of SlaveAddr: Slave Addrress[7:1] + r/nw bit[0]=0
	 */
	pI2Cx->DR = SlaveAddr;						//Cấu hình DR[7:0]-> Đưa data vào DR để gửi data đi
}

/*********************************************************************
 * @fn      	- I2C_ExecuteAddressPhaseRead
 *
 * @brief       - Giai đoạn thực thi xác nhận địa chỉ của Slave(Slave->Master)
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 * @param[in]   - SlaveAddr: địa chỉ Slave muốn gửi Data đến Master
 *
 * @return      - none
 *
 * @Note        - SlaveAddr : sẽ đc cài đặt ở phía phần cứng làm Slave
 */
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx,uint8_t SlaveAddr)
{
	//Tiến hành dịch trái biến SlaveAddr 1 đơn vị vì bit0 sẽ đc cấu hình cho yêu cầu write(send) hoặc read(receiver)
	SlaveAddr = SlaveAddr << 1;					//bit field addr[7:1] -> sẽ chứa địa chỉ SlaveAddr
	SlaveAddr |= 1; 							//hoặc: SlaveAddr |= (1 << 0);
	/*
	 * Giải thích: thiết lập bit0 = 1 = read(gửi data từ slave->master)
	 * Data of SlaveAddr: Slave Addrress[7:1] + r/nw bit[0]=1
	 */

	pI2Cx->DR = SlaveAddr;				//Cấu hình DR[7:0]-> Đưa data vào DR để gửi data đi
}

/*********************************************************************
 * @fn      	- I2C_ClearADDRFlag (Interface address)
 *
 * @brief       - Xóa cờ flag của bit field ADDR của SR1 Register
 *
 * @param[in]   - pI2Cx: con trỏ là cấu hình I2C Register của Processor
 *
 * @return      - none
 *
 * @Note        - Đạt đc EVx Condition - Xóa cờ này là cần thiết để I2C có thể nhận các sự kiện tiếp theo
 */
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	//Cách 1: Gián tiếp
	uint32_t dummyRead = pI2Cx->SR1;			//biến tạm lưu value SR1 Register 32bit
	dummyRead = pI2Cx->SR2;						//biến tạm lưu value SR2 Register 32bit
	(void)dummyRead;							//Ngăn chặn trình biên dịch tối ưu hóa vì biến đc sử dụng
	//Việc đọc(gán) giá trị của thanh ghi SR1 và SR2 sẽ làm xóa cờ ADDR.
	//Bằng cách ép kiểu dummyRead thành void, chúng ta cho trình biên dịch biết rằng chúng ta không quan tâm đến giá trị
	//đọc được từ các thanh ghi trạng thái -> Nghĩa là bỏ qua giá trị của biến tạm là để tránh lãng phí tài nguyên bộ nhớ.
	//Lưu ý: trong một số trường hợp cách 1 sẽ ko thể xóa ADDR flag

	//Cách 2: Trực tiếp
	//pI2Cx->SR1 &= ~(1 << I2C_SR1_ADDR);
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
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	//Set bit field STOP[9] lên high
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);		//Cấu hình thanh ghi CR1 cho processor
}

/*********************************************************************
 * @fn      	- I2C_ManageAcking
 *
 * @brief       - Quản lý cho phép cấp bit Ack từ Master(Processor)
 *
 * @param[in]   - pI2Cx: con trỏ là địa chỉ SPIx muốn điều khiển xung clock đến
 * @param[in]  	- EnorDi : trạng thái Enable hoặc Disable cho việc cấp Ack
 *
 * @return      - none
 *
 * @Note        - Việc Enable hoặc Disable sẽ do user cài đặt
 *

 */
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//Enable the Ack(set bit) -> 1: "Acknowledge returned" after a byte is received (matched address or data)
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);				//Cấu hình thanh ghi CR1[ACK] cho processor
	}
	else
	{
		//Disable the Ack(Clear bit) -> 0: No acknowledge returned
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
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
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}

}
