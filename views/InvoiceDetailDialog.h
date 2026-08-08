#pragma once

#include "controllers/InvoiceController.h"

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
