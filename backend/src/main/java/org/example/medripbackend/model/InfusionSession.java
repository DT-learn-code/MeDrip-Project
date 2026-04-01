package org.example.medripbackend.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Table(name = "infusion_sessions")
@Data
public class InfusionSession {
    @Id
    @Column(name = "session_id")
    private Integer sessionId;

    @Column(name = "device_id", length = 50)
    private String deviceId;

    @Column(name = "current_rate")
    private Float currentRate;

    @Column(name = "remaining_volume")
    private Float remainingVolume;

    @Column(name = "session_status", length = 20)
    private String sessionStatus;
}
