#include <bits/stdc++.h>
#include <ctime>

using namespace std;

#define MEMORY_SIZE 1024
#define PAGE_SIZE 2

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

    int usedBit;
    time_t addedTime;
    time_t lastAccessedTime;

    Page(){

      usedBit = 0;
      addedTime = -1;
      lastAccessedTime = -1;

    }
};

class PageTableEntry{
  public:

    int frame;
    int valid;
    int pageNumber;

    PageTableEntry(){
      valid = 0;
      frame = -1;
    }
};

class Process {
  public:
	  int processId;
	  int totalMemory;
    vector<Page>pages;
    vector<PageTableEntry>pageTable;
};

int numProcesses;

class MemoryEntry{
  public:

    int occupied;
    int processId;
    int pageNumber;

    MemoryEntry(){
      occupied = 0;
      processId = -1;
      pageNumber = -1;
    }
};

class ProcessTraceEntry {
  public:
    int processId;
    int memoryLocation;
};

vector<Process> processes;
vector<MemoryEntry> memory(MEMORY_SIZE);

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

  int processId, memoryLocation;

  while(file>>processId>>memoryLocation){

    ProcessTraceEntry processTraceEntry;

    processTraceEntry.processId = processId;
    processTraceEntry.memoryLocation = memoryLocation-1;

    processTraceList.push(processTraceEntry);

  }

  cout<<"Number of Process Trace Read : "<<processTraceList.size()<<endl;

  file.close();

}

void replacement(){

  int firstComeFrame = 0;
  time_t firstComeTime = processes[memory[0].processId].pages[memory[0].pageNumber].addedTime;

  for(int i=PAGE_SIZE;i<MEMORY_SIZE;i+=PAGE_SIZE){

      if(processes[memory[i].processId].pages[memory[i].pageNumber].addedTime < firstComeTime){

        firstComeTime = processes[memory[i].processId].pages[memory[i].pageNumber].addedTime;
        firstComeFrame = i/PAGE_SIZE;

      }
  }

  for(int i=0;i<PAGE_SIZE;i++){

    memory[firstComeFrame*PAGE_SIZE+i].occupied = 0;
    memory[firstComeFrame*PAGE_SIZE+i].processId = -1;
    memory[firstComeFrame*PAGE_SIZE+i].pageNumber = -1;

  }

}

void load(int processId, int pageNumber){

  bool loaded = false;

  for(int i=0;i<MEMORY_SIZE;i+=PAGE_SIZE){
    
    if(!memory[i].occupied){

      loaded = true;

      processes[processId].pageTable[pageNumber].valid = 1;
      processes[processId].pageTable[pageNumber].frame = i/PAGE_SIZE;

      processes[processId].pages[pageNumber].addedTime = time(0);

      for(int j=0;j<PAGE_SIZE;j++){

        memory[i+j].occupied = 1;
        memory[i+j].processId = processId;
        memory[i+j].pageNumber = pageNumber;

      }

      break;

    }

  }

  if(!loaded){

    replacement();
    load(processId, pageNumber);

  }

}

int main(int argc,char* argv[]){

  string processListFilename=argv[1];
  string processTraceFilename=argv[2];

  readProcessList(processListFilename);
  readProcessTrace(processTraceFilename);

  numProcesses = processes.size();

  for(int i=0;i<numProcesses;i++){

    int pageCount = processes[i].totalMemory/PAGE_SIZE;

    processes[i].pages = vector<Page>(pageCount);

    for(int j=0;j<pageCount;j++){
      processes[i].pages[j].pageNumber = j;
      processes[i].pages[j].processId = i;
    }
    
    processes[i].pageTable = vector<PageTableEntry>(pageCount);
    for(int j=0;j<pageCount;j++){
      processes[i].pageTable[j].pageNumber = j;
    }

  }

  while(!processTraceList.empty()){

    ProcessTraceEntry ptrace = processTraceList.front();
    processTraceList.pop();

    int processNumber = ptrace.processId;
    int pageNumber = ptrace.memoryLocation/PAGE_SIZE;

    if(!processes[processNumber].pageTable[pageNumber].valid){
      load(processNumber,pageNumber);
    }

  }

  return 0;
}