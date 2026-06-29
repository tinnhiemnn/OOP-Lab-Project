#pragma once

#include "models/Customer.h"
#include <vector>
#include <optional>

class CustomerRepository {
public:
    bool add(const Customer& customer);
    bool update(const Customer& customer);
    bool remove(const std::string& id);
    std::vector<Customer> findAll();
    std::vector<Customer> search(const std::string& keyword);
    std::optional<Customer> findById(const std::string& id);
    std::string lastError() const { 
        return lastErrorMessage; 
    }
    
    static Customer mapCustomer(QSqlQuery& q);

private:
    std::string lastErrorMessage;
};
