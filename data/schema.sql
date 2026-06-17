CREATE TABLE rooms (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    room_type TEXT NOT NULL, -- "Standard", "Deluxe", "President"
    base_price INTEGER
);

CREATE TABLE customers (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    phone TEXT,
);

CREATE TABLE receptionists (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    phone TEXT,
);

CREATE TABLE bookings (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    customer_id INTEGER,
    receptionist_id INTEGER,
    room_id INTEGER,         
    check_in_date TEXT,      -- Format YYYY-MM-DD
    check_out_date TEXT,
    total_price REAL,
    status TEXT,
    FOREIGN KEY(customer_id) REFERENCES customers(id),
    FOREIGN KEY(receptionist_id) REFERENCES receptionists(id),
    FOREIGN KEY(room_id) REFERENCES rooms(id)
);