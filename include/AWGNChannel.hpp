#ifndef AWGNCHANNEL_HPP
#define AWGNCHANNEL_HPP

#include <complex>
#include <vector>
#include <random>
#include <QAMModulator.hpp>

class AWGNChannel {
    public:
        AWGNChannel(double snr_db);
        ~AWGNChannel();
        QAMModulation::ComplexVector_t Transmit(QAMModulation::ComplexVector_t symbols) const;
    private:
        double _snr_db;
        mutable std::mt19937 _generator;
};

#endif