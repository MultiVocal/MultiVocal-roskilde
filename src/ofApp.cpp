#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Setup font
    font.setup("font/Calibre-Semibold.ttf", 1.0, 1024, false, 8, 1.0);
    
    // Load transcriptions
    transcriptions.openLocal("transcriptions.json");
    
    // Load transcription index from we left off last time
    ofxJSON jsonConfig;
    jsonConfig.openLocal("config.json");
    currentTranscriptionIndex = jsonConfig["transcriptionIndex"].asInt();
    
    // Setup audio recorder
    //    audioRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg"));
    bEncodeMp3 = jsonConfig["mp3_ecode"].asBool();
    if(bEncodeMp3){
        audioRecorder.setAudioCodec("mp3");
        audioRecorder.setAudioBitrate("192k");
    }else{
        audioRecorder.setAudioCodec("pcm_s16le");
        audioRecorder.setAudioBitrate("1411k");
    }
    soundStream.setup(this, 0, inputChannels, sampleRate, 256, 4);
    
    // setup uploader
    bool uploadSetup = fileUploader.setup(clientId, jsonConfig["endpoint_url"].asString());
    if(!uploadSetup){
        ofLog(OF_LOG_ERROR) << "fileUploader error" << endl;
        exit();
    }
    
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
    fileUploader.update();
}

//--------------------------------------------------------------
void ofApp::exit(){
    // Save progress in config file
    ofxJSON json;
    json.openLocal("config.json");
    json["transcriptionIndex"] = currentTranscriptionIndex;
    json.save("config.json");
    
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
        s << "\nAudio queue size: " + ofToString(audioRecorder.getVideoQueueSize());
        s << "\nFile queue size: " + ofToString(fileUploader.getQueueSize());
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
    
    std::string fileExt;
    if(bEncodeMp3){
        fileExt = ".mp3";
    }else{
        fileExt = ".wav";
    }
    std::string fileName = ofToString(currentTranscriptionIndex) + "_" + ofGetTimestampString(timeStampFormat) + fileExt;
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
    
    
    ofFile file;
    file.open(path);
    
    if(file.getSize() > 10000){
        // Upload
        fileUploader.addFile(path, transcriptions[currentTranscriptionIndex]["transcription_id"].asString());
        
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
