#pragma once

#include "controllers/RoomController.h"

#include <DashboardCard.h>
#include <QWidget>
#include <QDate>
#include <memory>
#include <vector>

class QComboBox;
class QDoubleSpinBox;
class QLineEdit;
class QSpinBox;
class QTableWidget;
class QDateEdit;
class Room;

class RoomView : public QWidget {
public:
    explicit RoomView(QWidget* parent = nullptr);

private:
    void refresh(std::vector<std::unique_ptr<Room>> rows);
    void reload();
    void selected();
    void add();
    void update();
    void remove();
    void search();
    void checkAvailability();   // NEW: nut "Check Available"
    void error(const QString& message);

    RoomController controller;  // OK: RoomController() co constructor mac dinh, khong can sua

    QLineEdit* idEdit;
    QComboBox* typeEdit;
    QDoubleSpinBox* priceEdit;
    QComboBox* statusEdit;
    QSpinBox* bedsEdit;
    QComboBox* filterType;
    QComboBox* filterStatus;

    QDateEdit* checkInEdit;      // NEW
    QDateEdit* checkOutEdit;     // NEW

    QTableWidget* table;         // CHANGED: thay cho QGridLayout* roomGrid

    DashboardCard* formCard;
    DashboardCard* tableCard;
};