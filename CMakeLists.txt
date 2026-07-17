cmake_minimum_required(VERSION 3.16)

project(HotelManagement VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/views
)

set(PROJECT_SOURCES
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/patterns/RoomFactory.cpp
    src/controllers/CustomerController.cpp
    src/controllers/ReceptionistController.cpp
    src/controllers/RoomController.cpp
    src/controllers/BookingController.cpp
    src/controllers/InvoiceController.cpp
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
)

add_executable(HotelManagement ${PROJECT_SOURCES}
    resources/resources.qrc
)
target_include_directories(HotelManagement PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(HotelManagement PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql)
target_compile_definitions(HotelManagement PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Build target UtilsTests
add_executable(UtilsTests tests/utils_tests.cpp)
target_include_directories(UtilsTests PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(UtilsTests PRIVATE Qt6::Core)
target_compile_definitions(UtilsTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Build target DatabaseTests
add_executable(DatabaseTests 
    tests/database_tests.cpp
    src/database/DatabaseManager.cpp
    src/patterns/RoomFactory.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/RoomRepository.cpp
    src/repositories/BookingRepository.cpp
)
target_include_directories(DatabaseTests PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(DatabaseTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(DatabaseTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Build target RoomFactoryTests
add_executable(RoomFactoryTests tests/room_factory_tests.cpp src/patterns/RoomFactory.cpp)
target_include_directories(RoomFactoryTests PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(RoomFactoryTests PRIVATE Qt6::Core)
target_compile_definitions(RoomFactoryTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Build target ControllerTests
add_executable(ControllerTests 
    tests/controller_tests.cpp
    src/database/DatabaseManager.cpp
    src/repositories/CustomerRepository.cpp
    src/controllers/CustomerController.cpp
)
target_include_directories(ControllerTests PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(ControllerTests PRIVATE Qt6::Core Qt6::Sql)
target_compile_definitions(ControllerTests PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# Enable CTest
enable_testing()
add_test(NAME UtilsTest COMMAND UtilsTests)
add_test(NAME DatabaseTest COMMAND DatabaseTests)
add_test(NAME RoomFactoryTest COMMAND RoomFactoryTests)
add_test(NAME ControllerTest COMMAND ControllerTests)