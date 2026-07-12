#pragma once

//#include "controllers/InvoiceController.h"
#include "DashboardCard.h"
#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QTableWidget;
class QFrame;

class InvoiceView : public QWidget {
public:
    explicit InvoiceView(QWidget* parent = nullptr);

private:
    void reload();
    void selected();
    void add();
    void search();
    void error(const QString& message);

    QLineEdit* bookingIdEdit;
    QDoubleSpinBox* serviceChargeEdit;
    QComboBox* discountEdit;
    QComboBox* paymentEdit;
    QLineEdit* searchEdit;
    QVBoxLayout* invoiceList;
    DashboardCard* formCard;
    DashboardCard* tableCard;
};