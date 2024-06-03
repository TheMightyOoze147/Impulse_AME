#include "include/qam.hpp"
#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

// Конструктор класса QAMModulator, инициализирует тип модуляции
QAMModulator::QAMModulator(QAMType type) : type(type) {}

// Метод для модуляции данных в соответствии с выбранным типом QAM
std::vector<std::complex<double>> QAMModulator::modulate(const std::vector<bool>& bits) {
    std::vector<std::complex<double>> symbols;
    switch (type) {
        case QPSK: symbols = modulateQPSK(bits); break; // Вызов модуляции QPSK
        case QAM16: symbols = modulateQAM16(bits); break; // Вызов модуляции QAM16
        case QAM64: symbols = modulateQAM64(bits); break; // Вызов модуляции QAM64
    }

    return symbols;
}

// Метод для модуляции данных в формате QPSK
std::vector<std::complex<double>> QAMModulator::modulateQPSK(std::vector<bool> bits) {
    std::vector<std::complex<double>> symbols;
    for (size_t i = 0; i < bits.size(); i += 2) {
        // Маппинг I и Q по каждым двум битам последовательности;
        // Каждая квадратура принимает значения 1 или -1
        symbols.push_back(std::complex<double>(bits[i] ? 1 : -1, bits[i + 1] ? 1 : -1));
    }

    return symbols;
}

// Метод для модуляции данных в формате QAM16
std::vector<std::complex<double>> QAMModulator::modulateQAM16(std::vector<bool> bits) {
    std::vector<std::complex<double>> symbols;

    for (size_t i = 0; i < bits.size(); i += 4) {
        // Сдвигаем четыре бита (для QAM16) и объединяет их через ИЛИ, бита 4, поэтому index принимает значения от 0 до 15 -- 16 значений
        int index = (bits[i] << 3) | (bits[i + 1] << 2) | (bits[i + 2] << 1) | bits[i + 3];
        
        // Извлечение двух младших бит index для real и двух старших бит для imag; 
        // при помощи формулы (* 2 - 3) преобразуется в значения -3, -1, 1, 3
        double realPart = (index & 0b11) * 2 - 3; 
        double imagPart = ((index >> 2) & 0b11) * 2 - 3;
        
        symbols.push_back(std::complex<double>(realPart, imagPart));
    }

    return symbols;
}

// Метод для модуляции данных в формате QAM64
std::vector<std::complex<double>> QAMModulator::modulateQAM64(std::vector<bool> bits) {
    std::vector<std::complex<double>> symbols;

    for (size_t i = 0; i < bits.size(); i += 6) {
        // Объединение шести бит после сдвигов; index принимает значение от 0 до 63
        int index = (bits[i] << 5) | (bits[i + 1] << 4) | (bits[i + 2] << 3) | (bits[i + 3] << 2) | (bits[i + 4] << 1) | bits[i + 5];
        
        // Вычисление квадратуры I по трём младшим битам и Q по трём старшим битам;
        // принимает значения -7, -5, -3, -1, 1, 3, 5, 7
        double realPart = (index & 0b111) * 2 - 7; 
        double imagPart = ((index >> 3) & 0b111) * 2 - 7; 

        symbols.push_back(std::complex<double>(realPart, imagPart));
    }

    return symbols;
}

// Конструктор класса QAMDemodulator, инициализирует тип модуляции
QAMDemodulator::QAMDemodulator(QAMType type) : type(type) {}

// Метод для демодуляции данных в соответствии с выбранным типом QAM
std::vector<bool> QAMDemodulator::demodulate(const std::vector<std::complex<double>>& symbols) {
    std::vector<bool> bits;
    switch (type) {
        case QPSK: bits = demodulateQPSK(symbols); break; // Вызов демодуляции QPSK
        case QAM16: bits = demodulateQAM16(symbols); break; // Вызов демодуляции QAM16
        case QAM64: bits = demodulateQAM64(symbols); break; // Вызов демодуляции QAM64
    }

    return bits;
}

// Метод для демодуляции данных в формате QPSK
std::vector<bool> QAMDemodulator::demodulateQPSK(const std::vector<std::complex<double>>& symbols) {
    std::vector<bool> bits;
    for (const auto& symbol : symbols) {
        // Сравнение возвращает true, если квадратура больше 0 или false в ином случае,
        // соответственно 1 или 0. 
        bits.push_back(symbol.real() > 0);
        bits.push_back(symbol.imag() > 0);
    }

    return bits;
}

// Метод для демодуляции данных в формате QAM16
std::vector<bool> QAMDemodulator::demodulateQAM16(const std::vector<std::complex<double>>& symbols) {
    std::vector<bool> bits;
    for (const auto& symbol : symbols) {
        double realPart = symbol.real();
        double imagPart = symbol.imag();

        // Квантование до ближайших уровней -3, -1, 1, 3
        // Необходимо, поскольку сигнал может быть с шумом
        int nearestReal = std::round((realPart + 3) / 2.0) * 2 - 3;
        int nearestImag = std::round((imagPart + 3) / 2.0) * 2 - 3;

        // Вычисление индекса символа QAM16 по двум младшим и двум старшим битам (от 0 до 15)
        int index = ((nearestImag + 3) / 2) << 2 | ((nearestReal + 3) / 2);

        // Извлечение n-ого бита (от старшего к младшему), если есть 1, иначе 0
        bits.push_back((index & 0b1000) != 0);
        bits.push_back((index & 0b0100) != 0);
        bits.push_back((index & 0b0010) != 0);
        bits.push_back((index & 0b0001) != 0);
    }

    return bits;
}

// Метод для демодуляции данных в формате QAM64
std::vector<bool> QAMDemodulator::demodulateQAM64(const std::vector<std::complex<double>>& symbols) {
    std::vector<bool> bits;
    for (const auto& symbol : symbols) {
        double realPart = symbol.real();
        double imagPart = symbol.imag();
        
        // Квантование до ближайших уровней -7, -5, -3, -1, 1, 3, 5, 7
        // Необходимо, поскольку сигнал может быть с шумом
        int nearestReal = std::round((realPart + 7) / 2.0) * 2 - 7;
        int nearestImag = std::round((imagPart + 7) / 2.0) * 2 - 7;

        // Вычисление индекса символа QAM64 по трём младшим и трём старшим битам (от 0 до 63)
        int index = ((nearestImag + 7) / 2) << 3 | ((nearestReal + 7) / 2);

        // Извлечение n-ого бита (от старшего к младшему), если есть 1, иначе 0
        bits.push_back((index & 0b100000) != 0);
        bits.push_back((index & 0b010000) != 0);
        bits.push_back((index & 0b001000) != 0);
        bits.push_back((index & 0b000100) != 0);
        bits.push_back((index & 0b000010) != 0);
        bits.push_back((index & 0b000001) != 0);
    }

    return bits;
}

