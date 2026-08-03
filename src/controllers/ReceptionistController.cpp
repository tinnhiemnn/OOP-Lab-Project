#include "controllers/ReceptionistController.h"
#include "utils/ValidationUtils.h"

ReceptionistController::ReceptionistController() {}

std::vector<Receptionist> ReceptionistController::getAllReceptionists() 
{
    return repository.findAll();
}

std::vector<Receptionist> ReceptionistController::searchReceptionists(const QString& keyword) 
{
    return repository.search(keyword);
}

std::optional<Receptionist> ReceptionistController::getReceptionistById(const QString& id) 
{
    return repository.findById(id);
}

bool ReceptionistController::addReceptionist(Receptionist& receptionist, QString& error) 
{
    if (!ValidationUtils::isNonEmpty(receptionist.getName())) {
        error = "Receptionist name cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidEmail(receptionist.getEmail())) {
        error = "Invalid email format!";
        return false;
    }

    if (!ValidationUtils::isNonEmpty(receptionist.getPassword())) {
        error = "Receptionist password cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidPassword(receptionist.getPassword())) {
        error = "Password must be 6-10 characters long and contain no special characters.";
        return false;
    }

    receptionist.setId(repository.generateNextId());

    if (repository.add(receptionist)) return true;
    error = repository.lastError();
    return false;
}

bool ReceptionistController::updateReceptionist(const Receptionist& receptionist, QString& error) {
    if (!ValidationUtils::isNonEmpty(receptionist.getName())) {
        error = "Receptionist name cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidEmail(receptionist.getEmail())) {
        error = "Invalid email format!";
        return false;
    }

    if (!ValidationUtils::isNonEmpty(receptionist.getPassword())) {
        error = "Receptionist password cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidPassword(receptionist.getPassword())) {
        error = "Password must be 6-10 characters long and contain no special characters.";
        return false;
    }

    if (repository.update(receptionist)) {
        return true;
    }
    error = repository.lastError();
    return false;
}

bool ReceptionistController::deleteReceptionist(const QString& id, QString& error) {
     // Check receptionist ID có rỗng không
    if (!ValidationUtils::isNonEmpty(id))
    {
        error = "Receptionist ID cannot be empty.";
        return false;
    }

    auto receptionist = repository.findById(id);
    // Check receptionist có tồn tại không
    if (!receptionist.has_value())
    {
        error = "Receptionist does not exist.";
        return false;
    }

    if (repository.remove(id))
    {
        return true;
    }

    error = repository.lastError();
    return false;
}