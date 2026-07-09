#pragma once

#include "repositories/BookingRepository.h"
#include "repositories/InvoiceRepository.h"
#include "repositories/RoomRepository.h"

#include <QLocale>

struct OverallReport {
    QString totalRevenue;
    QString roomOccupancyRate;
};
struct RoomTypeReport {
    QString roomType;
    double revenue = 0.0;
    double revenuePercentage = 0.0;
    
    int totalBookings = 0;
    int cancelledBookings = 0;
    double cancellationRate = 0.0;
    double successRate = 0.0;   
}


class ReportService {
public:
    ReportService(InvoiceRepository& invoices, BookingRepository& bookings, RoomRepository& rooms);
    OverallReport getOverallReport();
    std::vector<double> getRevenueByYear(const QString& year);
    std::vector<RoomTypeReport> getRoomTypeReport();
    std::vector<ReceptionistKPI> getReceptionistReport();
    std::vector<TopCustomer> getTop5CustomersReport();
    
private:
    InvoiceRepository& invoices;
    BookingRepository& bookings;
    RoomRepository& rooms;
};

