package desenvolvimento.backend.service;

import desenvolvimento.backend.entity.TabelaDadosSensores;
import desenvolvimento.backend.model.Sensor;
import desenvolvimento.backend.repository.TabelaDadosSensoresRepository;
import jakarta.transaction.Transactional;
import org.springframework.stereotype.Service;
import org.springframework.messaging.simp.SimpMessagingTemplate;

import java.time.LocalTime;

@Service
public class TabelaDadosSensoresService {

    private final TabelaDadosSensoresRepository repository;
    private final SimpMessagingTemplate messagingTemplate;

    public TabelaDadosSensoresService(TabelaDadosSensoresRepository repository, SimpMessagingTemplate messagingTemplate){
        this.messagingTemplate = messagingTemplate;
        this.repository = repository;
    }

    @Transactional
    public void salvarDadosNoBancoDeDados(
            Sensor sensor
    ){
        TabelaDadosSensores sensores = new TabelaDadosSensores();

        sensores.setSensor(sensor.sensor());
        sensores.setTipo(sensor.tipo());
        sensores.setValor(sensor.valor());
        sensores.setReferencia(LocalTime.parse(sensor.referencia()));

        this.repository.save(sensores);
        this.messagingTemplate.convertAndSend("/topic/sensores", sensor);
    }
}
