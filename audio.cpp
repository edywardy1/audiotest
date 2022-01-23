#include <iostream>
#include "AudioFile.h"
#include <fftw3.h>
#include <complex.h>
#include <vector>
#include <cmath>

using namespace std;

//removes band of frequencies from dft array
//input is freq in hertz
void removeBand(vector<fftw_complex> &data, uint64_t start, uint64_t stop, int samplingRate, int numSamples) {
    uint64_t startHz = (uint64_t) start * numSamples/samplingRate;
    uint64_t stopHz = (uint64_t) stop * numSamples/samplingRate;
    if(stopHz >= (uint64_t)data.size()) {
        stopHz = (uint64_t)data.size() - 1;
    }
    if(startHz < 1) {
        startHz = 0;
    }
    cout << start << "\n";
    cout << stop << "\n";
    cout << samplingRate << "\n";
    cout << numSamples << "\n";
    cout << startHz << " " << stopHz << "\n";
    for(uint64_t i = startHz; i < stopHz; i++) {
        data[i][0] = 0;
        data[i][1] = 0;
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Format is: ./audio INPUT_FILE OUTPUT_FILE\n";
        exit(1);
    }
    //read in audio file
    AudioFile<double> audioFile;
    audioFile.load(argv[1]);
    cout << "Input File Summary:\n";
    audioFile.printSummary();
    int numSamples = audioFile.getNumSamplesPerChannel();
    int numChannels = audioFile.getNumChannels();
    int samplingRate = audioFile.getSampleRate();

    //turn audio to mono if not
    if(!audioFile.isMono()) {
        AudioFile<double>::AudioBuffer buffer;
        buffer.resize(1);
        buffer[0].resize(numSamples);
        //buffer[0].fill(buffer[0].begin(), buffer[0].end(), 0.0);
        for(int i = 0; i < numSamples; i++) {
            for(int j = 0; j < numChannels; j++) {
                buffer[0][i] += (float)(audioFile.samples[j][i]/numChannels);
            }
        }
        audioFile.setAudioBuffer(buffer);
        audioFile.setAudioBufferSize(1, numSamples);
        audioFile.setNumSamplesPerChannel(numSamples);
        audioFile.setNumChannels(1);
    }

    //dft output complex vector
    vector<fftw_complex> out(numSamples/2 + 1);
    //dft setup
    fftw_plan p = fftw_plan_dft_r2c_1d(numSamples, audioFile.samples[0].data(), out.data(), FFTW_ESTIMATE);
    //idft setup
    fftw_plan q = fftw_plan_dft_c2r_1d(numSamples, out.data(), audioFile.samples[0].data(), FFTW_ESTIMATE);
    //execute dft on audioFile input into out vector
    fftw_execute(p);
    fftw_destroy_plan(p);
    
    //do filtering here
    //removeBand(out, 240, 241, samplingRate, numSamples); 
    //removeBand(out,100,125,samplingRate,numSamples);
    //removeBand(out,275,300,samplingRate,numSamples);
    //removeBand(out,400,425,samplingRate,numSamples);
    //removeBand(out,700,725,samplingRate,numSamples);
    //filtering out freq beyond human hearing
    //removeBand(out, 0, 21, samplingRate, numSamples);
    removeBand(out, 20000, (numSamples/2 +1), samplingRate, numSamples);
    samplingRate++;
    
    //do idft and some cleanup
    fftw_execute(q);
    fftw_destroy_plan(q);
    fftw_cleanup();
    //normalize after idft
    for(int i = 0; i < numSamples; i++) {
        audioFile.samples[0][i] /= numSamples;
    }

    //write audiofile to output file
    cout << "\n\n\nOutput File Summary:\n";
    audioFile.printSummary();
    audioFile.save(argv[2]);

    return 0;
}
