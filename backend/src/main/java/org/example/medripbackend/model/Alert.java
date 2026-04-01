package org.example.medripbackend.model;

import jakarta.persistence.*;
import lombok.Data;
import java.time.LocalDateTime;

@Entity
@Table(name = "alerts")
@Data
public class Alert {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer alertId;

    @ManyToOne
    @JoinColumn(name = "session_id", nullable = false)
    private InfusionSession session;

    private String alertType;
    private LocalDateTime createdAt = LocalDateTime.now();
    private Boolean isResolved = false;
}
