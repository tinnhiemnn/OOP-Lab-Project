#pragma once

#include "controllers/BookingController.h"
#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "services/BookingService.h"
#include "MultiBookingRequest.h"

#include <DashboardCard.h>
#include <QWidget>
#include <QVector>

class QDateEdit;
class QLineEdit;
class QTableWidget;
class QFrame;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QVBoxLayout;

// Mỗi dòng trong "Room List" = 1 phòng sẽ trở thành 1 Booking record riêng khi bấm "Book"
// (BookingController::createBooking hiện đã nhận đúng 1 phòng/1 lần gọi, xem BookingView::add()).
// Dịch vụ (buffet/laundry/decoration) đi kèm TỪNG phòng vì Booking.h lưu field này theo từng record.
struct RoomServiceRow {
    QWidget* rowWidget = nullptr;
    QLineEdit* roomIdEdit = nullptr;
    QSpinBox* buffetQtyEdit = nullptr;
    QCheckBox* laundryCheck = nullptr;
    QCheckBox* decorCheck = nullptr;
    QLineEdit* decorNotesEdit = nullptr;
};

class BookingView : public QWidget {
public:
    explicit BookingView(QWidget* parent = nullptr);

private:
    void refresh(const std::vector<Booking>& rows);
    void reload();
    void selected(int row, int column);   // click 1 dòng trên table -> đổ dữ liệu lên form
    void add();
    void checkIn();
    void checkOut();
    void cancel();
    void search();
    void error(const QString& message);

    void addRoomRow(const QString& roomId = QString());
    void clearExtraRoomRows();   // giữ lại đúng 1 dòng Room khi đổ dữ liệu 1 booking đã chọn lên form

    // TODO (backend): add() bên dưới gọi controller.createMultiBookings(request, error).
    // BookingController hiện CHƯA có hàm này, cần backend bổ sung
    // (xem danh sách hàm cần thiết đã gửi kèm). Hàm này thay thế hoàn toàn cách gọi
    // createBooking() lặp lại từng phòng như trước, để có transaction (atomic) thật sự.
    BookingRepository repository;
    RoomRepository roomRepository;
    BookingService service;
    BookingController controller;

    QLineEdit* bookingIdEdit;    // Booking ID (READ-ONLY)
    QLineEdit* groupCodeEdit;
    QLineEdit* customerIdEdit;
    QLineEdit* receptionistIdEdit;
    QLineEdit* searchEdit;
    QDateEdit* checkInEdit;
    QDateEdit* checkOutEdit;

    QVector<RoomServiceRow> roomRows;   // danh sách các dòng Room + dịch vụ riêng của từng phòng
    QVBoxLayout* roomsLayout;
    QPushButton* addRoomBtn;

    QTableWidget* bookingTable;
    std::vector<Booking> currentRows;   // cache dữ liệu đang hiển thị trên table, dùng khi selected()

    DashboardCard* formCard;
    DashboardCard* tableCard;
};