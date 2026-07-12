#include "controllers/InvoiceController.h"

InvoiceController::InvoiceController(InvoiceService& service)
    : invoiceService(service) {}

bool InvoiceController::processCreateInvoice(const QString& bookingId, const QString& receptionistId, double subtotalAmount, double discountAmount, const QString& discountName, const QString& paymentMethodStr, QString& error)
{
    if (bookingId.trimmed().isEmpty()) {
        error = "Mã đặt phòng không được để trống khi xuất hóa đơn!";
        return false;
    }
    if (receptionistId.trimmed().isEmpty()) {
        error = "Không xác định được mã lễ tân lập hóa đơn!";
        return false;
    }
    if (subtotalAmount < 0) {
        error = "Số tiền gốc (Subtotal) trên giao diện không hợp lệ!";
        return false;
    }
    if (discountAmount < 0) {
        error = "Số tiền giảm giá không được là số âm!";
        return false;
    }

    PaymentMethod method = Invoice::paymentMethodFromString(paymentMethodStr);

    return invoiceService.createInvoice(bookingId, receptionistId, subtotalAmount, 
                                        discountAmount, discountName, method, error);
}

// Lấy danh sách hóa đơn
std::vector<Invoice> InvoiceController::handleGetAllInvoices() const {
    return invoiceService.getAllInvoices();
}

// Tìm kiếm hóa đơn theo mã hóa đơn
std::optional<Invoice> InvoiceController::handleGetInvoiceById(const QString& id) const {
    if (id.trimmed().isEmpty()) return std::nullopt;
    return invoiceService.getInvoiceById(id);
}

// Tìm kiếm hóa đơn theo mã đặt phòng
std::optional<Invoice> InvoiceController::handleGetInvoiceByBookingId(const QString& bookingId) const {
    if (bookingId.trimmed().isEmpty()) return std::nullopt;
    return invoiceService.getInvoiceByBookingId(bookingId);
}