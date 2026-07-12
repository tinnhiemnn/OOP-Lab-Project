#pragma once

#include <DashboardCard.h>
#include <QWidget>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QSpinBox;
class QTableWidget;
class QGridLayout;

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
    void error(const QString& message);

    QLineEdit* idEdit;
    QComboBox* typeEdit;
    QDoubleSpinBox* priceEdit;
    QComboBox* statusEdit;
    QSpinBox* bedsEdit;
    QComboBox* servicesEdit;
    QComboBox* filterType;
    QComboBox* filterStatus;
    QGridLayout* roomGrid;

    DashboardCard* formCard;
    DashboardCard* tableCard;
};

