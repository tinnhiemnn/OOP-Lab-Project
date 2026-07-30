#include "ReportView.h"
#include "DashboardCard.h"
#include "OccupancyRing.h"

#include <QComboBox>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QScrollArea>
#include <QStyle>

#include <QChart>
#include <QChartView>
#include <QSplineSeries>
#include <QAreaSeries>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSet>
#include <QBarSeries>
#include <QHorizontalStackedBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QAbstractAxis>

#include <algorithm>

#ifdef QT_CHARTS_NAMESPACE
using namespace QT_CHARTS_NAMESPACE;
#endif

namespace {
const QStringList kMonthLabels = {
    "T1","T2","T3","T4","T5","T6","T7","T8","T9","T10","T11","T12"
};
constexpr int kMonthCount = 12;
}

ReportView::ReportView(QWidget* parent)
    : QWidget(parent), controller() {

    setAttribute(Qt::WA_StyledBackground, true);

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("reportScrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Viewport nội bộ của QScrollArea cũng là QWidget trơn, cần bật thuộc
    // tính này thì rule "QScrollArea#reportScrollArea > QWidget" trong QSS
    // (làm viewport trong suốt) mới có tác dụng.
    scrollArea->viewport()->setAttribute(Qt::WA_StyledBackground, true);

    scrollContent = new QWidget(scrollArea);
    scrollContent->setObjectName("reportScrollContent");
    scrollContent->setAttribute(Qt::WA_StyledBackground, true);

    auto* layout = new QVBoxLayout(scrollContent);
    layout->setContentsMargins(28, 20, 28, 24);
    layout->setSpacing(16);

    buildTopBar(layout);
    buildHeroRow(layout);
    buildRevenueCard(layout);
    buildRoomTypeCard(layout);
    buildBottomRow(layout);
    layout->addStretch(); // giữ card không giãn khi nội dung ngắn hơn viewport

    scrollArea->setWidget(scrollContent);
    outer->addWidget(scrollArea);

    reload();
}

// ----------------------------------------------------------------------------
// TOP BAR: nút Làm mới (refresh) - gọi lại reload() để tải lại toàn bộ dữ
// liệu report (doanh thu, tỉ lệ lấp đầy, thống kê phòng, nhân viên, top
// khách hàng...), giống pattern nút Refresh ở CustomerView.
// ----------------------------------------------------------------------------
void ReportView::buildTopBar(QVBoxLayout* root) {
    auto* topBar = new QHBoxLayout;
    topBar->addStretch();

    refreshButton = new QPushButton(QString::fromUtf8("Refresh"), this);
    refreshButton->setProperty("variant", "ghost");
    connect(refreshButton, &QPushButton::clicked, this, &ReportView::reload);

    topBar->addWidget(refreshButton);
    root->addLayout(topBar);
}

// ----------------------------------------------------------------------------
// HERO ROW: doanh thu hiện tại + vòng tròn tỉ lệ lấp đầy phòng
// ----------------------------------------------------------------------------
void ReportView::buildHeroRow(QVBoxLayout* root) {
    auto* heroRow = new QHBoxLayout;
    heroRow->setSpacing(20);

    auto* revBox = new QFrame(this);
    revBox->setProperty("heroCard", true);
    revBox->setProperty("variant", "revenue");

    auto* revLayout = new QVBoxLayout(revBox);
    revLayout->setContentsMargins(24, 22, 24, 22);
    revLayout->setSpacing(8);

    auto* revTitle = new QLabel(QString::fromUtf8("Total revenue"), this);
    revTitle->setProperty("role", "heroLabel");

    auto* revLabelRow = new QHBoxLayout;
    revLabelRow->setSpacing(8);
    revLabelRow->addWidget(revTitle);
    revLabelRow->addStretch();

    revenueValueLabel = new QLabel(this);
    revenueValueLabel->setProperty("role", "heroValue");
    revenueTrendLabel = new QLabel(this);
    revenueTrendLabel->setProperty("role", "heroTrend");
    revenueTrendLabel->setVisible(false); // chỉ hiện khi tính được growth

    revLayout->addLayout(revLabelRow);
    revLayout->addWidget(revenueValueLabel);
    revLayout->addWidget(revenueTrendLabel);
    revLayout->addStretch();

    auto* occBox = new QFrame(this);
    occBox->setProperty("heroCard", true);
    occBox->setProperty("variant", "occupancy");

    auto* occOuter = new QVBoxLayout(occBox);
    occOuter->setContentsMargins(24, 22, 24, 22);
    occOuter->setSpacing(10);

    auto* occTitle = new QLabel(QString::fromUtf8("Occupancy rate"), this);
    occTitle->setProperty("role", "heroLabel");

    auto* occLabelRow = new QHBoxLayout;
    occLabelRow->setSpacing(8);
    occLabelRow->addWidget(occTitle);
    occLabelRow->addStretch();

    auto* occRow = new QHBoxLayout;
    occRow->setSpacing(18);
    occupancyRing = new OccupancyRing(this);
    occupancyLegend = new QLabel(this);
    occupancyLegend->setProperty("role", "occLegend");
    occupancyLegend->setWordWrap(true);
    // Rich text để bold số % (giống số "3" bold trong mockup), tránh chữ bị
    // tràn dòng kỳ lạ khi occBox đã đủ rộng sau khi sửa tỉ lệ heroRow ở trên.
    occupancyLegend->setTextFormat(Qt::RichText);
    occupancyLegend->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    occRow->addWidget(occupancyRing);
    occRow->addWidget(occupancyLegend, 1);
    occRow->setAlignment(occupancyRing, Qt::AlignVCenter);

    occOuter->addLayout(occLabelRow);
    occOuter->addLayout(occRow);

    // Dùng (13, 10) ~ đúng tỉ lệ 1.3:1 như mockup.
    heroRow->addWidget(revBox, 13);
    heroRow->addWidget(occBox, 10);

    root->addLayout(heroRow);
}

// ----------------------------------------------------------------------------
// CARD: Doanh thu theo tháng (area chart + chọn năm)
// ----------------------------------------------------------------------------
void ReportView::buildRevenueCard(QVBoxLayout* root) {
    auto* headerRow = new QHBoxLayout;
    auto* title = new QLabel(QString::fromUtf8("Monthly revenue report"), this);
    title->setProperty("role", "cardTitle");

    yearSelect = new QComboBox(this);
    yearSelect->addItems({"2026", "2025"});
    connect(yearSelect, &QComboBox::currentTextChanged,
            this, &ReportView::onYearChanged);

    headerRow->addWidget(title);
    headerRow->addStretch();
    headerRow->addWidget(yearSelect);

    revenueChartView = makeChartView(240);

    auto* contentLayout = new QVBoxLayout;
    contentLayout->addLayout(headerRow);
    contentLayout->addWidget(revenueChartView);

    revenueCard = new DashboardCard(QString(), "blue", this);
    revenueCard->addContentLayout(contentLayout);
    root->addWidget(revenueCard);
}

// ----------------------------------------------------------------------------
// CARD: Thống kê theo loại phòng (donut + bar ngang hoàn thành/hủy)
// ----------------------------------------------------------------------------
void ReportView::buildRoomTypeCard(QVBoxLayout* root) {
    auto* title = new QLabel(QString::fromUtf8("Statistics by room type"), this);
    title->setProperty("role", "cardTitle");

    // Trước đây chỉ setMinimumHeight(190) nên 2 chart giãn hết chiều cao còn
    // lại của card, khiến donut phóng to gần kín cột trái. Giới hạn thêm
    // maxHeight để giữ kích thước gọn gàng hơn.
    roomPieChartView = makeChartView(150, 210);
    cancelBarChartView = makeChartView(150, 210);
    // Donut vẽ theo cạnh nhỏ hơn (min width/height) của view; giới hạn thêm
    // bề rộng tối đa để vòng tròn không bị to lệch hẳn sang cột trái so với
    // bar chart bên cạnh.
    roomPieChartView->setMaximumWidth(280);

    // Mỗi chart có 1 hàng chú thích (chấm màu + nhãn) riêng phía trên, nội
    // dung được build lại trong updateRoomTypeChart() vì phụ thuộc dữ liệu
    // (tên loại phòng, %) và màu Q_PROPERTY (có thể đổi theo QSS/theme).
    roomPieLegendLayout = new QHBoxLayout;
    roomPieLegendLayout->setSpacing(18);

    cancelBarLegendLayout = new QHBoxLayout;
    cancelBarLegendLayout->setSpacing(18);

    auto* pieCol = new QVBoxLayout;
    pieCol->setSpacing(10);
    pieCol->addLayout(roomPieLegendLayout);
    pieCol->addWidget(roomPieChartView);
    pieCol->setAlignment(roomPieChartView, Qt::AlignHCenter);

    auto* barCol = new QVBoxLayout;
    barCol->setSpacing(10);
    barCol->addLayout(cancelBarLegendLayout);
    barCol->addWidget(cancelBarChartView);

    auto* splitRow = new QHBoxLayout;
    splitRow->setSpacing(26);
    splitRow->addLayout(pieCol, 1);
    splitRow->addLayout(barCol, 1);

    auto* contentLayout = new QVBoxLayout;
    contentLayout->addWidget(title);
    contentLayout->addLayout(splitRow);

    roomTypeCard = new DashboardCard(QString(), "purple", this);
    roomTypeCard->addContentLayout(contentLayout);
    root->addWidget(roomTypeCard);
}

// ----------------------------------------------------------------------------
// HÀNG DƯỚI: Doanh thu theo nhân viên + Top 5 khách hàng
// ----------------------------------------------------------------------------
void ReportView::buildBottomRow(QVBoxLayout* root) {
    auto* bottomRow = new QHBoxLayout;
    bottomRow->setSpacing(20);

    auto* recTitle = new QLabel(QString::fromUtf8("Revenue Per Employee"), this);
    recTitle->setProperty("role", "cardTitle");
    auto* recSub = new QLabel(QString::fromUtf8("KPI evaluation"), this);
    recSub->setProperty("role", "cardSubtle");

    auto* recHeader = new QHBoxLayout;
    recHeader->addWidget(recTitle);
    recHeader->addStretch();
    recHeader->addWidget(recSub);

    receptionistChartView = makeChartView(225);
    

    auto* recLayout = new QVBoxLayout;
    recLayout->addLayout(recHeader);
    recLayout->addWidget(receptionistChartView);

    receptionistCard = new DashboardCard(QString(), "orange", this);
    receptionistCard->addContentLayout(recLayout);

    auto* topTitle = new QLabel(QString::fromUtf8("Top 5 customers"), this);
    topTitle->setProperty("role", "cardTitle");

    topCustomersLayout = new QVBoxLayout;
    topCustomersLayout->setSpacing(10);

    auto* topOuter = new QVBoxLayout;
    topOuter->addWidget(topTitle);
    topOuter->addLayout(topCustomersLayout);
    topOuter->addStretch();

    topCustomerCard = new DashboardCard(QString(), "green", this);
    topCustomerCard->addContentLayout(topOuter);

    // Tỉ lệ 2fr : 1fr giống .two-col trong mockup
    bottomRow->addWidget(receptionistCard, 2);
    bottomRow->addWidget(topCustomerCard, 1);

    root->addLayout(bottomRow);
}

// ----------------------------------------------------------------------------
// HELPER: tạo QChartView đã set style nền sáng dùng chung (gọi 1 dòng thay vì
// lặp 4 dòng setRenderHint/setMinimumHeight/setStyleSheet/setBackgroundBrush
// ở mỗi card).
// ----------------------------------------------------------------------------
QChartView* ReportView::makeChartView(int minHeight, int maxHeight) const {
    auto* view = new QChartView();
    view->setRenderHint(QPainter::Antialiasing);
    view->setMinimumHeight(minHeight);
    view->setMaximumHeight(maxHeight);
    view->setStyleSheet("background: transparent; border: none;");
    view->setBackgroundBrush(QBrush(chartBackground));
    return view;
}

// ----------------------------------------------------------------------------
// STYLE CHUNG cho QChart / trục - đọc toàn bộ màu từ property (set qua QSS),
// không hardcode. Không phụ thuộc theme app đang sáng hay tối.
// ----------------------------------------------------------------------------
void ReportView::styleChart(QChart* chart) const {
    chart->setBackgroundBrush(QBrush(chartBackground));
    chart->setBackgroundPen(QPen(Qt::NoPen));
    chart->setBackgroundRoundness(12);
    chart->setPlotAreaBackgroundVisible(false);
    chart->legend()->hide();
    chart->setMargins(QMargins(6, 6, 6, 6));
}

void ReportView::styleAxis(QAbstractAxis* axis) const {
    axis->setLabelsColor(axisTextColor);
    axis->setLinePenColor(axisLineColor);
    if (auto* valueAxis = qobject_cast<QValueAxis*>(axis))
        valueAxis->setGridLineColor(gridLineColor);
    if (auto* catAxis = qobject_cast<QBarCategoryAxis*>(axis))
        catAxis->setGridLineColor(gridLineColor);
}

// ----------------------------------------------------------------------------
// LEGEND: chấm màu tròn (QFrame bo góc) + nhãn, dùng cho donut/bar loại
// phòng thay vì legend mặc định của QChart để đồng bộ font/màu với QSS.
// ----------------------------------------------------------------------------
void ReportView::addLegendItem(QHBoxLayout* row, const QColor& color, const QString& text) const {
    auto* dot = new QFrame(const_cast<ReportView*>(this));
    dot->setFixedSize(10, 10);
    dot->setStyleSheet(
        QString("background-color:%1; border-radius:5px; border:none;")
            .arg(color.name()));

    auto* label = new QLabel(text, const_cast<ReportView*>(this));
    label->setProperty("role", "legendLabel");

    auto* item = new QHBoxLayout;
    item->setSpacing(6);
    item->addWidget(dot);
    item->addWidget(label);

    row->addLayout(item);
}

// Xoá đệ quy toàn bộ widget/sub-layout con của 1 layout, để rebuild legend
// mỗi lần updateRoomTypeChart() chạy lại (giống cách updateTopCustomers()
// xoá row cũ trước khi render lại).
void ReportView::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QLayout* childLayout = item->layout()) {
            clearLayout(childLayout);
        }
        if (QWidget* w = item->widget()) {
            delete w;
        }
        delete item;
    }
}

// ----------------------------------------------------------------------------
// RELOAD: nạp lại toàn bộ dữ liệu
// ----------------------------------------------------------------------------
void ReportView::reload() {
    updateHero();
    updateRevenueChart(yearSelect->currentText());
    updateRoomTypeChart();
    updateReceptionistChart();
    updateTopCustomers();
}

void ReportView::onYearChanged(const QString& year) {
    updateRevenueChart(year);
    updateHero(); // growth pill phụ thuộc năm đang chọn, cần tính lại
}

int ReportView::extractPercent(const QString& text) {
    static const QRegularExpression re("(\\d+)");
    auto match = re.match(text);
    return match.hasMatch() ? match.captured(1).toInt() : 0;
}

void ReportView::updateHero() {
    OverallReport report = controller.getOverallReport();

    revenueValueLabel->setText(report.totalRevenue);

    int pct = extractPercent(report.roomOccupancyRate);
    occupancyRing->setPercent(pct);
    occupancyLegend->setText(
        QString::fromUtf8("<b>%1%</b> occupied<br>%2% available")
            .arg(pct).arg(100 - pct));

    double growthPct = 0.0;
    std::vector<double> monthly = controller.getRevenueByYear(yearSelect->currentText());
    if (computeLastMonthGrowth(monthly, growthPct)) {
        const QString arrow = growthPct >= 0 ? QString::fromUtf8("↗") : QString::fromUtf8("↘");
        const QString sign = growthPct >= 0 ? "+" : "";
        revenueTrendLabel->setText(
            QString::fromUtf8("%1 %2%3% so với tháng trước")
                .arg(arrow, sign)
                .arg(growthPct, 0, 'f', 1));
        revenueTrendLabel->setProperty("trendDown", growthPct < 0);
        revenueTrendLabel->style()->unpolish(revenueTrendLabel);
        revenueTrendLabel->style()->polish(revenueTrendLabel);
        revenueTrendLabel->setVisible(true);
    } else {
        revenueTrendLabel->setVisible(false);
    }
}

bool ReportView::computeLastMonthGrowth(const std::vector<double>& monthly, double& growthPct) {
    // Tìm tháng gần nhất có dữ liệu > 0 và tháng liền trước đó để so sánh.
    int lastIdx = -1;
    for (int i = static_cast<int>(monthly.size()) - 1; i >= 0; --i) {
        if (monthly[static_cast<size_t>(i)] > 0.0) { lastIdx = i; break; }
    }
    if (lastIdx <= 0) return false; // cần ít nhất 2 tháng liên tiếp có dữ liệu

    double curr = monthly[static_cast<size_t>(lastIdx)];
    double prev = monthly[static_cast<size_t>(lastIdx - 1)];
    if (prev <= 0.0) return false;

    growthPct = (curr - prev) * 100.0 / prev;
    return true;
}

void ReportView::updateRevenueChart(const QString& year) {
    std::vector<double> data = controller.getRevenueByYear(year);

    // LƯU Ý: QAreaSeries KHÔNG vẽ cong dù upperSeries truyền vào là
    // QSplineSeries - nó luôn nối các điểm bằng đường thẳng cho phần biên
    // của area (giới hạn đã biết của Qt Charts, xem QTBUG liên quan). Vì
    // vậy tách làm 2 series riêng:
    //   - boundaryLine: QSplineSeries dùng làm biên cho phần tô gradient,
    //     pen để NoPen (không vẽ ra, chỉ dùng để tính hình fill).
    //   - smoothLine: QSplineSeries thứ hai, thêm thẳng vào chart (không
    //     bọc trong QAreaSeries), vẽ ĐÈ LÊN TRÊN phần fill để hiển thị
    //     đường cong mượt thật sự.
    auto* boundaryLine = new QSplineSeries();
    auto* smoothLine = new QSplineSeries();
    double maxVal = 0;
    for (int i = 0; i < static_cast<int>(data.size()) && i < kMonthCount; ++i) {
        boundaryLine->append(i, data[static_cast<size_t>(i)]);
        smoothLine->append(i, data[static_cast<size_t>(i)]);
        maxVal = qMax(maxVal, data[static_cast<size_t>(i)]);
    }

    // Vùng fill gradient dưới đường line, giống hiệu ứng trong mockup.
    // Alpha giảm dần 45 -> 18 -> 0 (thay vì 60 -> 0 cứng) để fill mờ nhạt
    // hơn và mượt hơn, đồng thời che bớt phần spline hơi lệch khỏi biên
    // area thẳng ở các đoạn dốc gắt. Chỉnh 3 số alpha bên dưới nếu muốn
    // đậm/nhạt khác.
    auto* areaSeries = new QAreaSeries(boundaryLine);
    QLinearGradient gradient(0, 0, 0, 1);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    QColor fillTop = revenueLineColor; fillTop.setAlpha(45);
    QColor fillMid = revenueLineColor; fillMid.setAlpha(18);
    QColor fillBottom = revenueLineColor; fillBottom.setAlpha(0);
    gradient.setColorAt(0.0, fillTop);
    gradient.setColorAt(0.55, fillMid);
    gradient.setColorAt(1.0, fillBottom);
    areaSeries->setBrush(gradient);
    areaSeries->setPen(Qt::NoPen); // biên area không vẽ, để smoothLine đảm nhiệm

    // borderCapStyle/borderJoinStyle 'round' bên Chart.js -> QPen tương đương.
    // Đây mới là đường thực sự người dùng nhìn thấy.
    QPen linePen(revenueLineColor, 2.5);
    linePen.setCapStyle(Qt::RoundCap);
    linePen.setJoinStyle(Qt::RoundJoin);
    smoothLine->setPen(linePen);
    smoothLine->setPointsVisible(false);

    auto* chart = new QChart();
    chart->addSeries(areaSeries); // fill vẽ trước (nằm dưới)
    chart->addSeries(smoothLine); // đường cong vẽ sau (nằm trên fill)
    styleChart(chart);

    auto* axisX = new QBarCategoryAxis;
    axisX->append(kMonthLabels);
    axisX->setGridLineVisible(false);

    auto* axisY = new QValueAxis;
    axisY->setRange(0, maxVal > 0 ? maxVal * 1.15 : 10);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    areaSeries->attachAxis(axisX);
    areaSeries->attachAxis(axisY);
    smoothLine->attachAxis(axisX);
    smoothLine->attachAxis(axisY);
    styleAxis(axisX);
    styleAxis(axisY);

    revenueChartView->setChart(chart);
}

void ReportView::updateRoomTypeChart() {
    std::vector<RoomTypeReport> rooms = controller.getRoomTypeReport();
    const QColor roomColors[] = { roomStandardColor, roomDeluxeColor, roomPresidentColor };

    // --- Donut: tỉ lệ doanh thu theo loại phòng ---
    clearLayout(roomPieLegendLayout);
    auto* pieSeries = new QPieSeries();
    pieSeries->setHoleSize(0.52); // giảm từ 0.62 -> vòng donut dày hơn
    for (int i = 0; i < static_cast<int>(rooms.size()); ++i) {
        const auto& r = rooms[static_cast<size_t>(i)];
        auto* slice = pieSeries->append(r.roomType, r.revenuePercentage);
        slice->setColor(roomColors[i % 3]);
        slice->setLabelVisible(false);
        slice->setBorderColor(chartBackground);
        slice->setBorderWidth(3);

        // Chú thích tương ứng slice này, ví dụ "Standard 35%" - dùng đúng
        // màu roomColors[i % 3] để khớp màu slice trên donut.
        const QString legendText =
            QString("%1 %2%").arg(r.roomType).arg(qRound(r.revenuePercentage));
        addLegendItem(roomPieLegendLayout, roomColors[i % 3], legendText);
    }
    roomPieLegendLayout->addStretch();

    auto* pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    styleChart(pieChart);
    pieChart->setMargins(QMargins(0, 0, 0, 0));
    roomPieChartView->setChart(pieChart);

    // --- Bar ngang: hoàn thành / hủy theo loại phòng ---
    clearLayout(cancelBarLegendLayout);
    addLegendItem(cancelBarLegendLayout, completedColor, QString::fromUtf8("Completed"));
    addLegendItem(cancelBarLegendLayout, cancelledColor, QString::fromUtf8("Cancelled"));
    cancelBarLegendLayout->addStretch();

    auto* completedSet = new QBarSet(QString::fromUtf8("Completed"));
    auto* cancelSet = new QBarSet(QString::fromUtf8("Cancelled"));
    completedSet->setColor(completedColor);
    cancelSet->setColor(cancelledColor);

    QStringList typeNames;
    for (const auto& r : rooms) {
        *completedSet << r.successRate;
        *cancelSet << r.cancellationRate;
        typeNames << r.roomType;
    }

    auto* barSeries = new QHorizontalStackedBarSeries();
    barSeries->append(completedSet);
    barSeries->append(cancelSet);

    auto* barChart = new QChart();
    barChart->addSeries(barSeries);
    styleChart(barChart);

    auto* catAxis = new QBarCategoryAxis;
    catAxis->append(typeNames);
    auto* valAxis = new QValueAxis;
    valAxis->setRange(0, 100);
    valAxis->setLabelFormat("%d%%");

    barChart->addAxis(catAxis, Qt::AlignLeft);
    barChart->addAxis(valAxis, Qt::AlignBottom);
    barSeries->attachAxis(catAxis);
    barSeries->attachAxis(valAxis);
    styleAxis(catAxis);
    styleAxis(valAxis);

    cancelBarChartView->setChart(barChart);
}

void ReportView::updateReceptionistChart() {
    std::vector<ReceptionistKPI> kpis = controller.getReceptionistReport();

    // Sắp xếp giảm dần theo doanh thu để cột cao nhất luôn nằm bên trái,
    // khớp với độ đậm nhạt gán theo thứ hạng ngay bên dưới.
    std::sort(kpis.begin(), kpis.end(), [](const ReceptionistKPI& a, const ReceptionistKPI& b) {
        return a.totalRevenue > b.totalRevenue;
    });

    const int n = static_cast<int>(kpis.size());
    const double maxVal = n > 0 ? kpis.front().totalRevenue : 0.0;

    auto* series = new QBarSeries();
    // QBarSeries chia đều barWidth cho số QBarSet cùng 1 category; ở đây mỗi
    // cột là 1 set riêng (xem bên dưới) nên set 1.0 (tối đa cho phép) để cột
    // hiển thị chiếm hết phần dành cho nó, không bị mảnh hơn cần thiết.
    series->setBarWidth(1.0);

    QStringList names;
    for (int i = 0; i < n; ++i) {
        const auto& k = kpis[static_cast<size_t>(i)];
        names << k.name;

        // Qt Charts chỉ cho 1 brush/màu chung cho toàn bộ 1 QBarSet, nên để
        // mỗi cột có màu đậm nhạt khác nhau, mỗi nhân viên phải là 1 QBarSet
        // riêng - toàn giá trị 0, trừ đúng vị trí category (index i) của
        // chính họ. Các set khác đều 0 tại vị trí này nên không vẽ đè lên.
        auto* set = new QBarSet(k.name);
        for (int j = 0; j < n; ++j)
            *set << (j == i ? k.totalRevenue : 0.0);

        // Độ đậm nhạt theo tỉ lệ so với doanh thu cao nhất: doanh thu càng
        // thấp thì cột càng nhạt màu. Cột đứng đầu (ratio = 1) giữ nguyên
        // receptionistBarColor gốc, cột thấp nhất nhạt nhất (lighter ~190).
        const double ratio = maxVal > 0 ? (k.totalRevenue / maxVal) : 1.0;
        const int lightenAmount = 100 + static_cast<int>((1.0 - ratio) * 90);
        const QColor base = receptionistBarColor.lighter(lightenAmount);

        // Vẫn giữ gradient dọc (đậm ở đỉnh -> nhạt hơn ở đáy) như trước để
        // cột trông "mềm" hơn, chỉ đổi gốc màu (base) theo thứ hạng ở trên.
        QLinearGradient barGradient(0, 0, 0, 1);
        barGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        barGradient.setColorAt(0.0, base);
        barGradient.setColorAt(1.0, base.lighter(135));
        set->setBrush(barGradient);
        set->setPen(QPen(base.darker(115), 1));

        series->append(set);
    }

    auto* chart = new QChart();
    chart->addSeries(series);
    styleChart(chart);

    auto* axisX = new QBarCategoryAxis;
    axisX->append(names);
    auto* axisY = new QValueAxis;

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    styleAxis(axisX);
    styleAxis(axisY);

    receptionistChartView->setChart(chart);
}

void ReportView::updateTopCustomers() {
    // Xoá các row cũ trước khi render lại
    QLayoutItem* item;
    while ((item = topCustomersLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    std::vector<TopCustomer> customers = controller.getTop5CustomersReport();

    for (int i = 0; i < static_cast<int>(customers.size()); ++i) {
        const auto& c = customers[static_cast<size_t>(i)];

        auto* row = new QFrame(this);
        row->setProperty("rankItem", true);
        if (i == 0) row->setProperty("rankTop1", true);

        auto* h = new QHBoxLayout(row);
        h->setContentsMargins(14, 10, 14, 10);
        h->setSpacing(12);

        auto* num = new QLabel(QString::number(i + 1), this);
        num->setProperty("role", "rankNum");
        num->setAlignment(Qt::AlignCenter);
        num->setFixedSize(26, 26);

        auto* nameCol = new QVBoxLayout;
        nameCol->setSpacing(2);
        auto* nameLabel = new QLabel(c.name, this);
        nameLabel->setProperty("role", "rankName");
        auto* idLabel = new QLabel(c.id, this);
        idLabel->setProperty("role", "rankId");
        nameCol->addWidget(nameLabel);
        nameCol->addWidget(idLabel);

        double totalMillion = c.totalSpending / 1000000.0;
        auto* amount = new QLabel(QString::number(totalMillion, 'f', 1) + "tr ₫", this);
        amount->setProperty("role", "rankAmt");

        h->addWidget(num);
        h->addLayout(nameCol, 1);
        h->addWidget(amount);

        topCustomersLayout->addWidget(row);
    }
}