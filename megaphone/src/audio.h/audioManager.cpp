
#include "audioManager.h"

    


void audioManager::setup(){
    dywapitch_inittracking(&pitchtracker);   
    audioDataDouble = NULL;
}

void audioManager::update(float * audio, int _bufferSize){
    
    
    bufferSize = _bufferSize;
    audioData = audio;
    if (audioDataDouble == NULL){
        audioDataDouble = new double[bufferSize];
    }
    
    for (int i = 0; i < bufferSize; i++){
        audioDataDouble[i] = audioData[i];
    }
    
    calculatePitch();
    calculateRMS();

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


void audioManager::calculateFFT(){

}


void audioManager::calculateOnset(){
    
}
