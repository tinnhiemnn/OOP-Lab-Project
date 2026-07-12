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

    bool processCreateInvoice(const QString& bookingId, const QString& receptionistId, double subtotalAmount, double discountAmount, const QString& discountName, const QString& paymentMethodStr, QString& error);

    // Lấy toàn bộ danh sách hóa đơn để hiển thị
    std::vector<Invoice> handleGetAllInvoices() const;
    
    // Tìm kiếm hóa đơn phục vụ chức năng tra cứu trên UI
    std::optional<Invoice> handleGetInvoiceById(const QString& id) const;
    std::optional<Invoice> handleGetInvoiceByBookingId(const QString& bookingId) const;
};