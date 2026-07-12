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

    // 1. TẠO HÓA ĐƠN: Khớp hoàn toàn với bộ tham số và constructor của Invoice.h
    bool createInvoice(const QString& bookingId,
                       const QString& receptionistId,
                       double subtotalAmount,
                       double discountAmount,
                       const QString& discountName,
                       PaymentMethod paymentMethod,
                       QString& error);

    // 2. TRUY VẤN: Lấy danh sách toàn bộ hóa đơn
    std::vector<Invoice> getAllInvoices() const;

    // 3. TÌM KIẾM: Tìm hóa đơn theo mã hóa đơn hoặc mã đặt phòng
    std::optional<Invoice> getInvoiceById(const QString& id) const;
    std::optional<Invoice> getInvoiceByBookingId(const QString& bookingId) const;
};