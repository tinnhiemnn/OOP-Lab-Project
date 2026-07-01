#pragma once

//#include "controllers/CustomerController.h"

#include <QWidget>

class QLineEdit;
class QTableWidget;

class CustomerView : public QWidget {
public:
    explicit CustomerView(QWidget* parent = nullptr);

private:
    void reload();
    void selected();
    void add();
    void update();
    void remove();
    void search();
    void error(const std::string& message);

    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QLineEdit* emailEdit;
    QLineEdit* phoneEdit;
    QLineEdit* searchEdit;
    QTableWidget* table;
};

