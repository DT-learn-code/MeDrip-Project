package org.example.medripbackend.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Table(name = "medical_records")
@Data
public class MedicalRecord {
    @Id
    @Column(name = "record_id", length = 50)
    private String recordId; //

    private String patientName;
    private String roomNumber;
    private String bedNumber;
}