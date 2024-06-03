#include "include/qam.hpp"
#include "include/noise.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    // Если при запуске программы передать "1", то в терминал будет выводиться статистика
    std::string enable_logging;
    if (!argv[1]) {
        enable_logging = "0";
    } else {
        enable_logging = argv[1];
    }
    srand(time(NULL));

    // Инициализация модуляторов
    QAMModulator qpskMod(QAMType::QPSK);
    QAMModulator QAM16Mod(QAMType::QAM16);
    QAMModulator QAM64Mod(QAMType::QAM64);

    // Инициализация демодуляторов
    QAMDemodulator qpskDemod(QAMType::QPSK);
    QAMDemodulator QAM16Demod(QAMType::QAM16);
    QAMDemodulator QAM64Demod(QAMType::QAM64);

    // Создание файла для записи статистики
    std::ofstream outputFile("result.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: result.txt" << std::endl;
        return 1;
    }

    // Создание вектора бит и запись их в файл
    std::vector<bool> randomBits(36000);
    for (size_t i = 0; i < randomBits.size(); ++i) {
        randomBits[i] = rand() % 2;
    }
    for (auto bit : randomBits) {
        outputFile << bit;
    }
    outputFile << std::endl;

    for (auto i = 0.1; i < 1.5; i+=0.1) {
        // Инициализация шума и запись значения дисперсии в файл
        GaussianNoiseAdder noise(i);
        outputFile << i << std::endl;

        // Модуляция бит в символы
        auto symbolsQPSK = qpskMod.modulate(randomBits);
        auto symbolsQAM16 = QAM16Mod.modulate(randomBits);
        auto symbolsQAM64 = QAM64Mod.modulate(randomBits);

        // Зашумление символов
        auto noisySymbolsQPSK = noise.addNoise(symbolsQPSK);
        auto noisySymbolsQAM16 = noise.addNoise(symbolsQAM16);
        auto noisySymbolsQAM64 = noise.addNoise(symbolsQAM64);

        // Демодуляция символов в биты
        auto bitsAfterQPSK = qpskDemod.demodulate(noisySymbolsQPSK);
        auto bitsAfterQAM16 = QAM16Demod.demodulate(noisySymbolsQAM16);
        auto bitsAfterQAM64 = QAM64Demod.demodulate(noisySymbolsQAM64);

        // Запись демодулированных бит в файл
        for (auto bit : bitsAfterQPSK) {
            outputFile << bit;
        }
        outputFile << std::endl;

        for (auto bit : bitsAfterQAM16) {
            outputFile << bit;
        }
        outputFile << std::endl;

        for (auto bit : bitsAfterQAM64) {
            outputFile << bit;
        }
        outputFile << std::endl;

        // Вывод статистики в терминал, если был передан флаг 1
        if (enable_logging == "1") {
            std::cout << "Dispersion: " << i << std::endl << std::endl;
            std::cout << "Bits before any modulation:" << std::endl;
            for (size_t i = 0; i < randomBits.size(); ++i) {
                std::cout << randomBits[i] << " ";
            }
            std::cout << std::endl << std::endl;

            std::cout << "QPSK mod result:" << std::endl;
            for (const auto& symbol : symbolsQPSK) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;

            std::cout << "Noisy symbols:" << std::endl;
            for (const auto& symbol : noisySymbolsQPSK) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;

            std::cout << "Bits after demod:" << std::endl;
            for (const auto& symbol : bitsAfterQPSK) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl << std::endl;

            std::cout << "QAM16 mod result:" << std::endl;
            for (const auto& symbol : symbolsQAM16) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;

            std::cout << "Noisy symbols:" << std::endl;
            for (const auto& symbol : noisySymbolsQAM16) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;

            std::cout << "Bits after demod:" << std::endl;
            for (const auto& symbol : bitsAfterQAM16) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl << std::endl;

            std::cout << "QAM64 mod result:" << std::endl;
            for (const auto& symbol : symbolsQAM64) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;

            std::cout << "Noisy symbols:" << std::endl;
            for (const auto& symbol : noisySymbolsQAM64) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl;

            std::cout << "Bits after demod:" << std::endl;
            for (const auto& symbol : bitsAfterQAM64) {
                std::cout << symbol << " ";
            }
            std::cout << std::endl << std::endl;
        }
        
    }
    // Закрытие файла
    outputFile.close();
    return 0;
}
