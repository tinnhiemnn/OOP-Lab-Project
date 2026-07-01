#pragma once

#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QSpinBox;
class QTableWidget;

class RoomView : public QWidget {
public:
    explicit RoomView(QWidget* parent = nullptr);

private:
    void reload();
    void selected();
    void add();
    void update();
    void remove();
    void search();
    void error(const std::string& message);

    QLineEdit* idEdit;
    QComboBox* typeEdit;
    QDoubleSpinBox* priceEdit;
    QComboBox* statusEdit;
    QSpinBox* bedsEdit;
    QLineEdit* servicesEdit;
    QComboBox* filterType;
    QComboBox* filterStatus;
    QTableWidget* table;
};

