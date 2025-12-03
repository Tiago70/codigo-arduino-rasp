import asyncio
import os
import serial
import time
from datetime import datetime
from typing import Literal

ARQUIVO = 'dados.csv'
BOMBA = 'BOMBA'
TEMPERATURA = 'TEMP'
CONDUTIVIDADE = 'COND'

if not os.path.exists(ARQUIVO):
    with open(ARQUIVO, 'w') as arq:
        arq.write('modulo,valor,data e hora\n')

# No Linux/Raspberry a porta geralmente é /dev/ttyUSB0 ou /dev/ttyACM0
# No Windows seria 'COM3', 'COM4', etc.
PORTA_SERIAL = '/dev/ttyUSB0' 
VELOCIDADE = 9600

try:
    arduino = serial.Serial(PORTA_SERIAL, VELOCIDADE, timeout=1)
    
    print("Aguardando boot do arduino...")
    time.sleep(2) 

except serial.SerialException as e:
    print(f"Erro ao conectar na porta {PORTA_SERIAL}. Verifique a conexão do arduino")
    print(f"Detalhe: {e}")

except KeyboardInterrupt:
    print("\nEncerrando...")
    if 'arduino' in locals() and arduino.is_open:
        arduino.close()

# Função auxiliar para enviar e receber
def solicitar(metodo: Literal['GET', 'SET'], modulo: str, valor: int | None = 0):
    # Limpa qualquer lixo que possa estar na fila de entrada/saída
    arduino.reset_input_buffer()
    
    # Precisamos adicionar o \n porque o Arduino usa readStringUntil('\n')
    if valor:
        mensagem = f"{metodo} {modulo} {valor}\n"
    else: 
        mensagem = f"{metodo} {modulo}\n"
    
    # .encode() transforma string em bytes (o fio só passa bytes)
    arduino.write(mensagem.encode('utf-8'))
    print(f"Enviado: {mensagem}")

    # Fica esperando até chegar um \n ou estourar o timeout
    resposta_bytes = arduino.readline()
    
    if not resposta_bytes:
        print("Erro: Sem resposta (Timeout)")
        # acionar um erro aqui
        return None

    # .decode() transforma bytes de volta em string
    resposta_texto = resposta_bytes.decode('utf-8').strip()
    print(f"Recebido (Raw): {resposta_texto}")
    return resposta_texto

def escrever(modulo: str, valor: float):
    with open(ARQUIVO, 'a') as arq:
        arq.write(f'{modulo},{valor},{datetime.now()}\n')

async def temperatura():
    while True:
        await asyncio.sleep(2)
        respostas = solicitar('GET', TEMPERATURA).split(' ')
        escrever(respostas[0], respostas[1])


async def condutividade():
    while True:
        await asyncio.sleep(5)
        respostas = solicitar('GET', CONDUTIVIDADE).split(' ')
        escrever(respostas[0], respostas[1])

async def bomba():
    while True:
        # tempo desligado
        await asyncio.sleep(20)
        respostas = solicitar('GET', BOMBA, 1).split(' ')
        escrever(respostas[0], 1)

        # tempo ligado
        await asyncio.sleep(5)
        respostas = solicitar('GET', BOMBA, 0).split(' ')
        escrever(respostas[0], 0)
        

async def main():
    task1 = asyncio.create_task(temperatura())
    task2 = asyncio.create_task(condutividade())
    task3 = asyncio.create_task(bomba())

    await asyncio.gather(task1, task2, task3)

asyncio.run(main())
