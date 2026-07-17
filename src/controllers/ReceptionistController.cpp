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

bool ReceptionistController::addReceptionist(const Receptionist& receptionist, QString& error) 
{
    if (!ValidationUtils::isNonEmpty(receptionist.getName())) {
        error = "Receptionist name cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isValidEmail(receptionist.getEmail())) {
        error = "Invalid email format!";
        return false;
    }

    if (repository.add(receptionist)) return true;
    error = repository.lastError();
    return false;
}