#include "services/InvoiceService.h"

#include "patterns/PricingContext.h"
#include "patterns/SeasonalDiscount.h"
#include "patterns/MemberDiscount.h"

#include "utils/DateUtils.h"

#include "models/Booking.h"

#include <QDateTime>
#include <optional>

InvoiceService::InvoiceService(InvoiceRepository& invoiceRepo)
    : invoiceRepo(invoiceRepo) {}

struct servicePrice {
    double buffet = 200000;
    double laundry = 50000;
    double decoration = 500000;
};

double InvoiceService::servicesTotal(const QString& bookingId) {
    servicePrice prices;

    auto booking = bookingRepo.findById(bookingId);

    if (!booking) return 0.0;

    const int days = DateUtils::daysBetween(booking->getCheckIn().toString(), booking->getCheckOut().toString());

    double laundryTotal = 0.0;
    if (booking->isUsingLaundry()){
        laundryTotal = (days + 1)/2 * prices.laundry;
    }

    double decorationTotal = 0.0;
    if (booking->isUsingDecoration()){
        decorationTotal = prices.decoration;
    }

    double total = (booking->getBuffetQuantity()*prices.buffet) + laundryTotal + decorationTotal;

    return total;
}

bool InvoiceService::createInvoice(const QString& bookingId, const QString& receptionistId, const QString& discountName, PaymentMethod paymentMethod, QString& error) 
{
    auto booking = bookingRepo.findById(bookingId);
    if (!booking) {
        error = "Booking does not exist.";
        return false;
    }

    auto room = roomRepo.findById(booking->getRoomId());
    if (!room) {
        error = "Room does not exist.";
        return false;
    }

    PricingContext pricing;
    if (discountName == "Seasonal") {
        pricing.setStrategy(std::make_unique<SeasonalDiscount>());
    }
    else if (discountName == "Member") {
        pricing.setStrategy(std::make_unique<MemberDiscount>());
    }

    const int days = DateUtils::daysBetween(booking->getCheckIn().toString(), booking->getCheckOut().toString());

    double subtotalAmount = servicesTotal(bookingId) + room->calculatePrice(days);

    double totalAmount = pricing.calculateFinalAmount(subtotalAmount);
    if (totalAmount < 0) {
        totalAmount = 0.0; //Tránh giảm giá nhiều hơn tiền phòng
    }

    double discountAmount = subtotalAmount - totalAmount;

    // Tự sinh mã hóa đơn duy nhất và lấy ngày hiện tại
    QString invoiceId = generateInvoiceId();
    QDate issuedDate = QDate::currentDate();

    Invoice newInvoice( invoiceId,  bookingId,   receptionistId,  issuedDate,  subtotalAmount,  totalAmount,  discountAmount,  paymentMethod, discountName
    );

    // Lưu vào database
    if (!invoiceRepo.add(newInvoice)) {
        error = "Database error when saving the invoice: " + invoiceRepo.lastError();
        return false;
    }

    return true;
}

bool InvoiceService::createAllInvoice(const QString& groupcode,
                       const QString& receptionistId,
                       const QString& discountName,
                       PaymentMethod paymentMethod,
                       QString& error) {
    auto bookings = bookingRepo.search(groupcode);
             
    bool found = false;

    for (const auto& booking : bookings)
    {
        if (booking.getGroupCode() != groupcode) continue;

        found = true;

        if (!createInvoice(booking.getId(),
                           receptionistId,
                           discountName,
                           paymentMethod,
                           error))
        {
            return false;
        }
    }

    if (!found)
    {
        error = "No bookings found for this group.";
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