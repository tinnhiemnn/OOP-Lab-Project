#pragma once
#include <QString>
#include "repositories/ReceptionistRepository.h"

class LoginController {
private:
    ReceptionistRepository repository;

public:
    LoginController();
    QString authenticate(const QString& username, const QString& password, QString& error);
};