#pragma once

#include "models/Invoice.h"

#include <optional>
#include <QString>
#include <vector>

struct TopCustomer {
    QString id;
    QString name;
    double totalSpending;
};
struct ReceptionistKPI {
    QString id;
    QString name;
    double totalRevenue;
};

class InvoiceRepository {
public:
    bool add(const Invoice& invoice);
    std::vector<Invoice> findAll();
    std::vector<Invoice> search(const QString& keyword);
    std::optional<Invoice> findById(const QString& id);
    double totalRevenue();
    std::vector<double> getMonthlyRevenue(const QString& year);
    double getTotalRevenueByRoomType(const QString& roomType);
    std::vector<ReceptionistKPI> getRevenueByReceptionist();
    std::vector<TopCustomer> getTop5Customers();
    QString lastError() const { return lastErrorMessage; }

private:
    QString lastErrorMessage;
};

