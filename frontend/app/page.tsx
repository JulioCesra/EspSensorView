'use client';
import { useState, useEffect } from 'react';
import { Client } from '@stomp/stompjs';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';

export default function Home() {
  const [dadosSensores, setDadosSensores] = useState<any>([]);

  useEffect(() => {
    const cliente = new Client({
      brokerURL: 'ws://localhost:7080/ws-sensores',
      onConnect: () => {
        console.log('Conectado ao monitor de sensores!');
        cliente.subscribe('/topic/sensores', (resposta) => {
          const dadoNovo = JSON.parse(resposta.body);
          
          if (dadoNovo.tipo === "temperatura"){
            setDadosSensores((dadosAtuais: any) => {
              const novaLista = [...dadosAtuais, dadoNovo];
              if (novaLista.length > 20) {
                novaLista.shift(); 
              }
              return novaLista;
            });
          }
        });
      },
    });

    cliente.activate();

    return () => { cliente.deactivate(); };
  }, []);

  return (
    <div style={{ width: '100%', maxWidth: '700px', height: '600px', margin: 'auto', padding: '20px' }}>
      <h2 style={{ textAlign: 'center', marginBottom: '20px' }}>Monitoramento em Tempo Real</h2>
      
      <ResponsiveContainer width="100%" height="100%">
        <LineChart
          data={dadosSensores}
          margin={{
            top: 5,
            right: 20,
            left: -10, 
            bottom: 5,
          }}
        >
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="referencia" stroke="var(--color-text-3)" />
          <YAxis stroke="var(--color-text-3)" />
          <Tooltip
            cursor={{ stroke: 'var(--color-border-2)' }}
            contentStyle={{
              backgroundColor: 'var(--color-surface-raised)',
              borderColor: 'var(--color-border-2)',
            }}
          />
          <Legend />
          <Line
            type="monotone"
            dataKey="valor" 
            name="Leitura do Sensor" 
            stroke="var(--color-chart-2, #3b82f6)" 
            dot={{ fill: 'var(--color-surface-base, #ffffff)' }}
            activeDot={{ stroke: 'var(--color-surface-base, #ffffff)', r: 6 }}
          />
        </LineChart>
      </ResponsiveContainer>
    </div>
  );
}
