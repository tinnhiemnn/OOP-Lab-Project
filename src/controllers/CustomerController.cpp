#include "controllers/CustomerController.h"
#include "utils/ValidationUtils.h"

CustomerController::CustomerController() {}

std::vector<Customer> CustomerController::listCustomers() {
    return customerRepo.findAll();
}

std::vector<Customer> CustomerController::searchCustomers(const QString& keyword) {
    return customerRepo.search(keyword);
}

std::optional<Customer> CustomerController::getCustomerById(const QString& id) {
    return customerRepo.findById(id);
}

bool CustomerController::validateCustomer(const Customer& customer, QString& error)
{
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

    auto emailCustomers = customerRepo.search(customer.getEmail());

    for (const auto& c : emailCustomers)
    {
        if (c.getEmail() == customer.getEmail() &&
            c.getId() != customer.getId())
        {
            error = "Email has already been registered.";
            return false;
        }
    }

    auto phoneCustomers = customerRepo.search(customer.getPhone());

    for (const auto& c : phoneCustomers)
    {
        if (c.getPhone() == customer.getPhone() &&
            c.getId() != customer.getId())
        {
            error = "Phone number has already been registered.";
            return false;
        }
    }

    return true;
}

bool CustomerController::addCustomer (Customer& customer, QString& error) {
    customer.setId(customerRepo.generateNextId());

    if (!validateCustomer(customer, error)) {
        return false;
    }

    if (customerRepo.add(customer)) {
        return true;
    }

    error = customerRepo.lastError();
    return false;
}

bool CustomerController::updateCustomer(const Customer& customer, QString& error) {
    if (!validateCustomer(customer, error)) {
        return false;
    }
    
    if (customerRepo.update(customer)) {
        return true;
    }
    error = customerRepo.lastError();
    return false;
}

bool CustomerController::deleteCustomer(const QString& id, QString& error) {
    if (!ValidationUtils::isNonEmpty(id)) {
        error = "Customer ID cannot be empty.";
        return false;
    }

    auto customer = customerRepo.findById(id);
    if (!customer) {
        error = "Customer does not exist.";
        return false;
    }

    std::vector<Booking> allBookings = bookingRepo.search(id);

    for (const auto& booking : allBookings) {
        if (booking.getCustomerId() == id) {
            if (booking.getStatus() == BookingStatus::Booked || 
                booking.getStatus() == BookingStatus::CheckedIn) {
                
                error = "Cannot delete a customer who currently has active Booked or Checked In bookings.";
                return false;
                }
            }
        }

    if (customerRepo.remove(id)) {
        return true;
    }
    
    error = customerRepo.lastError();
    return false;
}