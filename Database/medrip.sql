DROP DATABASE IF EXISTS iv_monitoring_system;
CREATE DATABASE iv_monitoring_system CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE iv_monitoring_system;

CREATE TABLE `users` (
    `user_id` VARCHAR(20) PRIMARY KEY,
    `full_name` VARCHAR(100) NOT NULL
);

CREATE TABLE `medical_records` (
    `record_id` VARCHAR(50) PRIMARY KEY,
    `patient_name` VARCHAR(100) NOT NULL
    
);

CREATE TABLE `devices` (
    `device_id` VARCHAR(50) PRIMARY KEY,
    `status` VARCHAR(20)
);

CREATE TABLE `infusion_sessions` (
    `session_id` INT PRIMARY KEY, -- Để nhập đúng ID 1, 5, 6 như hình
    `record_id` VARCHAR(50),
    `device_id` VARCHAR(50),
    `nurse_id` VARCHAR(20),
    `fluid_type` VARCHAR(100),
    `target_rate` FLOAT,
    `current_rate` FLOAT,
    `remaining_volume` FLOAT,
    `expected_end_time` DATETIME DEFAULT NULL,
    `session_status` VARCHAR(20),
    
    FOREIGN KEY (`record_id`) REFERENCES `medical_records`(`record_id`),
    FOREIGN KEY (`device_id`) REFERENCES `devices`(`device_id`),
    FOREIGN KEY (`nurse_id`) REFERENCES `users`(`user_id`)
);



