//
//  TranscriptionRecorder.h
//  MultiVocal-roskilde
//
//  Created by Frederik Tollund Juutilainen on 05/06/2017.
//
//

#ifndef TranscriptionRecorder_h
#define TranscriptionRecorder_h

#include "ofxCenteredTrueTypeFont.h";

class TranscriptionRecorder{
public:
    TranscriptionRecorder(){
        // load transcriptions from local file
        transcriptions.openLocal("txt.done.data.json");

        // load font
        font.load("font/Calibre-Semibold.ttf",26);
    };
    
    void update(){
        
    }
    
    void draw(){
        ofSetColor(0);
        std::string transcription = transcriptions[currentTranscriptionIndex]["transcription_text"].asString();
        float fontWidth = font.stringWidth(transcription);
        float widthScale = (ofGetWidth()-150)/fontWidth;
        
        std::cout << widthScale << endl;
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofScale(widthScale,widthScale);
        font.drawStringCentered(transcription,0,0);
        ofPopMatrix();
    };
    
    void goToNextTranscription(){
        currentTranscriptionIndex++;
    }
    
    
    
private:
    // Graphics
    ofxCenteredTrueTypeFont font;
    ofColor textColor = ofColor::blue;
    
    // Sounds
    
    //TTS
    ofxJSON transcriptions;
    int currentTranscriptionIndex;

};


#endif /* TranscriptionRecorder_h */
