import matplotlib.pyplot as plt
import sys

# Воруем данные из файла
def parse_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    # Форматирует строки в нужные нам значения
    reference_bits = [int(bit) for bit in lines[0].strip()]
    sigma = []
    QPSK = []
    QAM16 = []
    QAM64 = []

    for i in range(1, len(lines)):
        if i % 4 == 1:
            sigma.append(float(lines[i].strip()))
        elif i % 4 == 2:
            QPSK.append([int(bit) for bit in lines[i].strip()])
        elif i % 4 == 3:
            QAM16.append([int(bit) for bit in lines[i].strip()])
        elif i % 4 == 0:
            QAM64.append([int(bit) for bit in lines[i].strip()])

    return reference_bits, sigma, QPSK, QAM16, QAM64

# Подсчёт количества ошибок
def count_errors(reference_bits, demodulated_bits):
    if len(reference_bits) != len(demodulated_bits):
        raise ValueError("Длина референсных и демодулированных бит должна быть одинаковой")
    
    errors = 0
    for ref, demod in zip(reference_bits, demodulated_bits):
        if ref != demod:
            errors += 1
    
    return errors

# Подсчёт BER
def calculate_BER(reference_bits, demodulated_bits):
    errors = count_errors(reference_bits, demodulated_bits)
    return errors / len(reference_bits)

filename = "result.txt"
reference_bits, sigma, QPSK, QAM16, QAM64 = parse_file(filename)

# Расчеты BER
QPSK_BER = [calculate_BER(reference_bits, bits) for bits in QPSK]
QAM16_BER = [calculate_BER(reference_bits, bits) for bits in QAM64]
QAM64_BER = [calculate_BER(reference_bits, bits) for bits in QAM16]

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(sigma, QPSK_BER, label='QPSK BER')
plt.plot(sigma, QAM16_BER, label='QAM16 BER')
plt.plot(sigma, QAM64_BER, label='QAM64 BER')
plt.xlabel('Дисперсия шума (sigma)')
plt.ylabel('Вероятность ошибки на бит (BER)')
plt.title(f'Зависимость BER от дисперсии шума для {len(reference_bits)} бит')
plt.legend()
plt.grid(True)
plt.show()