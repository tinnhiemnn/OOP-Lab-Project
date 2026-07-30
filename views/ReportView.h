#pragma once

#include "controllers/ReportController.h"
#include "DashboardCard.h"
#include "OccupancyRing.h"

#include <QWidget>
#include <QColor>

class QLabel;
class QComboBox;
class QVBoxLayout;
class QHBoxLayout;
class QChartView;
class QChart;
class QScrollArea;
class QAbstractAxis;
class QPushButton;

// ReportView hiển thị tab "Reports" giống file mockup HTML:
//  - Hero row: doanh thu hiện tại + vòng tròn tỉ lệ lấp đầy phòng
//  - Card "Doanh thu theo tháng": line/area chart theo năm được chọn
//  - Card "Thống kê theo loại phòng": donut (tỉ lệ doanh thu) + bar ngang
//    (tỉ lệ hoàn thành / hủy)
//  - Hàng dưới cùng (2 cột): bar chart doanh thu theo nhân viên + rank-list
//    top 5 khách hàng
//
// TOÀN BỘ MÀU dùng trong các QChart được khai báo dưới dạng Q_PROPERTY(QColor).
// QChart không đọc được QSS trực tiếp (vẽ bằng QGraphicsItem, không phải
// QWidget), nên đây là cách chuẩn để vẫn cấu hình màu từ file .qss:
// set qua "qproperty-<tên>: <mã màu>;" trên selector "ReportView" trong QSS,
// Qt sẽ tự gán vào các property này lúc load stylesheet. Code C++ chỉ đọc
// biến member, không có màu nào hardcode trong .cpp.
class ReportView : public QWidget {
    Q_OBJECT

    Q_PROPERTY(QColor chartBackground   MEMBER chartBackground)
    Q_PROPERTY(QColor axisTextColor     MEMBER axisTextColor)
    Q_PROPERTY(QColor axisLineColor     MEMBER axisLineColor)
    Q_PROPERTY(QColor gridLineColor     MEMBER gridLineColor)

    Q_PROPERTY(QColor revenueLineColor  MEMBER revenueLineColor)
    Q_PROPERTY(QColor roomStandardColor MEMBER roomStandardColor)
    Q_PROPERTY(QColor roomDeluxeColor   MEMBER roomDeluxeColor)
    Q_PROPERTY(QColor roomPresidentColor MEMBER roomPresidentColor)
    Q_PROPERTY(QColor completedColor    MEMBER completedColor)
    Q_PROPERTY(QColor cancelledColor    MEMBER cancelledColor)
    Q_PROPERTY(QColor receptionistBarColor MEMBER receptionistBarColor)

public:
    explicit ReportView(QWidget* parent = nullptr);

private slots:
    void reload();
    void onYearChanged(const QString& year);

private:
    // --- Dựng UI ---
    void buildTopBar(QVBoxLayout* root);
    void buildHeroRow(QVBoxLayout* root);
    void buildRevenueCard(QVBoxLayout* root);
    void buildRoomTypeCard(QVBoxLayout* root);
    void buildBottomRow(QVBoxLayout* root);

    // --- Cập nhật dữ liệu cho từng khối ---
    void updateHero();
    void updateRevenueChart(const QString& year);
    void updateRoomTypeChart();
    void updateReceptionistChart();
    void updateTopCustomers();

    // Chú thích (chấm màu + nhãn) cho donut và bar ngang trong card "Thống kê
    // theo loại phòng" - tự vẽ bằng QLabel/QFrame thay vì dùng QChart legend
    // mặc định (khó style theo QSS), rebuild mỗi lần updateRoomTypeChart().
    void addLegendItem(QHBoxLayout* row, const QColor& color, const QString& text) const;
    static void clearLayout(QLayout* layout);

    // Trích số nguyên % từ chuỗi kiểu "67%" trả về từ ReportController
    static int extractPercent(const QString& text);

    // Tính % tăng trưởng tháng gần nhất so với tháng liền trước, dựa trên
    // dữ liệu getRevenueByYear() đã có sẵn - không cần thêm API backend mới.
    // Trả về true và gán growthPct nếu tìm được ít nhất 2 tháng có dữ liệu.
    static bool computeLastMonthGrowth(const std::vector<double>& monthly, double& growthPct);

    // Áp style chung (nền, margin, legend) + màu trục cho mọi QChart,
    // dùng chartBackground/axisTextColor/axisLineColor/gridLineColor.
    void styleChart(QChart* chart) const;
    void styleAxis(QAbstractAxis* axis) const;
    QChartView* makeChartView(int minHeight, int maxHeight = QWIDGETSIZE_MAX) const;

    ReportController controller;

    QScrollArea* scrollArea = nullptr;
    QWidget* scrollContent = nullptr;

    // Top bar
    QPushButton* refreshButton = nullptr;

    // Hero
    QLabel* revenueValueLabel = nullptr;
    QLabel* revenueTrendLabel = nullptr;
    OccupancyRing* occupancyRing = nullptr;
    QLabel* occupancyLegend = nullptr;

    // Doanh thu theo tháng
    QComboBox* yearSelect = nullptr;
    QChartView* revenueChartView = nullptr;

    // Thống kê theo loại phòng
    QChartView* roomPieChartView = nullptr;
    QChartView* cancelBarChartView = nullptr;
    QHBoxLayout* roomPieLegendLayout = nullptr;
    QHBoxLayout* cancelBarLegendLayout = nullptr;

    // Hàng dưới
    QChartView* receptionistChartView = nullptr;
    QVBoxLayout* topCustomersLayout = nullptr;

    DashboardCard* revenueCard = nullptr;
    DashboardCard* roomTypeCard = nullptr;
    DashboardCard* receptionistCard = nullptr;
    DashboardCard* topCustomerCard = nullptr;

    // --- Giá trị màu mặc định (dùng khi project chưa load file .qss có
    // qproperty- tương ứng; production nên luôn override qua QSS) ---
    QColor chartBackground   { 255, 255, 255, 0 };
    QColor axisTextColor     { "#334155" };
    QColor axisLineColor     { "#CBD5E1" };
    QColor gridLineColor     { 15, 23, 42, 25 };

    QColor revenueLineColor    { "#2563EB" };
    QColor roomStandardColor  { "#2563EB" };
    QColor roomDeluxeColor    { "#7C3AED" };
    QColor roomPresidentColor { "#D97706" };
    QColor completedColor     { "#0D9488" };
    QColor cancelledColor     { "#E11D48" };
    QColor receptionistBarColor { "#EA580C" };
};