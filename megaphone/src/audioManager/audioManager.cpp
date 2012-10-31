
#include "audioManager.h"

    


void audioManager::setup(){
    dywapitch_inittracking(&pitchtracker);   
    audioDataDouble = NULL;
    AA.setup();//1024
    
    aubioBuffer.reserve(1024);
    aubioArray = new float[1024];

    magnitude = new float[1024];
    phase = new float[1024];
    power = new float[1024];
    freq = new float[1024];
    
    FFTanalyzer.setup(44100, 1024/2, 2);
    
    //FFTOctaveAnalyzer davebollinger 
    
    // ------> 1024/2, 2
    
    // fe:  2f for octave bands, sqrt(2) for half-octave bands, cuberoot(2) for third-octave bands, etc
    // this isn't currently configurable (used once here then no effect), but here's some reasoning:
    // 43 is a good value if you want to approximate "computer" octaves: 44100/2/2/2/2/2/2/2/2/2/2
    // 55 is a good value if you'd rather approximate A-440 octaves: 440/2/2/2
    // 65 is a good value if you'd rather approximate "middle-C" octaves:  ~262/2/2
    // you could easily double it if you felt the lowest band was just rumble noise (as it probably is)
    // but don't go much smaller unless you have a huge fft window size (see below for more why)
    // keep in mind, if you change it, that the number of actual bands may change +/-1, and
    // for some values, the last averaging band may not be very useful (may extend above nyquist)
    
    FFTanalyzer.peakHoldTime    = 30;
    FFTanalyzer.peakDecayRate   = 0.095f;
    FFTanalyzer.linearEQIntercept = 0.9f;
    FFTanalyzer.linearEQSlope   = 0.01f;
    
    results.fftOctaves = new float[ FFTanalyzer.nAverages];
    results.nFftOctaves = FFTanalyzer.nAverages;

};



void audioManager::update(float * audio, int _bufferSize){
    
    
    bufferSize = _bufferSize;
    audioData = audio;
    if (audioDataDouble == NULL){
        audioDataDouble = new double[bufferSize];
    }
    
    for (int i = 0; i < bufferSize; i++){
        audioDataDouble[i] = audioData[i];
    }
    
    if (aubioBuffer.size() == 1024){
        aubioBuffer.erase(aubioBuffer.begin(), aubioBuffer.begin() + _bufferSize);
    }
    for (int i = 0; i < bufferSize; i++){
        aubioBuffer.push_back(audio[i]);
    }

    //cout << aubioBuffer.size() << endl;
    calculatePitch();
    calculateRMS();
    
    
    
    
    if (aubioBuffer.size() == 1024){
        for (int i = 0; i < 1024; i++){
            aubioArray[i] = aubioBuffer[i];
        }
        calculateAubio();
        calculateFFT();
    }
}

void audioManager::calculatePitch(){
    results.pitch = dywapitch_computepitch(&pitchtracker, audioDataDouble, 0, bufferSize);
}

void audioManager::calculateRMS(){
    float total;
    for (int i = 0; i < bufferSize; i++){
        total += audioData[i] * audioData[i];
    }
    total /= (float)bufferSize;
    total = sqrt(total);
    results.RMS = total;
}

void audioManager::calculateAubio(){
    AA.processAudio(aubioArray, 1024);
    results.aubioPitch = AA.pitch;
    results.aubioRMS = AA.amplitude;
}


void audioManager::calculateFFT(){

    
    float avg_power = 0.0f;
    myfft.powerSpectrum(0, (int) 1024/2, aubioArray, 1024, &magnitude[0], &phase[0], &power[0], &avg_power);
    for(int i = 0; i< 1024/2; i++){
        freq[i] = magnitude[i];
    }
    FFTanalyzer.calculate(freq);
    
    //results.fftOctaves.clear();
    for (int i = 0; i < FFTanalyzer.nAverages; i++){
        results.fftOctaves[i] = FFTanalyzer.averages[i];
    }
    
}


void audioManager::calculateOnset(){
    
}

