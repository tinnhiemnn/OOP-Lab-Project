#pragma once

//#include "controllers/BookingController.h"

#include <DashboardCard.h>
#include <QWidget>
#include <QVector>

class QComboBox;
class QDateEdit;
class QLineEdit;
class QTableWidget;
class QFrame;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QVBoxLayout;

class BookingView : public QWidget {
public:
    explicit BookingView(QWidget* parent = nullptr);

private:
    //void refresh(const std::vector<Booking>& rows);
    void reload();
    void selected();
    void add();
    void checkIn();
    void checkOut();
    void cancel();
    void search();
    void error(const QString& message);

    void addRoomRow(const QString& roomId = QString()); // bấm "+" để thêm 1 dòng Room ID vào nhóm booking
    void payGroup(const QString& groupCode);            // bấm "Thanh toán" trên thẻ booking -> tự điền mã nhóm lên form

    //BookingController controller;
    QLineEdit* groupCodeEdit;   // Mã nhóm booking (thay cho Booking ID cũ) - 1 nhóm có thể gồm nhiều phòng
    QLineEdit* customerIdEdit;
    QLineEdit* searchEdit;
    QDateEdit* checkInEdit;
    QDateEdit* checkOutEdit;

    QVector<QLineEdit*> roomIdEdits; // danh sách Room ID trong nhóm, thêm/bớt bằng nút "+"/"×"
    QVBoxLayout* roomsLayout;        // layout chứa các dòng Room ID
    QPushButton* addRoomBtn;         // nút "+" thêm phòng

    // --- Dịch vụ đi kèm booking (trước đây gắn ở Room, giờ chuyển qua đây) ---
    QSpinBox* buffetQtyEdit;   // số lượng suất buffet sử dụng
    QCheckBox* laundryCheck;   // có sử dụng giặt ủi hay không
    QCheckBox* decorCheck;     // có trang trí phòng hay không
    QLineEdit* decorNotesEdit; // ghi chú trang trí, chỉ bật khi decorCheck được tick

    QVBoxLayout* colBooked;
    QVBoxLayout* colCheckedIn;
    QVBoxLayout* colCheckedOut;
    QVBoxLayout* colCancelled;

    DashboardCard* formCard;
    DashboardCard* tableCard;
    QFrame* createBookingCard(const QString& groupCode, const QString& customerId,
                              const QString& roomIds, const QString& checkIn,
                              const QString& checkOut, const QString& status);
    void clearColumn(QVBoxLayout* col);
};