#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
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

private:
    void error(const QString& message);

    QLineEdit* m_txtUser;
    QLineEdit* m_txtPass;
    QPushButton* m_btnLogin;
    
    LoginController m_controller; 

};