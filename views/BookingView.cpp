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
#include <QScrollArea>
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QSize>

namespace {
QString text(QLineEdit* edit) { return edit->text().trimmed(); }

// Vẽ icon dấu "+"/"-" bằng QPainter thay vì dùng ký tự text ("+", "×"),
QIcon makePlusIcon(const QColor& color, int size = 16) {
    QPixmap pm(size, size);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    const int thickness = qMax(2, size / 6);
    const int margin = size / 6;
    p.drawRoundedRect(margin, size / 2 - thickness / 2, size - 2 * margin, thickness, thickness / 2, thickness / 2);
    p.drawRoundedRect(size / 2 - thickness / 2, margin, thickness, size - 2 * margin, thickness / 2, thickness / 2);
    return QIcon(pm);
}

QIcon makeMinusIcon(const QColor& color, int size = 16) {
    QPixmap pm(size, size);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    const int thickness = qMax(2, size / 6);
    const int margin = size / 6;
    p.drawRoundedRect(margin, size / 2 - thickness / 2, size - 2 * margin, thickness, thickness / 2, thickness / 2);
    return QIcon(pm);
}
}

BookingView::BookingView(QWidget* parent)
    : QWidget(parent),
    controller(),
    bookingIdEdit(new QLineEdit(this)),
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
    bookingIdEdit->setPlaceholderText("Booking ID");
    bookingIdEdit->setReadOnly(true);
    
    customerIdEdit->setPlaceholderText("Customer ID");
    receptionistIdEdit->setPlaceholderText("Receptionist ID");


    // --- Nhóm thông tin cố định: Booking ID, Group ID, Customer ID, Receptionist ID, Check-in, Check-out ---
    auto* form = new QGridLayout;
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(10);
    form->addWidget(new QLabel("Booking ID", this), 0, 0);
    form->addWidget(bookingIdEdit, 0, 1);
    form->addWidget(new QLabel("Check-in", this), 0, 2);
    form->addWidget(checkInEdit, 0, 3);

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
    addRoomBtn->setObjectName("addRoomBtn"); // hook để style.qss chỉnh hover riêng cho nút thêm phòng
    addRoomBtn->setFixedWidth(32);
    addRoomBtn->setToolTip("Add room into group");
    addRoomBtn->setText(QString());
    // Dùng --primary-prs (#D66A8C) của theme hồng pastel, đồng bộ với màu chữ
    // của variant="secondary"/nav active thay vì màu xám tự chọn không ăn nhập.
    addRoomBtn->setIcon(makePlusIcon(QColor("#D66A8C")));
    addRoomBtn->setIconSize(QSize(14, 14));
    roomsHeader->addWidget(addRoomBtn);

    roomsLayout = new QVBoxLayout();
    roomsLayout->setContentsMargins(2, 2, 2, 2); // tăng padding vì giờ có nền, tránh dòng Room sát viền
    roomsLayout->setSpacing(4);
    roomsLayout->addStretch(1); // giữ các dòng Room dồn lên trên khi ít hơn chiều cao khung cuộn

    // --- Container widget chứa các dòng Room, được đặt trong QScrollArea riêng ---
    // Nhờ vậy Room List có thanh cuộn dọc như table, không làm formCard phình cao khi thêm nhiều phòng.
    roomsContainer = new QWidget(this);
    roomsContainer->setLayout(roomsLayout);
    roomsContainer->setObjectName("roomListContainer");
    roomsContainer->setAttribute(Qt::WA_StyledBackground, true);

    roomsScrollArea = new QScrollArea(this);
    roomsScrollArea->setObjectName("roomsScrollArea"); // de style.qss ap dung nen trong suot
    roomsScrollArea->setWidget(roomsContainer);
    roomsScrollArea->setWidgetResizable(true);
    roomsScrollArea->setFrameShape(QFrame::NoFrame);
    roomsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    roomsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    roomsScrollArea->setMinimumHeight(115);
    roomsScrollArea->setMaximumHeight(170);

    auto* roomsBox = new QVBoxLayout;
    roomsBox->addLayout(roomsHeader);
    roomsBox->addWidget(roomsScrollArea);

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
    formCard = new DashboardCard(QString(), "blue", this);
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

    // --- Booking List ---
    // Thêm 2 cột "Check-in" và "Check-out" để xem trực tiếp ngày nhận/trả phòng
    // của từng booking ngay trên table, không cần bấm chọn dòng rồi xem lại form.
    bookingTable = new QTableWidget(0, 9, this);
    bookingTable->setHorizontalHeaderLabels(
        {"Booking ID", "Customer ID", "Receptionist ID", "Group Code", "Room ID",
         "Check-in", "Check-out", "Services", "Status"});
    bookingTable->horizontalHeader()->setStretchLastSection(true);
    bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookingTable->setSelectionMode(QAbstractItemView::SingleSelection);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingTable->verticalHeader()->setVisible(false);

    auto* tableLayout = new QVBoxLayout;
    tableLayout->addWidget(bookingTable);

    // --- Card 2: ô tìm kiếm + Booking List table ---
    tableCard = new DashboardCard(QString(), "purple", this);
    tableCard->addContentLayout(searching);
    tableCard->addContentLayout(tableLayout);

    // --- Layout tổng thể card 1 + card 2 ---
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(28, 20, 28, 20);
    layout->setSpacing(10); // tăng khoảng cách giữa 2 card, đẩy Booking List xuống dưới một chút
    layout->addWidget(formCard, /*stretch=*/0);   // card Booking giữ nguyên kích thước cần thiết, không giãn thêm
    layout->addWidget(tableCard, /*stretch=*/1);  // Booking List chiếm hết phần còn lại -> to hơn

    connect(addBtn, &QPushButton::clicked, this, [this] { add(); });
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
    RoomServiceRow r;

    r.rowWidget = new QWidget(this);
    auto* row = new QHBoxLayout(r.rowWidget);
    row->setContentsMargins(0, 0, 0, 0);
    row->setSpacing(8);

    r.roomIdEdit = new QLineEdit(this);
    r.roomIdEdit->setPlaceholderText("Room ID");
    r.roomIdEdit->setMaximumWidth(110);
    if (!roomId.isEmpty()) r.roomIdEdit->setText(roomId);

    auto* buffetLbl = new QLabel("Buffet:", this);

    r.buffetQtyEdit = new QSpinBox(this);
    r.buffetQtyEdit->setRange(0, 50);
    r.buffetQtyEdit->setSuffix(" ticket");

    r.laundryCheck = new QCheckBox("Laundry", this);
    r.decorCheck = new QCheckBox("Decor", this);
    // Đổi màu riêng cho từng loại dịch vụ khi tick (xem style.qss), tránh 2 checkbox
    // Laundry/Decor cùng 1 màu nhìn dễ lẫn khi cả 2 đều được chọn trên cùng 1 dòng Room.
    r.laundryCheck->setObjectName("laundryCheck");
    r.decorCheck->setObjectName("decorCheck");

    r.decorNotesEdit = new QLineEdit(this);
    r.decorNotesEdit->setPlaceholderText("Decoration notes");
    r.decorNotesEdit->setEnabled(false);
    connect(r.decorCheck, &QCheckBox::toggled, r.decorNotesEdit, &QLineEdit::setEnabled);

    auto* removeBtn = new QPushButton(this);
    removeBtn->setProperty("variant", "ghost");
    removeBtn->setObjectName("removeRoomBtn");
    removeBtn->setFixedWidth(28);
    removeBtn->setToolTip("Delete room");
    removeBtn->setIcon(makeMinusIcon(QColor("#C96257")));
    removeBtn->setIconSize(QSize(14, 14));

    row->addWidget(r.roomIdEdit, /*stretch=*/0);
    row->addWidget(buffetLbl, /*stretch=*/0);
    row->addWidget(r.buffetQtyEdit, /*stretch=*/0);
    row->addWidget(r.laundryCheck);
    row->addWidget(r.decorCheck);
    row->addWidget(r.decorNotesEdit, /*stretch=*/1);
    row->addWidget(removeBtn);

    roomRows.append(r);
    // Chen truoc phan tu stretch (luon la item cuoi cung) de cac dong Room dong len tren.
    roomsLayout->insertWidget(roomsLayout->count() - 1, r.rowWidget);

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
        setCell(5, b.getCheckIn().toString("yyyy-MM-dd"));
        setCell(6, b.getCheckOut().toString("yyyy-MM-dd"));
        setCell(7, services.isEmpty() ? "-" : services.join(", "));
        setCell(8, Booking::statusToString(b.getStatus()));
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
    std::vector<QString> roomIds;
    std::vector<RoomServiceSelection> services;   // mỗi phòng có 1 service riêng, khớp index với roomIds
    for (const auto& r : roomRows) {
        const QString roomId = text(r.roomIdEdit);
        if (roomId.isEmpty()) continue;
        roomIds.push_back(roomId);

        RoomServiceSelection service;
        service.buffetQty = r.buffetQtyEdit->value();
        service.laundry = r.laundryCheck->isChecked();
        service.decoration = r.decorCheck->isChecked();
        service.decorationNote = text(r.decorNotesEdit);
        services.push_back(service);
    }

    if (roomIds.empty()) {
        error("Please enter at least one Room ID!");
        return;
    }

    QString e;
    if (!controller.createMultiBookings(text(customerIdEdit), roomIds, checkInEdit->date(), checkOutEdit->date(), text(receptionistIdEdit), services, e)) {
        error(e);
        return;
    }

    customerIdEdit->clear();
    receptionistIdEdit->clear();
    clearExtraRoomRows();
    if (!roomRows.isEmpty()) {
        roomRows[0].roomIdEdit->clear();
        roomRows[0].buffetQtyEdit->setValue(0);
        roomRows[0].laundryCheck->setChecked(false);
        roomRows[0].decorCheck->setChecked(false);
        roomRows[0].decorNotesEdit->clear();
    }

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