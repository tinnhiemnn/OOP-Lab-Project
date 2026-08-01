#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include "controllers/LoginController.h"

class LoginView : public QWidget {
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);

signals:
    // Signal phát ra khi đăng nhập thành công để main.cpp nhận biết và chuyển sang MainWindow
    void loginSuccess(const QString& role, const QString& username);

private slots:
    void handleLogin();
    void togglePasswordVisibility();

private:
    void error(const QString& message);
    void loadRememberedUser();
    void saveRememberedUser();

    QLineEdit*   m_txtUser;
    QLineEdit*   m_txtPass;
    QPushButton* m_btnTogglePass;
    QCheckBox*   m_chkRemember;
    QPushButton* m_btnForgot;
    QPushButton* m_btnLogin;

    LoginController m_controller;
};