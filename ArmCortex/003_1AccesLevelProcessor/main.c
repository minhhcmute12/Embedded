/*
 * main.c
 *  Created on: May 14, 2023
 *      Author: Minh
 */
/*
 * Access level of the Processor - Mức độ truy cập của vi xử lý
 * ------------------M0/M3/M4-----------------------------------
 *
 * 1) Processor cung cấp 2 chế độ truy cập
 *  a) Mức truy cập đặc quyền (Privileged Access Level - PAL)
 *  b) Mức truy cập Không đặc quyền (Non-Privileged Access Level - NPAL)
 *
 * 2) Nếu code của bạn đang chạy với mức truy cập đặc quyền(PAL) thì code của bạn có quyền truy cập đầy đủ.
 *  Điều đó có nghĩa là bạn có toàn bộ đặc quyền để truy cập tất cả các tài nguyên cụ thể của Processor và các
 *  thanh ghi bị hạn chế.
 *  Thanh ghi bị hạn chế(Restricted Register): Trong vi xử lý có một số thanh ghi bị hạn chế truy cập. Bộ vi xử lý
 *  yêu cầu mức độ truy cập của bộ xử lý phải là PAL để có thể thay đổi nôi dung của các thanh ghi bị hạn chế đó.
 *
 * 3) Nếu code của bạn đang chạy với mức truy cập là NPAL thì mã của bạn KHÔNG có quyền truy cập vào một số thanh
 *  ghi bị hạn chế của bộ vi xử lý.
 *
 * 4) Theo mặc định, code của bạn sẽ luôn chạy ở cấp PAL.
 * Lưu ý: Cần kiểm tra mức truy cập trước khi chạy, do có thể trước đó bạn đã đổi mức truy cập
 * Vd: CONTROL 1; //NPAL   hoặc CONTROL 0; //PAL
 * Tuy nhiên, ở các project thực tế thì thường CONTROL sẽ luôn ở dạng 1(NPAL) để hạn chế một số trường hợp hy hữu
 * có thể tác động vào các thanh ghi đặc biệt mà người lập trình ko mong muốn.
 *
 * 5) Khi processor chạy trong chế độ "Thread mode", ta có thể chuyển đổi processor sang mức NPAL.
 *  Ngoài ra, khi bạn chuyển từ PAL sang NPAL khi đang chạy "thread_mode" thì sau đó nó sẽ không thể chuyển về
 *  dạng PAL trừ khi bạn chuyển chế độ processor về chế độ "Handler mode".
 *
 * 6) Khi bộ processor ở chế độ "Hander mode"(đang xử lý exception hoặc interrupt) thì mức truy cập sẽ luôn là PAL
 *
 * 7) Khi bạn đang sử dụng thanh ghi "CONTROL" của procesor, nếu bạn muốn chuyển đổi giữa các mức truy cập(acces levels)
 * thì bạn phải định lại cấu hình nếu muốn thay đổi mức độ truy cập thành NPAL.
 *
 */

