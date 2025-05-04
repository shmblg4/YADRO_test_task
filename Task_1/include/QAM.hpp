#ifndef QAM_HPP
#define QAM_HPP

#include <complex>
#include <vector>

namespace QAM {
    enum ModulationMode {
        QPSK = 0,
        QAM16 = 1,
        QAM64 = 2
    };

    const double QPSK_NORM = 1.0 / std::sqrt(2.0);
    const double QAM16_NORM = 1.0 / std::sqrt(10.0);
    const double QAM64_NORM = 1.0 / std::sqrt(42.0);

    typedef bool Bit_t;
    typedef std::vector<std::complex<double>> ComplexVector_t;
    typedef std::vector<Bit_t> BitVector_t;
    
    class Modulator {
    public:
        void Modulate(const BitVector_t &input_bits, ComplexVector_t &output_symbols, ModulationMode mode = QPSK);
        Modulator(bool normalize_flag = true);
        void ToggleNormalizeFlag();
        ~Modulator();
        
    private:
        bool _normalize_flag;
    
        ComplexVector_t QPSKModulation(const BitVector_t &input_bits);
        ComplexVector_t QAM16Modulation(const BitVector_t &input_bits);
        ComplexVector_t QAM64Modulation(const BitVector_t &input_bits);
        static void Normalize(ComplexVector_t &input_symbols, double normalization_factor);
    };

    class Demodulator {
    public:
        Demodulator(bool normalize_flag = true);
        ~Demodulator();
        void Demodulate(const ComplexVector_t &input_symbols, BitVector_t &output_bits, ModulationMode mode = QPSK);
        void ToggleNormalizeFlag();
    private:
        bool _normalize_flag;
        BitVector_t QPSKDemodulation(const ComplexVector_t &input_symbols);
        BitVector_t QAM16Demodulation(const ComplexVector_t &input_symbols);
        BitVector_t QAM64Demodulation(const ComplexVector_t &input_symbols);
    };
}

#endif