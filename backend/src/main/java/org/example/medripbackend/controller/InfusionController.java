package org.example.medripbackend.controller;

import org.example.medripbackend.model.*;
import org.example.medripbackend.repository.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.util.Map;

@RestController
@RequestMapping("/api/infusion")
public class InfusionController {

    @Autowired private InfusionRepository infusionRepo;
    @Autowired private MedicalRecordRepository recordRepo;
    @Autowired private DeviceRepository deviceRepo;
    @Autowired private UserRepository userRepo;
    @Autowired private AlertRepository alertRepo;

    @PostMapping("/update")
    public String receiveData(@RequestBody Map<String, Object> payload) {
        try {
            // 1. Lấy dữ liệu ID từ payload
            String rId = (String) payload.get("record_id");
            String dId = (String) payload.get("device_id");
            String nId = (String) payload.get("nurse_id");

            // 2. Tìm các thực thể trong DB
            MedicalRecord record = recordRepo.findById(rId).orElseThrow(() -> new Exception("Bệnh nhân không tồn tại"));
            Device device = deviceRepo.findById(dId).orElseThrow(() -> new Exception("Thiết bị không tồn tại"));
            User nurse = userRepo.findById(nId).orElseThrow(() -> new Exception("Điều dưỡng không tồn tại"));

            // 3. Tạo và lưu Session
            InfusionSession session = new InfusionSession();
            session.setMedicalRecord(record);
            session.setDevice(device);
            session.setNurse(nurse);
            session.setCurrentRate(Float.valueOf(payload.get("rate").toString()));
            session.setRemainingVolume(Float.valueOf(payload.get("vol").toString()));
            session.setSessionStatus("IN_PROGRESS");

            // Lấy targetRate mẫu từ DB (hoặc gán cứng để test)
            session.setTargetRate(60.0f);

            infusionRepo.save(session);

            // --- ĐÂY LÀ NƠI ĐẶT LOGIC KIỂM TRA (PHẢI NẰM TRONG HÀM) ---
            Float currentRate = session.getCurrentRate();
            Float targetRate = session.getTargetRate();
            Float threshold = targetRate * 0.10f; // Ngưỡng 10%

            if (Math.abs(currentRate - targetRate) > threshold) {
                saveAlert(session, "RATE_ANOMALY"); //
                System.out.println("⚠️ Cảnh báo: Tốc độ lệch quá 10%");
            }

            return "Cập nhật thành công cho: " + record.getPatientName();

        } catch (Exception e) {
            return "Lỗi: " + e.getMessage();
        }
    }

    // Hàm phụ để lưu cảnh báo (Nằm ngoài hàm receiveData nhưng trong Class)
    private void saveAlert(InfusionSession session, String type) {
        Alert alert = new Alert();
        alert.setSession(session);
        alert.setAlertType(type);
        alertRepo.save(alert);
    }
}