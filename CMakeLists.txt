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

    src/controllers/LoginController.cpp
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

    views/LoginView.cpp
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

add_executable(UtilsTests tests/utils_tests.cpp)
target_include_directories(UtilsTests PRIVATE ${PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(UtilsTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(UtilsTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(ProgramTests
    tests/all_program_tests.cpp
    tests/database_tests.cpp
    tests/booking_service_tests.cpp
    tests/controller_tests.cpp
    src/database/DatabaseManager.cpp
    src/repositories/BookingRepository.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/ReceptionistRepository.cpp
    src/repositories/RoomRepository.cpp
    src/services/BookingService.cpp
    src/controllers/BookingController.cpp
    src/controllers/CustomerController.cpp
    src/patterns/RoomFactory.cpp
)
target_include_directories(ProgramTests PRIVATE ${PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(ProgramTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(ProgramTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(DatabaseTests
    tests/database_tests_runner.cpp
    tests/database_tests.cpp
    src/database/DatabaseManager.cpp
    src/repositories/BookingRepository.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/ReceptionistRepository.cpp
    src/repositories/RoomRepository.cpp
    src/services/BookingService.cpp
    src/patterns/RoomFactory.cpp
)
target_include_directories(DatabaseTests PRIVATE ${PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(DatabaseTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(DatabaseTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(RepositoryTests
    tests/repository_tests.cpp
    src/database/DatabaseManager.cpp
    src/repositories/BookingRepository.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/ReceptionistRepository.cpp
    src/repositories/RoomRepository.cpp
    src/services/BookingService.cpp
    src/controllers/BookingController.cpp
    src/controllers/CustomerController.cpp
    src/controllers/RoomController.cpp
    src/patterns/RoomFactory.cpp
)
target_include_directories(RepositoryTests PRIVATE ${PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(RepositoryTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(RepositoryTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" REPOSITORY_TESTS_STANDALONE)

add_executable(RoomFactoryTests
    tests/utils_tests.cpp
    src/patterns/RoomFactory.cpp
)
target_include_directories(RoomFactoryTests PRIVATE ${PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(RoomFactoryTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(RoomFactoryTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(ControllerTests
    tests/controller_tests.cpp
    src/database/DatabaseManager.cpp
    src/repositories/BookingRepository.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/ReceptionistRepository.cpp
    src/repositories/RoomRepository.cpp
    src/services/BookingService.cpp
    src/controllers/BookingController.cpp
    src/controllers/CustomerController.cpp
    src/patterns/RoomFactory.cpp
)
target_include_directories(ControllerTests PRIVATE ${PROJECT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(ControllerTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(ControllerTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" CONTROLLER_TESTS_STANDALONE)
