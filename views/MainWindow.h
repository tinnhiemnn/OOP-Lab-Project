#pragma once

#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow {
private:
    QLabel* m_pageTitle = nullptr;
    QLabel* m_pageSubtitle = nullptr;
    QString m_userRole;

    void applyRolePermissions(); 
    void handleLogout();
public:
    explicit MainWindow(const QString& role, const QString& username, QWidget* parent = nullptr);
};

