package desenvolvimento.backend.model;

import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;

public record Sensor(
        @NotNull(message = "É preciso ter o sensor identificado!")
        @Size(min = 3, max = 60)
        String sensor,

        @NotNull(message = "É preciso identificar qual o tipo do sensor (temperatura, presença, luminosidade)!")
        @Size(min = 3, max = 60)
        String tipo,

        @NotNull(message = "É necessário ter algum valor sendo gerado pelos sensor!")
        Double valor,

        @NotNull(message = "É preciso ter o tempo em que os dados foram obtidos pelos sensor")
        String referencia

) {
}
