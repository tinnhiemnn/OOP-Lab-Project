#pragma once
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>

class DashboardCard : public QFrame {
    Q_OBJECT
public:
    explicit DashboardCard(const QString& title,
                           QWidget* parent = nullptr) : QFrame(parent)
    {
        setProperty("cardStyle", true);
        auto* outer = new QVBoxLayout(this);
        outer->setContentsMargins(22, 20, 22, 22);
        outer->setSpacing(14);

        if (!title.isEmpty()) {
            auto* head = new QHBoxLayout();
            head->setSpacing(10);
            auto* titleLbl = new QLabel(title, this);
            titleLbl->setProperty("role", "cardTitle");
            head->addWidget(titleLbl);
            head->addStretch();
            outer->addLayout(head);
        }

        m_body = new QVBoxLayout();
        m_body->setSpacing(10);
        outer->addLayout(m_body);

        auto* shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(24);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(15, 23, 42, 30));
        setGraphicsEffect(shadow);
    }

    void addContent(QWidget* w) { m_body->addWidget(w); }
    void addContentLayout(QLayout* l) { m_body->addLayout(l); }

private:
    QVBoxLayout* m_body;
};