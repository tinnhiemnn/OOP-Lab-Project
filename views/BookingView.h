#pragma once

#include <QWidget>

class QDateEdit;
class QLineEdit;
class QTableWidget;

class BookingView : public QWidget {
public:
    explicit BookingView(QWidget* parent = nullptr);

private:
    void reload();
    void selected();
    void add();
    void checkIn();
    void checkOut();
    void cancel();
    void search();
    void error(const std::string& message);

    QLineEdit* bookingIdEdit;
    QLineEdit* customerIdEdit;
    QLineEdit* roomIdEdit;
    QLineEdit* searchEdit;
    QDateEdit* checkInEdit;
    QDateEdit* checkOutEdit;
    QTableWidget* table;
};

