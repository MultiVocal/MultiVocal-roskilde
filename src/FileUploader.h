//
//  FileUploader.h
//  MultiVocal-roskilde
//
//  Created by Frederik Tollund Juutilainen on 11/06/2017.
//
//

#ifndef FileUploader_h
#define FileUploader_h

#include "ofMain.h"
#include "ofxJSON.h"

class FileUploader : public ofThread{
public:
    FileUploader(){
//        jsonQueue.openLocal("upload_queue.json");
        
        // TODO: iterate backwards
        for(auto& i : jsonQueue){
            queue.push(i.asString());
        }
    }
    
    ~FileUploader(){
        jsonQueue.save("upload_queue.json");
    }
    
    void addFile(std::string path){
        // Add to queue
        queue.push(path);
        
        // Add to JSON-queue
        Json::Value value = path;
        jsonQueue.append(value);
    }
    
    int getQueueSize(){
        return queue.size();
    }
    
    void update(){
        if(!this->isThreadRunning()){
            this->startThread();
        }
    }
    
private:
    bool upload(std::string path){
        std::string compareStr = "{\"status\":0,\"message\":\"succesfully added file\"}";
        std::string command;
        command = "curl -X POST http://madsnewsio.eu.ngrok.io/transcription  'content-type: multipart/form-data;' -F transcription_id=arctic_a0001 -F 'transcription_text=other stuff' -F file=@";
        command += path;
//        string returnString = ofSystem(command);
//        ofLog(OF_LOG_NOTICE) << "Uploaded file " << path;
        
        std::string resp = exec(command.c_str());
        
        std::cout << "boolean: " << resp << endl;
        
        return true;
    }
    
    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != NULL)
                result += buffer.data();
        }
        return result;
    }
    
    void threadedFunction(){
        while(queue.size() != 0){
            bool uploaded = upload(queue.front());
            
            if(uploaded){
                queue.pop();
            }
        }
    };

    
    std::queue<std::string> queue;
    ofxJSON jsonQueue;
    
};


#endif /* FileUploader_h */
