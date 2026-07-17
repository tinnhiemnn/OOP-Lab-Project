#include "database/DatabaseManager.h"
#include "MainWindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

int main(int argc, char* argv[]) {
    QApplication::setStyle("Fusion");
    QApplication app(argc, argv);

    //load file qss toan cuc
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        QString stylesheet = stream.readAll();
        app.setStyleSheet(stylesheet); // Áp dụng giao diện cho toàn bộ app
        file.close();
    } else {
        qWarning("Warning: Could not open style.qss file from resources.");
    }

    DatabaseManager& database = DatabaseManager::getInstance();
    if (!database.openDatabase()) {
        QMessageBox::critical(nullptr, "Database Error", QString("Cannot open database:\n") + database.lastError());
        return 1;
    }

    MainWindow window;
    window.show();
    return app.exec();
}

