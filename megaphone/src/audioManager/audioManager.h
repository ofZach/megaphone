#pragma once

#include "dywapitchtrack.h"
#include <vector.h>
#include "aubioAnalyzer.h"
#include "fft.h"
#include "fftOctaveAnalyzer.h"


//---------------------------------------------------------
typedef struct {
    float pitch;
    float aubioPitch;
    float aubioRMS;
    float RMS;              // root mean square
    
    float * fftOctaves; 
    int nFftOctaves;
    
    
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
    
    
    // pitch tracker : 
    double * audioDataDouble;
    dywapitchtracker pitchtracker;
    float * audioData;
    int bufferSize;
    
    
    
    analysisResults resultsFromMainThread;
    analysisResults results;
    
    aubioAnalyzer AA;
    
    vector < float > aubioBuffer;
    float * aubioArray;
    void calculateAubio();

    
    // fft
    
    fft myfft;
    FFTOctaveAnalyzer FFTanalyzer;
    
    float * magnitude ;//[BUFFER_SIZE];
    float * phase;//[BUFFER_SIZE];
    float * power;// [BUFFER_SIZE];
    float * freq;//[BUFFER_SIZE];
    
};






