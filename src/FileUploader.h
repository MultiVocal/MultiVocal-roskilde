//
//  FileUploader.h
//  MultiVocal-roskilde
//
//  Created by Frederik Tollund Juutilainen on 11/06/2017.
//
//

#ifndef FileUploader_h
#define FileUploader_h

class FileUploader : public ofThread{
public:
    bool upload(std::string path){
        std::string command;
        command = "curl -X POST http://madsnewsio.eu.ngrok.io/transcription  'content-type: multipart/form-data;' -F transcription_id=arctic_a0001 -F 'transcription_text=other stuff' -F file=@";
        command += path;
        string returnString = ofSystem(command);
        
        ofLog(OF_LOG_NOTICE) << "Uploaded file " << path;
    }
    
    void threadedFunction(){};
    
    
};


#endif /* FileUploader_h */
