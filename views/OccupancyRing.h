#pragma once

#include <QWidget>
#include <QColor>

// Widget vẽ vòng tròn tỉ lệ lấp đầy phòng (tương ứng .ring / conic-gradient
// trong file mockup HTML). Tự vẽ bằng QPainter vì Qt Widgets không có
// conic-gradient dựng sẵn.
//
// Màu track/active/text khai báo Q_PROPERTY để set được từ QSS qua
// "qproperty-trackColor: ...;" trên selector "OccupancyRing" - vì widget này
// tự paintEvent (không dùng QSS cho border/background), đây là cách duy nhất
// để cấu hình màu từ bên ngoài mà không sửa code C++.
class OccupancyRing : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor trackColor  MEMBER trackColor)
    Q_PROPERTY(QColor activeColor MEMBER activeColor)
    Q_PROPERTY(QColor textColor   MEMBER textColor)

public:
    explicit OccupancyRing(QWidget* parent = nullptr);

    // pct: 0-100
    void setPercent(int pct);
    int percentValue() const { return percent; }

    QSize sizeHint() const override { return QSize(112, 112); }
    QSize minimumSizeHint() const override { return QSize(112, 112); }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    int percent = 0;

    QColor trackColor  { "#EEF1F5" };
    QColor activeColor { "#0D9488" }; // --teal trong mockup
    QColor textColor   { "#0F172A" }; // --text trong mockup
};