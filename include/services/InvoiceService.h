#pragma once

#include "repositories/InvoiceRepository.h"
#include "models/Invoice.h"
#include <QString>
#include <vector>
#include <optional>

class InvoiceService {
private:
    QString generateInvoiceId() const;
    InvoiceRepository& invoiceRepo;
    
public:
    // Constructor nhận vào InvoiceRepository để làm việc với DB
    explicit InvoiceService(InvoiceRepository& invoiceRepo);

    bool createInvoice(const QString& bookingId,
                       const QString& receptionistId,
                       const QString& discountName,
                       PaymentMethod paymentMethod,
                       QString& error);

    double servicesTotal(const QString& bookingId);

    std::vector<Invoice> getAllInvoices() const;

    std::optional<Invoice> getInvoiceById(const QString& id) const;
    std::optional<Invoice> getInvoiceByBookingId(const QString& bookingId) const;
};