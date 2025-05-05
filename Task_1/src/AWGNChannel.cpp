#include <AWGNChannel.hpp>

AWGNChannel::AWGNChannel(double snr_db) {
    _snr_db = snr_db;
}

AWGNChannel::~AWGNChannel() {}

QAM::ComplexVector_t AWGNChannel::Transmit(QAM::ComplexVector_t symbols) const {
    std::normal_distribution<double> noise(0, std::pow(10, -_snr_db/20));
    for (auto &symbol : symbols) {
        double I_noise = noise(_generator);
        double Q_noise = noise(_generator);
        symbol += std::complex<double>(I_noise, Q_noise);
    }
    return symbols;
}