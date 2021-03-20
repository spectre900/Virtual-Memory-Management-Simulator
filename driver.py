import os
import subprocess

PAGE_SIZE = 2
PAGING = 'DEMAND'
REPLACEMENT = 'FIFO'

PAGE_SIZE = str(PAGE_SIZE)
os.system('g++ -I ./ simulator.cpp')

processInfo = ['./a.out', 'plist.txt', 'ptrace.txt', REPLACEMENT, PAGING, PAGE_SIZE]

backend   = subprocess.Popen(processInfo, stdout=subprocess.PIPE)
dataBytes = backend.communicate()[0]
dataStr   = dataBytes.decode('utf-8')
data      = list(map(int,dataStr.split(' ')))

print('Data received from backend is : ', data)