#include "include/noise.hpp"

// Конструктор, принимает стандартное отклонение для гауссовского шума
GaussianNoiseAdder::GaussianNoiseAdder(double sigma) : sigma(sigma) {};
GaussianNoiseAdder::~GaussianNoiseAdder() {}
// Метод для добавления гауссовского шума к вектору символов
std::vector<std::complex<double>> GaussianNoiseAdder::addNoise(const std::vector<std::complex<double>>& symbols) {
    std::default_random_engine generator;
    std::normal_distribution <double> distribution(0.0, sigma);

    // Копирование исходных символов и добавление шума
    std::vector <std::complex<double>> noisySymbols = symbols;
    for (auto& symbol : noisySymbols) {
        symbol += std::complex<double> (distribution(generator), distribution(generator));
    }
    
    return noisySymbols;
}