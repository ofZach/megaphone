

#pragma once

#include "ofMain.h"
#include    <stdio.h>
#include    "sndfile.h"

#define BUFFER_LEN      1024
#define MAX_CHANNELS    6

class audioSampleLoader {
    
public: 
    
    vector < float > audioData;
    float data [BUFFER_LEN] ;
    SNDFILE      *infile;
    SF_INFO      sfinfo ;
    int          readcount ;
    int          channels;
    
    
    bool bPlaying;
    int playbackCount;
    
    
    //
    void loadFile( string fileName ){
        
        
        if (! (infile = sf_open( ofToDataPath(fileName).c_str(), SFM_READ, &sfinfo)))
        {   /* Open failed so print an error message. */
            printf ("Not able to open input file %s.\n", "input.wav") ;
            /* Print the error message fron libsndfile. */
            sf_perror (NULL) ;
           
        } ;
        
        channels = sfinfo.channels;
        
        if (sfinfo.channels > MAX_CHANNELS)
        {   printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
            
        } ;
        
       
        while ((readcount = sf_read_float(infile, data, BUFFER_LEN))){   
            
            for (int i = 0; i < readcount; i++){
                audioData.push_back(data[i]);
            }
        } ;
        
        sf_close (infile) ;
            
        printf("loaded audio %i samples \n", audioData.size());
        
        bPlaying = false;
        playbackCount = 0;
    }
    
    
    
};