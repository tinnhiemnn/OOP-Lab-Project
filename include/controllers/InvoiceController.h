#pragma once

#include "services/InvoiceService.h"
#include "repositories/InvoiceRepository.h"

#include <QString>
#include <vector>
#include <optional>

class InvoiceController {
private:
    InvoiceRepository invoiceRepo;
    InvoiceService invoiceService;

public:
    InvoiceController();

    bool createInvoice(const QString& bookingId, const QString& receptionistId, const QString& discountName, const QString& invoiceId, const QString& paymentMethodStr, QString& error);

    bool createAllInvoice(const QString& bookingId, const QString& receptionistId, const QString& discountName, const QString& paymentMethodStr, QString& error);

    std::vector<Invoice> handleGetAllInvoices() const;
    std::optional<Invoice> handleGetInvoiceById(const QString& id) const;
    std::optional<Invoice> handleGetInvoiceByBookingId(const QString& bookingId) const;
    std::vector<Invoice> searchInvoices(const QString& keyword, const QString& discountName, const QString& PaymentMethod);
};