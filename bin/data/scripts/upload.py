#!/usr/bin/python
import requests
import json
import sys
import getopt
import os


def main(argv):
    url = ""
    file_name = ""
    transcription_id = ""

    try:
        opts, args = getopt.getopt(argv, "hf:u:", ["file=", "url="])
    except getopt.GetoptError:
        print 'test.py -i <inputfile> -o <outputfile>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'test.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-f", "--file"):
            file_name = arg
        elif opt in ("-u", "--url"):
            url = arg

    if file_name.endswith('.mp3'):
        transcription_id = file_name.replace(".mp3", "")
    if file_name.endswith('.wav'):
        transcription_id = file_name.replace(".wav", "")

    transcription_id = os.path.basename(transcription_id)
    print(transcription_id)

    file_ = {'file': (file_name, open(file_name, 'rb'))}
    r = requests.post(url, files=file_, data={"transcription_id":
                                              transcription_id})

    print (r.text)


if __name__ == "__main__":
    main(sys.argv[1:])
