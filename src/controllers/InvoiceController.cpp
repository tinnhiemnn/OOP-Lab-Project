#include "controllers/InvoiceController.h"
#include "utils/ValidationUtils.h"

InvoiceController::InvoiceController(InvoiceService& service)
    : invoiceService(service) {}

bool InvoiceController::createInvoice(const QString& bookingId, const QString& receptionistId,  const QString& discountName, const QString& invoiceId, const QString& paymentMethodStr, QString& error)
{
    if (!ValidationUtils::isNonEmpty(bookingId)) {
        error = "Booking ID cannot be empty!";
        return false;
    }
    if (!ValidationUtils::isNonEmpty(receptionistId)) {
        error = "Receptionist ID cannot be empty.";
        return false;
    }

    PaymentMethod method = Invoice::paymentMethodFromString(paymentMethodStr);

    return invoiceService.createInvoice(bookingId, receptionistId, discountName, invoiceId, method, error);
}

bool InvoiceController::createAllInvoice( const QString& bookingId, const QString& receptionistId, const QString& discountName, const QString& paymentMethodStr, QString& error){
    if (!ValidationUtils::isNonEmpty(bookingId))
    {
        error = "Booking Id cannot be empty.";
        return false;
    }

    if (!ValidationUtils::isNonEmpty(receptionistId))
    {
        error = "Receptionist ID cannot be empty.";
        return false;
    }

    PaymentMethod method = Invoice::paymentMethodFromString(paymentMethodStr);
    
    return invoiceService.createAllInvoice(bookingId, receptionistId, discountName, method, error);
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