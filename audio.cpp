#include <iostream>
#include "AudioFile.h"
#include <fftw3.h>

using namespace std;

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

    for(int i = 0; i < numSamples; i++) {
        audioFile.samples[0][i] *= 8;
    }

    //write audiofile
    cout << "\n\n\nOutput File Summary:\n";
    audioFile.printSummary();
    audioFile.save(argv[2]);



    return 0;
}
