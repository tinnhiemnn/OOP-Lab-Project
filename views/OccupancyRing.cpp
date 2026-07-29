#include "OccupancyRing.h"

#include <QPainter>
#include <QPaintEvent>
#include <QFont>

OccupancyRing::OccupancyRing(QWidget* parent) : QWidget(parent) {
    setFixedSize(120, 120);
}

void OccupancyRing::setPercent(int pct) {
    percent = qBound(0, pct, 100);
    update();
}

void OccupancyRing::paintEvent(QPaintEvent* /*event*/) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int penWidth = 14;
    QRectF ring(penWidth / 2.0, penWidth / 2.0,
                width() - penWidth, height() - penWidth);

    p.setPen(QPen(trackColor, penWidth, Qt::SolidLine, Qt::FlatCap));
    p.drawArc(ring, 0, 360 * 16);

    if (percent > 0) {
        p.setPen(QPen(activeColor, penWidth, Qt::SolidLine, Qt::FlatCap));
        const int startAngle = 90 * 16;
        const int spanAngle = static_cast<int>(-(percent / 100.0) * 360 * 16);
        p.drawArc(ring, startAngle, spanAngle);
    }

    p.setPen(textColor);
    QFont font("Space Grotesk", 17, QFont::Bold);
    if (!QFontInfo(font).exactMatch()) {
        font.setFamily("Arial");
        font.setBold(true);
        font.setPointSize(17);
    }
    p.setFont(font);
    p.drawText(rect(), Qt::AlignCenter, QString("%1%").arg(percent));
}