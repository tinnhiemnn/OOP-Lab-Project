#include "BookingView.h"

#include <QDate>
#include <QComboBox>
#include <QDateEdit>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QString>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

namespace {
    QString text(QLineEdit* edit) { return edit->text().trimmed(); }
}

BookingView::BookingView(QWidget* parent)
    : QWidget(parent), groupCodeEdit(new QLineEdit(this)), customerIdEdit(new QLineEdit(this)),
      searchEdit(new QLineEdit(this)), checkInEdit(new QDateEdit(QDate::currentDate(), this)),
      checkOutEdit(new QDateEdit(QDate::currentDate().addDays(1), this)), addRoomBtn(new QPushButton("+", this)),
      buffetQtyEdit(new QSpinBox(this)), laundryCheck(new QCheckBox("Laundry service", this)),
      decorCheck(new QCheckBox("Decoration service", this)), decorNotesEdit(new QLineEdit(this)) {
    checkInEdit->setCalendarPopup(true);
    checkOutEdit->setCalendarPopup(true);
    checkInEdit->setDisplayFormat("yyyy-MM-dd");
    checkOutEdit->setDisplayFormat("yyyy-MM-dd");

    groupCodeEdit->setPlaceholderText("Group ID");

    // --- Dịch vụ đi kèm booking (trước đây gắn ở Room) ---
    buffetQtyEdit->setRange(0, 50);
    buffetQtyEdit->setSuffix(" ticket");

    decorNotesEdit->setPlaceholderText("Decoration notes (flowers, balloons, banners...)");
    decorNotesEdit->setEnabled(false);
    connect(decorCheck, &QCheckBox::toggled, decorNotesEdit, &QLineEdit::setEnabled);

    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(8);
    form->addWidget(new QLabel("Group ID", this), 0, 0);
    form->addWidget(groupCodeEdit, 0, 1);
    form->addWidget(new QLabel("Customer ID", this), 1, 0);
    form->addWidget(customerIdEdit, 1, 1);
    form->addWidget(new QLabel("Check-in", this), 0, 2);
    form->addWidget(checkInEdit, 0, 3);
    form->addWidget(new QLabel("Check-out", this), 1, 2);
    form->addWidget(checkOutEdit, 1, 3);
    form->setColumnStretch(1, 1);
    form->setColumnStretch(3, 1);

    // --- Danh sách phòng trong nhóm booking, bấm "+" để thêm phòng ---
    auto* roomsHeader = new QHBoxLayout;
    roomsHeader->addWidget(new QLabel("Room List", this));
    roomsHeader->addStretch();
    addRoomBtn->setProperty("variant", "ghost");
    addRoomBtn->setFixedWidth(32);
    addRoomBtn->setToolTip("Add room into group");
    roomsHeader->addWidget(addRoomBtn);

    roomsLayout = new QVBoxLayout();
    roomsLayout->setSpacing(6);

    auto* roomsBox = new QVBoxLayout;
    roomsBox->addLayout(roomsHeader);
    roomsBox->addLayout(roomsLayout);

    connect(addRoomBtn, &QPushButton::clicked, this, [this] { addRoomRow(); });

    auto* services = new QGridLayout;
    services->setHorizontalSpacing(16);
    services->setVerticalSpacing(8); // Tăng khoảng cách dòng một chút cho thoáng và dễ nhìn

    services->addWidget(new QLabel("Buffet:", this), 0, 0);
    services->addWidget(buffetQtyEdit, 0, 1);
    services->addWidget(laundryCheck, 1, 0, 1, 2); 
    services->addWidget(decorCheck, 2, 0);
    services->addWidget(decorNotesEdit, 2, 1);

    services->setColumnStretch(0, 0);
    services->setColumnStretch(1, 1);

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Book", this);
    addBtn->setProperty("variant", "primary");

    auto* inBtn = new QPushButton("Check-in", this);
    inBtn->setProperty("variant","ghost");

    auto* outBtn = new QPushButton("Check-out", this);
    outBtn->setProperty("variant", "ghost");

    auto* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setProperty("variant", "danger");

    auto* reloadBtn = new QPushButton("Refresh", this);
    reloadBtn->setProperty("variant", "ghost");

    actions->addWidget(addBtn);
    actions->addWidget(inBtn);
    actions->addWidget(outBtn);
    actions->addWidget(cancelBtn);
    actions->addWidget(reloadBtn);

    // --- Card 1: form đặt phòng (nhóm phòng + dịch vụ) + các nút hành động ---
    formCard = new DashboardCard("Booking", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(roomsBox);
    formCard->addContentLayout(services);
    formCard->addContentLayout(actions);

    addRoomRow(); // luôn có sẵn 1 dòng Room ID đầu tiên khi mở form

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setProperty("variant", "primary");

    searchEdit->setPlaceholderText("Search bookings");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);


    // --- Kanban board: 4 cột trạng thái ---
    auto buildColumn = [this](const QString& title, QVBoxLayout*& colRef) {
        auto* colFrame = new QFrame(this);
        colFrame->setObjectName("kanbanColumn");
        auto* outer = new QVBoxLayout(colFrame);

        auto* colTitle = new QLabel(title, this);
        colTitle->setProperty("role", "columnTitle");
        outer->addWidget(colTitle);

        colRef = new QVBoxLayout();
        colRef->setSpacing(10);
        colRef->addStretch(); // giữ thẻ dồn lên trên
        outer->addLayout(colRef);

        return colFrame;
    };

    auto* kanbanRow = new QHBoxLayout();
    kanbanRow->addWidget(buildColumn("Booked", colBooked));
    kanbanRow->addWidget(buildColumn("Checked-in", colCheckedIn));
    kanbanRow->addWidget(buildColumn("Checked-out", colCheckedOut));
    kanbanRow->addWidget(buildColumn("Cancelled", colCancelled));

    // --- Card 2: ô tìm kiếm + kanban board ---
    tableCard = new DashboardCard("Booking List", "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContentLayout(kanbanRow);

    // --- Layout tổng thể card 1 + card 2 ---
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10);
    layout->addWidget(formCard);
    layout->addWidget(tableCard, /*stretch=*/1);
}

void BookingView::addRoomRow(const QString& roomId) {
    // Mỗi dòng gồm 1 ô nhập Room ID + 1 nút "×" để xoá dòng đó khỏi nhóm booking
    auto* rowWidget = new QWidget(this);
    auto* row = new QHBoxLayout(rowWidget);
    row->setContentsMargins(0, 0, 0, 0);

    auto* roomEdit = new QLineEdit(this);
    roomEdit->setPlaceholderText("Room ID");
    if (!roomId.isEmpty()) roomEdit->setText(roomId);

    auto* removeBtn = new QPushButton("×", this);
    removeBtn->setProperty("variant", "ghost");
    removeBtn->setFixedWidth(28);
    removeBtn->setToolTip("Bỏ phòng này khỏi nhóm");

    row->addWidget(roomEdit, /*stretch=*/1);
    row->addWidget(removeBtn);

    roomIdEdits.append(roomEdit);
    roomsLayout->addWidget(rowWidget);

    connect(removeBtn, &QPushButton::clicked, this, [this, roomEdit, rowWidget] {
        if (roomIdEdits.size() <= 1) return; // luôn giữ ít nhất 1 phòng trong nhóm
        roomIdEdits.removeOne(roomEdit);
        rowWidget->deleteLater();
    });
}

void BookingView::payGroup(const QString& groupCode) {
    groupCodeEdit->setText(groupCode);
}

QFrame* BookingView::createBookingCard(const QString& groupCode, const QString& customerId,
                                       const QString& roomIds, const QString& checkIn,
                                       const QString& checkOut, const QString& status) {
    auto* card = new QFrame(this);
    card->setObjectName("bookingCard");

    auto* layout = new QVBoxLayout(card);
    layout->setSpacing(4);

    auto* idLbl = new QLabel("Nhóm #" + groupCode, this);
    idLbl->setProperty("role", "cardId");

    auto* customerLbl = new QLabel("Khách: " + customerId, this);
    auto* roomLbl = new QLabel("Phòng: " + roomIds, this);
    auto* dateLbl = new QLabel(checkIn + " → " + checkOut, this);

    auto* statusLbl = new QLabel(status, this);
    QString key = status.toLower().remove('-');
    statusLbl->setProperty("status", key);
    statusLbl->style()->unpolish(statusLbl);
    statusLbl->style()->polish(statusLbl);

    auto* payBtn = new QPushButton("Thanh toán", this);
    payBtn->setProperty("variant", "primary");
    connect(payBtn, &QPushButton::clicked, this, [this, groupCode] { payGroup(groupCode); });

    layout->addWidget(idLbl);
    layout->addWidget(customerLbl);
    layout->addWidget(roomLbl);
    layout->addWidget(dateLbl);
    layout->addWidget(statusLbl);
    layout->addWidget(payBtn);

    return card;
}

void BookingView::clearColumn(QVBoxLayout* col) {
    while (col->count() > 1) {
        QLayoutItem* item = col->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }
}