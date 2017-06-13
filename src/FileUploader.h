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
#include <curl/curl.h>

struct file{
    std::string transcriptionId;
    std::string path;
};

class FileUploader : public ofThread{
public:
    FileUploader(std::string clientId){
        // set client id
        this->clientId = clientId;
        
        
        // Open local queue
        ofxJSON jsonQueue;
        jsonQueue.openLocal("upload_queue.json");
        
        // TODO: iterate backwards
        for(auto& i : jsonQueue){
            file file;
            file.path = i["path"].asString();
            file.transcriptionId = i["transcriptionId"].asString();
            queue.push(file);
        }
    }
    
    FileUploader(const FileUploader &obj){
        
    };
    
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
    
private:
    bool upload(file file){
        bool uploadSuccess = true;

        ofFilePath path;
        std::string url = "https://madsnewsio.eu.ngrok.io/api/recording";
        
        auto res = ofSystem("/usr/local/bin/python " + path.getAbsolutePath("scripts/upload.py") +
                            " -f"  + file.path + " -u " + url);
        
        std::cout << res << endl;
        
        return uploadSuccess;
    }
    
    void threadedFunction(){
        while(queue.size() != 0){
            bool uploaded = upload(queue.front());
            if(uploaded){
                queue.pop();
            }
        }
    };

    std::queue<file> queue;
    std::string clientId = "";
};


#endif /* FileUploader_h */
