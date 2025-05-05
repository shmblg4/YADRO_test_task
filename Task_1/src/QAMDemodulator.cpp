#include <QAM.hpp>
#include <limits>
#include <cmath>
#include <iostream>

using namespace QAM;

Demodulator::Demodulator(bool normalize_flag) : _normalize_flag(normalize_flag) {}
Demodulator::~Demodulator() {}

void Demodulator::Demodulate(const ComplexVector_t &input_symbols, BitVector_t &output_bits, ModulationMode mode) {
    if (input_symbols.empty()) {
        throw std::invalid_argument("Input input_symbols cannot be empty");
    }

    switch (mode) {
        case QPSK:
            output_bits = QPSKDemodulation(input_symbols);
            break;
        case QAM16:
            output_bits = QAM16Demodulation(input_symbols);
            break;
        case QAM64:
            output_bits = QAM64Demodulation(input_symbols);
            break;
        default:
            throw std::invalid_argument("Unsupported modulation mode");
    }
}

BitVector_t Demodulator::QPSKDemodulation(const ComplexVector_t &input_symbols) {
    if (input_symbols.size() == 0) {
        throw std::invalid_argument("Input symbols vector is empty");
    }
    BitVector_t output_bits;
    output_bits.reserve(input_symbols.size() * 2);
    for (auto &symbol : input_symbols) {
        output_bits.push_back(symbol.real() > 0);
        output_bits.push_back(symbol.imag() > 0);
    }
    return output_bits;
}

BitVector_t Demodulator::QAM16Demodulation(const ComplexVector_t &input_symbols) {
    if (input_symbols.empty()) {
        throw std::invalid_argument("Input symbols vector is empty");
    }
    
    BitVector_t output_bits;
    output_bits.reserve(input_symbols.size() * 4);

    double threshold = _normalize_flag ? 2.0 * QAM16_NORM : 2.0;

    for (auto &symbol : input_symbols) {
        double I = symbol.real();
        double Q = symbol.imag();

        output_bits.push_back(I > 0);
        output_bits.push_back(std::abs(I) > threshold);
        output_bits.push_back(Q > 0);
        output_bits.push_back(std::abs(Q) > threshold);
    }

    return output_bits;
}

BitVector_t Demodulator::QAM64Demodulation(const ComplexVector_t &input_symbols) {
    if (input_symbols.empty()) {
        throw std::invalid_argument("Input symbols vector is empty");
    }
    
    BitVector_t output_bits;
    output_bits.reserve(input_symbols.size() * 6);

    const double denorm_factor = _normalize_flag ? 1.0/QAM64_NORM : 1.0;
    const double levels[8] = {-7, -5, -3, -1, 1, 3, 5, 7};

    for (const auto &symbol : input_symbols) {
        double I = symbol.real() * denorm_factor;
        double Q = symbol.imag() * denorm_factor;

        auto get_bits = [&levels](double value) -> u_int8_t {
            double min_dist = INFINITY;
            u_int8_t best_index = 0;
            
            for (u_int8_t i = 0; i < 8; i++) {
                double dist = fabs(value - levels[i]);
                if (dist < min_dist) {
                    min_dist = dist;
                    best_index = i;
                }
            }
            
            return best_index;
        };

        u_int8_t i_bits = get_bits(I);
        u_int8_t q_bits = get_bits(Q);

        for (int j = 2; j >= 0; j--) {
            output_bits.push_back((i_bits >> j) & 1);
        }
        for (int j = 2; j >= 0; j--) {
            output_bits.push_back((q_bits >> j) & 1);
        }
    }

    return output_bits;
}

void Demodulator::ToggleNormalizeFlag() {
    _normalize_flag = !_normalize_flag;
}