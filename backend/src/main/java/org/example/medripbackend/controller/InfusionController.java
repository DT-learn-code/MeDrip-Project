package org.example.medripbackend.controller;

import org.example.medripbackend.model.InfusionSession;
import org.example.medripbackend.repository.InfusionRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@RestController
@RequestMapping("/api/infusion")
public class InfusionController {

    @Autowired
    private InfusionRepository repository;

    @PostMapping("/update")
    public String receiveData(@RequestBody Map<String, Object> payload) {
        try {
            Integer sId = (Integer) payload.get("session_id");

            InfusionSession session = repository.findById(sId).orElse(new InfusionSession());

            session.setSessionId(sId);
            session.setDeviceId((String) payload.get("device_id"));
            session.setCurrentRate(Float.valueOf(payload.get("rate").toString()));
            session.setRemainingVolume(Float.valueOf(payload.get("vol").toString()));
            session.setSessionStatus((String) payload.get("status"));

            repository.save(session);
            System.out.println("Đã nhận dữ liệu từ ESP32 cho Session: " + sId);
            return "OK";
        } catch (Exception e) {
            return "Error: " + e.getMessage();
        }
    }
}
