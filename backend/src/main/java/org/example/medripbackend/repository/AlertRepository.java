package org.example.medripbackend.repository;

import org.example.medripbackend.model.Alert;
import org.springframework.data.jpa.repository.JpaRepository;

public interface AlertRepository extends JpaRepository<Alert, Integer> { }
