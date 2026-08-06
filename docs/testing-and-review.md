# Báo cáo Kiểm thử, Xử lý lỗi và Đánh giá mã nguồn (Review Code)

---

## 1. Kiểm thử (Testing)

### 1.1. Danh sách test cases

| Mã Test | Chức năng kiểm thử | Dữ liệu đầu vào | Kết quả mong đợi | Kết quả thực tế | Trạng thái | Target / File test |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **TC01** | Kiểm tra ngày hợp lệ | `"2026-06-28"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC02** | Kiểm tra năm nhuận hợp lệ | `"2024-02-29"`, `"2400-02-29"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC03** | Kiểm tra ngày không tồn tại | `"2026-02-29"`, `"2100-02-29"`, `"2026-13-01"`, `"2026-00-01"`, `"2026-12-32"`, `"invalid-date"` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC04** | Kiểm tra khoảng ngày đặt phòng hợp lệ | Check-in: `"2026-06-28"`, Check-out: `"2026-06-29"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC05** | Kiểm tra khoảng ngày ngược/bằng nhau | Check-in sau hoặc trùng ngày Check-out (`"2026-06-29"`, `"2026-06-28"` hoặc cùng ngày) | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC06** | Tính số ngày thuê phòng | Check-in: `"2026-06-01"`, Check-out: `"2026-06-11"` | Trả về `10` ngày | Trả về `10` | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC07** | Kiểm tra trùng lịch đặt phòng (Overlap) | Giao nhau (`"2026-06-10"`–`12` & `"2026-06-11"`–`13`) vs Liền kề / Tách biệt | Trả về `true` khi trùng, `false` khi liền kề / không trùng | Đúng như kỳ vọng | Đạt | `UtilsTests` (`testDateUtils`) |
| **TC08** | Kiểm tra chuỗi bắt buộc (Non-empty) | `"abc"` vs Chuỗi rỗng `""` hoặc chỉ khoảng trắng `"   "` | `"abc"` -> `true`, chuỗi rỗng/khoảng trắng -> `false` | Trả về chính xác | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC09** | Kiểm tra email định dạng chuẩn | `"test@example.com"` | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC10** | Kiểm tra email sai định dạng | `"test.example@com"`, `"@example.com"`, `"test@.com"` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC11** | Kiểm tra số điện thoại hợp lệ | `"0987654321"` (10 chữ số) | Trả về `true` (Hợp lệ) | Trả về `true` | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC12** | Kiểm tra số điện thoại sai định dạng | Chứa chữ: `"098765abcd"`, Quá ngắn: `"12345678"`, Quá dài: `"1234567890123"` | Trả về `false` (Không hợp lệ) | Trả về `false` | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC13** | Kiểm tra số tiền hợp lệ | `100.5`, `0.0`, `-5.0` | Chỉ giá trị dương (`100.5`) trả về `true` | Trả về `true` cho `100.5`, `false` cho `0.0` và `-5.0` | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC14** | Kiểm tra `isValidBookingInput` | Thiếu Customer ID, thiếu Room ID hoặc ngày sai | Trả về `false` kèm lỗi tương ứng | Đúng thông báo lỗi từng trường hợp | Đạt | `UtilsTests` (`testValidationUtils`) |
| **TC15** | Kiểm tra kết nối lại CSDL | Đóng kết nối DB tạm -> mở lại -> thực thi `SELECT 1` | `openDatabase` trả về `true` và câu lệnh `SELECT 1` thực thi thành công | Mở lại thành công | Đạt | `DatabaseTests` / `ProgramTests` (`testDatabaseReconnect`) |
| **TC16** | Ràng buộc xóa phòng có Booking | Xóa phòng `"R999"` đang có booking ở tầng Repo | Repo cho phép xóa (`true`), constraint được đảm bảo xử lý ở Controller (`RoomController::deleteRoom`) | Repo xóa `true` (đúng thiết kế phân tầng) | Đạt | `DatabaseTests` / `ProgramTests` (`testRoomDeleteConstraint`) |
| **TC17** | Ràng buộc xóa khách hàng có Booking | Xóa khách hàng `"CUS998"` đang có booking ở tầng Repo | Repo cho phép xóa (`true`), constraint xử lý ở Controller (`CustomerController::deleteCustomer`) | Repo xóa `true` (đúng thiết kế phân tầng) | Đạt | `DatabaseTests` / `ProgramTests` (`testCustomerDeleteConstraint`) |
| **TC18** | Đặt phòng với Receptionist ID rỗng (Service layer) | `receptionistId = ""` trong `BookingService::createMultiBookings` | Trả về `false` kèm lỗi (do ràng buộc FK/CSDL) | Trả về `false` và `error` không rỗng | Đạt | `DatabaseTests` / `ProgramTests` (`testBookingEmptyReceptionistInsert`) |
| **TC19** | Kiểm tra logic nghiệp vụ Booking Service | Đặt phòng không chọn phòng, ngày sai, quá khứ, thông tin dịch vụ sai, phòng bảo trì, Check-In/Check-Out/Hủy không đúng trạng thái | Trả về `false` kèm thông báo lỗi chuẩn xác tương ứng với từng điều kiện vi phạm | Đúng các chuỗi thông báo lỗi nghiệp vụ | Đạt | `ProgramTests` / Service Test (`runBookingServiceTests`) |
| **TC20** | Kiểm tra Validation tầng Controller (Booking) | Khách hàng rỗng, Lễ tân rỗng, Check-In/Out/Cancel rỗng hoặc chỉ chứa khoảng trắng | Trả về `false` kèm lỗi đúng quy định ("Please select or enter...", "Receptionist ID cannot be empty!", ...) | Đúng thông báo lỗi | Đạt | `ControllerTests` (`runControllerTests`) |
| **TC21** | Bypass validation ở tầng Controller (Customer) | Thêm `Customer` sai format ("CBAD", "Bad Name", "not-an-email", "abc1234") qua `CustomerController` | Controller trả về `false` (từ chối thêm dữ liệu không hợp lệ) | Trả về `false` | Đạt | `ControllerTests` (`runControllerValidationTests`) |
| **TC22** | Ràng buộc xóa Phòng / Khách hàng ở tầng Controller | Gọi `deleteRoom("P301")` / `deleteCustomer("CUS003")` khi có booking đang hoạt động | Trả về `false` kèm lỗi "Cannot delete a room/customer..." | Trả về `false` kèm lỗi chuẩn | Đạt | `RepositoryTests` (`runRepositoryTests`) |
| **TC23** | Thêm phòng trùng lặp ở Repository | Thêm 2 lần phòng `"R101"` | Lần thứ 2 trả về `false`, `lastError()` không rỗng | Trả về `false` | Đạt | `RepositoryTests` (`runRepositoryTests`) |
| **TC24** | Chèn Booking với Receptionist rỗng ở Repository | Chèn `Booking` có `receptionistId = ""` | DB từ chối do vi phạm FK, trả về `false` | Trả về `false` | Đạt | `RepositoryTests` (`runRepositoryTests`) |

---

## 1.2. Hướng dẫn chạy biên dịch và kiểm thử từ dòng lệnh (Terminal Guide)

Các lệnh dưới đây áp dụng cho dự án sử dụng **Qt 6**, **CMake** và bộ biên dịch C++ (MinGW / MSVC / Clang).

#### Bước 1: Mở terminal tại thư mục gốc dự án

```powershell
cd <duong-dan-toi-thu-muc-OOP-Lab-Project>
```

Nếu lệnh `cmake` chưa có trong `PATH`, sử dụng đường dẫn đầy đủ tới CMake trên máy:

```powershell
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --version
```

#### Bước 2: Cấu hình thư mục build (bật flag `-DBUILD_TESTS=ON` để build test)

Mặc định dự án tắt build test để tối ưu thời gian biên dịch chương trình chính (`HotelManagement`). Khi muốn chạy test, hãy truyền flag `-DBUILD_TESTS=ON`:

```powershell
cmake -S . -B build -DBUILD_TESTS=ON
```

Nếu dùng MinGW trên Windows và cần chỉ định generator:

```powershell
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER="g++" -S . -B build -DBUILD_TESTS=ON
```

Nếu CMake không tự tìm thấy Qt, truyền thêm tham số `CMAKE_PREFIX_PATH`:

```powershell
cmake -S . -B build -DBUILD_TESTS=ON -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
```

#### Bước 3: Build các target kiểm thử

Build toàn bộ project và tất cả target test:

```powershell
cmake --build build
```

Hoặc build từng target cụ thể:

```powershell
cmake --build out/build/tests --target UtilsTests
cmake --build out/build/tests --target DatabaseTests
cmake --build out/build/tests --target RepositoryTests
cmake --build out/build/tests --target ControllerTests
cmake --build out/build/tests --target ProgramTests
```

**Ý nghĩa các target**:
- **UtilsTests**: Kiểm tra utility (`DateUtils`, `ValidationUtils`).
- **DatabaseTests**: Kiểm tra kết nối lại database, xóa phòng/khách hàng ở tầng repo, kiểm tra chèn lễ tân rỗng.
- **RepositoryTests**: Kiểm tra thao tác repository cơ bản và các ràng buộc bổ sung ở controller/repo.
- **ControllerTests**: Kiểm tra validation ở tầng controller (bao gồm `BookingController` và `CustomerController`).
- **ProgramTests**: Chạy tổng hợp các bài test hệ thống (Database, Service, Controller).

#### Bước 4: Chạy executable test

Trên Windows PowerShell:

```powershell
.\out\build\tests\UtilsTests.exe
.\out\build\tests\DatabaseTests.exe
.\out\build\tests\RepositoryTests.exe
.\out\build\tests\ControllerTests.exe
.\out\build\tests\ProgramTests.exe
```

Trên macOS / Linux:

```bash
./out/build/tests/UtilsTests
./out/build/tests/DatabaseTests
./out/build/tests/RepositoryTests
./out/build/tests/ControllerTests
./out/build/tests/ProgramTests
```

Chạy tự động toàn bộ test suite trên PowerShell:

```powershell
$tests = "UtilsTests", "DatabaseTests", "RepositoryTests", "ControllerTests", "ProgramTests"
foreach ($test in $tests) {
    Write-Host "Running $test..." -ForegroundColor Green
    & ".\out\build\tests\$test.exe"
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Test $test failed!" -ForegroundColor Red
        exit $LASTEXITCODE
    }
}
```

Ví dụ output thành công từ **ProgramTests**:

```text
Database reconnect test passed!
Room delete constraint test passed! (Note: constraint enforced at Controller level)
Customer delete constraint test passed! (Note: constraint enforced at Controller level)
Booking empty receptionist test passed!
Booking service tests passed!
Controller tests passed!
Controller validation tests passed!
All program tests passed successfully!
```

---

## 2. Nhật ký phát hiện và xử lý lỗi (Project-wide Bug Audit Log & Reproduce Steps)

### Chi tiết các lỗi phát hiện và hướng dẫn kiểm thử:

### [Controllers/Repositories] - Xóa phòng đang có booking vẫn xóa được ở Repository

* **Target build:** `DatabaseTests` / `ProgramTests` / `RepositoryTests`
* **File test:** `tests/database_tests.cpp` (`testRoomDeleteConstraint`) & `tests/repository_tests.cpp`

* **Miêu tả bug**: Nếu xóa trực tiếp ở `RoomRepository::remove(id)`, repository sẽ thực thi lệnh xóa và phụ thuộc vào `ON DELETE SET NULL` trong CSDL. Tuy nhiên, quy tắc nghiệp vụ yêu cầu **chặn không cho xóa phòng đang có booking hoạt động (`Booked` hoặc `CheckedIn`)**.
* **Trạng thái xử lý**: Ràng buộc này đã được đưa lên xử lý tại tầng Controller (`RoomController::deleteRoom`). Repository được giữ nguyên thiết kế xóa thuần túy CSDL.
* **Minh họa kiểm thử**:
  - `testRoomDeleteConstraint()` xác nhận `roomRepo.remove()` trả về `true` (Repository chấp nhận lệnh xóa).
  - `runRepositoryTests()` xác nhận `RoomController::deleteRoom("P301", error)` trả về `false` với thông báo lỗi: `"Cannot delete a room that currently has Booked or Checked In bookings."`.

---

### [Customers] - Xóa khách hàng đang có booking vẫn xóa được ở Repository

* **Target build:** `DatabaseTests` / `ProgramTests` / `RepositoryTests`
* **File test:** `tests/database_tests.cpp` (`testCustomerDeleteConstraint`) & `tests/repository_tests.cpp`

* **Miêu tả bug**: Tương tự như với Phòng, việc gọi trực tiếp `CustomerRepository::remove()` sẽ xóa khách hàng khỏi CSDL.
* **Trạng thái xử lý**: Ràng buộc nghiệp vụ được chặn tại `CustomerController::deleteCustomer()`.
* **Minh họa kiểm thử**:
  - `testCustomerDeleteConstraint()` xác nhận `customerRepo.remove()` trả về `true`.
  - `runRepositoryTests()` xác nhận `CustomerController::deleteCustomer("CUS003", error)` trả về `false` với câu thông báo lỗi: `"Cannot delete a customer who currently has active Booked or Checked In bookings."`.

---

### [DatabaseManager] - Mở lại kết nối sau khi đóng (`closeConnection`)

* **Target build:** `DatabaseTests` / `ProgramTests`
* **File test:** `tests/database_tests.cpp` (`testDatabaseReconnect`)

* **Miêu tả bug**: Khi ứng dụng hoặc các bộ test gọi `DatabaseManager::getInstance().closeConnection()`, nếu không khởi tạo/khôi phục lại đối tượng CSDL đúng cách, lệnh mở lại CSDL `openDatabase(path)` sẽ thất bại.
* **Trạng thái xử lý**: Hàm `testDatabaseReconnect()` đảm bảo rằng sau khi đóng kết nối, việc gọi `openDatabase` và thực thi truy vấn SQL (`SELECT 1`) diễn ra bình thường.

---

### [Controllers] - Kiểm tra dữ liệu đầu vào khách hàng (Validation Bypass)

* **Target build:** `ControllerTests` / `ProgramTests`
* **File test:** `tests/controller_tests.cpp` (`runControllerValidationTests`)

* **Miêu tả bug**: Nếu Controller không gọi `ValidationUtils` trước khi chuyển dữ liệu sang Repository, các thông tin khách hàng không hợp lệ (như email không đúng định dạng `not-an-email`, SĐT `abc1234`) sẽ bị lưu trực tiếp vào CSDL.
* **Trạng thái xử lý**: `CustomerController` kiểm tra validation hợp lệ trước khi lưu.
* **Minh họa kiểm thử**:
  ```cpp
  Customer badCust("CBAD", "Bad Name", "not-an-email", "abc1234");
  bool result = custCtrl.addCustomer(badCust, error);
  assert(result == false); // Controller phải chặn thành công
  ```

---

### [BookingService] - Xử lý trường hợp ID Lễ tân rỗng (Empty Receptionist ID)

* **Target build:** `DatabaseTests` / `ControllerTests` / `ProgramTests`
* **File test:** `tests/database_tests.cpp` (`testBookingEmptyReceptionistInsert`), `tests/controller_tests.cpp` (`runControllerTests`)

* **Miêu tả bug**: `BookingService` hoặc `BookingController` chấp nhận tham số Lễ tân rỗng (`""`), dẫn tới việc tạo ra bản ghi booking vi phạm khóa ngoại trong CSDL.
* **Trạng thái xử lý**: 
  - Tại `BookingController::createMultiBookings`: Kiểm tra và trả về `false` ngay lập tức với lỗi `"Receptionist ID cannot be empty!"`.
  - Tại `BookingService` / `BookingRepository`: Lệnh chèn dữ liệu vi phạm FK bị CSDL từ chối và trả về `false`.

---

## 3. Thực hiện kiểm tra tính hợp lệ dữ liệu đầu vào (Input Validation)

Các quy tắc kiểm tra dữ liệu trong `ValidationUtils` và `DateUtils`:

1. **Kiểm tra Ngày & Khoảng thời gian (`DateUtils`)**:
   - `isValidDate`: Chuỗi ngày phải theo định dạng `YYYY-MM-DD` hợp lệ (xử lý chính xác năm nhuận như `2024-02-29`, `2400-02-29` và loại bỏ ngày bất hợp lệ `2026-02-29`, `2100-02-29`).
   - `isDateRangeValid`: Ngày Check-out phải **sau** ngày Check-in.
   - `datesOverlap`: Kiểm tra hai khoảng thời gian thuê phòng có bị đè lên nhau hay không (hai khoảng thời gian giao nhau thì overlap, liền kề nhau thì không overlap).

2. **Kiểm tra Định dạng & Chuỗi (`ValidationUtils`)**:
   - `isNonEmpty`: Chuỗi phải chứa ký tự khác khoảng trắng (chuỗi rỗng `""` hoặc chỉ chứa khoảng trắng `"   "` đều không hợp lệ).
   - `isValidEmail`: Email phải chứa `@` và `.`, không được để trống domain/username.
   - `isValidPhone`: Số điện thoại chỉ chứa các chữ số từ `0` đến `9`, độ dài từ 9–12 ký tự.
   - `isPositiveMoney`: Số tiền/giá phòng phải lớn hơn `0.0`.
   - `isValidBookingInput`: Kiểm tra tổng thể mã Khách hàng, mã Phòng và tính hợp lệ của ngày Check-in/Check-out.

---

## 4. Audit Log - Danh sách các lỗi (Bugs) được kiểm tra và xử lý

### Bug 1: Room Deletion Constraint
- **Triệu chứng**: Xóa phòng trực tiếp từ Repository có thể gây ảnh hưởng tới lịch sử booking.
- **Giải pháp**: Xử lý logic tại `RoomController::deleteRoom()` - chặn xóa phòng nếu phòng có booking ở trạng thái `Booked` hoặc `CheckedIn`.
- **Mã test kiểm tra**: `testRoomDeleteConstraint()` và `runRepositoryTests()`.

### Bug 2: Customer Deletion Constraint
- **Triệu chứng**: Xóa khách hàng trực tiếp từ Repository làm mất thông tin liên kết booking.
- **Giải pháp**: Xử lý logic tại `CustomerController::deleteCustomer()` - chặn xóa khách hàng nếu đang có booking `Booked` hoặc `CheckedIn`.
- **Mã test kiểm tra**: `testCustomerDeleteConstraint()` và `runRepositoryTests()`.

### Bug 3: Customer Controller Input Validation Bypass
- **Triệu chứng**: Dữ liệu khách hàng sai định dạng email/SĐT bị ghi vào CSDL nếu Controller không validate.
- **Giải pháp**: Gọi `ValidationUtils` trong `CustomerController` trước khi thực hiện lưu.
- **Mã test kiểm tra**: `runControllerValidationTests()`.

### Bug 4: Booking Creation with Empty Receptionist ID
- **Triệu chứng**: Đặt phòng thành công mà không có thông tin Lễ tân thực hiện.
- **Giải pháp**: Bắt lỗi tại `BookingController` (trả về lỗi `"Receptionist ID cannot be empty!"`) và ràng buộc khóa ngoại ở tầng DB/Repository.
- **Mã test kiểm tra**: `testBookingEmptyReceptionistInsert()`, `runControllerTests()`, `runRepositoryTests()`.

### Bug 5: Database Connection Re-open Failure
- **Triệu chứng**: Không thể mở lại CSDL sau khi gọi `closeConnection()`.
- **Giải pháp**: Đảm bảo `DatabaseManager::openDatabase` thiết lập lại kết nối SQLite nếu trạng thái kết nối bị đóng.
- **Mã test kiểm tra**: `testDatabaseReconnect()`.

### Bug 6: Booking Business Rules Violation
- **Triệu chứng**: Đặt phòng cho ngày trong quá khứ, phòng đang bảo trì, hoặc thao tác Check-In/Check-Out không đúng quy trình trạng thái phòng.
- **Giải pháp**: Kiểm tra điều kiện trong `BookingService` và trả về đúng thông báo lỗi (ví dụ: `"No rooms selected!"`, `"Check-out date must be after check-in date."`, `"The room is under maintenance and cannot be booked!"`, v.v.).
- **Mã test kiểm tra**: `runBookingServiceTests()`.

---

## 5. Đánh giá mã nguồn (Code Review)

- **Phân tầng rõ ràng (Architectural Layering)**:
  - Tầng **Repository** (`RoomRepository`, `CustomerRepository`, `BookingRepository`) tập trung vào các thao tác CRUD CSDL cơ bản.
  - Tầng **Service** (`BookingService`) xử lý nghiệp vụ đặt phòng, tính toán ngày và trạng thái phòng.
  - Tầng **Controller** (`BookingController`, `CustomerController`, `RoomController`) đóng vai trò kiểm tra tính hợp lệ dữ liệu đầu vào (Input Validation) và áp dụng các ràng buộc loại trừ dữ liệu trước khi gọi Service/Repository.
- **Sử dụng Assertion trong Unit Test**: Các hàm test sử dụng `cassert` (`assert(...)`) giúp phát hiện chính xác vị trí bị lỗi khi chạy test suite.
- **Quản lý CSDL tạm thời cho Testing**: Việc sử dụng các helper function trong `TestHelpers.h` (`openTemporaryDatabase`, `cleanupTemporaryDatabase`) giúp tạo CSDL SQLite tạm thời riêng biệt cho từng bài test, tránh gây xung đột dữ liệu giữa các lần test.