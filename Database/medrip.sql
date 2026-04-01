CREATE DATABASE IF NOT EXISTS iv_monitoring_system
CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE iv_monitoring_system;

CREATE TABLE users (
    user_id VARCHAR(20) PRIMARY KEY,
    password_hash VARCHAR(255) NOT NULL,
    full_name NVARCHAR(100) NOT NULL,
    role VARCHAR(20) NOT NULL,
    is_active BOOLEAN DEFAULT TRUE
);

CREATE TABLE medical_records (
    record_id VARCHAR(50) PRIMARY KEY,
    patient_name NVARCHAR(100) NOT NULL,
    room_number VARCHAR(20) NOT NULL,
    bed_number VARCHAR(20) NOT NULL
);

CREATE TABLE devices (
    device_id VARCHAR(50) PRIMARY KEY,
    battery_level INT CHECK (battery_level >= 0 AND battery_level <= 100),
    status VARCHAR(20)
);

CREATE TABLE infusion_sessions (
    session_id INT AUTO_INCREMENT PRIMARY KEY,
    record_id VARCHAR(50) NOT NULL,
    device_id VARCHAR(50) NOT NULL,
    nurse_id VARCHAR(20) NOT NULL,
    fluid_type NVARCHAR(100) NOT NULL,
    target_rate FLOAT CHECK (target_rate > 0),
    current_rate FLOAT CHECK (current_rate >= 0),
    remaining_volume FLOAT CHECK (remaining_volume >= 0),
    expected_end_time DATETIME,
    session_status VARCHAR(20) NOT NULL,

    FOREIGN KEY (record_id) REFERENCES medical_records(record_id) ON DELETE CASCADE,
    FOREIGN KEY (device_id) REFERENCES devices(device_id) ON DELETE RESTRICT,
    FOREIGN KEY (nurse_id) REFERENCES users(user_id) ON DELETE RESTRICT
);

CREATE TABLE alerts (
    alert_id INT AUTO_INCREMENT PRIMARY KEY,
    session_id INT NOT NULL,
    alert_type VARCHAR(50) NOT NULL,
    created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    is_resolved BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (session_id)
        REFERENCES infusion_sessions (session_id)
        ON DELETE CASCADE
);