package org.example.medripbackend.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Table(name = "devices")
@Data
public class Device {
    @Id
    @Column(name = "device_id", length = 50)
    private String deviceId; //

    private Integer batteryLevel; //

    private String status; // 'ONLINE', 'OFFLINE', 'MAINTENANCE'
}