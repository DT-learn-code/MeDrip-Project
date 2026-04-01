# MeDrip-Project
Mein erstes Projekt

# MeDrip - Hệ thống Giám sát Dịch truyền Thông minh

## 📝 Giới thiệu
**MeDrip** là một giải pháp IoT tích hợp được thiết kế để hiện đại hóa quy trình giám sát dịch truyền trong môi trường y tế. Hệ thống cho phép theo dõi tự động tốc độ nhỏ giọt và khối lượng dịch còn lại, từ đó đưa ra các cảnh báo kịp thời cho nhân viên y tế, đảm bảo an toàn tuyệt đối cho bệnh nhân.

Dự án này là sự kết hợp giữa kỹ thuật vi điều khiển (Hardware) và phát triển phần mềm hệ thống (Backend).

---

## 🚀 Chức năng chính
* **Giám sát thời gian thực:** Theo dõi chính xác tốc độ truyền (giọt/phút) và thể tích còn lại trong túi dịch.
* **Cảnh báo tự động:** Hệ thống tự động nhận diện và gửi thông báo khi:
    * Tốc độ truyền bị tắc nghẽn hoặc chảy quá nhanh so với chỉ định.
    * Dịch truyền sắp hết (dựa trên dữ liệu từ cảm biến trọng lượng).
* **Quản lý dữ liệu tập trung:** Lưu trữ lịch sử các ca truyền dịch vào cơ sở dữ liệu MySQL để phục vụ công tác tra cứu y bạ.
* **Kết nối không dây:** ESP32 truyền dữ liệu qua WiFi tới Server Spring Boot thông qua giao thức HTTP RESTful API.

---

## 🛠 Công nghệ sử dụng

### 1. Phần cứng (Firmware)
* **Vi điều khiển:** ESP32.
* **Cảm biến:** * Cảm biến hồng ngoại (đo tốc độ rơi của giọt dịch).
    * Loadcell 5kg kết hợp Module HX711 (đo khối lượng túi dịch).
* **Ngôn ngữ:** C++ (Arduino Framework).

### 2. Backend & Database
* **Framework:** Java 26, Spring Boot 4.0.
* **Database:** MySQL (Triển khai trên Docker).
* **Thư viện hỗ trợ:** Spring Data JPA, Hibernate, Lombok, Maven.

---

## 📂 Cấu trúc dự án
Dự án được tổ chức theo mô hình module hóa để dễ dàng quản lý và nâng cấp:

```text
MeDripServer/
├── backend/    # Mã nguồn Spring Boot (Xử lý logic và lưu trữ DB)
├── Database/   # File thiết kế SQL và cấu trúc dữ liệu (.sql)
└── Esp32/      # Mã nguồn Firmware cho vi điều khiển (.ino)

