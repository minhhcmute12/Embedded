/*
 * tutorial.c(Gồm bài 10,11,12)
 *
 *  Created on: Nov 20, 2023
 *      Author: Minh
 */
/**
*===Exercise(S27)
*==Exercise: Led toggling with Push-Pull configuration(V102)
+ Tham khảo Schematic(sơ đồ mạch điện) của các led trên board STM32F407DISC1
 ^User manual Discovery - Figure 14. Peripherals -> LEDs
 ^Theo sơ đồ ta có 4 led từ LD4(PD12)->LD3(PD13)->LD5(PD14)->LD6(PD15) và tất cả đều nối GND ở cuối mạch
 ^Trong bài này sẽ sử dụng Led_PD12

+ Khi muốn xem xung chạy thì có thể sử dụng "USB Logic Analyzer- Kênh CH3" kết nối chân PD12

*==Exercise: Led toggling with Open-Drain configuration(V103)
+ Hãy xem lại cách hoạt động của trạng thái OPEN-DRAIN
+ Hướng dẫn cách xử lý lập trình led ở chế độ Open-Drain
Lưu ý: Không nên sử dụng Open Drain cho chương trình nhúng

*==Exercise: Handling on board LED and BUTTON(V104)
+ Tham khảo Schematic(sơ đồ mạch điện) của các led trên board STM32F407DISC1
 ^User manual Discovery - Figure 14. Peripherals -> LEDs(PD12)
 ^User manual Discovery - Figure 14. Peripherals -> User & Wake up Button(PA0)
 ^Giải thích sơ đồ dòng điện: khi bạn nhấn nút nhấn, thì chân PA0 sẽ đc kết nối(kéo lên) VDD, có nghĩa là có tín
 hiệu nhấn nút, dòng điện từ VDD-> button -> R35(330 Ohm) -> PA0. Ngoài ra việc nhấn nút sẽ ko làm dòng điện
 từ VDD -> R39(220KOhm) -> GND vì R39 có điện trở quá lớn sẽ ngăn dòng điện chảy xuống GND. Khi ko có nút nhấn thì
 dòng điện từ PA0 -> R35 -> R39 -> GND.
 ^Vì chân PA0 sẽ đc kết nối(kéo lên) VDD: Press->PA0->HIGH or Release->PA0->LOW

+Mẹo: chọn Properties -> Tích ô "Exclude resource from build" nếu muốn ko build file source đó khi có nhiều file
source giống nhau

*==Exercise: Connecting external button and circuit explanation - Kết nối nút bên ngoài và giải thích mạch(105+106)
+ Linh kiện:  Button: PB12 ; Led: PA8 ; Rx1(22KOhm), Rx2(470Ohm hoặc 330Ohm)
+ Sơ đồ nối dây: 	  BTN_Pin1	-	BTN_Pin2			  VCC
						|				|				   |
				PB12---(X)			   GND	   ==> 		  Rx	 + 	PA8---Rx2(470)---LED---GND
						|							PB12---|
						Rx - VCC						  BTN
														   |
														  GND

+ Ở mạch Button ta sẽ sử dụng một điện trở kéo lên, nguyên nhân là ta sử dụng một Rx(22KOhm) nên sẽ ngăn dòng
điện từ VCC xuống cũng như dòng điện từ VDD(của điện trở kéo) đi qua Rx(22KOhm).
 -> Khi dùng điện trở kéo lên ta có mạch khi đc nhấn: VDD - PB12 -BTN - GND

+ Ở V105 dùng Led: PA14 nhưng do đây là Pin đang đc set tính năng Alternate(ban đầu) nên sẽ ko đc set output pin
Vì vậy ở V106 ta sẽ chuyển sang dùng PA8

*==Exercise: Button interrupt introduction(V107)
*/

