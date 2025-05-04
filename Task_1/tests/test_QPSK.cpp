#include <QAM.hpp>
#include <AWGNChannel.hpp>
#include <iostream>

int main() {
    QAM::Modulator modulator;
    QAM::Demodulator demodulator;
    AWGNChannel channel(10);
    QAM::ComplexVector_t symbols;
    QAM::BitVector_t bits = {1,1,0,1, 0,1,1,1, 1,0,0,1, 0,0,1,0};
    try {
        modulator.Modulate(bits, symbols);
        for (int i = 0; i < symbols.size(); i++) {
            std::cout << symbols[i] << std::endl;
        }
    } catch (const std::exception &e) {    
        std::cerr << e.what() << std::endl;
    }
    auto trans_symbols = channel.Transmit(symbols);
    std::cout << "Transmitted symbols:" << std::endl;
    for (int i = 0; i < trans_symbols.size(); i++) {
        std::cout << trans_symbols[i] << std::endl;
    }
    QAM::BitVector_t demodulated_bits;
    try {
        demodulator.Demodulate(trans_symbols, demodulated_bits);
        for (int i = 0; i < demodulated_bits.size(); i++) {
            std::cout << demodulated_bits[i] << std::endl;
        }
    } catch (const std::exception &e) {    
        std::cerr << e.what() << std::endl;
    }

    if (demodulated_bits != bits) {
        std::cerr << "Demodulation failed" << std::endl;
    } else {
        std::cout << "Demodulation passed" << std::endl;
    }
    
    return 0;
}