from fastapi import FastAPI
from pydantic import BaseModel
from servicos.salvamento import dadosParaSalvamento

class DadosSensor(BaseModel):
    temperatura: float
    registro: str

app = FastAPI()

@app.post("/dados_sensor")
def dados_sensor(dados: DadosSensor):
    dadosParaSalvamento(dados.temperatura, dados.registro)
    return dados
