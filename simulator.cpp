#include <bits/stdc++.h>
#include <ctime>

using namespace std;
using namespace chrono;

#define MEMORY_SIZE 512
#define PAGE_SIZE 2

const int FIFO  = 0;
const int LRU   = 1;
const int CLOCK = 2;
const int PRE   = 3;
const int DEMAND= 4;

int pagingType = DEMAND;
int replacementType = FIFO;

auto start = chrono::high_resolution_clock::now();

class Page{
  public:
  
    int processId;
    int pageNumber;

    long long addedTime;
    long long lastAccessedTime;

    Page(){
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
    int usedBit;
    
    MemoryEntry(){
      occupied = 0;
      processId = -1;
      pageNumber = -1;
      usedBit = 0;
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

int pointer = 0;

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

  cout<<processes.size()<<' ';
  
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

  cout<<processTraceList.size()<<' ';

  file.close();

}

void fifoReplace(){

  int firstComeFrame = 0;
  long long firstComeTime = processes[memory[0].processId].pages[memory[0].pageNumber].addedTime;

  for(int i=PAGE_SIZE;i<MEMORY_SIZE;i+=PAGE_SIZE){

      if(processes[memory[i].processId].pages[memory[i].pageNumber].addedTime < firstComeTime){

        firstComeTime = processes[memory[i].processId].pages[memory[i].pageNumber].addedTime;
        firstComeFrame = i/PAGE_SIZE;

      }
  }

  int processId = memory[firstComeFrame*PAGE_SIZE].processId;
  int pageNumber = memory[firstComeFrame*PAGE_SIZE].pageNumber;

  for(int i=0;i<PAGE_SIZE;i++){

    memory[firstComeFrame*PAGE_SIZE+i].occupied = 0;
    memory[firstComeFrame*PAGE_SIZE+i].processId = -1;
    memory[firstComeFrame*PAGE_SIZE+i].pageNumber = -1;

  }

  processes[processId].pageTable[pageNumber].valid = 0;
  processes[processId].pageTable[pageNumber].frame = -1;
  processes[processId].pages[pageNumber].addedTime = -1;
  processes[processId].pages[pageNumber].lastAccessedTime = -1;

}

void lruReplace(){

  int firstAccessedFrame = 0;
  long long firstAccessedTime = processes[memory[0].processId].pages[memory[0].pageNumber].lastAccessedTime;

  for(int i=PAGE_SIZE;i<MEMORY_SIZE;i+=PAGE_SIZE){

      if(processes[memory[i].processId].pages[memory[i].pageNumber].lastAccessedTime < firstAccessedTime){

        firstAccessedTime = processes[memory[i].processId].pages[memory[i].pageNumber].lastAccessedTime;
        firstAccessedFrame = i/PAGE_SIZE;

      }
  }

  int processId = memory[firstAccessedFrame*PAGE_SIZE].processId;
  int pageNumber = memory[firstAccessedFrame*PAGE_SIZE].pageNumber;

  for(int i=0;i<PAGE_SIZE;i++){

    memory[firstAccessedFrame*PAGE_SIZE+i].occupied = 0;
    memory[firstAccessedFrame*PAGE_SIZE+i].processId = -1;
    memory[firstAccessedFrame*PAGE_SIZE+i].pageNumber = -1;

  }

  processes[processId].pageTable[pageNumber].valid = 0;
  processes[processId].pageTable[pageNumber].frame = -1;
  processes[processId].pages[pageNumber].addedTime = -1;
  processes[processId].pages[pageNumber].lastAccessedTime = -1;

}

void clockReplace()
{
    while(memory[pointer*PAGE_SIZE].usedBit != 0)
    {
        for(int i=0; i<PAGE_SIZE; i++){
          memory[pointer*PAGE_SIZE + i].usedBit = 0;
        }

        int frame_count = MEMORY_SIZE/PAGE_SIZE;
        pointer=(pointer+1)%frame_count;
    }

    if(memory[pointer*PAGE_SIZE].occupied){
        int frame_processId = memory[pointer*PAGE_SIZE].processId;
        int frame_pageNumber = memory[pointer*PAGE_SIZE].pageNumber;

        for(int i=0;i<PAGE_SIZE;i++){

        memory[pointer*PAGE_SIZE+i].occupied = 0;
        memory[pointer*PAGE_SIZE+i].processId = -1;
        memory[pointer*PAGE_SIZE+i].pageNumber = -1;

        }
        
        processes[frame_processId].pageTable[frame_pageNumber].valid = 0;
        processes[frame_processId].pageTable[frame_pageNumber].frame = -1;
    }
}

void replacement(){
  if (replacementType==FIFO){
    fifoReplace();
  }
  else if(replacementType==LRU){
    lruReplace();
  }
  else if(replacementType == CLOCK)
  {
      clockReplace();
  }
}

void load(int processId, int pageNumber){

  bool loaded = false;

  if(replacementType==FIFO or replacementType==LRU){
    for(int i=0;i<MEMORY_SIZE;i+=PAGE_SIZE){
      if(!memory[i].occupied){

        loaded = true;

        processes[processId].pageTable[pageNumber].valid = 1;
        processes[processId].pageTable[pageNumber].frame = i/PAGE_SIZE;

        auto elapsed = high_resolution_clock::now() - start;

        processes[processId].pages[pageNumber].addedTime = duration_cast<nanoseconds>(elapsed).count();
        processes[processId].pages[pageNumber].lastAccessedTime = duration_cast<nanoseconds>(elapsed).count();

        for(int j=0;j<PAGE_SIZE;j++){

          memory[i+j].occupied = 1;
          memory[i+j].processId = processId;
          memory[i+j].pageNumber = pageNumber;

        }

        break;

      }

    }
  }
  else if(replacementType==CLOCK){
    
    if(memory[pointer*PAGE_SIZE].usedBit == 0){
                

        processes[processId].pageTable[pageNumber].valid = 1;
        processes[processId].pageTable[pageNumber].frame = pointer;
                
        for(int i=0;i<PAGE_SIZE;i++){

          memory[pointer*PAGE_SIZE+i].occupied = 1;
          memory[pointer*PAGE_SIZE+i].processId = processId;
          memory[pointer*PAGE_SIZE+i].pageNumber = pageNumber;
          memory[pointer*PAGE_SIZE+i].usedBit = 1;

        }
        loaded = true;
    }

    
    
  }

  if(!loaded){

    replacement();
    load(processId, pageNumber);

  }

}

int findNext(int processId,int pageNumber)
{
  int nextPageNumber=-1,flag=0;
  int pageCount = ceil(float(processes[processId].totalMemory)/PAGE_SIZE);
  for(int i=pageNumber+1;i<pageCount;i++)
  {
    if(!processes[processId].pageTable[i].valid)
    {
      nextPageNumber=i;
      flag=1;
      break;
    }
  }
  if(flag==0)
  {
    for(int i=0;i<pageNumber;i++)
    {
      if(!processes[processId].pageTable[i].valid)
      {
        nextPageNumber=i;
        break;
      }
    }
  }
  return nextPageNumber;
}

void prePaging(int processId, int pageNumber){
  int nextPageNumber;
  load(processId,pageNumber);
  nextPageNumber = findNext(processId,pageNumber);
  if(nextPageNumber!=-1){
    load(processId,nextPageNumber);
  }
}

void demandPaging(int processId, int pageNumber){
  load(processId, pageNumber);
}

void paging(int processId, int pageNumber){
  if(pagingType==PRE){
    prePaging(processId, pageNumber);
  }
  else if(pagingType==DEMAND){
    demandPaging(processId, pageNumber);
  }
}

int main(int argc,char* argv[]){

  string processListFilename=argv[1];
  string processTraceFilename=argv[2];
  string replacementParam =argv[3];
  string pagingParam = argv[4];

  if(pagingParam=="DEMAND") pagingType=DEMAND;
  else if(pagingParam=="PRE") pagingType=PRE;
  else pagingType=DEMAND;

  if(replacementParam=="FIFO") replacementType=FIFO;
  else if(replacementParam=="LRU") replacementType=LRU;
  else if(replacementParam=="CLOCK") replacementType=CLOCK;
  else replacementType=FIFO;

  readProcessList(processListFilename);
  readProcessTrace(processTraceFilename);

  numProcesses = processes.size();

  for(int i=0;i<numProcesses;i++){

    int pageCount = ceil(float(processes[i].totalMemory)/PAGE_SIZE);

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

  int pageFault=0;
  while(!processTraceList.empty()){

    ProcessTraceEntry ptrace = processTraceList.front();
    processTraceList.pop();

    int processNumber = ptrace.processId;
    int pageNumber = ptrace.memoryLocation/PAGE_SIZE;


    if(!processes[processNumber].pageTable[pageNumber].valid){
      pageFault++;
      paging(processNumber,pageNumber);
    }
    else{

      auto elapsed = high_resolution_clock::now() - start;
      processes[processNumber].pages[pageNumber].lastAccessedTime = duration_cast<nanoseconds>(elapsed).count();

      int frame = processes[processNumber].pageTable[pageNumber].frame;

      for(int i=0; i<PAGE_SIZE; i++){
        memory[frame*PAGE_SIZE + i].usedBit = 1;
      }
    }
  }

  cout<<pageFault;

  return 0;
}