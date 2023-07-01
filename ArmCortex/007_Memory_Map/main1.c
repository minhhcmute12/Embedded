/*
 * main1.c *
 *  Created on: 2023/05/27
 *      Author: Minh
 */
/**
 * 2.Bus Protocols and Bus Interface(V36)
 * ------------------------------
 * Trong bộ xử lý Cortex Mx, các giao diện bus dựa trên một đặc điểm kỹ thuật được gọi là kiến trúc bus vi điều khiển nâng
 * cao (AMBA). Đó là một đặc điểm kỹ thuật được thiết kế bởi hãng ARM, điều chỉnh tiêu chuẩn cho giao tiếp bên trong một
 * chip của hệ thống. Thông số kỹ thuật AMBA hỗ trợ một số Bus Protocols:
 *  . AHB Lite (AMBA High-performance Bus)
 *  . APB (AMBA Peripheral Bus)
 *
 *  Bus AHB Lite chủ yếu được sử dụng cho các giao diện bus chính trên vi điều khiển.
 *  Bus AHB Lite đc sử dụng chủ yếu để giao tiếp tốc độ cao với các thiết bị ngoại vi yêu cầu bus hoạt động với tốc độ cao.
 *
 *  Bus APB được sử dụng cho truy cập ngoại vi riêng và cho một số truy cập ngoại vi trên chip bằng cách sử dụng một cầu
 *  đc gọi là cầu AHB-APB.
 *  Bus APB đc sử dụng để giao tiếp bus AHB, và hầu hết các thiết bị ngoại vi không yêu cầu tốc độ họat động khi đc kết
 *  nối với bus này
 *
 *  Việc sử dụng các Bus Protocols trong vi điều khiển là cầu nối giao tiếp dữ liệu qua lại giữa Vxl(processor) <-> Memory Map
 *  Vd: Ta có các cổng nằm bên trái là cổng giao tiếp của vxl và các phần bên phải là các memory(thanh ghi) vi điều khiển
 *   . PPB <-----AHB 32 bit Data Access -----> PPB
 *   . SYSTEM <----- AHB 32 bit Any Access(read/write)-----> SRAM,Peripheral,Ext RAM, Device regions,...
 *   . D-CODE, I-CODE <-----AHB 32 bit Data Access-----> CODE region
 *  Qua vd ta thấy rằng việc giao tiếp giữa vxl và memory map (các thiết bị ngoại vi) sẽ thông qua bus AHB
 *
 * **Tham khảo vi điều khiển STM32F407: Phần 2-Description-> 2.2 Device overview (STM32F40xxx block diagram-sơ đồ khối)
 * Vd1: Ta có phần Flash Memory(nơi lưu code lập trình) trên vi điều khiển. Vxl sẽ giao tiếp với Flash_Memory qua 3 cổng
 * là D-Busg hoặc I-Bus hoặc S-Bus(Luu ý: trong STM32F407, S-Bus là viết tắt đại diện của System Bus). Hai phần này được
 * giao tiếp với nhau thông qua AHB Bus Matrix.
 *
 * Vd2: Ta còn có thêm 1 bus có tên là AHB1 168MHz, Bus này giúp vxl giao tiếp với các GPIO Port. Khi vxl ra lệnh sẽ
 * đc truyền qua AHB Bus Matrix <-> AHB1 168MHz <-> GPIO PORT_X
 * Hay ta có bus AHB2 168Hz giúp vxl giao tiếp với USB OTG FS.
 * ==>Ở vd1 và vd2 ta sử dụng cầu AHB là vì đây là các công việc yêu cầu tốc độ kết nối giao tiếp cao.
 * -> Ở một số công việc cần yêu cầu tốc độ hoạt động cao đặc định vì vậy các bus AHB1, AHB2 đc sinh ra để đáp ứng nhu cầu này.
 *
 * Vd3: Giao tiếp vxl với các chức năng khác (TIMER, PWM,ADC UART) thông qua cầu AHB-APB
 * Cầu AHB-APB : cầu sẽ chuyển đổi tín hiệu AHB thành tín hiệu APB. Nguyên nhân vì có một số thiết bị ngoại vi KHÔNG yêu cầu tốc
 * độ hoạt động cao khi đc kết nối. Và APB Bus đc sử dụng lúc này. Thiết kế này giúp cho nhà thiết kế vi điều khiển giảm mức tiêu
 * thụ năng lượng thay vì tất cả các hoạt động dùng AHB Bus(dùng nhiều năng lượng do yêu cầu tốc độ cao)
 *
 * Vd4:
 * Ngoài ra: Với mỗi thiết bị ngoại vi khác nhau thì APB Bus cũng có sự khác nhau để đa dạng sự lựa chọn mục đích hoạt động.
 * vd4.1: theo sơ đồ khối thì các phần như TIM1/PMW, TIM8,TIM9,USART1, SPI1,ADC1,.. sẽ giao tiếp thông qua ABP2 84MHz.
 * vd4.2: Còn các phần như TIM12,TIM13,USART2,SP2/I2S2,I2C1,TIM6,WWDG,DAC1... sẽ giao tiếp thông qua APB1 42MHz.
 *
 * Lưu ý: Trên STM32F407 thì vxl chủ yếu sử dụng 4 bus chính sau để giao tiếp với thanh ghi thiết bị ngoại vi:
 *  I-Bus, D-Bus: đc sử dụng để giao tiếp với code memory, để tìm và nạp các code và dữ liệu đồng thời.
 *  S-Bus: đc sử dụng để giao tiếp với SRAM và các thiết bị ngoại vi có trên chip vi điều khiển.
 *  PPB: đc sử dụng để giao tiếp phần thanh ghi PPB của vi điều khiển
 */


