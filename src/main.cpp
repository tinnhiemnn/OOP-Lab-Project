#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <optional>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QSqlQuery>
#include <QSqlError>

// Include Database Manager and Repositories
#include "database/DatabaseManager.h"
#include "repositories/CustomerRepository.h"
#include "repositories/ReceptionistRepository.h"

// System Models
#include "models/Customer.h"
#include "models/Receptionist.h"

void printHeader(const std::string& title) {
    std::cout << "\n==================================================\n";
    std::cout << " ▶️ TESTING FEATURES: " << title << "\n";
    std::cout << "==================================================\n";
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // 1. Initialize Database Connection
    printHeader("DATABASE CONNECTION");

    if (!DatabaseManager::getInstance().openDatabase()) {
        std::cerr << "🚨 Connection failed! Aborting all test scenarios.\n";
        return -1;
    }
    std::cout << "✅ Database connection established successfully!\n";

    // 2. Clear existing data to ensure deterministic ID generation numbers
    printHeader("CLEANING UP TEST ENVIRONMENT");
    QSqlQuery clearQuery(DatabaseManager::getInstance().database());
    clearQuery.exec("DELETE FROM customers;");
    clearQuery.exec("DELETE FROM receptionists;");
    std::cout << "🧹 Cleaned up existing customer and receptionist records.\n";

    // Instantiate Repositories
    CustomerRepository customerRepo;
    ReceptionistRepository recRepo;

    // =========================================================================
    // BLOCK 1: TESTING CUSTOMER REPOSITORY (ID GENERATION & SOFT DELETE)
    // =========================================================================
    printHeader("CUSTOMER REPOSITORY - AUTO ID & ANONYMIZATION");

    // Test 1.1: Verify sequential ID Generation on empty database
    QString nextCustId1 = customerRepo.generateNextId();
    std::cout << "🆕 Next generated Customer ID (Empty DB): " << nextCustId1.toStdString() << " (Expected: CUS001)\n";

    // Create and Add Customer 1
    Customer c1(nextCustId1, "John Doe", "john.doe@gmail.com", "0901234567");
    if (customerRepo.add(c1)) {
        std::cout << "✅ Successfully added Customer: " << c1.getName().toStdString() << "\n";
    }

    // Test 1.2: Verify sequential ID Generation incrementing correctly
    QString nextCustId2 = customerRepo.generateNextId();
    std::cout << "🆕 Next generated Customer ID (After 1 insertion): " << nextCustId2.toStdString() << " (Expected: CUS002)\n";

    // Create and Add Customer 2
    Customer c2(nextCustId2, "Alice Smith", "alice@gmail.com", "0987654321");
    customerRepo.add(c2);

    // Test 1.3: Execute Soft Delete with Privacy Anonymization
    std::cout << "\n🔒 Executing Privacy Soft Delete on [CUS001] (John Doe)...\n";
    if (customerRepo.remove("CUS001")) {
        std::cout << "✅ Soft delete command executed successfully.\n";
    } else {
        std::cerr << "❌ Soft delete execution failed: " << customerRepo.lastError().toStdString() << "\n";
    }

    // Test 1.4: Verify the data state in the database after Soft Delete
    auto optCust = customerRepo.findById("CUS001");
    if (optCust.has_value()) {
        std::cout << "🔍 Inspecting DB record for [CUS001] post-deletion:\n";
        std::cout << "   - ID:     " << optCust->getId().toStdString() << "\n";
        std::cout << "   - Name:   " << optCust->getName().toStdString() << " (Expected: Deleted Customer)\n";
        std::cout << "   - Phone:  " << optCust->getPhone().toStdString() << " (Expected: N/A)\n";
        std::cout << "   - Email:  " << optCust->getEmail().toStdString() << " (Expected: N/A)\n";
    }
    else std::cout << "No info of CUS001\n";


    // =========================================================================
    // BLOCK 2: TESTING RECEPTIONIST REPOSITORY (ID GENERATION & SOFT DELETE)
    // =========================================================================
    printHeader("RECEPTIONIST REPOSITORY - AUTO ID & ANONYMIZATION");

    // Test 2.1: Verify sequential ID Generation for Receptionist
    QString nextRecId1 = recRepo.generateNextId();
    std::cout << "🆕 Next generated Receptionist ID (Empty DB): " << nextRecId1.toStdString() << " (Expected: REC001)\n";

    // Create and Add Receptionist 1
    Receptionist r1(nextRecId1, "Staff Member A", "staff.a@hotel.com");
    if (recRepo.add(r1)) {
        std::cout << "✅ Successfully added Receptionist: " << r1.getName().toStdString() << "\n";
    }

    // Test 2.2: Verify sequential ID Generation increment
    QString nextRecId2 = recRepo.generateNextId();
    std::cout << "🆕 Next generated Receptionist ID (After 1 insertion): " << nextRecId2.toStdString() << " (Expected: REC002)\n";

    // Test 2.3: Execute Soft Delete with Privacy Anonymization on Receptionist
    std::cout << "\n🔒 Executing Privacy Soft Delete on [REC001]...\n";
    if (recRepo.remove("REC001")) {
        std::cout << "✅ Soft delete command executed successfully.\n";
    }

    // Test 2.4: Verify the data state post-deletion
    auto optRec = recRepo.findById("REC001");
    if (optRec.has_value()) {
        std::cout << "🔍 Inspecting DB record for [REC001] post-deletion:\n";
        std::cout << "   - ID:     " << optRec->getId().toStdString() << "\n";
        std::cout << "   - Name:   " << optRec->getName().toStdString() << " (Expected: Deleted Staff)\n";
        std::cout << "   - Email:  " << optRec->getEmail().toStdString() << " (Expected: N/A)\n";
    }
    else std::cout << "No info of REC001\n";

    // =========================================================================
    // RESOURCE CLEANUP ON EXIT
    // =========================================================================
    std::cout << "\n--------------------------------------------------\n";
    std::cout << "🎉 Auto-ID and Soft-Delete test pipeline completed successfully!\n";
    
    DatabaseManager::getInstance().closeConnection();
    return 0;
}