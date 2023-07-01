/*
 * main.c
 *
 *  Created on: 2023/05/07
 *      Author: Minh
 */

/**
 * Operational mode of the processor (Chế độ hoạt động của bộ vi xử lý)
 * --------------------------------------------------------------------
 * M0/M3/M4
 *
 * **Bộ vi xử lý(processor) cung cấp 2 chế độ hoạt động:
 * 1) Thread mode (Chế độ luồng) *
 * 2) Handler mode(Chế độ xử lý)
 *
 * Tất cả code của bạn sẽ được thực thi dưới chế độ 'thread(luồng)' của bộ vi xử lý. Điều này cũng được gọi là chế
 * độ người dùng "User mode".
 *
 * Tất cả các ngoại lệ xử lý(exceptions handlers) hoặc là xử lý ngắt(interrupt handlers) sẽ được thực thi
 * dưới chế độ "handler mode" của vi xử lý. Có nghĩa là với bất cứ khi nào bộ xử lý của bạn xuất hiện exception
 * hoặc interrupt đến từ các thiết bị ngoại vi, thì bộ vi xử lý sẽ thay đổi ngay lập tức sang chế độ "handler mode"
 * để tiến hành thực thi hành động đó.
 *
 * Vi xử lý luôn bất đầu với chế độ "Thread mode"
 *
 * Bất cứ khi nào lõi xử lý(core) cần đáp ứng exceptions hoặc interrupt sẽ thay đổi chế độ của nó thành "Handler Mode"
 * để phục vụ chức năng "The Interrupt Service Routine - ISR" được liên kết với exception hoặc interrupt.
 */
