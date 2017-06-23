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
        opts, args = getopt.getopt(
            argv, "hf:u:t:", ["file=", "url=", "transcription_id="])
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
        elif opt in ("-t", "--transcription_id"):
            transcription_id = arg
    try:
        file_ = {'file': (file_name, open(file_name, 'rb'))}
        r = requests.post(url, files=file_, data={"transcription_id":
                                                  transcription_id})
        print (r.status_code)
    except IOError:
        file_ = ""
        print(666)


if __name__ == "__main__":
    main(sys.argv[1:])
