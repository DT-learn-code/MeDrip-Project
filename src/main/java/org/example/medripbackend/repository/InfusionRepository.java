package org.example.medripbackend.repository;

import org.example.medripbackend.model.InfusionSession;
import org.springframework.data.jpa.repository.JpaRepository;

public interface InfusionRepository extends JpaRepository<InfusionSession, Integer> {
}
