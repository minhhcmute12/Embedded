/*
 * main.c
 *
 *  Created on: 2021/01/19
 *      Author: Minh
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//Tao cau truc hoc sinh chua cac thong tin
typedef struct
{
	int 	rollNumber;		//ma so ca nhan
	char 	name[100];		//ten
	char 	branch[50];		//chi nhanh
	char 	dob[15];		//ngay thang nam sinh
	int 	semister;		//vi tri
}STUDENT_INFO_t;

int const max_record = 2;

STUDENT_INFO_t student[2] ={
		{9876,"ashokar","mechanical","12/11/1999",7},
		{8888,"ram kumar","compter","12/11/1999",7}
};		//mang cau truc co hai phan tu
//moi student se duoc luu trong tu dia chi,bat dau bang student[0]

void display_all_records(STUDENT_INFO_t *pBaseAddofRecord,int max_record)
{
	/*printf("Roll number of the element of the array = %d\n",pBaseAddofRecord->rollNumber);
	pBaseAddofRecord++;
	printf("Roll number of the element of the array = %d\n",pBaseAddofRecord->rollNumber);*/

	for(uint32_t i = 0; i < max_record ; i++)
	{
		printf("Roll number %d of the element of the array = %d\n",i,pBaseAddofRecord[i].rollNumber);
		//pBaseAddofRecord[i] == *(pBaseAddofRecord+i)

	}
}

int main()
{
	 display_all_records(student,max_record);
	 return 0;
}

