#pragma once

#include "ofMain.h"
#include "FileUploader.h"
#include "ofxFontStash.h"
#include "ofxJSON.h"
#include "ofxVideoRecorder.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void startRecording();
    void stopRecording();
    void goToNextTranscription();
    void upload(std::string path);
    void audioIn(float * input, int bufferSize, int nChannels);

    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxJSON transcriptions;
    int currentTranscriptionIndex = 0;
    ofxFontStash font;
    bool bDebugDraw = true;
    
    // sound recording
    ofxVideoRecorder audioRecorder;
    ofSoundStream soundStream;
    int inputChannels = 2;
    int sampleRate = 44100;

    FileUploader fileUploader;
};
