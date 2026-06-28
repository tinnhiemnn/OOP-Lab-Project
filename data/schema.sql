CREATE TABLE IF NOT EXISTS rooms (
    id TEXT PRIMARY KEY,
    type TEXT NOT NULL,
    base_price REAL NOT NULL,
    status TEXT NOT NULL,
    beds INTEGER NOT NULL DEFAULT 1
);

CREATE TABLE IF NOT EXISTS customers (
    id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    phone TEXT NOT NULL,
    email TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS receptionists (
    id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    phone TEXT NOT NULL,
    email TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS bookings (
    id TEXT PRIMARY KEY,
    customer_id TEXT,
    receptionist_id TEXT,
    room_id TEXT,
    group_code TEXT NOT NULL,
    check_in TEXT NOT NULL,
    check_out TEXT NOT NULL,
    num_buffet INTEGER NOT NULL DEFAULT 0,
    laundry_service BOOLEAN NOT NULL DEFAULT 0,
    decor_service BOOLEAN NOT NULL DEFAULT 0, 
    decor_note TEXT,
    total_price REAL NOT NULL,
    status TEXT NOT NULL,
    FOREIGN KEY(customer_id) REFERENCES customers(id) ON DELETE SET NULL,
    FOREIGN KEY(receptionist_id) REFERENCES receptionists(id) ON DELETE SET NULL,
    FOREIGN KEY(room_id) REFERENCES rooms(id) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS invoices (
    id TEXT PRIMARY KEY,
    group_code TEXT NOT NULL,
    receptionist_id TEXT,
    issued_date TEXT NOT NULL,
    payment_method TEXT NOT NULL,
    discount_name TEXT NOT NULL,
    total_amount REAL NOT NULL,
    FOREIGN KEY(receptionist_id) REFERENCES receptionists(id) ON DELETE SET NULL,
    FOREIGN KEY(group_code) REFERENCES bookings(group_code)
);