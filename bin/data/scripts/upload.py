#!/usr/bin/python
import getopt
import json
import os
import sys
import urllib2

import numpy as np
import requests


def main(argv):
    url = ""
    file_name = ""
    transcription_id = ""
    client_id = ""

    try:
        opts, args = getopt.getopt(
            argv, "hf:u:t:c:", ["file=", "client_id="])
    except getopt.GetoptError:
        print('test.py -i <inputfile> -o <outputfile>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-f", "--file"):
            file_name = arg
        elif opt in ("-c", "--client_id"):
            client_id = arg
        elif opt in ("-t", "--transcription_id"):
            transcription_id = arg

    try:
        urllib2.urlopen('http://www.google.com', timeout=1)
    except urllib2.URLError as err:
        print(123)  # no internet
        print('wtf')
        exit()
    try:
        # print(file_name + "m")
        file_ = {'file': (file_name, open(file_name, 'rb'))}
        # file_name = str(np.random.randint(0, 124))
        url = 'https://multivocal.org/api/recording/upload/'
        r = requests.post(url, files=file_)
        r_json = r.json()
        file_name = r_json["file_name"]
        url = 'https://multivocal.org/api/recording/submit/'
        r = requests.post(url, json={"transcription_id":
                                     transcription_id,
                                     "client_id":
                                     client_id,
                                     "file_name": file_name})
        print(r.status_code)
    except (IOError, ValueError):
        print(666)  # File not found


if __name__ == "__main__":
    main(sys.argv[1:])
