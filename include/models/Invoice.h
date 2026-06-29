#pragma once

#include <string>

enum class PaymentMethod
{
    Cash,
    CreditCard,
    EWallet,
    BankTransfer
};

class Invoice
{
private:
    std::string id;
    std::string bookingId;
    std::string receptionistId;
    std::string issuedDate;

    double totalAmount = 0.0;
    double discountAmount = 0.0;

    PaymentMethod paymentMethod = PaymentMethod::Cash;

    std::string discountName;

public:
    Invoice() = default;
    Invoice(std::string id,
            std::string bookingId,
            std::string receptionistId,
            std::string issuedDate,
            double totalAmount = 0,
            double discountAmount = 0,
            PaymentMethod paymentMethod = PaymentMethod::Cash,
            std::string discountName = "")
        : id(std::move(id)),
        bookingId(std::move(bookingId)),
        receptionistId(std::move(receptionistId)),
        issuedDate(std::move(issuedDate)),
        totalAmount(totalAmount),
        discountAmount(discountAmount),
        paymentMethod(paymentMethod),
        discountName(std::move(discountName)) {}

    const std::string& getId() const { return id; }
    const std::string& getBookingId() const { return bookingId; }
    const std::string& getReceptionistId() const { return receptionistId; }
    const std::string& getIssuedDate() const { return issuedDate; }
    double getTotalAmount() const { return totalAmount; }
    double getDiscountAmount() const { return discountAmount; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    const std::string& getDiscountName() const { return discountName; }

    void setTotalAmount(double amount)
    {
        totalAmount = amount;
    }

    void setPaymentMethod(PaymentMethod method)
    {
        paymentMethod = method;
    }

    void setDiscountName(const std::string& value)
    {
        discountName = value;
    }

    void setDiscountAmount(double amount)
    {
        discountAmount = amount;
    }

    //chuyển đổi pttt
    static std::string paymentMethodToString(PaymentMethod method)
    {
        switch (method)
        {
        case PaymentMethod::CreditCard:
            return "Credit Card";
        case PaymentMethod::EWallet:
            return "E-Wallet";
        case PaymentMethod::BankTransfer:
            return "Bank Transfer";
        default:
            return "Cash";
        }
    }

    static PaymentMethod paymentMethodFromString(const std::string& method)
    {
        if (method == "Credit Card")
            return PaymentMethod::CreditCard;
        if (method == "E-Wallet")
            return PaymentMethod::EWallet;
        if (method == "Bank Transfer")
            return PaymentMethod::BankTransfer;

        return PaymentMethod::Cash;
    }
};