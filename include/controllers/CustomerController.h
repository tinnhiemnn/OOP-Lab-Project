#pragma once

#include "repositories/CustomerRepository.h"

class CustomerController
{
private:
    CustomerRepository repository;
    BookingRepository bookingRepo;

public:
    CustomerController();

    std::vector<Customer> listCustomers();
    std::vector<Customer> searchCustomers(const QString& keyword);
    std::optional<Customer> getCustomerById(const QString& id);
    
    bool addCustomer(Customer& customer, QString& error);
    bool updateCustomer(const Customer& customer, QString& error);
    bool deleteCustomer(const QString& id, QString& error);
};