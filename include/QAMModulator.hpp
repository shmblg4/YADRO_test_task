#ifndef QAMMODULATOR_HPP
#define QAMMODULATOR_HPP

#include <complex>
#include <vector>

namespace QAMModulation {
    enum ModulationMode {
        QPSK = 0,
        QAM16 = 1,
        QAM64 = 2
    };

    typedef bool Bit_t;
    typedef std::vector<std::complex<double>> ComplexVector_t;
    typedef std::vector<Bit_t> BitVector_t;
    
    class QAMModulator {
    public:
        void Modulate(const BitVector_t &input_bits, ComplexVector_t &output_symbols, ModulationMode mode = QPSK);
        QAMModulator(bool normalize_flag);
        ~QAMModulator();
        
    private:
        bool _normalize_flag;
        const double QPSK_NORM = 1.0 / std::sqrt(2.0);
        const double QAM16_NORM = 1.0 / std::sqrt(10.0);
        const double QAM64_NORM = 1.0 / std::sqrt(42.0);
    
        ComplexVector_t QPSKModulation(const BitVector_t &input_bits);
        ComplexVector_t QAM16Modulation(const BitVector_t &input_bits);
        ComplexVector_t QAM64Modulation(const BitVector_t &input_bits);
        void Log(const char *message);
        void ToggleNormalizeFlag();
        static void Normalize(ComplexVector_t &input_symbols, double normalization_factor);
    }; 
}

#endif