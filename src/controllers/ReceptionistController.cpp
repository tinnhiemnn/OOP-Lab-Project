#include "controllers/ReceptionistController.h"

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
    if (repository.add(receptionist)) return true;
    error = repository.lastError();
    return false;
}