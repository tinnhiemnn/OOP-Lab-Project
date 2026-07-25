#include "controllers/ReportController.h"

ReportController::ReportController()
    : reportService(invoices, bookings, rooms) {}

OverallReport ReportController::getOverallReport() {
    return reportService.getOverallReport();
}

std::vector<double> ReportController::getRevenueByYear(const QString& year) {
    return reportService.getRevenueByYear(year);
}

std::vector<RoomTypeReport> ReportController::getRoomTypeReport() {
    return reportService.getRoomTypeReport();
}

std::vector<ReceptionistKPI> ReportController::getReceptionistReport(){
    return reportService.getReceptionistReport();
}

std::vector<TopCustomer> ReportController::getTop5CustomersReport() {
    return reportService.getTop5CustomersReport();
}