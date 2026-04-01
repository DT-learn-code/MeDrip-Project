package org.example.medripbackend.model;

import jakarta.persistence.*;
import lombok.Data;

@Entity
@Table(name = "users")
@Data
public class User {
    @Id
    @Column(name = "user_id", length = 20)
    private String userId; //

    private String passwordHash;
    private String fullName;
    private String role; //
    private Boolean isActive = true;
}