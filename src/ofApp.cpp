#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Setup font
    font.setup("font/Calibre-Semibold.ttf", 1.0, 1024, false, 8, 1.0);
    
    // Setup audio recorder
    //    audioRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg"));
    audioRecorder.setAudioCodec("mp3");
    audioRecorder.setAudioBitrate("192k");
    soundStream.setup(this, 0, inputChannels, sampleRate, 256, 4);
    
    // Load transcriptions
    transcriptions.openLocal("transcriptions.json");
    
    // Load transcription index from we left off last time
    ofxJSON json;
    json.openLocal("config.json");
    currentTranscriptionIndex = json["transcriptionIndex"].asInt();
    std::cout << transcriptions.size() << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(255);
    
    // Draw transcription text
    ofSetColor(0);
    std::string transcription = transcriptions[currentTranscriptionIndex]["transcription_text"].asString();
    int numLines;
    bool wordsWereCropped;
    int size = 38;
    int x = 0;
    int margin = 40;
    int y = ofGetHeight()/2;
    
    if(audioRecorder.isRecording()){
        ofSetColor(0);
    }else{
        ofSetColor(ofColor::darkGray);
    }
    
    font.drawMultiLineColumn(transcription, size, margin, y, ofGetWidth()-margin, numLines, false, 2,true, &wordsWereCropped,true);
    
    // Draw info
    if(bDebugDraw){
        stringstream s;
        s << "Transcription #" + ofToString(currentTranscriptionIndex);
        s << "\nFPS: " + ofToString(ofGetFrameRate());
        s << "\nQueue size: " + ofToString(audioRecorder.getVideoQueueSize());
        s << endl;
        ofDrawBitmapStringHighlight(s.str(), 10, 10);
    }
    
    // draw recording button:
    if(audioRecorder.isRecording()){
        ofSetColor(127+(sin(ofGetFrameNum()*0.03)*127),0,0);
        ofDrawEllipse(ofGetWidth()-50, 50, 25, 25);
    }
}

//--------------------------------------------------------------
void ofApp::startRecording(){
    std::string timeStampFormat = std::string("%d_%b_%Y_" + ofToString("%Hh_") + ofToString("%Mm_") + ofToString("%Ss"));
    //    Create directory if it doesn't exist
    std::string fileName = ofToString(currentTranscriptionIndex) + "_" + ofGetTimestampString(timeStampFormat) + ".mp3";
    std::string path = "audio_recordings/";
    ofDirectory dir(path);
    if(!dir.exists()){
        dir.create(true);
    }
    // Start recording
    audioRecorder.setup(path + fileName, 0,0,0, sampleRate, inputChannels); // no video
    audioRecorder.start();
    
    // Log
    ofLog(OF_LOG_NOTICE) << "Recording started for index: " + ofToString(currentTranscriptionIndex) << endl;
}

//--------------------------------------------------------------
void ofApp::stopRecording(){
    std::string path = audioRecorder.getMoviePath();
    audioRecorder.close();
    
    ofLog(OF_LOG_NOTICE) << "Recording stopped for index: " + ofToString(currentTranscriptionIndex) << endl;
    
    ofFile file;
    file.open(path);
    
    if(file.getSize() > 10000){
        // Upload        
        fileUploader.upload(path);
        
        // todo: if recording OK
        goToNextTranscription();
    }else{
        // Delete file
        file.remove();
    }
}

//--------------------------------------------------------------
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if(audioRecorder.isRecording()){
        audioRecorder.addAudioSamples(input, bufferSize, nChannels);
    }
}

//--------------------------------------------------------------
void ofApp::goToNextTranscription(){
    // Increment index
    currentTranscriptionIndex++;
    
    // Start over if limited was reached
    if(currentTranscriptionIndex >= transcriptions.size()){
        currentTranscriptionIndex = 0;
    }
    
    // Save in config
    ofxJSON json;
    json["transcriptionIndex"] = currentTranscriptionIndex;
    json.save("config.json");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if((key == 'g' || key == 'G') && !audioRecorder.isRecording()){
        startRecording();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' '){
        goToNextTranscription();
    }
    
    if((key == 'g' || key == 'G') && audioRecorder.isRecording()){
        stopRecording();
    }
    
    if(key == 'd' || key == 'D'){
        bDebugDraw = !bDebugDraw;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
