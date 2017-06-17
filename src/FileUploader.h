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

struct file{
    std::string transcriptionId;
    std::string path;
};

class FileUploader : public ofThread{
public:
    bool setup(std::string clientId, std::string url){
        // set client id
        this->clientId = clientId;
        this->url = url;
        if(url == "" || clientId == ""){
            std::cout << "No url assigned in config" << endl;
            return false;
        }
        
        // Open local queue
        ofxJSON jsonQueue;
        jsonQueue.openLocal("upload_queue.json");
        
        for(auto& i : jsonQueue){
            file file;
            file.path = i["path"].asString();
            file.transcriptionId = i["transcriptionId"].asString();
            queue.push(file);
        }
        
        return true;
    }
    
    ~FileUploader(){
        // Add all elements to JSON
        ofxJSON json;
        
        // Add to JSON-queue
        while(queue.size() != 0){
            ofxJSONElement element;
            element["path"] = queue.front().path;
            element["transcriptionId"] = queue.front().transcriptionId;
            json.append(element);
            queue.pop();
        }
        json.save("upload_queue.json");
    }
    
    void addFile(std::string path, std::string transcriptionId){
        // Add to queue
        file file;
        file.path = path;
        file.transcriptionId = transcriptionId;
        queue.push(file);
    }
    
    int getQueueSize(){
        return queue.size();
    }
    
    void update(){
        if(!this->isThreadRunning()){
            this->startThread();
        }
    }
    
    std::string getLastUploadCode(){
        return lastUploadCode;
    }
    
    std::string getUrl(){
        return url;
    }
    
private:
    bool upload(file file){
        bool uploadSuccess = false;
        ofFilePath path;
        std::string res = ofSystem("/usr/local/bin/python " + path.getAbsolutePath("scripts/upload.py") +
                            " -f"  + file.path + " -u " + this->url + " -t " + file.transcriptionId);
        uploadSuccess = ofToInt(res) == 200;
        lastUploadCode = res;
        if(lastUploadCode == ""){
            lastUploadCode = "Py error";
        }
        return uploadSuccess;
    }
    
    void threadedFunction(){
        while(queue.size() != 0){
            bool uploaded = upload(queue.front());
            if(uploaded){
                ofFile file = ofFile(queue.front().path);
                file.remove();
                queue.pop();
            }
        }
    };

    std::queue<file> queue;
    std::string clientId = "";
    std::string url = "";
    std::string lastUploadCode = "n/a";
};


#endif /* FileUploader_h */
