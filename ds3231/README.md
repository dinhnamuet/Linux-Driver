- Driver được chia làm 2 loại:
    + Loadable Kernel Module: một module hạt nhân (có đuôi .ko), có thể được nạp/tháo khỏi Linux Kernel trong quá trình run-time
    + Built-in Driver: Driver được built trực tiếp với Linux Kernel, tự động khởi chạy khi Linux Kernel được chạy
Việc xây dựng Built-in driver cần sự hiểu biết lớn về hệ thống, trong 1 kỳ học thì khó có thể nắm được
Chính vì vậy Driver này được cung cấp dưới dạng Kernel module.

- Dưới đây là hướng dẫn sử dụng Driver DS3231:
    + Tại máy của khách hàng, dịch file bcm2710-rpi-3-b.dtb ra file dts bằng lệnh
        "cd /boot
	        dtc -I dtb -O dts -o bcm2710-rpi-3-b.dts bcm2710-rpi-3-b.dtb"
    + Tìm đến node mô tả Bus I2C muốn sử dụng, thêm một node con vào trong Node đó với nội dung như sau:
        ds3231@68 {
                compatible = "rtc-ds3231";
                reg = <0x68>;
                status = "okay";
        };
    + Dịch lại file bcm2710-rpi-3-b.dts ra dtb bằng lệnh
        "cd /boot
        	dtc -I dts -O dtb -o -f bcm2710-rpi-3-b.dtb bcm2710-rpi-3-b.dts"
    + Khởi động lại hệ thống bằng lệnh: sudo reboot.
    + Khi raspberry pi khởi động lại, kiểm tra trong folder /sys/class/i2c-adapter/i2c-x/of_node/
        với x là bus i2c đã define trong bước trước.
        Nếu có fodler ds3231@68 thì việc chuẩn bị đã thành công.
    + Folder Driver khi biên dịch sẽ tạo ra file "ds3231.ko", cung cấp cho khách hàng file "ds3231.ko" này.
    + Khi cần dùng Driver, dùng lệnh "sudo insmod ds3231.ko" để insert module vào linux kernel
    + Cung cấp cho khách hàng folder Src và Inc.
    + Trong dự án khách hàng muốn sử dụng phần cứng ds3231 để đọc thời gian, chỉ cần include "ds3231_lib.h" vào source code.
    + khi biên dịch, thêm "ds3231.c -I ./Inc/"

*** Chú ý:
    Makefile được gửi kèm folder này được dùng để cross-compile trên máy Ubuntu, để biên dịch trên board raspberry pi cần sửa
    đổi lại (dùng file Makefile trong buổi ggmeet hôm trước).
