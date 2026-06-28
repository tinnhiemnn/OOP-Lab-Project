#pragma once

#include <string>
#include <utility>

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
    std::string issuedDate;

    double totalAmount = 0.0;

    PaymentMethod paymentMethod = PaymentMethod::Cash;

    bool paid = false;

public:
    Invoice() = default;
    Invoice(std::string id, std::string bookingId, std::string issuedDate, double totalAmount = 0, PaymentMethod paymentMethod = PaymentMethod::Cash, bool paid = false)
        : id(std::move(id)),
          bookingId(std::move(bookingId)),
          issuedDate(std::move(issuedDate)),
          totalAmount(totalAmount),
          paymentMethod(paymentMethod),
          paid(paid) {}

    const std::string& getId() const { return id; }
    const std::string& getBookingId() const { return bookingId; }
    const std::string& getIssuedDate() const { return issuedDate; }
    double getTotalAmount() const { return totalAmount; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    bool isPaid() const { return paid; }

    void setTotalAmount(double amount)
    {
        totalAmount = amount;
    }

    void setPaymentMethod(PaymentMethod method)
    {
        paymentMethod = method;
    }

    //methods
    void generateInvoice(double amount)
    {
        totalAmount = amount;
    }

    void pay()
    {
        paid = true;
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