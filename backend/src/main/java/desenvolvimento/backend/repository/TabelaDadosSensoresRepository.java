package desenvolvimento.backend.repository;

import desenvolvimento.backend.entity.TabelaDadosSensores;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface TabelaDadosSensoresRepository extends JpaRepository<TabelaDadosSensores, Integer> {
    List<TabelaDadosSensores> findBySensor(String sensor);
}
