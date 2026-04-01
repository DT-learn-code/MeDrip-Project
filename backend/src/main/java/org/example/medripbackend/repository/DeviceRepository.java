package org.example.medripbackend.repository;

import org.example.medripbackend.model.Device;
import org.springframework.data.jpa.repository.JpaRepository;

public interface DeviceRepository extends JpaRepository<Device, String> { }
