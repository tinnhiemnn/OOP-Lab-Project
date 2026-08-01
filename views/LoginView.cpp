#include "LoginView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

LoginView::LoginView(QWidget *parent) : QWidget(parent) {
    // 1. Layout chính căn giữa màn hình
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    // 2. Khung Card đăng nhập
    auto* card = new QWidget(this);
    card->setObjectName("loginCard");
    card->setFixedSize(380, 420);
    
    // Đổ bóng cho Card
    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 40));
    card->setGraphicsEffect(shadow);
    
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(30, 40, 30, 40);
    cardLayout->setSpacing(15);
    
    // Header Title
    auto* lblTitle = new QLabel("Pink Home", this);
    lblTitle->setObjectName("loginTitle");
    lblTitle->setAlignment(Qt::AlignCenter);
    
    auto* lblSub = new QLabel("Hotel Management Login", this);
    lblSub->setObjectName("loginSub");
    lblSub->setAlignment(Qt::AlignCenter);
    
    // Inputs
    m_txtUser = new QLineEdit(this);
    m_txtUser->setPlaceholderText("Username / Staff ID");
    
    m_txtPass = new QLineEdit(this);
    m_txtPass->setPlaceholderText("Password");
    m_txtPass->setEchoMode(QLineEdit::Password);
    
    // Label báo lỗi
    // m_lblError = new QLabel(this);
    // m_lblError->setObjectName("lblError");
    // m_lblError->setStyleSheet("color: red; font-size: 12px;");
    // m_lblError->setAlignment(Qt::AlignCenter);
    
    // Nút Login
    m_btnLogin = new QPushButton("LOGIN", this);
    m_btnLogin->setObjectName("btnLogin");
    m_btnLogin->setCursor(Qt::PointingHandCursor);
    
    cardLayout->addWidget(lblTitle);
    cardLayout->addWidget(lblSub);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(m_txtUser);
    cardLayout->addWidget(m_txtPass);
   // cardLayout->addWidget(m_lblError);
    cardLayout->addWidget(m_btnLogin);
    
    mainLayout->addWidget(card);
    connect(m_btnLogin, &QPushButton::clicked, this, [this] { handleLogin(); });
}

void LoginView::handleLogin() {
    QString e;
    QString user = m_txtUser->text();
    QString pass = m_txtPass->text();

    QString result = m_controller.authenticate(user, pass, e);

    if (result != "NONE") {
        // Phát Signal thông báo đăng nhập thành công cho main.cpp
        emit loginSuccess(result, user); 
    } else {
        error(e);
    }
}

void LoginView::error(const QString& message) { QMessageBox::warning(this, "Login Error", message); }