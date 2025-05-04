#include <QAM.hpp>
#include <iostream>

using namespace QAM;

Modulator::Modulator(bool normalize_flag): _normalize_flag(normalize_flag) {}

Modulator::~Modulator() {}

void Modulator::Modulate(const BitVector_t &input_bits, ComplexVector_t &output_symbols, ModulationMode mode) {
    if (input_bits.empty()) {
        throw std::invalid_argument("Input bits cannot be empty");
    }
    switch (mode) { 
        case QPSK:
            output_symbols = QPSKModulation(input_bits);
            break;
        case QAM16:
            output_symbols = QAM16Modulation(input_bits);
            break;
        case QAM64:
            output_symbols = QAM64Modulation(input_bits);
            break;
        default:
            throw std::invalid_argument("Invalid modulation mode");
    }
}

ComplexVector_t Modulator::QPSKModulation(const BitVector_t &input_bits) {
    if (input_bits.size() % 2 != 0) {
        throw std::invalid_argument("Input bits must be even length");
    }
    double normalization_factor = _normalize_flag ? QPSK_NORM : 1.0;
    ComplexVector_t output_symbols;
    output_symbols.reserve(input_bits.size() / 2);
    for (int i = 0; i < input_bits.size(); i += 2) {
        Bit_t I_bit = input_bits[i];
        Bit_t Q_bit = input_bits[i + 1];
        output_symbols.push_back(std::complex<double>(I_bit ? 1.0 : -1.0, Q_bit ? 1.0 : -1.0));
    }
    Normalize(output_symbols, normalization_factor);
    return output_symbols;
}

ComplexVector_t Modulator::QAM16Modulation(const BitVector_t &input_bits) {
    if (input_bits.size() % 4 != 0) {
        throw std::invalid_argument("Input bits must be a multiple of 4");
    }
    double normalization_factor = _normalize_flag ? QAM16_NORM : 1.0;
    ComplexVector_t output_symbols;
    output_symbols.reserve(input_bits.size() / 4);
    for (int i = 0; i < input_bits.size(); i += 4) {
        Bit_t b0 = input_bits[i];
        Bit_t b1 = input_bits[i + 1];
        Bit_t b2 = input_bits[i + 2];
        Bit_t b3 = input_bits[i + 3];
        output_symbols.push_back(std::complex<double>((b0 ? 1.0 : -1.0) * (b1 ? 3.0 : 1.0), (b2 ? 1.0 : -1.0) * (b3 ? 3.0 : 1.0)));
    }
    Normalize(output_symbols, normalization_factor);
    return output_symbols;
}

ComplexVector_t Modulator::QAM64Modulation(const BitVector_t &input_bits) {
    if (input_bits.size() % 6 != 0) {
        throw std::invalid_argument("Input bits must be a multiple of 6");
    }

    double normalization_factor = _normalize_flag ? QAM64_NORM : 1.0;
    ComplexVector_t output_symbols;
    output_symbols.reserve(input_bits.size() / 6);

    const double levels[8] = {-7, -5, -3, -1, 1, 3, 5, 7};

    for (int i = 0; i < input_bits.size(); i += 6) {
        u_int8_t i_bits = (input_bits[i] << 2) | (input_bits[i+1] << 1) | input_bits[i+2];
        u_int8_t q_bits = (input_bits[i+3] << 2) | (input_bits[i+4] << 1) | input_bits[i+5];
        
        double I = levels[i_bits];
        double Q = levels[q_bits];

        output_symbols.push_back(std::complex(I, Q));
    }
    Normalize(output_symbols, normalization_factor);
    return output_symbols;
}

void Modulator::ToggleNormalizeFlag() {
    _normalize_flag = !_normalize_flag;
}

void Modulator::Normalize(ComplexVector_t &input_symbols, double normalization_factor) {
    if (normalization_factor == 1.0) {
        return;
    }
    if (input_symbols.size() == 0) {
        throw std::invalid_argument("Input symbols vector is empty");
    }
    for (auto &val : input_symbols) {
        val *= normalization_factor;
    }
}