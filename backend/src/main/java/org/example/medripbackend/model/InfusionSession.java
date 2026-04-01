package org.example.medripbackend.model;

import jakarta.persistence.*;
import lombok.Data;
import java.time.LocalDateTime;

@Entity
@Table(name = "infusion_sessions")
@Data
public class InfusionSession {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer sessionId;

    @ManyToOne
    @JoinColumn(name = "record_id", nullable = false)
    private MedicalRecord medicalRecord;

    @ManyToOne
    @JoinColumn(name = "device_id", nullable = false)
    private Device device;

    @ManyToOne
    @JoinColumn(name = "nurse_id", nullable = false)
    private User nurse;

    private String fluidType;
    private Float targetRate;
    private Float currentRate;
    private Float remainingVolume;
    private LocalDateTime expectedEndTime;
    private String sessionStatus;
}