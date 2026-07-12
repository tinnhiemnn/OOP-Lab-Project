#include "services/InvoiceService.h"
#include <QDateTime>

InvoiceService::InvoiceService(InvoiceRepository& invoiceRepo)
    : invoiceRepo(invoiceRepo) {}

bool InvoiceService::createInvoice(const QString& bookingId,
                                   const QString& receptionistId,
                                   double subtotalAmount,
                                   double discountAmount,
                                   const QString& discountName,
                                   PaymentMethod paymentMethod,
                                   QString& error) 
{
    // Kiểm tra tính hợp lệ dữ liệu
    if (subtotalAmount < 0) {
        error = "Số tiền gốc (Subtotal) không được nhỏ hơn 0!";
        return false;
    }
    if (discountAmount < 0) {
        error = "Số tiền giảm giá không được nhỏ hơn 0!";
        return false;
    }

    // Tính toán số tiền thực tế khách phải trả (Total = Subtotal - Discount)
    double totalAmount = subtotalAmount - discountAmount;
    if (totalAmount < 0) {
        totalAmount = 0.0; // Tránh giảm giá nhiều hơn tiền phòng
    }

    // Tự sinh mã hóa đơn duy nhất và lấy ngày hiện tại
    QString invoiceId = generateInvoiceId();
    QDate issuedDate = QDate::currentDate();

    Invoice newInvoice( invoiceId,  bookingId,   receptionistId,  issuedDate,  subtotalAmount,  totalAmount,  discountAmount,  paymentMethod, discountName
    );

    // Lưu vào database
    if (!invoiceRepo.add(newInvoice)) {
        error = "Lỗi Database khi lưu hóa đơn: " + invoiceRepo.lastError();
        return false;
    }

    return true;
}

std::vector<Invoice> InvoiceService::getAllInvoices() const {
    return invoiceRepo.findAll();
}

std::optional<Invoice> InvoiceService::getInvoiceById(const QString& id) const {
    return invoiceRepo.findById(id);
}

std::optional<Invoice> InvoiceService::getInvoiceByBookingId(const QString& bookingId) const {
    auto allInvoices = invoiceRepo.findAll();
    for (const auto& inv : allInvoices) {
        if (inv.getBookingId() == bookingId) {
            return inv;
        }
    }
    return std::nullopt;
}
//hàm sinh mã hóa đơn tự động định dạng: INV_timestamp
QString InvoiceService::generateInvoiceId() const {
    return "INV_" + QString::number(QDateTime::currentMSecsSinceEpoch());
}