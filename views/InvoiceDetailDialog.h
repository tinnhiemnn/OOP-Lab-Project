#pragma once


#include "repositories/BookingRepository.h"
#include "repositories/CustomerRepository.h"
#include "repositories/ReceptionistRepository.h"
#include "repositories/RoomRepository.h"

#include "models/Invoice.h"
#include <QDialog>

class InvoiceDetailDialog : public QDialog {
    Q_OBJECT
public:
    explicit InvoiceDetailDialog(const Invoice& invoice, QWidget* parent = nullptr);

private: 
    BookingRepository bookingRepo;
    CustomerRepository customerRepo;
    ReceptionistRepository receptionistRepo;
    RoomRepository roomRepo;
};
