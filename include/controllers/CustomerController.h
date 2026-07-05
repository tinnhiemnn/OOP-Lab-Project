#pragma once

#include "repositories/CustomerRepository.h"

class CustomerController
{
private:
    CustomerRepository repository;

public:
    CustomerController() {}

    std::vector<Customer> listCustomers()
    {
        return repository.findAll();
    }

    std::vector<Customer> searchCustomers(const QString& keyword)
    {
        return repository.search(keyword);
    }

    std::optional<Customer> getCustomerById(const QString& id)
    {
        return repository.findById(id);
    }

    bool addCustomer(const Customer& customer, QString& error)
    {
        //co the chen logic kiem tra (validate) o day truoc khi goi

        if (repository.add(customer)) {
            return true;
        } else {
            //lay loi tu database va tra ve thong bao loi
            error = repository.lastError();
            return false;
        }
    }

    bool updateCustomer(const Customer& customer, QString& error)
    {
        if (repository.update(customer)) {
            return true;
        } else {
            error = repository.lastError();
            return false;
        }
    }

    bool deleteCustomer(const QString& id, QString& error)
    {
        if (id.isEmpty()) {
            error = "ID Customer Invalid!";
            return false;
        }

        if (repository.remove(id)) {
            return true;
        } else {
            error = repository.lastError();
            return false;
        }
    }
};