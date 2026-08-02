cmake_minimum_required(VERSION 3.16)

project(HotelManagement VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Charts)

set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/views
)

set(PROJECT_SOURCES
    src/main.cpp
    src/database/DatabaseManager.cpp

    src/patterns/RoomFactory.cpp
    src/patterns/PricingContext.cpp
    src/patterns/MemberDiscount.cpp
    src/patterns/SeasonalDiscount.cpp
    src/patterns/NoDiscount.cpp

    src/controllers/CustomerController.cpp
    src/controllers/ReceptionistController.cpp
    src/controllers/RoomController.cpp
    src/controllers/BookingController.cpp
    src/controllers/InvoiceController.cpp
    src/controllers/ReportController.cpp

    src/repositories/CustomerRepository.cpp
    src/repositories/ReceptionistRepository.cpp
    src/repositories/RoomRepository.cpp
    src/repositories/BookingRepository.cpp
    src/repositories/InvoiceRepository.cpp

    src/services/BookingService.cpp
    src/services/ReportService.cpp
    src/services/InvoiceService.cpp

    views/BookingView.cpp
    views/CustomerView.cpp
    views/MainWindow.cpp
    views/RoomView.cpp
    views/ReportView.cpp 
    views/ReceptionistView.cpp 
    views/InvoiceView.cpp 
    views/InvoiceDetailDialog.cpp
    views/OccupancyRing.cpp

    views/DashboardCard.h 
)

add_executable(HotelManagement ${PROJECT_SOURCES}
    resources/resources.qrc
    views/InvoiceDetailDialog.h
)
target_include_directories(HotelManagement PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(HotelManagement PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql Qt6::Charts)
target_compile_definitions(HotelManagement PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
