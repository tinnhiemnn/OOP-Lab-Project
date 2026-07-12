#pragma once

//#include "controllers/BookingController.h"

#include <DashboardCard.h>
#include <QWidget>


class QComboBox;
class QDateEdit;
class QLineEdit;
class QTableWidget;
class QFrame;

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

    //BookingController controller;
    QLineEdit* bookingIdEdit;
    QLineEdit* customerIdEdit;
    QLineEdit* roomIdEdit;
    QLineEdit* searchEdit;
    QDateEdit* checkInEdit;
    QDateEdit* checkOutEdit;
    QComboBox* servicesEdit;
    QVBoxLayout* colBooked;
    QVBoxLayout* colCheckedIn;
    QVBoxLayout* colCheckedOut;
    QVBoxLayout* colCancelled;

    DashboardCard* formCard;
    DashboardCard* tableCard;
    QFrame* createBookingCard(const QString& id, const QString& customerId,
                              const QString& roomId, const QString& checkIn,
                              const QString& checkOut, const QString& status);
    void clearColumn(QVBoxLayout* col);
};

