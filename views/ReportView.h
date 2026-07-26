#pragma once

#include "controllers/ReportController.h"
#include "controllers/BookingController.h"
#include "controllers/RoomController.h"
#include "controllers/InvoiceController.h"
#include "DashboardCard.h"
#include <QWidget>

class QLabel;
class QTableWidget;
class QFrame;

class ReportView : public QWidget {
public:
    explicit ReportView(QWidget* parent = nullptr);

private:
    void reload();
    
    ReportController controller;
    BookingController bookingController;
    RoomController roomController;
    InvoiceController invoiceController;

    QLabel* revenueLabel;
    QLabel* bookingLabel;
    QLabel* roomLabel;
    QLabel* occupancyLabel;
    QTableWidget* table;
    DashboardCard* heroCard;
    DashboardCard* tableCard;
};