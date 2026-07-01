#pragma once

#include "models/Receptionist.h"
#include <vector>
#include <optional>

class ReceptionistRepository {
public:
    bool add(const Receptionist& Receptionist);
    bool update(const Receptionist& Receptionist);
    bool remove(const QString& id);
    std::vector<Receptionist> findAll();
    std::vector<Receptionist> search(const QString& keyword);
    std::optional<Receptionist> findById(const QString& id);
    QString lastError() const { 
        return lastErrorMessage; 
    }
    
    static Receptionist mapReceptionist(QSqlQuery& q);

private:
    QString lastErrorMessage;
};
