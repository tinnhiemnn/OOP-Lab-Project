#pragma once

#include <QString>
#include <QDate>

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
    QString id;
    QString bookingId;
    QString receptionistId;
    QDate issuedDate;

    double totalAmount = 0.0;
    double discountAmount = 0.0;

    PaymentMethod paymentMethod = PaymentMethod::Cash;

    QString discountName;

public:
    Invoice() = default;
    
    Invoice(QString id,
            QString bookingId,
            QString receptionistId,
            QDate issuedDate,
            double totalAmount = 0,
            double discountAmount = 0,
            PaymentMethod paymentMethod = PaymentMethod::Cash,
            QString discountName = "")
        : id(std::move(id)),
          bookingId(std::move(bookingId)),
          receptionistId(std::move(receptionistId)),
          issuedDate(issuedDate),
          totalAmount(totalAmount),
          discountAmount(discountAmount),
          paymentMethod(paymentMethod),
          discountName(std::move(discountName)) {}

    const QString& getId() const { return id; }
    const QString& getBookingId() const { return bookingId; }
    const QString& getReceptionistId() const { return receptionistId; }
    QDate getIssuedDate() const { return issuedDate; }
    double getTotalAmount() const { return totalAmount; }
    double getDiscountAmount() const { return discountAmount; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    const QString& getDiscountName() const { return discountName; }

    void setTotalAmount(double amount) { totalAmount = amount; }
    void setPaymentMethod(PaymentMethod method) { paymentMethod = method; }
    void setDiscountName(const QString& value) { discountName = value; }
    void setDiscountAmount(double amount) { discountAmount = amount; }
    void setIssuedDate(QDate date) { issuedDate = date; }

    //chuyen doi PaymentMethod -> QString
    static QString paymentMethodToString(PaymentMethod method)
    {
        switch (method)
        {
        case PaymentMethod::CreditCard:   return "Credit Card";
        case PaymentMethod::EWallet:      return "E-Wallet";
        case PaymentMethod::BankTransfer: return "Bank Transfer";
        default:                          return "Cash";
        }
    }

    //chuyen doi QString -> PaymentMethod
    static PaymentMethod paymentMethodFromString(const QString& method)
    {
        if (method == "Credit Card")   return PaymentMethod::CreditCard;
        if (method == "E-Wallet")      return PaymentMethod::EWallet;
        if (method == "Bank Transfer") return PaymentMethod::BankTransfer;

        return PaymentMethod::Cash;
    }
};