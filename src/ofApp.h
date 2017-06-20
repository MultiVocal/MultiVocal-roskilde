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
    void exit();
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
    std::string clientId = "RoskildeFestival-2017";
    std::string legalNotice = "By participating you allow your recording to be used in the creation of a synthesized voice - more info at multivocal.org";
    std::string instructions = "Hold the button while reading the following sentence aloud:";
    
    // sound recording
    ofxVideoRecorder audioRecorder;
    ofSoundStream soundStream;
    bool bEncodeMp3 = false;
    int inputChannels = 2;
    int outputChannels = 2;
    int sampleRate = 44100;
    double minRecordingSize = 0;

    FileUploader fileUploader;
};
