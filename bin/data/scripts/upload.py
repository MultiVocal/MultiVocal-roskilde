#!/usr/bin/python
import requests
import json
import sys
import getopt
import os
import urllib2


def main(argv):
    url = ""
    file_name = ""
    transcription_id = ""
    client_id = ""

    try:
        opts, args = getopt.getopt(
            argv, "hf:u:t:c:", ["file=", "url=", "transcription_id=",
                                "client_id="])
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
        elif opt in ("-c", "--client_id"):
            client_id = arg
        elif opt in ("-t", "--transcription_id"):
            transcription_id = arg

    try:
        urllib2.urlopen('http://www.google.com', timeout=1)
    except urllib2.URLError as err:
        print(123)  # no internet
        exit()
    try:
        file_ = {'file': (file_name, open(file_name, 'rb'))}
        r = requests.post(url, files=file_, data={"transcription_id":
                                                  transcription_id,
                                                  "client_id":
                                                  client_id})
        print (r.status_code)
    except IOError:
        print(666)  # File not found
    except:
        print(123)  # All other errors


if __name__ == "__main__":
    main(sys.argv[1:])
