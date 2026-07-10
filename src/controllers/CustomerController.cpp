#include "controllers/CustomerController.h"

CustomerController::CustomerController() {}

std::vector<Customer> CustomerController::listCustomers() {
    return repository.findAll();
}

std::vector<Customer> CustomerController::searchCustomers(const QString& keyword) {
    return repository.search(keyword);
}

std::optional<Customer> CustomerController::getCustomerById(const QString& id) {
    return repository.findById(id);
}

bool CustomerController::addCustomer(const Customer& customer, QString& error) {
    if (repository.add(customer)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool CustomerController::updateCustomer(const Customer& customer, QString& error) {
    if (repository.update(customer)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool CustomerController::deleteCustomer(const QString& id, QString& error) {
    if (id.isEmpty()) {
        error = "ID Customer Invalid!";
        return false;
    }

    if (repository.remove(id)) {
        return true;
    }
    error = repository.lastError();
    return false;
}