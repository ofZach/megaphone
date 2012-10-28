#pragma once

#include "dywapitchtrack.h"
#include <vector.h>

//---------------------------------------------------------
typedef struct {
    float pitch;
    float RMS;              // root mean square
} analysisResults;


//---------------------------------------------------------
class audioManager {

public: 
    
    void setup();
    void update(float * audio, int bufferSize);
    void calculatePitch();
    void calculateRMS();
    void calculateFFT();
    void calculateOnset();
    
    
    dywapitchtracker pitchtracker;
    double * audioDataDouble;
    float * audioData;
    int bufferSize;
    
    analysisResults results;
    
};

