#!/usr/bin/python
import os
import sys
import requests
import json
import sys
import getopt
from pprint import pprint


scenario_uuid = '8fe4118729ec4e908f8212563d759fff'

payload = {
    'uuid': scenario_uuid,
    'tasks': [
        {
            'uuid': '4b2fd25ef4bf49e09e311f532fe9246a',
            'file': 'Task_0'
        }, {
            'uuid': '7196a0cfb2b544d28442099406d7b064',
            'file': 'Task_1'
        }, {
            'uuid': 'e9eebf5f030143209c16e7111207816e',
            'file': 'Task_2'
        }
    ]
}

if "Task_0.mp3" in sys.argv[1]:
    file_uuid = '4b2fd25ef4bf49e09e311f532fe9246a'
    file_name = 'Task_0'
elif "Task_1.mp3" in sys.argv[1]:
    file_uuid = '7196a0cfb2b544d28442099406d7b064'
    file_name = 'Task_1'
elif "Task_2.mp3" in sys.argv[1]:
    file_uuid = 'e9eebf5f030143209c16e7111207816e'
    file_name = 'Task_2'
else:
    print 'File error for ' + sys.argv[1]

payload['tasks'] = [{
        'uuid': file_uuid,
        'file': file_name
        }]

files = {file_name: (sys.argv[1], open(sys.argv[1],'rb'))}
# print json.dumps(payload)

response = requests.post('https://digitalebydele.alexandra.dk/phone/answer/', data={'scenario': json.dumps(payload)}, files=files)
print(response.json())
