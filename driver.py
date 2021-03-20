import subprocess

PAGING = 'DEMAND'
REPLACEMENT = 'FIFO'

subprocess.call(['g++','simulator.cpp'])

backend   = subprocess.Popen(['./a.out', 'plist.txt', 'ptrace.txt', REPLACEMENT, PAGING], stdout=subprocess.PIPE)
dataBytes = backend.communicate()[0]
dataStr   = dataBytes.decode("utf-8")
data      = dataStr.split(' ')

print('Data received from backend is : ', data)