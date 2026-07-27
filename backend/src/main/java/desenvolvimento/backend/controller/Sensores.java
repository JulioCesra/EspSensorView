package desenvolvimento.backend.controller;

import desenvolvimento.backend.model.Sensor;
import jakarta.validation.Valid;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;

import java.util.Map;

@RequestMapping("/api/sensores")
@Controller
public class Sensores {

    @PostMapping("/receberDados")
    public ResponseEntity<Map<String, String>> receberDadosDosSensores(
            @Valid @RequestBody Sensor sensor
    ){
        return ResponseEntity.status(HttpStatus.CREATED).body(
                Map.of("resposta", "Código 201 - Os dados do sensor foram recebidos pela API!")
        );
    }
}
