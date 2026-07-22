#include "BookingView.h"

#include <QDate>
#include <QDateEdit>
#include <QDateTime>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QString>
#include <QFrame>
#include <QLabel>

namespace {
QString text(QLineEdit* edit) { return edit->text().trimmed(); }
}

BookingView::BookingView(QWidget* parent)
    : QWidget(parent),
    repository(),
    roomRepository(),
    service(repository, roomRepository),
    controller(service),
    bookingIdEdit(new QLineEdit(this)),
    //groupCodeEdit(new QLineEdit(this)),
    customerIdEdit(new QLineEdit(this)),
    receptionistIdEdit(new QLineEdit(this)),
    searchEdit(new QLineEdit(this)),
    checkInEdit(new QDateEdit(QDate::currentDate(), this)),
    checkOutEdit(new QDateEdit(QDate::currentDate().addDays(1), this)),
    addRoomBtn(new QPushButton("+", this)) {

    checkInEdit->setCalendarPopup(true);
    checkOutEdit->setCalendarPopup(true);
    checkInEdit->setDisplayFormat("yyyy-MM-dd");
    checkOutEdit->setDisplayFormat("yyyy-MM-dd");

    // Booking ID chỉ được set khi chọn 1 dòng trên table (dùng cho Check-in/Check-out/Cancel),
    // không cho sửa tay để tránh gõ nhầm sang booking khác.
    bookingIdEdit->setPlaceholderText("Booking ID (chọn từ Booking List)");
    bookingIdEdit->setReadOnly(true);
    
    //groupCodeEdit->setPlaceholderText("Group ID (để trống nếu muốn hệ thống tự sinh)");
    customerIdEdit->setPlaceholderText("Customer ID");
    receptionistIdEdit->setPlaceholderText("Receptionist ID");


    // --- Nhóm thông tin cố định: Booking ID, Group ID, Customer ID, Receptionist ID, Check-in, Check-out ---
    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(6); // giảm để card Booking gọn hơn, nhường chỗ cho Booking List
    form->addWidget(new QLabel("Booking ID", this), 0, 0);
    form->addWidget(bookingIdEdit, 0, 1);
    form->addWidget(new QLabel("Check-in", this), 0, 2);
    form->addWidget(checkInEdit, 0, 3);

    //form->addWidget(new QLabel("Group ID", this), 1, 0);
    //form->addWidget(groupCodeEdit, 1, 1);
    form->addWidget(new QLabel("Check-out", this), 1, 2);
    form->addWidget(checkOutEdit, 1, 3);

    form->addWidget(new QLabel("Customer ID", this), 1, 0);
    form->addWidget(customerIdEdit, 1, 1);
    form->addWidget(new QLabel("Receptionist ID", this), 2, 0);
    form->addWidget(receptionistIdEdit, 2, 1);

    form->setColumnStretch(1, 1);
    form->setColumnStretch(3, 1);

    // --- Danh sách phòng trong nhóm booking, mỗi phòng có dịch vụ riêng, bấm "+" để thêm phòng ---
    auto* roomsHeader = new QHBoxLayout;
    roomsHeader->addWidget(new QLabel("Room List", this));
    roomsHeader->addStretch();
    addRoomBtn->setProperty("variant", "ghost");
    addRoomBtn->setFixedWidth(32);
    addRoomBtn->setToolTip("Add room into group");
    roomsHeader->addWidget(addRoomBtn);

    roomsLayout = new QVBoxLayout();
    roomsLayout->setSpacing(4); // giảm để mỗi dòng Room sát nhau hơn, card gọn lại

    auto* roomsBox = new QVBoxLayout;
    roomsBox->addLayout(roomsHeader);
    roomsBox->addLayout(roomsLayout);

    connect(addRoomBtn, &QPushButton::clicked, this, [this] { addRoomRow(); });

    auto* actions = new QHBoxLayout;
    auto* addBtn = new QPushButton("Book", this);
    addBtn->setProperty("variant", "primary");

    auto* inBtn = new QPushButton("Check-in", this);
    inBtn->setProperty("variant", "ghost");

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

    // --- Card 1: form đặt phòng (thông tin cố định + danh sách phòng kèm dịch vụ) + các nút hành động ---
    formCard = new DashboardCard("Booking", "blue", this);
    formCard->addContentLayout(form);
    formCard->addContentLayout(roomsBox);
    formCard->addContentLayout(actions);

    addRoomRow(); // luôn có sẵn 1 dòng Room đầu tiên khi mở form

    auto* searching = new QHBoxLayout;
    auto* searchBtn = new QPushButton("Search", this);
    searchBtn->setProperty("variant", "primary");

    searchEdit->setPlaceholderText("Search bookings");
    searching->addWidget(searchEdit);
    searching->addWidget(searchBtn);

    // --- Booking List: table thay cho 4 cột kanban trước đây ---
    bookingTable = new QTableWidget(0, 7, this);
    bookingTable->setHorizontalHeaderLabels(
        {"Booking ID", "Customer ID", "Receptionist ID", "Group Code", "Room ID", "Services", "Status"});
    bookingTable->horizontalHeader()->setStretchLastSection(true);
    bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookingTable->setSelectionMode(QAbstractItemView::SingleSelection);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingTable->verticalHeader()->setVisible(false);

    auto* tableLayout = new QVBoxLayout;
    tableLayout->addWidget(bookingTable);

    // --- Card 2: ô tìm kiếm + Booking List table ---
    tableCard = new DashboardCard("Booking List", "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContentLayout(tableLayout);

    // --- Layout tổng thể card 1 + card 2 ---
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 16, 24, 16);
    layout->setSpacing(8);
    layout->addWidget(formCard, /*stretch=*/0);   // card Booking giữ nguyên kích thước cần thiết, không giãn thêm
    layout->addWidget(tableCard, /*stretch=*/1);  // Booking List chiếm hết phần còn lại -> to hơn

    //connect(addBtn, &QPushButton::clicked, this, [this] { add(); });
    connect(inBtn, &QPushButton::clicked, this, [this] { checkIn(); });
    connect(outBtn, &QPushButton::clicked, this, [this] { checkOut(); });
    connect(cancelBtn, &QPushButton::clicked, this, [this] { cancel(); });
    connect(reloadBtn, &QPushButton::clicked, this, [this] { reload(); });
    connect(searchBtn, &QPushButton::clicked, this, [this] { search(); });
    connect(bookingTable, &QTableWidget::cellClicked, this, [this](int row, int col) { selected(row, col); });

    reload();
}

void BookingView::addRoomRow(const QString& roomId) {
    // Mỗi dòng gồm: Room ID + Buffet qty + Laundry + Decoration (riêng cho từng phòng)
    // + 1 nút "×" để xoá dòng đó khỏi nhóm booking.
    RoomServiceRow r;

    r.rowWidget = new QWidget(this);
    auto* row = new QHBoxLayout(r.rowWidget);
    row->setContentsMargins(0, 0, 0, 0);
    row->setSpacing(8);

    r.roomIdEdit = new QLineEdit(this);
    r.roomIdEdit->setPlaceholderText("Room ID");
    r.roomIdEdit->setMaximumWidth(110); // thu nhỏ ô Room ID lại, không cần chiếm nhiều chỗ
    if (!roomId.isEmpty()) r.roomIdEdit->setText(roomId);

    auto* buffetLbl = new QLabel("Buffet:", this);

    r.buffetQtyEdit = new QSpinBox(this);
    r.buffetQtyEdit->setRange(0, 50);
    r.buffetQtyEdit->setSuffix(" ticket");

    r.laundryCheck = new QCheckBox("Laundry", this);
    r.decorCheck = new QCheckBox("Decor", this);

    r.decorNotesEdit = new QLineEdit(this);
    r.decorNotesEdit->setPlaceholderText("Decoration notes");
    r.decorNotesEdit->setEnabled(false);
    connect(r.decorCheck, &QCheckBox::toggled, r.decorNotesEdit, &QLineEdit::setEnabled);

    auto* removeBtn = new QPushButton("×", this);
    removeBtn->setProperty("variant", "ghost");
    removeBtn->setFixedWidth(28);
    removeBtn->setToolTip("Bỏ phòng này khỏi nhóm");

    row->addWidget(r.roomIdEdit, /*stretch=*/0);
    row->addWidget(buffetLbl, /*stretch=*/0);
    row->addWidget(r.buffetQtyEdit, /*stretch=*/0);
    row->addWidget(r.laundryCheck);
    row->addWidget(r.decorCheck);
    row->addWidget(r.decorNotesEdit, /*stretch=*/1);
    row->addWidget(removeBtn);

    roomRows.append(r);
    roomsLayout->addWidget(r.rowWidget);

    connect(removeBtn, &QPushButton::clicked, this, [this, r] {
        if (roomRows.size() <= 1) return; // luôn giữ ít nhất 1 phòng trong nhóm
        for (int i = 0; i < roomRows.size(); ++i) {
            if (roomRows[i].rowWidget == r.rowWidget) {
                roomRows.removeAt(i);
                break;
            }
        }
        r.rowWidget->deleteLater();
    });
}

void BookingView::clearExtraRoomRows() {
    // Giữ lại đúng 1 dòng Room, xoá hết các dòng còn lại — dùng khi đổ dữ liệu
    // của 1 booking (1 phòng) lên form sau khi chọn từ table.
    while (roomRows.size() > 1) {
        RoomServiceRow last = roomRows.last();
        roomRows.removeLast();
        last.rowWidget->deleteLater();
    }
}

void BookingView::refresh(const std::vector<Booking>& rows) {
    currentRows = rows;

    bookingTable->setRowCount(0);
    bookingTable->setRowCount(static_cast<int>(rows.size()));

    int r = 0;
    for (const auto& b : rows) {
        QStringList services;
        if (b.getBuffetQuantity() > 0) services << QString("Buffet x%1").arg(b.getBuffetQuantity());
        if (b.isUsingLaundry()) services << "Laundry";
        if (b.isUsingDecoration()) {
            QString decor = "Decoration";
            if (!b.getDecorationNote().isEmpty()) decor += ": " + b.getDecorationNote();
            services << decor;
        }

        auto setCell = [this, r](int c, const QString& v) {
            bookingTable->setItem(r, c, new QTableWidgetItem(v));
        };
        setCell(0, b.getId());
        setCell(1, b.getCustomerId());
        setCell(2, b.getReceptionistId());
        setCell(3, b.getGroupCode());
        setCell(4, b.getRoomId());
        setCell(5, services.isEmpty() ? "-" : services.join(", "));
        setCell(6, Booking::statusToString(b.getStatus()));
        ++r;
    }
}

void BookingView::reload() {
    refresh(controller.getAllBookings());
}

void BookingView::selected(int row, int /*column*/) {
    if (row < 0 || row >= static_cast<int>(currentRows.size())) return;
    const Booking& b = currentRows[row];

    bookingIdEdit->setText(b.getId());
    groupCodeEdit->setText(b.getGroupCode());
    customerIdEdit->setText(b.getCustomerId());
    receptionistIdEdit->setText(b.getReceptionistId());
    checkInEdit->setDate(b.getCheckIn());
    checkOutEdit->setDate(b.getCheckOut());

    clearExtraRoomRows();
    auto& r = roomRows.first();
    r.roomIdEdit->setText(b.getRoomId());
    r.buffetQtyEdit->setValue(b.getBuffetQuantity());
    r.laundryCheck->setChecked(b.isUsingLaundry());
    r.decorCheck->setChecked(b.isUsingDecoration());
    r.decorNotesEdit->setText(b.getDecorationNote());
}

void BookingView::add() {
    MultiBookingRequest request;
    request.customerId = text(customerIdEdit);
    request.receptionistId = text(receptionistIdEdit);
    request.checkIn = checkInEdit->date();
    request.checkOut = checkOutEdit->date();

    request.groupCode = text(groupCodeEdit);

    for (const auto& r : roomRows) {
        const QString roomId = text(r.roomIdEdit);
        if (roomId.isEmpty()) continue;

        SingleRoomRequest room;
        room.roomId = roomId;
        room.buffetQty = r.buffetQtyEdit->value();
        room.laundryService = r.laundryCheck->isChecked();
        room.decorService = r.decorCheck->isChecked();
        room.decorNote = text(r.decorNotesEdit);
        request.rooms.push_back(room);
    }

    if (request.rooms.empty()) {
        error("Please enter at least one Room ID!");
        return;
    }

    QString e;
    // TODO (backend): BookingController hiện CHƯA có hàm createMultiBookings(). Cần bổ sung
    // (forward xuống BookingService -> BookingRepository, chạy trong 1 transaction, tự sinh
    // groupCode nếu request.groupCode rỗng, tự sinh từng bookingId cho mỗi phòng).
    /*if (!controller.createMultiBookings(request, e)) {
        error(e);
        return;
    }*/

    groupCodeEdit->clear();
    reload();
}

void BookingView::checkIn() {
    QString e;
    if (!controller.processCheckIn(text(bookingIdEdit), e)) error(e); else reload();
}
void BookingView::checkOut() {
    QString e;
    if (!controller.processCheckOut(text(bookingIdEdit), e)) error(e); else reload();
}
void BookingView::cancel() {
    QString e;
    if (!controller.processCancelBooking(text(bookingIdEdit), e)) error(e); else reload();
}

void BookingView::search() {
    refresh(controller.searchBookings(text(searchEdit)));
}

void BookingView::error(const QString& message) { QMessageBox::warning(this, "Booking Error", message); }