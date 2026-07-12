#pragma once

//#include "controllers/ReportController.h"
//#include "repositories/InvoiceRepository.h"
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

    //ReportController controller;
    //InvoiceRepository invoiceRepository;
    QLabel* revenueLabel;
    QLabel* bookingLabel;
    QLabel* roomLabel;
    QLabel* occupancyLabel;
    QTableWidget* table;
    DashboardCard* heroCard;
    DashboardCard* tableCard;;
};