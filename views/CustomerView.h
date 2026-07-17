#pragma once

#include "controllers/CustomerController.h"
#include <DashboardCard.h>
#include <QWidget>

class QLineEdit;
class QTableWidget;
class QLabel;
class QFrame;

class CustomerView : public QWidget {
public:
    explicit CustomerView(QWidget* parent = nullptr);

private:
    void refresh(const std::vector<Customer>& rows);
    void reload();
    void selected();
    void add();
    void update();
    void remove();
    void search();
    void error(const QString& message);

    CustomerController controller;
    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneEdit;
    QLineEdit* searchEdit;
    QTableWidget* table;

    DashboardCard* formCard;
    DashboardCard* tableCard;
};

