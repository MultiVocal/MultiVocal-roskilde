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
    }

    void saveQueueToFile(){
        // Delete old jsonQueue if current queue is empty
        if(queue.empty()){
            ofFile localQueue = ofFile("upload_queue.json");
            localQueue.remove();
        }else{
            // Add all elements to JSON
            ofxJSON json;
            std::queue<file> tempQueue = queue; //copy the original queue to the temporary queue

            // Add to JSON-queue
            while(!tempQueue.empty()){
                ofxJSONElement element;
                element["path"] = queue.front().path;
                element["transcriptionId"] = queue.front().transcriptionId;
                json.append(element);
                tempQueue.pop();
            }
            json.save("upload_queue.json");
        }
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
        std::string res;
        std::string pythonVersion;

#ifdef TARGET_OSX
        pythonVersion = "/usr/local/bin/python " + path.getAbsolutePath("scripts/upload.py");
#elif __arm__
        pythonVersion = "/usr/bin/python " + path.getAbsolutePath("scripts/upload.py");
#else
        pythonVersion = "/usr/bin/python " + path.getAbsolutePath("scripts/upload.py");
#endif

        res = ofSystem(pythonVersion +
                       " -f"  + file.path + " -u " + this->url + " -t " + file.transcriptionId + " -c " + this->clientId);

        uploadSuccess = ofToInt(res) == 200;

        if(ofToInt(res) == 666){
            // fileNotFound remove from queue
            uploadSuccess = true;
        }

        if(uploadSuccess){
            ofLog(OF_LOG_NOTICE) << "Successfully uploaded: " << file.transcriptionId << endl;
        }

        lastUploadCode = res;
        return uploadSuccess;
    }

    void threadedFunction(){
        while(queue.size() != 0){
            // Try uploading
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
