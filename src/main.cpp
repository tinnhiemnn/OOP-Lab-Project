#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <optional>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QSqlQuery>

// Include Database Manager and the two repositories to be tested
#include "database/DatabaseManager.h"
#include "repositories/BookingRepository.h"
#include "repositories/InvoiceRepository.h"

// System Models
#include "models/Booking.h"
#include "models/Invoice.h"

void printHeader(const std::string& title) {
    std::cout << "\n==================================================\n";
    std::cout << " ▶️ TESTING REPOSITORY: " << title << "\n";
    std::cout << "==================================================\n";
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Set up Vietnamese locale for formatting currency nicely in the terminal if needed
    QLocale viLocale(QLocale::Vietnamese, QLocale::Vietnam);

    // 1. Initialize Database Connection
    printHeader("DATABASE CONNECTION");

    if (!DatabaseManager::getInstance().openDatabase()) {
        std::cerr << "🚨 Connection failed! Aborting all test scenarios.\n";
        return -1;
    }
    std::cout << "✅ Database connection established successfully!\n";

    // 2. Database Seeding (Seed Data based on Schema)
    printHeader("DATABASE SEEDING (SCHEMA STANDARDIZED)");
    QSqlQuery seedQuery(DatabaseManager::getInstance().database()); 

    // Clear old data in the correct order (Child tables first, parent tables last to avoid FK constraints)
    seedQuery.exec("DELETE FROM invoices;");
    seedQuery.exec("DELETE FROM bookings;");
    seedQuery.exec("DELETE FROM rooms;");
    seedQuery.exec("DELETE FROM customers;");
    seedQuery.exec("DELETE FROM receptionists;");

    std::cout << "🧹 Cleaned up all existing table records.\n";

    // Populate 'rooms' table (id, base_price, status, type, beds)
    seedQuery.exec("INSERT INTO rooms (id, base_price, status, type, beds) VALUES "
                "('RM101', 500000.0, 'Available', 'Standard', 1),"
                "('RM102', 600000.0, 'Occupied', 'Standard', 2),"
                "('RM201', 1200000.0, 'Available', 'Deluxe', 2),"
                "('RM301', 3500000.0, 'Occupied', 'President', 1);");

    // Populate 'receptionists' table (id, name, email)
    seedQuery.exec("INSERT INTO receptionists (id, name, email) VALUES "
                "('REC01', 'Nguyen Le Tan A', 'a@hotel.com'),"
                "('REC02', 'Tran Letan B', 'b@hotel.com');");

    // Populate 'customers' table (id, name, phone, email)
    seedQuery.exec("INSERT INTO customers (id, name, phone, email) VALUES "
                "('C001', 'Khach Hang VIP A', '0901111111', 'a@gmail.com'),"
                "('C002', 'Khach Hang VIP B', '0902222222', 'b@gmail.com'),"
                "('C003', 'Nguyen Van C', '0903333333', 'c@gmail.com'),"
                "('C004', 'Le Thi D', '0904444444', 'd@gmail.com'),"
                "('C005', 'Hoang Nhat E', '0905555555', 'e@gmail.com');");

    // Populate 'bookings' table
    seedQuery.exec("INSERT INTO bookings (id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, num_buffet, laundry_service, decor_service, decor_note, status) VALUES "
                "('B01', 'C001', 'REC01', 'RM301', 'G01', '2026-01-10', '2026-01-15', 2, 1, 1, 'Trang tri sinh nhat', 'CheckedIn'),"
                "('B02', 'C002', 'REC01', 'RM201', 'G01', '2026-03-05', '2026-03-10', 0, 0, 0, NULL, 'CheckedIn'),"
                "('B03', 'C001', 'REC02', 'RM101', 'G02', '2026-03-12', '2026-03-15', 1, 0, 0, NULL, 'CheckedIn'),"
                "('B04', 'C003', 'REC02', 'RM102', 'G02', '2026-05-20', '2026-05-25', 4, 1, 0, NULL, 'CheckedIn'),"
                "('B05', 'C004', 'REC01', 'RM301', 'G03', '2026-07-01', '2026-07-05', 2, 0, 1, 'Kỷ niệm ngày cưới', 'CheckedIn'),"
                "('B06', 'C005', 'REC02', 'RM101', 'G03', '2026-07-08', '2026-07-10', 0, 0, 0, NULL, 'Cancelled');");

    // Populate 'invoices' table
    seedQuery.exec("INSERT INTO invoices (id, booking_id, receptionist_id, issued_date, payment_method, discount_name, subtotal_amount, discount_amount, total_amount) VALUES "
                "('INV01', 'B01', 'REC01', '2026-01-15', 'Cash', 'None', 50000000.0, 0.0, 50000000.0),"
                "('INV02', 'B02', 'REC01', '2026-03-10', 'Credit Card', 'None', 25000000.0, 0.0, 25000000.0),"
                "('INV03', 'B03', 'REC02', '2026-03-15', 'Banking', 'MEMBER10', 11111111.0, 1111111.0, 10000000.0),"
                "('INV04', 'B04', 'REC02', '2026-05-25', 'Cash', 'None', 8000000.0, 0.0, 8000000.0),"
                "('INV05', 'B05', 'REC01', '2026-07-05', 'Banking', 'SUMMERVIBE', 40000000.0, 5000000.0, 35000000.0);");

    std::cout << "🌱 Seed data successfully injected into hotel.db!\n";
    std::cout << "--------------------------------------------------\n";

    // Instantiate Repositories
    BookingRepository bookingRepo;
    InvoiceRepository invoiceRepo;

    // =========================================================================
    // BLOCK 1: TESTING BOOKING REPOSITORY (CRUD & BASIC METRICS)
    // =========================================================================
    printHeader("BOOKING REPOSITORY - CRUD & COUNT");

    QDate checkIn = QDate::fromString("2026-07-12", "yyyy-MM-dd");
    QDate checkOut = QDate::fromString("2026-07-15", "yyyy-MM-dd");
    
    Booking b1("B_TEST_01", "C001", "REC01", "RM101", "GRP01", checkIn, checkOut, BookingStatus::Booked);

    // Test 1.1: Add New Booking
    if (bookingRepo.add(b1)) {
        std::cout << "✅ Add Booking [B_TEST_01] succeeded.\n";
    } else {
        std::cerr << "❌ Add Booking failed: " << bookingRepo.lastError().toStdString() << "\n";
    }

    // Test 1.2: Find by ID and Update Status
    auto optBooking = bookingRepo.findById("B_TEST_01");
    if (optBooking.has_value()) {
        std::cout << "🔍 Found Booking B_TEST_01. Updating status to 'CheckedIn'...\n";
        
        optBooking->setStatus(Booking::statusFromString("CheckedIn")); 
        
        if (bookingRepo.update(*optBooking)) {
            std::cout << "✅ Update Booking status succeeded.\n";
        } else {
            std::cerr << "❌ Update failed: " << bookingRepo.lastError().toStdString() << "\n";
        }
    } else {
        std::cout << "⚠️ Booking ID [B_TEST_01] not found for Update test.\n";
    }

    // Test 1.3: Retrieve All Records (findAll)
    std::cout << "\n📋 Comprehensive Booking List (findAll):\n";
    auto allBookings = bookingRepo.findAll();
    for (const auto& b : allBookings) {
        std::cout << "   - ID: " << b.getId().toStdString() 
                  << " | Room: " << b.getRoomId().toStdString() 
                  << " | Status: " << Booking::statusToString(b.getStatus()).toStdString() << "\n";
    }

    // Test 1.4: Count Bookings by Room Type and Status
    QString testRoomType = "President";
    QString testStatus = "Booked";
    int count = bookingRepo.countBookings(testRoomType, testStatus);
    std::cout << "📊 Booking count for room type [" << testRoomType.toStdString() 
              << "] with status [" << testStatus.toStdString() << "]: " << count << "\n";


    // =========================================================================
    // BLOCK 2: TESTING INVOICE REPOSITORY (FINANCIAL STATS & REPORTS)
    // =========================================================================
    printHeader("INVOICE REPOSITORY - REVENUE REPORTS");

    Invoice inv1("INV_TEST_01", "B_TEST_01", "REC01", QDate::fromString("2026-07-15", "yyyy-MM-dd"), 4500000.0, 4000000, 500000, PaymentMethod::Cash, "MemberDiscount");

    // Test 2.1: Add New Invoice
    if (invoiceRepo.add(inv1)) {
        std::cout << "✅ Add Invoice [INV_TEST_01] succeeded.\n";
    } else {
        std::cerr << "❌ Add Invoice failed: " << invoiceRepo.lastError().toStdString() << "\n";
    }

    // Test 2.2: Total Accumulated System Revenue
    double totalRev = invoiceRepo.totalRevenue();
    std::cout << "💰 TOTAL SYSTEM REVENUE: " 
              << viLocale.toString(totalRev, 'f', 0).toStdString() << " VND\n";

    // Test 2.3: Monthly Revenue Breakdown
    int targetYear = 2026;
    std::cout << "\n📈 Monthly Revenue Breakdown for Year " << targetYear << ":\n";
    std::vector<double> monthlyData = invoiceRepo.getMonthlyRevenue(QString::number(targetYear));
    for (size_t i = 0; i < monthlyData.size(); ++i) {
        std::cout << "   - Month " << (i + 1) << ": " 
                  << viLocale.toString(monthlyData[i], 'f', 0).toStdString() << " VND\n";
    }

    // Test 2.4: Revenue by Specific Room Type
    QString roomTypeCheck = "Standard";
    double roomRev = invoiceRepo.getTotalRevenueByRoomType(roomTypeCheck);
    std::cout << "\n🏨 Revenue segment for room type [" << roomTypeCheck.toStdString() << "]: "
              << viLocale.toString(roomRev, 'f', 0).toStdString() << " VND\n";

    // Test 2.5: Receptionist Performance Ranking (KPI)
    std::cout << "\n👔 Receptionist Revenue Generation Ranking (KPI):\n";
    auto recRevenueList = invoiceRepo.getRevenueByReceptionist();
    if (recRevenueList.empty()) std::cout << invoiceRepo.lastError().toStdString() << '\n';
    for (const auto& rec : recRevenueList) {
        std::cout << "   - Receptionist ID: " << rec.id.toStdString() << " | Name: " << rec.name.toStdString()
                  << " ➔ Generated: " << viLocale.toString(rec.totalRevenue, 'f', 0).toStdString() << " VND\n";
    }

    // Test 2.6: Top 5 Highest Spending Customers for Dashboard
    std::cout << "\n👑 TOP 5 HIGHEST SPENDING CUSTOMERS:\n";
    auto topCustomers = invoiceRepo.getTop5Customers();
    int rank = 1;
    for (const auto& cust : topCustomers) {
        std::cout << "   " << rank++ << ". ID: " << cust.id.toStdString()
                  << " | Name: " << cust.name.toStdString()
                  << " | Total Spent: " << viLocale.toString(cust.totalSpending, 'f', 0).toStdString() << " VND\n";
    }

    // =========================================================================
    // RESOURCE CLEANUP ON EXIT
    // =========================================================================
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "🎉 Repository test pipeline execution completed successfully!\n";
    
    DatabaseManager::getInstance().closeConnection();
    std::cout << "\nNhan Enter de thoat...";
    std::cin.get();
    return 0;
}