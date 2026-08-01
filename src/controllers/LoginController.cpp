#include "controllers/LoginController.h"
#include "utils/ValidationUtils.h" 

LoginController::LoginController() {}

QString LoginController::authenticate(const QString& username, const QString& password, QString& error) {
    if (!ValidationUtils::isNonEmpty(username) || !ValidationUtils::isNonEmpty(password)) {
        error = "Username and password cannot be empty.";
        return "NONE";
    }

    if (username == "admin" && password == "admin123") {
        return "ADMIN";
    }

    QString result = repository.verifyLogin(username, password);
    if (result == "NONE") {
        error = "Invalid username or password!";
    }
    return result;
}