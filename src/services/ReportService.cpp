#include "services/ReportService.h"

ReportService::ReportService(InvoiceRepository& invoices, BookingRepository& bookings, RoomRepository& rooms)
    : invoices(invoices), bookings(bookings), rooms(rooms) {}

OverallReport ReportService::getOverallReport() {
    OverallReport report;
    QLocale viLocale(QLocale::Vietnamese, QLocale::Vietnam);
    report.totalRevenue = viLocale.toString(invoices.totalRevenue(), 'f', 0) + " VND";
    
    const auto roomRows = rooms.findAll();
    int totalRooms = static_cast<int>(roomRows.size());
    int inUseRooms = 0;
    for (const auto& room : roomRows) {
        inUseRooms += (room->getStatus == RoomStatus::InUse);
    }
    double occupancyRate = 0.0;
    if (totalRooms > 0) occupancyRate = (inUseRooms * 100.0) / totalRooms;
    report.roomOccupancyRate = viLocale.toString(occupancyRate, 'f', 1) + "%";
    return report;
}

std::vector<double> ReportService::getRevenueByYear(const QString& year) {
    return invoices.getMonthlyRevenue(year);
}

std::vector<RoomTypeReport> ReportService::getRoomTypeReport() {
    std::vector<RoomTypeReport> reports;
    std::vector<RoomType> types = { RoomType::Standard, RoomType::Deluxe, RoomType::President };
    
    double totalRevenue = 0.0;
    for (const auto& type : types) {
        RoomTypeReport item;
        item.roomType = Room::typeToString(type);
        item.revenue = invoices.getTotalRevenueByRoomType(item.roomType);
        totalRevenue += item.revenue;

        item.totalBookings = bookings.countBookings(item.roomType);
        item.cancelledBookings = bookings.countBookings(item.roomType, Booking::statusToString(BookingStatus::Cancelled));
        reports.push_back(item); 
    }
    for (auto& item : reports) {
        item.percentage = (totalRevenue > 0.0 ? (item.revenue * 100.0) / totalRevenue : 0.0);

        if (item.totalBookings > 0) {
            item.cancellationRate = (item.cancelledBookings * 100.0) / item.totalBookings;
            item.successRate = 100.0 - item.cancellationRate;
        } else {
            item.cancellationRate = 0.0;
            item.successRate = 0.0;
        }
    }
    return reports;
}

std::vector<ReceptionistKPI> ReportService::getReceptionistReport() {
    return invoices.getRevenueByReceptionist();
}

std::vector<TopCustomer> ReportService::getTop5CustomersReport() {
    return invoices.getTop5Customers();
}

