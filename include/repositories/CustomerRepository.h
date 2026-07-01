#pragma once

#include "models/Customer.h"
#include <vector>
#include <optional>

class CustomerRepository {
public:
    bool add(const Customer& customer);
    bool update(const Customer& customer);
    bool remove(const QString& id);
    std::vector<Customer> findAll();
    std::vector<Customer> search(const QString& keyword);
    std::optional<Customer> findById(const QString& id);
    QString lastError() const { 
        return lastErrorMessage; 
    }
    
    static Customer mapCustomer(QSqlQuery& q);

private:
    QString lastErrorMessage;
};
