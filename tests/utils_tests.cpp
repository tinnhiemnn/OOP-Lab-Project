#include "utils/DateUtils.h"
#include "utils/ValidationUtils.h"
#include <iostream>
#include <cassert>
#include <QString>

void testDateUtils() {
    //1. isValidDate
    assert(DateUtils::isValidDate("2026-06-28") == true);
    assert(DateUtils::isValidDate("2026-02-29") == false); // non leap
    assert(DateUtils::isValidDate("2024-02-29") == true);  // leap
    assert(DateUtils::isValidDate("2100-02-29") == false);
    assert(DateUtils::isValidDate("2400-02-29") == true); 
    assert(DateUtils::isValidDate("invalid-date") == false);
    assert(DateUtils::isValidDate("2026-13-01") == false);
    assert(DateUtils::isValidDate("2026-00-01") == false);
    assert(DateUtils::isValidDate("2026-12-32") == false);

    //2. isDateRangeValid
    assert(DateUtils::isDateRangeValid("2026-06-28", "2026-06-29") == true);
    assert(DateUtils::isDateRangeValid("2026-06-29", "2026-06-28") == false);
    assert(DateUtils::isDateRangeValid("2026-06-28", "2026-06-28") == false);

    //3. daysBetween
    assert(DateUtils::daysBetween("2026-06-28", "2026-06-29") == 1);
    assert(DateUtils::daysBetween("2026-06-01", "2026-06-11") == 10);
    assert(DateUtils::daysBetween("2026-06-29", "2026-06-28") == 0); //return 0

    //4. datesOverlap
    assert(DateUtils::datesOverlap("2026-06-10", "2026-06-12", "2026-06-11", "2026-06-13") == true);
    assert(DateUtils::datesOverlap("2026-06-10", "2026-06-12", "2026-06-12", "2026-06-13") == false); // Adjacent
    assert(DateUtils::datesOverlap("2026-06-10", "2026-06-12", "2026-06-13", "2026-06-15") == false); // Separate

    std::cout << "DateUtils tests passed!" << std::endl;
}

void testValidationUtils() {
    //1. isNonEmpty
    assert(ValidationUtils::isNonEmpty("abc") == true);
    assert(ValidationUtils::isNonEmpty("   ") == false);
    assert(ValidationUtils::isNonEmpty("") == false);

    //2. isValidEmail
    assert(ValidationUtils::isValidEmail("test@example.com") == true);
    assert(ValidationUtils::isValidEmail("test.example@com") == false);
    assert(ValidationUtils::isValidEmail("@example.com") == false);
    assert(ValidationUtils::isValidEmail("test@.com") == false);

    //3. isValidPhone
    assert(ValidationUtils::isValidPhone("0987654321") == true);
    assert(ValidationUtils::isValidPhone("12345678") == false);
    assert(ValidationUtils::isValidPhone("1234567890123") == false);
    assert(ValidationUtils::isValidPhone("098765abcd") == false);

    //4. isPositiveMoney
    assert(ValidationUtils::isPositiveMoney(100.5) == true);
    assert(ValidationUtils::isPositiveMoney(0.0) == false);
    assert(ValidationUtils::isPositiveMoney(-5.0) == false);

    //5. isValidBookingInput
    QString error;
    assert(ValidationUtils::isValidBookingInput("C001", "R101", "2026-06-10", "2026-06-12", error) == true);
    assert(ValidationUtils::isValidBookingInput("", "R101", "2026-06-10", "2026-06-12", error) == false);
    assert(error == "Customer ID is required.");
    assert(ValidationUtils::isValidBookingInput("C001", "", "2026-06-10", "2026-06-12", error) == false);
    assert(error == "Room ID is required.");
    assert(ValidationUtils::isValidBookingInput("C001", "R101", "2026-06-12", "2026-06-10", error) == false);
    assert(error == "Check-out date must be after check-in date.");

    std::cout << "ValidationUtils tests passed!" << std::endl;
}

int main() {
    testDateUtils();
    testValidationUtils();
    std::cout << "All utility tests passed successfully!" << std::endl;
    return 0;
}