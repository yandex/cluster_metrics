#pragma once

class TMeanCalculator {
private:
    double Mean = 0.;
    double SumWeights = 0.;
public:
    TMeanCalculator& operator += (const TMeanCalculator& rhs) {
        Add(rhs.Mean, rhs.SumWeights);
        return *this;
    }

    void Add(const double value, const double weight = 1.) {
        SumWeights += weight;
        Mean += weight * (value - Mean) / SumWeights;
    }

    double GetMean() const {
        return Mean;
    }
};
