
#include "audioManager.h"

    







void audioManager::setup(){
    dywapitch_inittracking(&pitchtracker);   
    audioDataDouble = NULL;
    AA.setup();//1024
    
    
    nBuffers = 0;
    
    largerBuffer = new float[1024];
    magnitude = new float[1024];
    phase = new float[1024];
    power = new float[1024];
    freq = new float[1024];
    
    FFTanalyzer.setup(44100, 1024/4, 2);
    
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
    FFTanalyzer.linearEQSlope   = 0.02f;
    
    results.fftOctaves = new float[ FFTanalyzer.nAverages];
    results.nFftOctaves = FFTanalyzer.nAverages;
    
    
    volume.bounds.set(300,40,100,40);
    volume.max = 0.1;
    volume.name = "volume (rms)";
    
    
    onset.bounds.set(300,140,100,40);
    onset.name = "onset";
    
    
    pitch.bounds.set(300, 240, 100,40);
    pitch.max = 2000;
    pitch.name = "pitch";
    
    vox.bounds.set(300, 340, 100,40);
    vox.max = 1;
    vox.name = "vox";
    
    spectralCentroid.min = 4.0;
    spectralCentroid.max = 13.0;
    
    spectralCentroid.name = "spectralCentroid";
    
    
    charts.push_back(&volume);
    charts.push_back(&onset);
    charts.push_back(&pitch);
    charts.push_back(&vox);
    charts.push_back(&spectralCentroid);
    
    
    
    
    for (int i = 0; i < charts.size(); i++){
        charts[i]->bounds.set(400, 40 + (80) * i, 100,40);
    }

};



void audioManager::update(float * audio, int _bufferSize){
    
    
    if (nBuffers < 1024/_bufferSize){
        memcpy(largerBuffer + nBuffers * _bufferSize, audio, _bufferSize * sizeof(float));
        nBuffers++;
    } else {
        int nTimes = 1024/_bufferSize;
        for (int i = 1; i < nTimes; i++){
            memcpy(largerBuffer + (i-1) * _bufferSize, 
                   largerBuffer + (i) * _bufferSize,
                   _bufferSize * sizeof(float));
            memcpy(largerBuffer + (nTimes-1) * _bufferSize, audio, _bufferSize * sizeof(float));
            
        }
        nBuffers ++;
    }
    
    
    
    bufferSize = _bufferSize;
    audioData = audio;
    if (audioDataDouble == NULL){
        audioDataDouble = new double[bufferSize];
    }
    
   
    //cout << aubioBuffer.size() << endl;
    //calculatePitch();
    calculateRMS();
    calculateAubio();
    
    if (nBuffers > 1024/_bufferSize){
        calculateFFT();
    }
    
    AOD.processframe(audio, _bufferSize);
    if (AOD.aubioOnsetFound == true){
        //printf("hello %f \n", ofGetElapsedTimef());
    }
    
    volume.addValue(AA.amplitude);
    
    onset.addValue(AOD.aubioOnsetFound == true ? 1 : 0);
    
    pitch.addValue( AA.pitch );
    
    
    
    
    if (AA.amplitude > 0.01){
        results.bVox = true;
    } else {
        results.bVox = false;
    }
    
    vox.addValue(results.bVox == true ? 1 : 0);
    
    
    
    
}


void audioManager::draw(){
   
    ofRectangle fftBounds;
    fftBounds.set(30,30, 300, 100);
    ofFill();
    ofSetColor(100);
    ofRect(fftBounds);
    ofSetColor(255);
    for (int i = 0; i < results.nFftOctaves; i++){
        float pctA = i/(float)results.nFftOctaves; 
        float pctB = (i+1)/(float)results.nFftOctaves; 
        
        float x = fftBounds.x + pctA * fftBounds.width;
        float y = fftBounds.y;
        ofRect(x, y + fftBounds.height, (pctB-pctA) * fftBounds.width, -results.fftOctaves[i] * 5);
    }
    ///cout << results.fftOctaves
    
    
    vector < string > strings;
    
    strings.push_back("rms: " + ofToString(results.RMS));
    strings.push_back("aubio rms: " + ofToString(results.aubioRMS));
    strings.push_back("aubio pitch: " + ofToString(results.aubioPitch));
    strings.push_back("aubio pitch (std dev): " + ofToString(results.pitchStdDev));
    strings.push_back("spectral centroid: " + ofToString(results.spectralCentroid));

    for (int i = 0; i < strings.size(); i++){
        ofDrawBitmapStringHighlight(strings[i], ofPoint(30, 250 + 30 * i));
    }
    
//    ofDrawBitmapStringHighlight("aubio rms: " + ofToString(results.aubioRMS), ofPoint(30,290));
//    ofDrawBitmapStringHighlight("aubio pitch: " + ofToString(results.aubioPitch), ofPoint(30,320));
//    ofDrawBitmapStringHighlight("aubio pitch (std dev): " + ofToString(results.pitchStdDev), ofPoint(30,350));
//    
    
    
    for (int i = 0; i < charts.size(); i++){
        charts[i]->draw();
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
    total /= (float)MAX(bufferSize,1);
    total = sqrt(total);
    
    if (bufferSize != 256){
        printf("buffer size is weird please look at me \n");
    }
    //cout << bufferSize << " " << bufferSize << endl;
    results.RMS = total;
}

void audioManager::calculateAubio(){
    AA.processAudio(audioData, 256);
    results.aubioPitch = AA.pitch;
    results.aubioRMS = AA.amplitude;
    
    double sum, mean, var, dev, skew, kurt;
    computeStats(pitch.values.begin( ), pitch.values.end( ), sum, mean, var, dev, skew, kurt);
    
    results.pitchStdDev = dev;
    
    
}


void audioManager::calculateFFT(){

    
    float avg_power = 0.0f;
    myfft.powerSpectrum(0, (int) 1024/2, largerBuffer, 1024, &magnitude[0], &phase[0], &power[0], &avg_power);
    for(int i = 0; i< 1024/2; i++){
        freq[i] = magnitude[i];
    }
    FFTanalyzer.calculate(freq);
    
    //results.fftOctaves.clear();
    for (int i = 0; i < FFTanalyzer.nAverages; i++){
        results.fftOctaves[i] = FFTanalyzer.averages[i];
    }
    
    float totalEnergy = 0;
    for (int i = 5; i < FFTanalyzer.nAverages; i++){
        totalEnergy += FFTanalyzer.averages[i];
    }
    
    float centroid = 0;
    for (int i = 5; i < FFTanalyzer.nAverages; i++){
        centroid +=  i * FFTanalyzer.averages[i];
    }
   
    centroid /= MAX(totalEnergy, 0.001f);
    
    results.spectralCentroid = centroid;
    
    spectralCentroid.addValue(centroid);
    
}


void audioManager::calculateOnset(){
    
}

