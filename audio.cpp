#include <iostream>
#include "AudioFile.h"

using namespace std;

int main() {
    AudioFile<double> audioFile;
    audioFile.load("./quick.wav");
    audioFile.printSummary();

    return 0;
}
