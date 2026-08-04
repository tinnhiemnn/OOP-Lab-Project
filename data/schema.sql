CREATE TABLE IF NOT EXISTS rooms (
    id TEXT PRIMARY KEY,
    base_price REAL NOT NULL,
    status TEXT NOT NULL,
    type TEXT NOT NULL,
    beds INTEGER NOT NULL DEFAULT 1
);

CREATE TABLE IF NOT EXISTS customers (
    id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    phone TEXT NOT NULL,
    email TEXT NOT NULL,
    status TEXT DEFAULT 'Active'
);

CREATE TABLE IF NOT EXISTS receptionists (
    id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    email TEXT NOT NULL,
    password TEXT NOT NULL,
    status TEXT DEFAULT 'Active'
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
    status TEXT NOT NULL,
    FOREIGN KEY(customer_id) REFERENCES customers(id),
    FOREIGN KEY(receptionist_id) REFERENCES receptionists(id),
    FOREIGN KEY(room_id) REFERENCES rooms(id) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS invoices (
    id TEXT PRIMARY KEY,
    booking_id TEXT NOT NULL,
    receptionist_id TEXT,
    issued_date TEXT NOT NULL,
    payment_method TEXT NOT NULL,
    discount_name TEXT NOT NULL,
    subtotal_amount REAL NOT NULL,
    discount_amount REAL NOT NULL,
    total_amount REAL NOT NULL,
    FOREIGN KEY(booking_id) REFERENCES bookings(id),
    FOREIGN KEY(receptionist_id) REFERENCES receptionists(id)
);

INSERT OR IGNORE INTO receptionists (id, name, email, password, status) VALUES
('REC001', 'Nguyễn Thị Thu Hà', 'rec.ha@hotel.com', '123456', 'Active'),
('REC002', 'Trần Văn Hoàng', 'rec.hoang@hotel.com', '123456', 'Active'),
('REC003', 'Lê Mỹ Duyên', 'rec.duyen@hotel.com', '123456', 'Active');

INSERT OR IGNORE INTO rooms (id, base_price, status, type, beds) VALUES
('P101', 500000, 'Available', 'Standard', 1),
('P102', 500000, 'Available', 'Standard', 1),
('P103', 550000, 'NeedCleaning', 'Standard', 2),
('P201', 900000, 'InUse', 'Deluxe', 2),
('P202', 950000, 'Available', 'Deluxe', 2),
('P203', 1000000, 'Maintenance', 'Deluxe', 2),
('P301', 2500000, 'InUse', 'President', 3),
('P302', 3000000, 'Available', 'President', 4);

INSERT OR IGNORE INTO customers (id, name, phone, email, status) VALUES
('CUS001', 'Nguyễn Văn An', '0901234567', 'an.nguyen@gmail.com', 'Active'),
('CUS002', 'Trần Bình', '0912345678', 'binh.tran@gmail.com', 'Active'),
('CUS003', 'Lê Hoàng Cường', '0923456789', 'cuong.le@gmail.com', 'Active'),
('CUS004', 'Phạm Minh Đức', '0934567890', 'duc.pham@gmail.com', 'Active'),
('CUS005', 'Vũ Thị Hoa', '0945678901', 'hoa.vu@gmail.com', 'Active'),
('CUS006', 'Đặng Quốc Khánh', '0956789012', 'khanh.dang@gmail.com', 'Active'),
('CUS007', 'Bùi Thiện Nhân', '0967890123', 'nhan.bui@gmail.com', 'Active'),
('CUS008', 'Đỗ Như Quỳnh', '0978901234', 'quynh.do@gmail.com', 'Active'),
('CUS009', 'Hồ Tấn Tài', '0989012345', 'tai.ho@gmail.com', 'Active'),
('CUS010', 'Ngô Thanh Vân', '0990123456', 'van.ngo@gmail.com', 'Active');

INSERT OR IGNORE INTO bookings (id, customer_id, receptionist_id, room_id, group_code, check_in, check_out, num_buffet, laundry_service, decor_service, decor_note, status) VALUES
('BK_1704067200_1', 'CUS001', 'REC001', 'P101', 'GRP_1704067200_1', '2025-01-08', '2025-01-10', 2, 1, 0, '', 'CheckedOut'),
('BK_1706745600_2', 'CUS002', 'REC002', 'P201', 'GRP_1706745600_2', '2025-02-12', '2025-02-14', 4, 0, 1, 'Valentine', 'CheckedOut'),
('BK_1709424000_3', 'CUS003', 'REC003', 'P301', 'GRP_1709424000_3', '2025-03-18', '2025-03-20', 6, 1, 1, 'Hội nghị', 'CheckedOut'),
('BK_1712102400_4', 'CUS004', 'REC001', 'P102', 'GRP_1712102400_4', '2025-04-12', '2025-04-15', 2, 0, 0, '', 'CheckedOut'),
('BK_1714780800_5', 'CUS005', 'REC002', 'P202', 'GRP_1714780800_5', '2025-04-28', '2025-05-01', 4, 1, 0, 'Lễ 30/4', 'CheckedOut'),
('BK_1717459200_6', 'CUS006', 'REC003', 'P203', 'GRP_1717459200_6', '2025-06-15', '2025-06-18', 2, 0, 0, '', 'CheckedOut'),
('BK_1720137600_7', 'CUS007', 'REC001', 'P302', 'GRP_1720137600_7', '2025-07-20', '2025-07-22', 4, 1, 1, 'Du lịch hè', 'CheckedOut'),
('BK_1722816000_8', 'CUS008', 'REC002', 'P103', 'GRP_1722816000_8', '2025-08-07', '2025-08-10', 2, 0, 0, '', 'CheckedOut'),
('BK_1725494400_9', 'CUS009', 'REC003', 'P201', 'GRP_1725494400_9', '2025-09-02', '2025-09-05', 3, 1, 0, 'Quốc khánh', 'CheckedOut'),
('BK_1728172800_10', 'CUS010', 'REC001', 'P202', 'GRP_1728172800_10', '2025-10-18', '2025-10-20', 2, 0, 0, '', 'CheckedOut'),
('BK_1730851200_11', 'CUS001', 'REC002', 'P301', 'GRP_1730851200_11', '2025-11-10', '2025-11-12', 4, 1, 1, 'Tri ân', 'CheckedOut'),
('BK_1733529600_12', 'CUS002', 'REC003', 'P302', 'GRP_1733529600_12', '2025-12-22', '2025-12-25', 6, 1, 1, 'Noel', 'CheckedOut'),

('BK_1735689600_13', 'CUS003', 'REC001', 'P101', 'GRP_1735689600_13', '2026-01-12', '2026-01-15', 2, 0, 0, '', 'CheckedOut'),
('BK_1738368000_14', 'CUS004', 'REC002', 'P201', 'GRP_1738368000_14', '2026-02-17', '2026-02-20', 4, 1, 0, '', 'CheckedOut'),
('BK_1741046400_15', 'CUS005', 'REC003', 'P301', 'GRP_1741046400_15', '2026-03-07', '2026-03-10', 6, 1, 1, 'Mùa xuân', 'CheckedOut'),
('BK_1743724800_16', 'CUS006', 'REC001', 'P102', 'GRP_1743724800_16', '2026-04-22', '2026-04-25', 2, 0, 0, '', 'CheckedOut'),
('BK_1746403200_17', 'CUS007', 'REC002', 'P202', 'GRP_1746403200_17', '2026-05-15', '2026-05-18', 3, 1, 0, '', 'CheckedOut'),
('BK_1749081600_18', 'CUS008', 'REC003', 'P203', 'GRP_1749081600_18', '2026-06-27', '2026-06-30', 2, 0, 0, '', 'CheckedOut'),
('BK_1751760000_19', 'CUS009', 'REC001', 'P302', 'GRP_1751760000_19', '2026-07-10', '2026-07-14', 8, 1, 1, 'Nghỉ dưỡng', 'CheckedOut'),
('BK_1754438400_20', 'CUS010', 'REC002', 'P101', 'GRP_1754438400_20', '2026-08-01', '2026-08-03', 2, 1, 0, '', 'CheckedOut'),
('BK_1754438401_21', 'CUS001', 'REC003', 'P201', 'GRP_1754438401_21', '2026-08-02', '2026-08-05', 4, 0, 1, 'Trang trí hoa hồng', 'CheckedOut'),
('BK_1754438402_22', 'CUS002', 'REC001', 'P202', 'GRP_1754438402_22', '2026-08-01', '2026-08-04', 2, 1, 0, '', 'CheckedOut'),
('BK_1754438403_23', 'CUS003', 'REC001', 'P301', 'GRP_1754438403_23', '2026-08-03', '2026-08-06', 6, 1, 1, 'Tiệc sinh nhật', 'CheckedIn'),
('BK_1754438404_24', 'CUS004', 'REC003', 'P102', 'GRP_1754438404_24', '2026-08-04', '2026-08-07', 0, 0, 0, '', 'Booked'),
('BK_1754438405_25', 'CUS006', 'REC001', 'P201', 'GRP_1754438405_25', '2026-08-05', '2026-08-08', 2, 0, 0, '', 'CheckedIn'),
('BK_1754438406_26', 'CUS007', 'REC003', 'P103', 'GRP_1754438406_26', '2026-08-06', '2026-08-09', 1, 0, 0, '', 'Booked');

INSERT OR IGNORE INTO invoices (id, booking_id, receptionist_id, issued_date, payment_method, discount_name, subtotal_amount, discount_amount, total_amount) VALUES
('INV_1704067200_1', 'BK_1704067200_1', 'REC001', '2025-01-10', 'Cash', 'Member Discount', 1200000, 120000, 1080000),
('INV_1706745600_2', 'BK_1706745600_2', 'REC002', '2025-02-14', 'Credit Card', 'Seasonal Discount', 2200000, 330000, 1870000),
('INV_1709424000_3', 'BK_1709424000_3', 'REC003', '2025-03-20', 'Bank Transfer', 'Member Discount', 5800000, 580000, 5220000),
('INV_1712102400_4', 'BK_1712102400_4', 'REC001', '2025-04-15', 'Cash', 'No Discount', 1700000, 0, 1700000),
('INV_1714780800_5', 'BK_1714780800_5', 'REC002', '2025-05-01', 'Credit Card', 'Seasonal Discount', 3350000, 502500, 2847500),
('INV_1717459200_6', 'BK_1717459200_6', 'REC003', '2025-06-18', 'Bank Transfer', 'No Discount', 3200000, 0, 3200000),
('INV_1720137600_7', 'BK_1720137600_7', 'REC001', '2025-07-22', 'Credit Card', 'Member Discount', 6800000, 680000, 6120000),
('INV_1722816000_8', 'BK_1722816000_8', 'REC002', '2025-08-10', 'Cash', 'No Discount', 1850000, 0, 1850000),
('INV_1725494400_9', 'BK_1725494400_9', 'REC003', '2025-09-05', 'Bank Transfer', 'Seasonal Discount', 3100000, 465000, 2635000),
('INV_1728172800_10', 'BK_1728172800_10', 'REC001', '2025-10-20', 'Cash', 'Member Discount', 2100000, 210000, 1890000),
('INV_1730851200_11', 'BK_1730851200_11', 'REC002', '2025-11-12', 'Credit Card', 'Seasonal Discount', 5800000, 870000, 4930000),
('INV_1733529600_12', 'BK_1733529600_12', 'REC003', '2025-12-25', 'Bank Transfer', 'Member Discount', 9800000, 980000, 8820000),

('INV_1735689600_13', 'BK_1735689600_13', 'REC001', '2026-01-15', 'Cash', 'No Discount', 1700000, 0, 1700000),
('INV_1738368000_14', 'BK_1738368000_14', 'REC002', '2026-02-20', 'Credit Card', 'Member Discount', 3100000, 310000, 2790000),
('INV_1741046400_15', 'BK_1741046400_15', 'REC003', '2026-03-10', 'Bank Transfer', 'Seasonal Discount', 8300000, 1245000, 7055000),
('INV_1743724800_16', 'BK_1743724800_16', 'REC001', '2026-04-25', 'Cash', 'No Discount', 1700000, 0, 1700000),
('INV_1746403200_17', 'BK_1746403200_17', 'REC002', '2026-05-18', 'Credit Card', 'Member Discount', 3200000, 320000, 2880000),
('INV_1749081600_18', 'BK_1749081600_18', 'REC003', '2026-06-30', 'Bank Transfer', 'No Discount', 3200000, 0, 3200000),
('INV_1751760000_19', 'BK_1751760000_19', 'REC001', '2026-07-14', 'Credit Card', 'Seasonal Discount', 13200000, 1980000, 11220000),
('INV_1754438400_20', 'BK_1754438400_20', 'REC002', '2026-08-03', 'Cash', 'Member Discount', 1200000, 120000, 1080000),
('INV_1754438401_21', 'BK_1754438401_21', 'REC003', '2026-08-05', 'Credit Card', 'Seasonal Discount', 2800000, 420000, 2380000),
('INV_1754438402_22', 'BK_1754438402_22', 'REC001', '2026-08-04', 'Bank Transfer', 'No Discount', 2850000, 0, 2850000);