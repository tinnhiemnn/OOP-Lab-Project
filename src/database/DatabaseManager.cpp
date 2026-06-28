#include "database/DatabaseManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::contains("hotel_connection")
             ? QSqlDatabase::database("hotel_connection")
             : QSqlDatabase::addDatabase("QSQLITE", "hotel_connection");
}

DatabaseManager& DatabaseManager::getInstance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::openDatabase(const QString& databasePath) {
    if (db.isOpen()) return true;
    const QString resolved = resolveDatabasePath(databasePath);
    QDir().mkpath(QFileInfo(resolved).absolutePath());
    db.setDatabaseName(resolved);
    if (!db.open()) {
        lastErrorMessage = db.lastError().text();
        return false;
    }
    return initializeSchema();
}

QString DatabaseManager::resolveDatabasePath(const QString& databasePath) const {
    if (!databasePath.isEmpty()) return databasePath;
    return QDir(QStringLiteral(PROJECT_SOURCE_DIR)).absoluteFilePath("data/hotel.db");
}

 bool DatabaseManager::initializeSchema() {
    const QString schemaPath = QDir(QStringLiteral(PROJECT_SOURCE_DIR)).absoluteFilePath("data/schema.sql");
    
    QFile file(schemaPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        lastErrorMessage = "Cannot open schema.sql";
        return false;
    }

    const QStringList statements = QString::fromUtf8(file.readAll()).split(';', Qt::SkipEmptyParts);
    for (const QString& raw : statements) {
        const QString sql = raw.trimmed();
        if (sql.isEmpty() || sql.startsWith("--")) continue;
        QSqlQuery query(db);
        if (!query.exec(sql)) {
            lastErrorMessage = query.lastError().text() + "\nSQL: " + sql;
            db.rollback();
            return false;
        }
    }
    QSqlQuery query(db);
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        lastErrorMessage = query.lastError().text();
        db.rollback();
        return false;
    };
    return true;
 }

 QSqlDatabase DatabaseManager::database() const {
    return db;
}

QString DatabaseManager::lastError() const {
    return lastErrorMessage;
}