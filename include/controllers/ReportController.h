#pragma once

#include "services/ReportService.h"

class ReportController {
public:
    ReportController();
    OverallReport getOverallReport();
    std::vector<double> getRevenueByYear(const QString& year);
    std::vector<RoomTypeReport> getRoomTypeReport();
    std::vector<ReceptionistKPI> getReceptionistReport();
    std::vector<TopCustomer> getTop5CustomersReport();

private:
    InvoiceRepository invoices;
    BookingRepository bookings;
    RoomRepository rooms;
    ReportService reportService;
};

