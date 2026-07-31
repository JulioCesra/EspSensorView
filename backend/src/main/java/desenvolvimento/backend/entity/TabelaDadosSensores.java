package desenvolvimento.backend.entity;

import jakarta.persistence.*;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import org.springframework.cglib.core.Local;

import java.time.Instant;
import java.time.LocalTime;

@Entity
@Table(name = "dados_sensor")
public class TabelaDadosSensores {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "id", nullable = false)
    private Integer id;

    @Size(max = 30)
    @NotNull
    @Column(name = "sensor", nullable = false, length = 30)
    private String sensor;

    @Size(max = 60)
    @NotNull
    @Column(name = "tipo", nullable = false, length = 60)
    private String tipo;

    @NotNull
    @Column(name = "valor", nullable = false)
    private Double valor;

    @NotNull
    @Column(name = "referencia", nullable = false)
    private LocalTime referencia;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getSensor() {
        return sensor;
    }

    public void setSensor(String sensor) {
        this.sensor = sensor;
    }

    public String getTipo() {
        return tipo;
    }

    public void setTipo(String tipo) {
        this.tipo = tipo;
    }

    public Double getValor() {
        return valor;
    }

    public void setValor(Double valor) {
        this.valor = valor;
    }

    public LocalTime getReferencia() {
        return referencia;
    }

    public void setReferencia(LocalTime referencia) {
        this.referencia = referencia;
    }

}