#include "controllers/CustomerController.h"
#include "utils/ValidationUtils.h"
#include "repositories/CustomerRepository.h"
#include "repositories/BookingRepository.h"

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

bool CustomerController::addCustomer (Customer& customer, QString& error) {
    if (!ValidationUtils::isNonEmpty(customer.getName())) {
        error = "Customer name cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidEmail(customer.getEmail())) {
        error = "Invalid email format!";
        return false;
    }

    if (!ValidationUtils::isValidPhone(customer.getPhone())) {
        error = "Invalid phone number!";
        return false;
    }

    customer.setId(repository.generateNextId());

    if (repository.add(customer)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool CustomerController::updateCustomer(const Customer& customer, QString& error) {
    if (!ValidationUtils::isNonEmpty(customer.getName())) {
        error = "Customer name cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidEmail(customer.getEmail())) {
        error = "Invalid email format!";
        return false;
    }

    if (!ValidationUtils::isValidPhone(customer.getPhone())) {
        error = "Invalid phone number!";
        return false;
    }
    
    if (repository.update(customer)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool CustomerController::deleteCustomer(const QString& id, QString& error) {
    //Check co phai customerID rong khong
    if (!ValidationUtils::isNonEmpty(id)) {
        error = "Customer ID cannot be empty.";
        return false;
    }

    //Check customer co ton tai khong
    auto customer = repository.findById(id);
    if (!customer) {
        error = "Customer does not exist.";
        return false;
    }

    std::vector<Booking> allBookings = bookingRepo.search(id);

    //Check xem customer co dang dat phong/o khong
    for (const auto& booking : allBookings) {
        if (booking.getCustomerId() == id) {
            if (booking.getStatus() == BookingStatus::Booked || 
                booking.getStatus() == BookingStatus::CheckedIn) {
                
                error = "Cannot delete a customer who currently has active Booked or Checked In bookings.";
                return false;
                }
            }
        }

    if (repository.remove(id)) {
        return true;
    }
    error = repository.lastError();
    return false;
}