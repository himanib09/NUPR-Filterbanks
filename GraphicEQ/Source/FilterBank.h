#ifndef FILTERBANK_H_INCLUDED
#define FILTERBANK_H_INCLUDED

#include "JuceHeader.h"
#include <vector>
#include <complex>

namespace FilterbankConstants
{
    enum
    {
        order44_1 = 12,
    };
    constexpr int fftSize44_1 = 4096; //pow (2, (int) order);
}

using namespace FilterbankConstants;
template <typename Type>
using Complex = std::complex<Type>;

class Filterbank
{
public:
    Filterbank();

    void reset (double sampleRate, int maxExpectedBlockSize);
    void process (float* buffer, int numSamples);

    void setGain (float gainDB, int band);

private:
    void doFFTProcessing (float* buffer, int numSamples);
    void dcells (Complex<float>* fftData, Array<Array<Complex<float>>>& cells);
    void dcells2spec (Complex<float>* fftData, Array<Array<Complex<float>>>& cells);
    void multGains (Array<Array<Complex<float>>>& cells);

    AudioBuffer<float> buffers[2];
    int readBuffer = 1;
    int readPtr = 0;
    int writeBuffer = 0;
    int writePtr = 0;

    int order = order44_1;
    int fftSize = fftSize44_1;
    int windowSize = fftSize / 2;

    Complex<float> fftInput[4*fftSize44_1];
    Complex<float> fftOutput[4*fftSize44_1];

    Complex<float> positiveFreq[4*fftSize44_1 / 2];
    Complex<float> negativeFreq[4*fftSize44_1 / 2];

    Array<Array<Complex<float>>> posCells;
    Array<Array<Complex<float>>> negCells;

    Complex<float> iFftInput[4*fftSize44_1];
    Complex<float> iFftOutput[4*fftSize44_1];

    std::unique_ptr<dsp::FFT> fft;

    float gainMultipliers[order44_1-1];
};

#endif //FILTERBANK_H_INCLUDED
