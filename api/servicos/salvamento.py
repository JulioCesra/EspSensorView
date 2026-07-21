from sqlalchemy import create_engine, MetaData, Table
from sqlalchemy.orm import sessionmaker
from datetime import datetime
import os
from dotenv import load_dotenv

load_dotenv()

URL = os.getenv("URL_SQLALCHEMY")

engine = create_engine(str(URL), echo=False)

metadata = MetaData()

tabela_sensor = Table("dados_sensor", metadata, autoload_with=engine)

Sessao = sessionmaker(bind=engine)

def dadosParaSalvamento(temperatura: float, registro: str):
    horario_objeto = datetime.strptime(registro, "%H:%M:%S").time()
    timestamp_pronto = datetime.combine(datetime.today(), horario_objeto)
    with Sessao() as sessao:
        novo_dado = tabela_sensor.insert().values(
            temperatura = round(temperatura, 2),
            registro = timestamp_pronto
        )
        sessao.execute(novo_dado)
        sessao.commit()
