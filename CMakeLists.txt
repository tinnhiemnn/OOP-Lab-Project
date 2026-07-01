cmake_minimum_required(VERSION 3.16)

project(HotelManagement VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(PROJECT_SOURCES
    src/main.cpp
    src/database/DatabaseManager.cpp
    src/repositories/CustomerRepository.cpp
    src/repositories/ReceptionistRepository.cpp
    src/repositories/RoomRepository.cpp
    src/repositories/BookingRepository.cpp
    
)

add_executable(HotelManagement ${PROJECT_SOURCES})
target_include_directories(HotelManagement PRIVATE ${PROJECT_INCLUDE_DIRS})
target_link_libraries(HotelManagement PRIVATE Qt6::Core Qt6::Widgets Qt6::Sql)
target_compile_definitions(HotelManagement PRIVATE PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
