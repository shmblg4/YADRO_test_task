#include <AWGNChannel.hpp>

AWGNChannel::AWGNChannel(double snr_db) {
    _snr_db = snr_db;
}

QAMModulation::ComplexVector_t AWGNChannel::Transmit(QAMModulation::ComplexVector_t symbols) const {
    std::normal_distribution<double> noise(0, std::pow(10, -_snr_db/20));
    for (auto &symbol : symbols) {
        symbol += noise(_generator);
    }
    return symbols;
}