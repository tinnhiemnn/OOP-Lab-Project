#pragma once

#include "controllers/InvoiceController.h"
#include "models/Invoice.h"
#include "DashboardCard.h"
#include <QWidget>
#include <vector>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QTableWidget;
class QFrame;

class InvoiceView : public QWidget {
public:
    explicit InvoiceView(QWidget* parent = nullptr);

private:
    void refresh(const std::vector<Invoice>& rows);
    void reload();
    void selected();
    void add();
    void addGroup();          // NEW: "Create All Invoices in Group"
    void search();
    void applyFilters();      // NEW: client-side filter by discount/payment
    void error(const QString& message);

    InvoiceController controller;

    // Cache of the last loaded/searched list, so filters can be applied
    // without re-querying the backend every time.
    std::vector<Invoice> currentInvoices;

    QLineEdit* bookingIdEdit;
    QLineEdit* receptionistIdEdit;
    QComboBox* discountEdit;
    QComboBox* paymentEdit;
    QLineEdit* searchEdit;
    QComboBox* discountFilter;   // NEW
    QComboBox* paymentFilter;    // NEW
    QTableWidget* table;
    DashboardCard* formCard;
    DashboardCard* tableCard;
};