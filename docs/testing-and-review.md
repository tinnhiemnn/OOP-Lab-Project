# Báo cáo Kiểm thử, Xử lý lỗi và Đánh giá mã nguồn (Review Code)

---

## 1. Kiểm thử (Testing)

### 1.1. Danh sách test cases

| Mã Test | Chức năng kiểm thử | Dữ liệu đầu vào | Kết quả mong đợi | Kết quả thực tế | Trạng thái | Target |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC01** | Kiểm tra ngày hợp lệ | `"2026-06-28"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` |
| **TC02** | Kiểm tra năm nhuận hợp lệ | `"2024-02-29"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` |
| **TC03** | Kiểm tra ngày không tồn tại | `"2026-02-29"`, `"2026-13-01"`, `"2026-12-32"` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` |
| **TC04** | Kiểm tra khoảng ngày đặt phòng hợp lệ | Check-in: `"2026-06-28"`, Check-out: `"2026-06-29"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` |
| **TC05** | Kiểm tra khoảng ngày ngược/bằng nhau | Check-in sau hoặc trùng ngày Check-out | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` |
| **TC06** | Tính số ngày thuê phòng | Check-in: `"2026-06-01"`, Check-out: `"2026-06-11"` | Trả về `10` ngày | Trả về `10` | Đạt | `UtilsTests` |
| **TC07** | Kiểm tra trùng lịch đặt phòng (Overlap) | Hai khoảng ngày giao nhau | Trả về `true` (Có trùng) | Trả về `true` | Đạt | `UtilsTests` |
| **TC08** | Kiểm tra chuỗi bắt buộc (Non-empty) | Chuỗi rỗng `""` hoặc chỉ gồm khoảng trắng `"   "` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` |
| **TC09** | Kiểm tra email định dạng chuẩn | `"test@example.com"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` |
| **TC10** | Kiểm tra email sai định dạng | `"test.example@com"`, `"@example.com"`, `"test@.com"` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` |
| **TC11** | Kiểm tra số điện thoại hợp lệ | `"0987654321"` (10 chữ số) | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` |
| **TC12** | Kiểm tra số điện thoại sai định dạng | Chứa chữ: `"098765abcd"`, Quá ngắn: `"12345678"`, Quá dài: `"1234567890123"` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` |
| **TC13** | Kiểm tra số tiền / số nguyên dương | `100.5`, `0.0`, `-5.0` | Chỉ giá trị dương (`100.5`) trả về `true` | Chỉ giá trị dương hợp lệ | Đạt | `UtilsTests` |
| **TC14 - TC19** | Xác thực Customer/Room/Booking input | Xem chi tiết mục 3 | Trả về đúng `true`/`false` kèm `error` tương ứng | Đúng như kỳ vọng | Đạt | `UtilsTests` |
| **TC20** | Chuyển đổi trạng thái/loại phòng và chuỗi | `RoomType::Standard`, `"Deluxe"`, `RoomStatus::InUse`, v.v. | Chuyển đổi chính xác hai chiều giữa định dạng enum và chuỗi | Chuyển đổi chuẩn | Đạt | `RoomFactoryTests` |
| **TC21** | Tạo phòng qua Factory (Standard) | Loại `"Standard"`, giá `500000`, 2 giường | Tạo ra đối tượng `StandardRoom`, tính giá 3 ngày = `1.800.000` | Tạo chính xác + tính giá đúng | Đạt | `RoomFactoryTests` |
| **TC22** | Tạo phòng qua Factory (Deluxe) | Loại `"Deluxe"`, giá `1000000` | Tạo ra đối tượng `DeluxeRoom`, tính giá 2 ngày × hệ số 1.2 = `2.400.000` | Tạo chính xác + tính giá đúng | Đạt | `RoomFactoryTests` |
| **TC23** | Tạo phòng qua Factory (President) | Loại `"President"`, giá `2000000` | Tạo ra đối tượng `PresidentRoom`, tính giá 1 ngày × hệ số 1.5 = `3.000.000` | Tạo chính xác + tính giá đúng | Đạt | `RoomFactoryTests` |
| **TC24** | Ràng buộc xóa phòng khi có booking | Mã phòng: `"R999"`, Booking `"B999"` đang trạng thái `"Booked"` | Trả về `false`, chặn xóa phòng, gán lỗi cụ thể vào repository | Trả về `false`, chặn và báo lỗi | Đạt | `DatabaseTests` |
| **TC25** | Ràng buộc xóa khách hàng khi có booking | Mã KH: `"C998"`, Booking `"B998"` đang trạng thái `"Booked"` | Trả về `false`, chặn xóa KH, gán lỗi cụ thể vào repository | Trả về `false`, chặn và báo lỗi | Đạt | `DatabaseTests` |
| **TC26** | Kết nối lại CSDL sau khi đóng | Mở CSDL → Đóng → Mở lại lần 2 | Cả hai lần mở đều trả về `true` và truy vấn bình thường | Kết nối lại thành công | Đạt | `DatabaseTests` |
| **TC27** | Thêm Booking khi chưa phân công Receptionist | `receptionistId = ""`, Booking `"B997"` | Trả về `true`, `receptionist_id` được lưu dưới dạng `NULL` thay vì chuỗi rỗng | Trả về `true`, insert thành công | Đạt | `DatabaseTests` |
| **TC28** | Bypass validation ở tầng Controller | Customer `"CBAD"` với email `"not-an-email"`, SĐT `"abc1234"` | Trả về `false`, không ghi bản ghi rác vào DB | Trả về `false`, chặn đúng | Đạt | `ControllerTests` |

---

### 1.2. Hướng dẫn chạy biên dịch và kiểm thử từ dòng lệnh (Terminal Guide)

#### Bước 1: Mở Terminal và di chuyển đến thư mục gốc của dự án

```powershell
cd <đường-dẫn-tới-thư-mục-dự-án>
```

#### Bước 2: Khởi tạo cấu hình cho hệ thống build CMake (chỉ làm 1 lần)

```powershell
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="g++" -S . -B out/build/mingw-tests
```
> [!NOTE]
> - `-G "MinGW Makefiles"`: Biên dịch tương thích với MinGW.
> - `-DCMAKE_CXX_COMPILER="g++"`: Trình biên dịch g++ (yêu cầu đã có sẵn trong PATH; nếu chưa, dùng đường dẫn đầy đủ tới `g++.exe` của MSYS2/MinGW).
> - `-S .`: Thư mục gốc chứa mã nguồn.
> - `-B out/build/mingw-tests`: Thư mục chứa các tệp build đầu ra.
> Bước này chỉ cần chạy lại khi sửa `CMakeLists.txt` (thêm/xóa file nguồn, thêm target mới).

#### Bước 3: Biên dịch đúng target cần kiểm thử

Mỗi target độc lập, chọn đúng target theo bug đang kiểm tra:

```powershell
# Bug liên quan ValidationUtils / DateUtils
cmake --build out/build/mingw-tests --target UtilsTests

# Bug liên quan Rooms / Customers / DatabaseManager / BookingRepository
cmake --build out/build/mingw-tests --target DatabaseTests

# Bug liên quan RoomFactory / enum RoomType-RoomStatus
cmake --build out/build/mingw-tests --target RoomFactoryTests

# Bug liên quan Controllers (bypass validation)
cmake --build out/build/mingw-tests --target ControllerTests
```

#### Bước 4: Chạy chương trình kiểm thử tương ứng và quan sát kết quả

```powershell
.\out\build\mingw-tests\UtilsTests.exe
.\out\build\mingw-tests\DatabaseTests.exe
.\out\build\mingw-tests\RoomFactoryTests.exe
.\out\build\mingw-tests\ControllerTests.exe
```
Mỗi file `.exe` chỉ in log của đúng phần nó phụ trách, ví dụ `DatabaseTests.exe`:
```text
Database reconnect test passed!
Room delete constraint test passed!
Customer delete constraint test passed!
Booking empty receptionist test passed!
All database & repository constraint tests passed successfully!
```

#### Bước 5 (tùy chọn): Chạy toàn bộ 4 target cùng lúc bằng CTest — dùng khi regression-check, không dùng để debug 1 bug cụ thể

```powershell
cmake --build out/build/mingw-tests
cd out/build/mingw-tests
ctest --output-on-failure
```
`ctest` chạy lần lượt `UtilsTest`, `DatabaseTest`, `RoomFactoryTest`, `ControllerTest` và báo rõ target nào fail  khác với việc chỉ chạy 1 file `.exe` để cô lập đúng 1 bug.

---

## 2. Nhật ký phát hiện và xử lý lỗi (Project-wide Bug Audit Log & Reproduce Steps)

### Chi tiết các lỗi phát hiện và hướng dẫn kiểm thử từng bước:

### [Rooms] - Xóa phòng đang có booking vẫn xóa được

* **Target build:** `DatabaseTests`  |  **File exe:** `DatabaseTests.exe`  |  **File test:** `tests/database_tests.cpp` → hàm `testDeleteRoomConstraint()`

* **Miêu tả bug**: `RoomRepository::remove(id)` xóa thẳng bản ghi phòng mà không kiểm tra phòng đó còn booking đang hoạt động (`Booked`/`CheckedIn`) hay không. Do khóa ngoại trong DB đặt `ON DELETE SET NULL`, khi xóa phòng thì `room_id` trong các booking liên quan tự bị gán `NULL` thay vì bị chặn làm mất liên kết dữ liệu.

* **Hướng dẫn chạy thử để minh họa bug**:
  1. Mở `tests/database_tests.cpp` (đã có sẵn hàm `testDeleteRoomConstraint()`).
  2. Điểm kiểm tra chính:
     ```cpp
     bool result = roomRepo.remove("R999");
     assert(result == false); // Diem kiem tra bug
     ```
  3. Biên dịch và chạy target `DatabaseTests`:
     ```powershell
     cmake --build out/build/mingw-tests --target DatabaseTests
     .\out\build\mingw-tests\DatabaseTests.exe
     ```
* **Cách xác định kết quả lỗi**:
  - *Chưa sửa*: dừng đột ngột, `Assertion failed: result == false` vì `remove()` trả `true` nhầm.
  - *Đã sửa*: in `Room delete constraint test passed!`, chạy tiếp các test khác.
* **Đề xuất cách sửa**: Trước khi xóa, đếm số booking có trạng thái `Booked`/`CheckedIn` tham chiếu tới `room_id` đó. Nếu > 0 thì trả `false` và set lỗi tương ứng, không cho xóa.

---

### [Customers] - Xóa khách hàng đang có booking vẫn xóa được

* **Target build:** `DatabaseTests`  |  **File exe:** `DatabaseTests.exe`  |  **File test:** `tests/database_tests.cpp` → hàm `testDeleteCustomerConstraint()`

* **Miêu tả bug**: Tương tự bug ở `Rooms` , `CustomerRepository::remove(id)` xóa khách hàng mà không kiểm tra khách còn booking chưa hoàn tất hay không, khiến `customer_id` trong bảng booking tự động bị gán `NULL`.

* **Hướng dẫn chạy thử để minh họa bug**:
  1. Mở `tests/database_tests.cpp` (đã có sẵn hàm `testDeleteCustomerConstraint()`, chung file với bug Rooms).
  2. Điểm kiểm tra chính:
     ```cpp
     bool result = custRepo.remove("C998");
     assert(result == false); // Diem kiem tra bug
     ```
  3. Biên dịch và chạy target `DatabaseTests`:
     ```powershell
     cmake --build out/build/mingw-tests --target DatabaseTests
     .\out\build\mingw-tests\DatabaseTests.exe
     ```
     > Muốn cô lập tuyệt đối chỉ mỗi bug này, tạm comment các lời gọi hàm test khác trong `main()`.
* **Cách xác định kết quả lỗi**:
  - *Chưa sửa*: dừng tại `Assertion failed: result == false`.
  - *Đã sửa*: in `Customer delete constraint test passed!` và chạy tiếp.
* **Đề xuất cách sửa**: Đếm booking đang hoạt động liên quan tới `customer_id` trước khi xóa. Nếu > 0, trả `false` kèm thông báo lỗi rõ ràng.

---

### [DatabaseManager] - Không mở lại được kết nối sau khi gọi closeConnection()

* **Target build:** `DatabaseTests`  |  **File exe:** `DatabaseTests.exe`  |  **File test:** `tests/database_tests.cpp` → hàm `testDatabaseReconnect()`

* **Miêu tả bug**: Sau khi gọi `closeConnection()`, biến kết nối `db` bị Qt gỡ driver SQLite, nên lần `openDatabase()` kế tiếp thất bại (`open()` trả `false`).

* **Lưu ý phạm vi**: Trong luồng chạy thực tế của ứng dụng, `closeConnection()` chỉ được gọi **một lần duy nhất khi chương trình kết thúc**, nên bug này gần như không xảy ra khi dùng app bình thường. Test `testDatabaseReconnect()` vẫn giữ lại để đảm bảo `DatabaseManager` mở-lại-được nếu sau này có nơi khác gọi `closeConnection()` giữa chừng (ví dụ: đổi file DB, logout, chạy nhiều test liên tiếp trong cùng 1 process).

* **Hướng dẫn chạy thử để minh họa bug**:
  1. Mở `tests/database_tests.cpp` (hàm `testDatabaseReconnect()`, chạy đầu tiên trong `main()`).
  2. Điểm kiểm tra chính:
     ```cpp
     dbMgr.closeConnection();
     bool openSecond = dbMgr.openDatabase(TEST_DB_PATH);
     assert(openSecond == true); // Crash tai day neu bug chua sua
     ```
  3. Biên dịch và chạy target `DatabaseTests`:
     ```powershell
     cmake --build out/build/mingw-tests --target DatabaseTests
     .\out\build\mingw-tests\DatabaseTests.exe
     ```
* **Cách xác định kết quả lỗi**:
  - *Chưa sửa*: Assertion failed ngay ở test đầu tiên, không in được cả dòng passed đầu tiên.
  - *Đã sửa*: in `Database reconnect test passed!`, chạy tiếp 3 test còn lại.
* **Đề xuất cách sửa**: Trong `openDatabase()`, kiểm tra `if (!db.isValid())` — nếu đúng, tạo lại kết nối bằng `db = QSqlDatabase::addDatabase("QSQLITE", "hotel_connection");` trước khi mở.

---

### [Controllers] - Bỏ qua kiểm tra dữ liệu đầu vào (Bypass Validation)

* **Target build:** `ControllerTests`  |  **File exe:** `ControllerTests.exe`  |  **File test:** `tests/controller_tests.cpp` → hàm `testControllerValidationBypass()`

* **Miêu tả bug**: Các Controller (`CustomerController`, `RoomController`, `ReceptionistController`) gọi thẳng `repository.add()`/`update()` mà không qua `ValidationUtils`. SQLite không tự chặn định dạng chuỗi, nên dữ liệu rác (email sai, SĐT chứa chữ, giá âm...) vẫn được ghi vào DB — khiến các unit test của `ValidationUtils` trở nên vô nghĩa trong thực tế vì không ai gọi tới chúng.

* **Hướng dẫn chạy thử để minh họa bug**:
  1. `tests/controller_tests.cpp` là target độc lập, không cần build chung với các file test khác.
  2. Điểm kiểm tra chính:
     ```cpp
     Customer badCust("CBAD", "Bad Name", "not-an-email", "abc1234");
     bool result = ctrl.addCustomer(badCust, error);
     assert(result == false); // Diem kiem tra bug
     ```
  3. Biên dịch và chạy target `ControllerTests`:
     ```powershell
     cmake --build out/build/mingw-tests --target ControllerTests
     .\out\build\mingw-tests\ControllerTests.exe
     ```
* **Cách xác định kết quả lỗi**:
  - *Chưa sửa*: `Assertion failed: result == false` — Controller đã lưu thành công dữ liệu rác.
  - *Đã sửa*: in `Controller Validation Bypass test passed!` rồi `All controller tests passed!`.
* **Đề xuất cách sửa**: Gọi `ValidationUtils` ngay đầu các hàm add/update của Controller, ví dụ:
  ```cpp
  if (!ValidationUtils::isValidEmail(customer.getEmail())) { error = "Email không hợp lệ!"; return false; }
  if (!ValidationUtils::isValidPhone(customer.getPhone())) { error = "Số điện thoại sai định dạng!"; return false; }
  ```
  Áp dụng tương tự cho các trường khác và cho `RoomController` (chặn giá phòng âm).

---

### [CMakeLists.txt] - Thiếu file BookingRepository.cpp khi cấu hình biên dịch

* **Target build:** `HotelManagement` (ứng dụng chính, không phải target test)

* **Miêu tả bug**: `src/repositories/BookingRepository.cpp` bị bỏ sót khỏi `PROJECT_SOURCES` trong `CMakeLists.txt`, khiến linker báo lỗi thiếu ký hiệu khi build app chính.

> **Ghi chú**: `CMakeLists.txt` hiện tại đã khai báo đúng file này trong cả `PROJECT_SOURCES` và source list của `DatabaseTests`. Mục này giữ lại để đối chiếu nếu bug tái xuất hiện sau khi thay đổi cấu trúc thư mục.

* **Hướng dẫn chạy thử để minh họa bug**:
  1. Mở terminal tại thư mục gốc dự án.
  2. Build app chính:
     ```powershell
     cmake --build out/build/mingw-tests --target HotelManagement
     ```
* **Cách xác định kết quả lỗi**:
  - *Chưa sửa*: Build dừng ở bước liên kết, in `undefined reference to 'BookingRepository::remove(QString const&)'` (hoặc các hàm khác cùng tiền tố).
  - *Đã sửa*: Build thành công, ra file `HotelManagement.exe`.
* **Đề xuất cách sửa**: Thêm `"src/repositories/BookingRepository.cpp"` vào `PROJECT_SOURCES` trong `CMakeLists.txt`.

---

### [ValidationUtils] - Kiểm tra định dạng Email chưa chặt chẽ

* **Target build:** `UtilsTests`  |  **File exe:** `UtilsTests.exe`  |  **File test:** `tests/utils_tests.cpp` → hàm `testValidationUtils()`

* **Miêu tả bug**: `ValidationUtils::isValidEmail` chỉ kiểm tra có `@` và `.`, nên vẫn chấp nhận nhầm các email sai như có khoảng trắng (`"test @ex.com"`), nhiều hơn 1 dấu `@` (`"test@ex.com@com"`), hoặc 2 dấu chấm liền nhau (`"test@ex..com"`).

* **Hướng dẫn chạy thử để minh họa bug**:
  1. Mở `tests/utils_tests.cpp`, vào hàm `testValidationUtils()`, phần Email.
  2. Thêm các case sau:
     ```cpp
     assert(ValidationUtils::isValidEmail("test @example.com") == false);
     assert(ValidationUtils::isValidEmail("test@example.com@com") == false);
     assert(ValidationUtils::isValidEmail("test@ex..com") == false);
     ```
  3. Biên dịch và chạy target `UtilsTests`:
     ```powershell
     cmake --build out/build/mingw-tests --target UtilsTests
     .\out\build\mingw-tests\UtilsTests.exe
     ```
* **Cách xác định kết quả lỗi**:
  - *Chưa sửa*: Assertion failed ở các dòng email mới thêm (hàm trả `true` nhầm cho email bẩn).
  - *Đã sửa*: chạy qua hết, in `ValidationUtils tests passed!`.
* **Đề xuất cách sửa**: Trong `isValidEmail`, kiểm tra thêm: không chứa khoảng trắng, đúng 1 ký tự `@`, không có `..`, và `@`/`.` không đứng đầu/cuối chuỗi.

---

## 3. Thực hiện kiểm tra tính hợp lệ dữ liệu đầu vào (Input Validation)

Các quy tắc nghiệp vụ về dữ liệu đầu vào được đóng gói trong `ValidationUtils`, đảm bảo dữ liệu hợp lệ trước khi ghi vào CSDL.

1. **Thông tin Khách hàng (Customer)**:
   - **Mã Khách hàng (ID)**: Không được rỗng hoặc chỉ toàn khoảng trắng.
   - **Họ và Tên**: Không được rỗng, loại bỏ khoảng trắng thừa bằng `.trimmed()`.
   - **Email**: Chứa đúng 1 ký tự `@`, không khoảng trắng, `@` phải đứng trước `.`, không đứng sát đầu/cuối chuỗi, không có `..` liền nhau.
   - **Số điện thoại**: Chỉ gồm số `0`–`9`, độ dài từ `9` đến `12` ký tự.

2. **Thông tin Phòng nghỉ (Room)**:
   - **Mã Phòng (Room ID)**: Bắt buộc không rỗng.
   - **Mức giá cơ bản (Price)**: Số thực lớn hơn `0.0`.
   - **Số lượng giường (Beds)**: Số nguyên dương lớn hơn `0`.

3. **Thông tin Đặt phòng (Booking)**:
   - **Mã khách hàng & Mã phòng**: Không được rỗng (đảm bảo liên kết khóa ngoại).
   - **Thời gian thuê phòng**: Check-in và Check-out phải hợp lệ, Check-out phải sau Check-in.
   - **Receptionist (tùy chọn)**: Được phép rỗng nếu chưa phân công, chuỗi rỗng phải chuyển sang `NULL` khi ghi CSDL (xem bug `[BookingRepository]` ở mục 2).

---

## 4. Đánh giá mã nguồn (Code Review)

- **Tách biệt mối quan tâm (Separation of Concerns)**: Không rải logic kiểm tra dữ liệu ở tầng Views hay Repositories. Gom hết vào lớp tĩnh `ValidationUtils` để dễ tái sử dụng và test độc lập.
- **Truyền lỗi rõ ràng**: Dùng `QString& error` trong các Repository/Controller để tầng giao diện hiển thị đúng nguyên nhân lỗi thay vì chỉ `true`/`false` chung chung.
- **Dùng kiểu ngày chuẩn của Qt**: Dùng `QDate` thay vì xử lý chuỗi ngày thủ công, tránh lỗi năm nhuận hoặc số ngày sai trong tháng.
- **Tách target test theo module**: Mỗi module (`Utils`, `Database`, `RoomFactory`, `Controllers`) có target CMake và file test riêng giúp build nhanh hơn, và lỗi ở module này không che khuất kết quả test của module khác.