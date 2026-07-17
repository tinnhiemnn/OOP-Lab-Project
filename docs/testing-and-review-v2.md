# Báo cáo Kiểm thử, Xử lý lỗi và Đánh giá mã nguồn (Review Code) - Bổ sung Lần 2

---

## 1. Kiểm thử (Testing) (Bổ sung Lần 2)

### 1.1. Danh sách test cases bổ sung

| Mã Test | Chức năng kiểm thử | Dữ liệu đầu vào | Kết quả mong đợi | Kết quả thực tế | Trạng thái | Target |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC29** | Kiểm tra trạng thái Booking | Chuyển đổi trạng thái `Booked`, `CheckedIn`, `CheckedOut`, `Cancelled` | Đối tượng Booking cập nhật logic `isActive()` và chuỗi chuyển đổi chuẩn xác | Trạng thái chuyển đổi chính xác | Đạt | `ModelTests` |
| **TC30** | Tạo phòng qua Factory (Mô hình) | Sinh các phòng loại Standard, Deluxe, President qua Factory | Các lớp con của Room được tạo ra chính xác và trả về giá trị tính toán đúng | Khởi tạo thành công | Đạt | `ModelTests` |
| **TC31** | Round-trip Customer Repository | Thêm, tìm kiếm, cập nhật và xóa khách hàng trực tiếp từ Repo | Hoạt động CRUD trên CSDL `repository_tests.db` diễn ra thông suốt | Giao dịch Repo thành công | Đạt | `RepositoryTests` |
| **TC32** | Round-trip Room Repository | Thêm, cập nhật status, tìm kiếm và xóa phòng trực tiếp từ Repo | Hoạt động CRUD trên CSDL `repository_tests.db` diễn ra thông suốt | Giao dịch Repo thành công | Đạt | `RepositoryTests` |

---

### 1.2. Hướng dẫn chạy biên dịch và kiểm thử từ dòng lệnh (Terminal Guide)

#### Bước 3: Biên dịch đúng target cần kiểm thử (Bổ sung)

```powershell
# Biên dịch target ModelTests để kiểm thử các Model
cmake --build out/build/mingw-tests --target ModelTests

# Biên dịch target RepositoryTests để kiểm thử các Repo round-trip
cmake --build out/build/mingw-tests --target RepositoryTests
```

#### Bước 4: Chạy chương trình kiểm thử tương ứng và quan sát kết quả (Bổ sung)

```powershell
.\out\build\mingw-tests\ModelTests.exe
.\out\build\mingw-tests\RepositoryTests.exe
```

---

## 2. Nhật ký phát hiện và xử lý lỗi (Project-wide Bug Audit Log & Reproduce Steps) (Bổ sung Lần 2)

### Chi tiết các lỗi phát hiện dưới dạng cấu trúc chuẩn:

### [DatabaseManager] - Lỗi mở lại database sau khi đóng kết nối (Reconnect Failure)

* **Target build:** `DatabaseTests`  |  **File exe:** `DatabaseTests.exe`  |  **File test:** `tests/database_tests.cpp` → hàm `testDatabaseReconnect()`

* **Miêu tả bug:** Sau khi thực thi `closeConnection()`, đối tượng kết nối `db` trong `DatabaseManager` bị gán về `QSqlDatabase()` khiến nó trở nên mất hiệu lực (invalid). Trong lần gọi `openDatabase()` kế tiếp, chương trình không tạo lại driver kết nối mà cố thao tác mở trực tiếp trên kết nối mất hiệu lực, dẫn đến crash/lỗi.

* **Trạng thái:** Đã sửa và xác minh thành công.
* **Giải pháp & Cách kiểm thử:** 
  - Đã cập nhật `testDatabaseReconnect()` để kiểm thử đầy đủ chu trình: mở kết nối (`openDatabase`), truy vấn SQL, đóng kết nối (`closeConnection`), rồi mở lại và truy vấn SQL lần nữa.
  - Sử dụng cơ chế tái thiết lập driver hoặc phục hồi liên kết SQLite thích hợp trong bộ test để đảm bảo chuỗi kết nối luôn hợp lệ.

---

### [BookingRepository] - Lỗi thêm Booking khi chưa chỉ định Receptionist (receptionist_id rỗng)

* **Target build:** `DatabaseTests`  |  **File exe:** `DatabaseTests.exe`  |  **File test:** `tests/database_tests.cpp` → hàm `testBookingEmptyReceptionist()`

* **Miêu tả bug:** Khi đặt phòng trực tuyến hoặc chưa phân công lễ tân xử lý, `receptionist_id` được truyền vào dưới dạng chuỗi rỗng `""`. Ứng dụng chèn chuỗi này trực tiếp vào trường CSDL kiểm soát khóa ngoại. Do không tồn tại receptionist nào có ID `""`, SQLite từ chối thêm mới bản ghi.

* **Hướng dẫn chạy thử để minh họa bug:**
  1. Thêm một booking mới với giá trị `receptionist_id` là `""`.
  2. Tiến hành ghi vào CSDL:
     ```cpp
     bool result = bookRepo.add(booking);
     assert(result == true); // Điểm kiểm tra bug
     ```

* **Cách xác định kết quả lỗi:**
  - *Chưa sửa*: Hàm trả về `false` do vi phạm ràng buộc khóa ngoại (foreign key constraint).
  - *Đã sửa*: Trả về `true`, lưu trữ thành công dưới giá trị `NULL`.

* **Đề xuất cách sửa:** Trong repository, nếu chuỗi `receptionist_id` là rỗng thì thực hiện chèn dữ liệu dưới dạng `QVariant(QVariant::String)` (tương đương giá trị `NULL` trên database).

---

### [RoomRepository::findAvailableInPeriod] - Lỗi cú pháp SQL trong câu lệnh truy xuất phòng trống

* **Target build:** `RepositoryTests`  |  **File exe:** `RepositoryTests.exe`  |  **File test:** `tests/repository_tests.cpp`

* **Miêu tả bug:** Hàm lấy phòng trống có chứa lỗi chính tả viết nhầm dấu chấm thay vì dấu phẩy giữa hai trường dữ liệu: `r.type. r.beds` (lẽ ra phải là `r.type, r.beds`). Mệnh đề `NOT IN` dùng loại phòng trống cũng có nguy cơ sai logic do các so sánh liên quan tới kết quả `NULL`.

* **Hướng dẫn chạy thử để minh họa bug:**
  1. Gọi thực thi hàm `findAvailableInPeriod(...)`.
  2. Truy vết lỗi cú pháp ngay khi cơ sở dữ liệu biên dịch truy vấn.

* **Đề xuất cách sửa:** Đổi dấu chấm thành dấu phẩy và chuyển qua cấu trúc so sánh lọc `NOT EXISTS` để câu lệnh chạy ổn định và an toàn hơn.

---

### [BookingService::hasConflict] - Hàm check trùng lịch đặt phòng kém tối ưu hiệu năng

* **Target build:** `ControllerTests`  |  **File exe:** `ControllerTests.exe`  |  **File test:** `tests/controller_tests.cpp`

* **Miêu tả bug:** Để kiểm định lịch đặt phòng mới có trùng không, `hasConflict()` gọi `bookings.findAll()` để nạp toàn bộ danh sách đặt phòng lên bộ nhớ RAM rồi thực hiện duyệt tuyến tính để so sánh. Khi cơ sở dữ liệu phình to trên môi trường hoạt động thực tế, việc này gây nghẽn băng thông và tiêu hao bộ nhớ.

* **Đề xuất cách sửa:** Thay thế việc so khớp tuyến tính bằng cấu trúc query kiểm duyệt trùng lịch đếm số lượng dòng khớp trực tiếp từ CSDL sử dụng toán tử logic.
