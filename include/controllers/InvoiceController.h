#pragma once

#include "services/InvoiceService.h"

#include <QString>
#include <vector>
#include <optional>

class InvoiceController {
private:
    InvoiceService& invoiceService;

public:
    explicit InvoiceController(InvoiceService& service);

    bool createInvoice(const QString& bookingId, const QString& receptionistId, const QString& discountName, const QString& invoiceId, const QString& paymentMethodStr, QString& error);

    bool createAllInvoice(const QString& groupcode, const QString& receptionistId, const QString& discountName, const QString& paymentMethodStr, QString& error);
    
    // Lấy toàn bộ danh sách hóa đơn để hiển thị
    std::vector<Invoice> handleGetAllInvoices() const;
    
    // Tìm kiếm hóa đơn phục vụ chức năng tra cứu trên UI
    std::optional<Invoice> handleGetInvoiceById(const QString& id) const;
    std::optional<Invoice> handleGetInvoiceByBookingId(const QString& bookingId) const;
};