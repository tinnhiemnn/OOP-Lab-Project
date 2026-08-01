#include "LoginView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QSettings>

LoginView::LoginView(QWidget *parent) : QWidget(parent) {
    setObjectName("loginRoot");
    setAttribute(Qt::WA_StyledBackground, true);

    auto* outer = new QVBoxLayout(this);
    outer->setAlignment(Qt::AlignCenter);

    // ================= KHỐI CARD TỔNG (2 panel: Brand + Form) =================
    auto* splitCard = new QWidget(this);
    splitCard->setObjectName("loginSplitCard");
    splitCard->setAttribute(Qt::WA_StyledBackground, true);
    splitCard->setFixedSize(920, 560);

    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(40);
    shadow->setOffset(0, 14);
    shadow->setColor(QColor(107, 69, 87, 60)); // đổ bóng ánh nâu hồng, hợp tông thay vì đen thuần
    splitCard->setGraphicsEffect(shadow);

    auto* splitLayout = new QHBoxLayout(splitCard);
    splitLayout->setContentsMargins(0, 0, 0, 0);
    splitLayout->setSpacing(0);

    // ===================== PANEL TRÁI: BRAND / HERO =====================
    auto* brandPanel = new QWidget(splitCard);
    brandPanel->setObjectName("loginBrandPanel");
    brandPanel->setAttribute(Qt::WA_StyledBackground, true);
    brandPanel->setFixedWidth(380);

    auto* brandLayout = new QVBoxLayout(brandPanel);
    brandLayout->setContentsMargins(40, 48, 40, 40);
    brandLayout->setSpacing(0);

    auto* brandMark = new QLabel("PH", brandPanel);
    brandMark->setObjectName("brandMarkXl");
    brandMark->setFixedSize(56, 56);

    auto* brandTitle = new QLabel("Pink Home", brandPanel);
    brandTitle->setObjectName("loginBrandTitle");

    auto* brandTagline = new QLabel("S T A F F   P O R T A L", brandPanel);
    brandTagline->setObjectName("loginBrandTagline");

    auto* divider = new QFrame(brandPanel);
    divider->setObjectName("loginDivider");
    divider->setFixedHeight(1);

    auto* notice = new QLabel(
        "Internal system for Pink Home staff only. "
        "Please sign in with the account issued by your administrator.",
        brandPanel);
    notice->setObjectName("loginNotice");
    notice->setWordWrap(true);

    auto* features = new QLabel(
        "•  Booking management\n•  Rooms & guest management\n•  Revenue reports",
        brandPanel);
    features->setObjectName("loginFeatures");

    brandLayout->addWidget(brandMark);
    brandLayout->addSpacing(22);
    brandLayout->addWidget(brandTitle);
    brandLayout->addSpacing(4);
    brandLayout->addWidget(brandTagline);
    brandLayout->addStretch();
    brandLayout->addWidget(divider);
    brandLayout->addSpacing(18);
    brandLayout->addWidget(notice);
    brandLayout->addSpacing(24);
    brandLayout->addWidget(features);

    // ===================== PANEL PHẢI: FORM ĐĂNG NHẬP =====================
    auto* formPanel = new QWidget(splitCard);
    formPanel->setObjectName("loginFormPanel");
    formPanel->setAttribute(Qt::WA_StyledBackground, true);

    auto* formLayout = new QVBoxLayout(formPanel);
    formLayout->setContentsMargins(56, 64, 56, 40);
    formLayout->setSpacing(0);
    formLayout->setAlignment(Qt::AlignTop);

    auto* eyebrow = new QLabel("STAFF SIGN IN", formPanel);
    eyebrow->setObjectName("loginEyebrow");

    auto* lblTitle = new QLabel("Welcome back", formPanel);
    lblTitle->setObjectName("loginTitle");

    auto* lblSub = new QLabel("Sign in with your staff account to continue", formPanel);
    lblSub->setObjectName("loginSub");
    lblSub->setWordWrap(true);

    auto* lblUser = new QLabel("Username / Staff ID", formPanel);
    lblUser->setObjectName("fieldLabel");

    m_txtUser = new QLineEdit(formPanel);
    m_txtUser->setObjectName("loginInput");
    m_txtUser->setPlaceholderText("Enter your username...");

    auto* lblPass = new QLabel("Password", formPanel);
    lblPass->setObjectName("fieldLabel");

    auto* passRow = new QHBoxLayout();
    passRow->setSpacing(8);

    m_txtPass = new QLineEdit(formPanel);
    m_txtPass->setObjectName("loginInput");
    m_txtPass->setPlaceholderText("Enter your password...");
    m_txtPass->setEchoMode(QLineEdit::Password);

    m_btnTogglePass = new QPushButton("Show", formPanel);
    m_btnTogglePass->setObjectName("btnTogglePass");
    m_btnTogglePass->setCursor(Qt::PointingHandCursor);
    m_btnTogglePass->setCheckable(true);
    m_btnTogglePass->setFixedWidth(56);

    passRow->addWidget(m_txtPass, 1);
    passRow->addWidget(m_btnTogglePass);

    auto* optionRow = new QHBoxLayout();

    m_chkRemember = new QCheckBox("Remember me", formPanel);
    m_chkRemember->setObjectName("rememberCheck");

    m_btnForgot = new QPushButton("Forgot password?", formPanel);
    m_btnForgot->setObjectName("btnForgotLink");
    m_btnForgot->setCursor(Qt::PointingHandCursor);
    m_btnForgot->setFlat(true);

    optionRow->addWidget(m_chkRemember);
    optionRow->addStretch();
    optionRow->addWidget(m_btnForgot);

    m_btnLogin = new QPushButton("SIGN IN", formPanel);
    m_btnLogin->setObjectName("btnLogin");
    m_btnLogin->setCursor(Qt::PointingHandCursor);
    m_btnLogin->setFixedHeight(46);

    auto* footer = new QLabel("© 2026 Pink Home · Internal system, not for guest use", formPanel);
    footer->setObjectName("loginFooter");

    formLayout->addWidget(eyebrow);
    formLayout->addSpacing(10);
    formLayout->addWidget(lblTitle);
    formLayout->addSpacing(6);
    formLayout->addWidget(lblSub);
    formLayout->addSpacing(32);
    formLayout->addWidget(lblUser);
    formLayout->addSpacing(6);
    formLayout->addWidget(m_txtUser);
    formLayout->addSpacing(18);
    formLayout->addWidget(lblPass);
    formLayout->addSpacing(6);
    formLayout->addLayout(passRow);
    formLayout->addSpacing(14);
    formLayout->addLayout(optionRow);
    formLayout->addSpacing(26);
    formLayout->addWidget(m_btnLogin);
    formLayout->addStretch();
    formLayout->addWidget(footer);

    splitLayout->addWidget(brandPanel);
    splitLayout->addWidget(formPanel, 1);

    outer->addWidget(splitCard);

    connect(m_btnLogin, &QPushButton::clicked, this, [this] { handleLogin(); });
    connect(m_btnTogglePass, &QPushButton::toggled, this, &LoginView::togglePasswordVisibility);
    connect(m_btnForgot, &QPushButton::clicked, this, [this] {
        QMessageBox::information(this, "Forgot Password",
            "Please contact your system administrator to reset your password.");
    });

    loadRememberedUser();
}

void LoginView::togglePasswordVisibility() {
    bool showing = m_btnTogglePass->isChecked();
    m_txtPass->setEchoMode(showing ? QLineEdit::Normal : QLineEdit::Password);
    m_btnTogglePass->setText(showing ? "Hide" : "Show");
}

void LoginView::handleLogin() {
    QString e;
    QString user = m_txtUser->text();
    QString pass = m_txtPass->text();

    QString result = m_controller.authenticate(user, pass, e);

    if (result != "NONE") {
        saveRememberedUser();
        // Phát Signal thông báo đăng nhập thành công cho main.cpp
        emit loginSuccess(result, user);
    } else {
        error(e);
    }
}

void LoginView::error(const QString& message) {
    QMessageBox::warning(this, "Sign In Failed", message);
}

// "Remember me" chỉ lưu username cục bộ qua QSettings — không đụng tới
// Controller/Service/Repository nên không cần backend hỗ trợ gì thêm.
void LoginView::loadRememberedUser() {
    QSettings settings("PinkHome", "HotelManagement");
    QString savedUser = settings.value("rememberedUser").toString();
    if (!savedUser.isEmpty()) {
        m_txtUser->setText(savedUser);
        m_chkRemember->setChecked(true);
    }
}

void LoginView::saveRememberedUser() {
    QSettings settings("PinkHome", "HotelManagement");
    if (m_chkRemember->isChecked())
        settings.setValue("rememberedUser", m_txtUser->text());
    else
        settings.remove("rememberedUser");
}