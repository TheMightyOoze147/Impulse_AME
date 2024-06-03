#ifndef QAM_HPP
#define QAM_HPP

#include <vector>
#include <complex>

enum QAMType { QPSK, QAM16, QAM64 };

class QAMModulator {
public:
    QAMModulator(QAMType type);
    std::vector<std::complex<double>> modulate(const std::vector<bool>& bits);

private:
    QAMType type;
    std::vector<std::complex<double>> modulateQPSK(std::vector<bool> bits);
    std::vector<std::complex<double>> modulateQAM16(std::vector<bool> bits);
    std::vector<std::complex<double>> modulateQAM64(std::vector<bool> bits);
};

class QAMDemodulator {
public:
    QAMDemodulator(QAMType type);
    std::vector<bool> demodulate(const std::vector<std::complex<double>>& symbols);
private:
    QAMType type;
    std::vector<bool> demodulateQPSK(const std::vector<std::complex<double>>& symbols);
    std::vector<bool> demodulateQAM16(const std::vector<std::complex<double>>& symbols);
    std::vector<bool> demodulateQAM64(const std::vector<std::complex<double>>& symbols);
};

#endif