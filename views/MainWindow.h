#pragma once

#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow {
private:
    QLabel* m_pageTitle = nullptr;
    QLabel* m_pageSubtitle = nullptr;
public:
    explicit MainWindow(QWidget* parent = nullptr);
};

