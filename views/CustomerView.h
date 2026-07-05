#pragma once

#include <QWidget>

class QLineEdit;
class QTableWidget;
class QLabel;
class QFrame;

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

    QFrame* formCard;
    QFrame* tableCard;
};

