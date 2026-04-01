package org.example.medripbackend.repository;

import org.example.medripbackend.model.User; // Khai báo để Java tìm thấy class User
import org.springframework.data.jpa.repository.JpaRepository;

public interface UserRepository extends JpaRepository<User, String> { }