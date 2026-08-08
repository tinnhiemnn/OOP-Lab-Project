# 🏨 Hotel Management System (Management System Application)

A desktop-based Hotel Management System designed for hotel reception staff and managers. Built with **C++17**, **Qt6 Widgets**, and **SQLite**, the application streamlines customer profile management, room availability tracking, multi-room group bookings, dynamic add-on services, invoicing with discount strategies, and real-time operational analytics.


## 📖 Overview

The **Hotel Management System** provides an all-in-one desktop interface for front-desk operations and operational reporting. It solves common workflow bottlenecks in hotel management by enforcing atomic multi-room reservations, strict lifecycle state tracking, group check-in/check-out execution, dynamic discount application, and real-time dashboard visualization.

### Target Users

* **Hotel Receptionists:** Manage day-to-day reservations, guest check-ins/check-outs, add-on services, and invoice printing.


* **Hotel Managers:** Monitor real-time hotel metrics, overall occupancy rates, monthly revenue trends, staff performance (KPIs), and top customer analytics.


## ✨ Key Features

* Authentication & Role-Based Access Control (RBAC)

* Dynamic Room Management & Polymorphic Pricing

* Booking Lifecycle & Multi-Room Group Operations

* Add-On Services

* Invoicing & Dynamic Discount Strategies

* Executive Analytics Dashboard



## 🛠️ Tech Stack & Dependencies

* **Language:** C++17


* **GUI Framework:** Qt6 Widgets (styled with a custom external `.qss` stylesheet)


* **Database Engine:** SQLite3 (managed via Qt SQL module)


* **Build Tool:** CMake (>= 3.16)


* **Architecture:** Model-View-Controller (MVC) Pattern




## 📁 Project Directory Structure

```text
├── data/                       # Database schema and local SQLite storage
│   ├── hotel.db                # SQLite database file
│   └── schema.sql              # Database initialization DDL script
├── docs/                       # Project documentation
├── include/                    # Header files (.h)
│   ├── controllers/            # Controller layer declarations
│   ├── database/               # Database manager & connection headers
│   ├── models/                 # Domain model entities (Room, Booking, etc.)
│   ├── patterns/               # Design pattern interfaces (Factory, Strategy)
│   ├── repositories/           # Data access repository declarations
│   ├── services/               # Core business services logic
│   ├── utils/                  # Helper utilities (DateUtils, ValidationUtils)
├── resources/                  # Project visual resources (icons, images, fonts)
├── src/                        # Source implementation files (.cpp)
│   ├── controllers/            # Controllers logic & input routing
│   ├── database/               # DatabaseSingleton & SQLite connection setup
│   ├── models/                 # Polymorphic pricing logic implementations
│   ├── patterns/               # Concrete factory & strategy implementations
│   ├── repositories/           # SQLite database persistence logic
│   ├── services/               # Workflows (Booking, Invoice, Report)
│   ├── main.cpp                # Application entry point (initializes Qt App & main window)
├── tests/                      # Automated unit and integration test executables
├── views/                      # Qt GUI UI components, custom widgets, & event handlers
├── CMakeLists.txt              # Root build system definition script
└── README.md                   

```



## ⚙️ Build & Installation Instructions

### Prerequisites

Before building the project, ensure you have installed:

* **C++17 Compatible Compiler** (MinGW-w64 GCC 8+, Clang 7+, or MSVC 2019+)


* **CMake** v3.16 or higher


* **Qt6 Framework** (Modules required: `Core`, `Gui`, `Widgets`, `Sql`, `Charts`)



### Compilation Steps

1. **Clone the Repository:**
```bash
git clone https://github.com/tinnhiemnn/OOP-Lab-Project.git
cd OOP-Lab-Project

```


2. **Configure CMake:**
*(Replace `/path/to/Qt6` with your local Qt installation path if not set in environment variables)*
```bash
cmake -B build -S . -DCMAKE_PREFIX_PATH="/path/to/Qt/6.x.x/gcc_64" -DBUILD_TESTS=OFF

```


3. **Build the Project:**
```bash
cmake --build build --config Release

```


4. **Run the Application:**
```bash
# On Linux / macOS
./build/HotelManagement

# On Windows
.\build\HotelManagement.exe

```


## 📖 User Guide

### 1. System Authentication
* **Default Admin Credentials:** Username: `admin` | Password: `admin123`
* **Role-Based Access Control (RBAC):**
  * **Admin:** Full access to all application tabs (Report, Room, Customer, Booking, Receptionist, Invoice).
  * **Receptionist:** Restricted access tailored to daily operations (Booking, Room, Customer, Invoice).

### 2. Booking Workflow
* **Creating a Booking (Single or Multi-Room):**
  1. Navigate to the **Booking** tab.
  2. Input or select the **Customer ID** (supports autocomplete search by name or phone number) and **Receptionist ID**.
  3. Select **Check-in** and **Check-out** dates.
  4. Select one or multiple rooms and check desired add-on services (Buffet Breakfast, Laundry, Room Decoration).
  5. Click submit. The system automatically assigns a shared `group_code` for batch reservations.

* **Check-In & Check-Out Execution:**
  * **Group Check-In:** Select any reservation within a group and click **Check-in**. All room reservations sharing the same `group_code` in `Booked` status transition to `CheckedIn`, and room statuses update to `InUse`.
  * **Group Check-Out:** Select a reservation and click **Check-out**. All room reservations in the group transition to `CheckedOut`, and the UI automatically switches to the **Invoice** tab pre-loaded with the group code.

### 3. Invoicing Workflow
1. Select a booking with `CheckedOut` status.
2. Choose the applicable discount policy (No Discount, Member Discount 5%, Seasonal Discount 10%).
3. Click **Create Single Invoice** (for one room) or **Create Group Invoice** (for all unpaid rooms sharing the same `group_code`).

### 4. Manager Dashboard & Analytics
* Access real-time business reports and charts:
  * **Revenue Chart:** Track monthly revenue trends with year filtering (2025/2026).
  * **Interactive Hover Tooltips:** Hover over chart data points or bars to view exact monetary values.
  * **Key Metrics:** View occupancy rates, receptionist KPI revenue performance, and top 5 spending customers.



## 📜 License & Copyright

### Copyright
Copyright © 2026 **Group 02 - FIT VNUHCM-US**. All rights reserved.

This project was developed as part of the **Object-Oriented Programming** course at the Faculty of Information Technology, VNUHCM - University of Science.

### Team Members
* **Authors:** [Huynh Minh Trung](https://github.com/mintrun-study), [Nguyen Tran Kim Cuong](https://github.com/KimCuongg), [Nguyen Mai Huong Binh](https://github.com/BynhNguyen), [Trang Tin Nhiem](https://github.com/tinnhiemnn), [Huynh Thanh An](https://github.com/htan2612).
* **Instructors:** M.Sc. Truong Tan Khoa & B.Sc. Vo Nhat Tan.

