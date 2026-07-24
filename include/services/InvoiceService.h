#pragma once

#include "repositories/InvoiceRepository.h"
#include "repositories/BookingRepository.h"
#include "repositories/RoomRepository.h"
#include "models/Invoice.h"

#include <QString>
#include <vector>
#include <optional>

class InvoiceService {
private:
    QString generateInvoiceId() const;
    InvoiceRepository& invoiceRepo;
    BookingRepository bookingRepo;
    RoomRepository roomRepo;
    
public:
    // Constructor nhận vào InvoiceRepository để làm việc với DB
    explicit InvoiceService(InvoiceRepository& invoiceRepo);

    double servicesTotal(const QString& bookingId);

    bool createInvoice(const QString& bookingId, const QString& receptionistId, const QString& discountName, const QString& invoiceId, PaymentMethod paymentMethod, QString& error);

    bool createAllInvoice(const QString& bookingId, const QString& receptionistId, const QString& discountName, PaymentMethod paymentMethod, QString& error);

    std::vector<Invoice> getAllInvoices() const;

    std::optional<Invoice> getInvoiceById(const QString& id) const;
    std::optional<Invoice> getInvoiceByBookingId(const QString& bookingId) const;
};