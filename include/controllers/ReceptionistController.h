#pragma once

#include "repositories/ReceptionistRepository.h"

class ReceptionistController 
{
private:
    ReceptionistRepository repository;

public:
    ReceptionistController();

    std::vector<Receptionist> getAllReceptionists();
    std::vector<Receptionist> searchReceptionists(const QString& keyword);
    std::optional<Receptionist> getReceptionistById(const QString& id);
    bool addReceptionist(Receptionist& receptionist, QString& error);
    bool updateReceptionist(const Receptionist& receptionist, QString& error);
    bool deleteReceptionist(const QString& id, QString& error);
};