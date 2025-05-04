#include <QAM.hpp>
#include <AWGNChannel.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

QAM::BitVector_t generate_random_bits(size_t num_bits, QAM::ModulationMode mode) {
    QAM::BitVector_t bits;
    bits.reserve(num_bits);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    size_t bits_per_symbol = (mode == QAM::QPSK) ? 2 : 
                           (mode == QAM::QAM16) ? 4 : 6;
    num_bits = (num_bits / bits_per_symbol) * bits_per_symbol;

    for (size_t i = 0; i < num_bits; ++i) {
        bits.push_back(dis(gen));
    }
    
    return bits;
}

double calculate_ber(const QAM::BitVector_t &original, const QAM::BitVector_t &received) {
    if (original.size() != received.size()) {
        throw std::invalid_argument("Bit vectors must have same size");
    }
    
    size_t errors = 0;
    for (size_t i = 0; i < original.size(); ++i) {
        if (original[i] != received[i]) {
            errors++;
        }
    }
    
    return static_cast<double>(errors) / original.size();
}

void run_test_sequence(QAM::ModulationMode mode, size_t num_bits, 
                      const std::vector<double> &noise_variances) {
    std::string mode_str = (mode == QAM::QPSK ? "QPSK" : mode == QAM::QAM16 ? "QAM16" : "QAM64");
    std::ofstream file;
    file.open(mode_str + "results.txt");
    QAM::BitVector_t original_bits = generate_random_bits(num_bits, mode);
    
    QAM::Modulator modulator;
    QAM::Demodulator demodulator;
    
    std::cout << "Testing " << 
        (mode == QAM::QPSK ? "QPSK" : mode == QAM::QAM16 ? "QAM16" : "QAM64") 
              << " with " << num_bits << " bits\n";
    
    for (double variance : noise_variances) {
        QAM::ComplexVector_t modulated;
        modulator.Modulate(original_bits, modulated, mode);
        
        AWGNChannel channel(10.0 * log10(1.0 / variance));
        QAM::ComplexVector_t noisy = channel.Transmit(modulated);
        
        QAM::BitVector_t demodulated_bits;
        demodulator.Demodulate(noisy, demodulated_bits, mode);
        
        double ber = calculate_ber(original_bits, demodulated_bits);
        
        file << std::to_string(variance) << " " << std::to_string(ber) << "\n";
    }
    file.close();
}

int main() {
    const size_t num_bits = 1024;
    const std::vector<double> noise_variances = {0.001, 0.01, 0.1, 0.2, 0.3, 0.4, 0.5};
    
    run_test_sequence(QAM::QPSK, num_bits, noise_variances);
    run_test_sequence(QAM::QAM16, num_bits, noise_variances);
    run_test_sequence(QAM::QAM64, num_bits, noise_variances);
    
    return 0;
}
