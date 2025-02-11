/*
 * stm32f407xx_rcc_driver.c
 *
 *  Created on: Jan 27, 2024
 *      Author: Minh
 */

#include "stm32f407xx_rcc_driver.h"

//Mảng chứa bộ chia có thể đc sử dụng trong System Clock
uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint16_t APB1_PreScaler[4] = {2,4,8,16};

/*****************************************************************************
 * @fn      	- RCC_GetPCLK1Value
 *
 * @brief       - Xác định xung PCLK1(APB1 Bus) đc sử dụng từ System Clock
 *
 * @param[in]   - none
 *
 * @return      - pclk1: xung clock đc sử dụng trong peripheral
 *
 * @Note        - Sử dụng sơ đồ khối liên quan đến xung Clock của System Clock
 * 				- Thanh ghi CFGR thuộc RCC Register đc sử dụng
 */
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1,SystemClk;					//biến tạm xung clock

	uint8_t clksrc,temp,ahbp,apb1;				//biến xung clock cho khối system clock

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
		apb1 = 1;			//Không sử dụng bộ chia
	}
	else
	{
		//Có sử dụng bộ chia, nếu temp=8 thì 8-8=0, sử dụng bộ chia 2,...
		apb1 = APB1_PreScaler[temp - 4];
	}

	//4. Xác định xung PCLK1 dùng cho I2C đc lấy từ từ System Clock
	pclk1 = (SystemClk / ahbp) / apb1;

	return pclk1;
}


/*****************************************************************************
 * @fn      	- RCC_GetPCLK2Value
 *
 * @brief       - Xác định xung PCLK2(APB2 Bus) đc sử dụng từ System Clock
 *
 * @param[in]   - none
 *
 * @return      - pclk1: xung clock đc sử dụng trong Peripheral
 *
 * @Note        - Sử dụng sơ đồ khối liên quan đến xung Clock của System Clock
 * 				- Thanh ghi CFGR thuộc RCC Register đc sử dụng
 */
uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t SystemClock=0,tmp,pclk2;

	uint8_t clk_src = ( RCC->CFGR >> 2) & 0X3;

	uint8_t ahbp,apb2p;

	if(clk_src == 0)
	{
		SystemClock = 16000000;
	}
	else
	{
		SystemClock = 8000000;
	}

	tmp = (RCC->CFGR >> 4 ) & 0xF;

	if(tmp < 0x08)
	{
		ahbp = 1;
	}
	else
	{
	   ahbp = AHB_PreScaler[tmp-8];
	}

	tmp = (RCC->CFGR >> 13 ) & 0x7;
	if(tmp < 0x04)
	{
		apb2p = 1;
	}
	else
	{
		apb2p = APB1_PreScaler[tmp-4];
	}

	pclk2 = (SystemClock / ahbp )/ apb2p;

	return pclk2;
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
