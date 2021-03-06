#include <bits/stdc++.h>

using namespace std;

#define MEMORY_SIZE 1024
#define PAGE_SIZE 4

const int FIFO  = 0;
const int LRU   = 1;
const int CLOCK = 2;
const int PRE   = 3;
const int DEMAND= 4;

int pagingType;
int replacementType;

class Page{
  public:
  
    int processId;
    int pageNumber;

    int lastAccessedTime;
    int addedTime;
    int usedBit;
};

class Process {
  public:
	  int processId;
	  int totalMemory;
};

int numProcesses;

class PageTableEntry{
  public:
    int pageNumber;
    int frame;
    int valid;
};

class PageTable{
  public:
    vector<PageTableEntry>pageTable;
};

class MemoryEntry {
  public:
    int frame;
    int occupied;
    int processId;
    int pageNumber;
};

class ProcessTraceEntry {
  public:
    int processId;
    int pageNumber;
};

vector<PageTable> pageTables;
vector<Process> processes;
vector<MemoryEntry> memory;

queue<ProcessTraceEntry> processTraceList;

void readProcessList(string inputFileName){
  
  ifstream file;
  file.open(inputFileName);

  int processId, totalMemory;

  while(file>>processId>>totalMemory){

    Process process;

    process.processId = processId;
    process.totalMemory = totalMemory;

    processes.push_back(process);

  }

  cout<<"Number of Processes Read : "<<processes.size()<<endl;
  
  file.close();

}


void readProcessTrace(string inputFileName){
  
  ifstream file;
  file.open(inputFileName);

  int processId, pageNumber;

  while(file>>processId>>pageNumber){

    ProcessTraceEntry processTraceEntry;

    processTraceEntry.processId = processId;
    processTraceEntry.pageNumber = pageNumber;

    processTraceList.push(processTraceEntry);

  }


  cout<<"Number of Process Trace Read : "<<processTraceList.size()<<endl;

  file.close();

}

int main(int argc,char* argv[]){

  string processListFilename=argv[1];
  string processTraceFilename=argv[2];

  readProcessList(processListFilename);
  readProcessTrace(processTraceFilename);

  return 0;
}

