import sys
import os
import subprocess
import shutil
import matplotlib.pyplot as plt
plt.rcParams['figure.figsize'] = [16, 9]
plt.rcParams.update({'font.size': 18})
plt.style.use('dark_background')
from tkinter import *
from tkinter.ttk import Progressbar

totalProgress = 0
CONTR_PROCESS = 7
CONTR_PLOT = 3


def generateStatistics_Plot1():
    global pageFaults, PATH_TO_PROCESS_LIST, PATH_TO_PROCESS_TRACE, REPLACEMENT, PAGING
    global totalProgress

    pageSizeList = ['1', '2', '4', '8', '16', '32']
    pageFaultList = []

    for i, pageSize in enumerate(pageSizeList):
        if pageSize == PAGE_SIZE:
            pageFaultList.append(pageFaults)
            pageIndex = i
        else:
            processInfo = ['./a.out', PATH_TO_PROCESS_LIST, PATH_TO_PROCESS_TRACE, REPLACEMENT, PAGING, pageSize, '0']

            process   = subprocess.Popen(processInfo, stdout=subprocess.PIPE)
            dataBytes = process.communicate()[0]
            dataStr   = dataBytes.decode('utf-8')
            data      = list(map(int,dataStr.split(' ')))
            pageFaultList.append(data[2])
        
        totalProgress += CONTR_PROCESS
        updateProgressBar()

    return pageSizeList, pageFaultList, pageIndex

def generateStatistics_Plot2():
    global pageFaults, PATH_TO_PROCESS_LIST, PATH_TO_PROCESS_TRACE, PAGE_SIZE
    global totalProgress
    
    simCombinations = [['DEMAND', 'FIFO'], ['DEMAND', 'LRU'], ['DEMAND', 'CLOCK'], ['PRE', 'FIFO'], ['PRE', 'LRU'], ['PRE', 'CLOCK']]

    for i, combination in enumerate(simCombinations):
        if combination[0] == PAGING and combination[1] == REPLACEMENT:
            simCombinations[i].append(pageFaults)
            mainIndex = i
        else:
            processInfo = ['./a.out', PATH_TO_PROCESS_LIST, PATH_TO_PROCESS_TRACE, combination[1], combination[0], PAGE_SIZE, '0']

            process   = subprocess.Popen(processInfo, stdout=subprocess.PIPE)
            dataBytes = process.communicate()[0]
            dataStr   = dataBytes.decode('utf-8')
            data      = list(map(int,dataStr.split(' ')))
            simCombinations[i].append(data[2])

        totalProgress += CONTR_PROCESS
        updateProgressBar()

    combinationList = [combination[0] + ' + ' + combination[1] for combination in simCombinations]
    pageFaultList = [combination[2] for combination in simCombinations]

    return combinationList, pageFaultList, mainIndex



def createPlot1():
    global totalProgress
    pageSizeList, pageFaultList, pageIndex = generateStatistics_Plot1()
    
    # Figure Size
    _, ax = plt.subplots(figsize =(16, 9))
    
    # Horizontal Bar Plot
    bars = plt.barh(pageSizeList, pageFaultList)  
    bars[pageIndex].set_color('r')  
    ax.spines['right'].set_color('black')
    ax.spines['top'].set_color('black')
    ax.xaxis.set_tick_params(pad = 5)
    ax.yaxis.set_tick_params(pad = 10)
    ax.grid(b = True, color ='grey', linestyle ='-.', linewidth = 0.5, alpha = 0.2)
    ax.invert_yaxis()
    
    # Add annotation to bars
    for i in ax.patches:
        plt.text(i.get_width()+0.2, i.get_y()+0.45, str(round((i.get_width()), 2)), color ='white')

    # plt.title('Number of pagefaults VS Page size', fontweight ="bold")
    plt.ylabel('Page size')
    plt.xlabel('Number of page faults')
    plt.tight_layout()

    # Save figure
    plt.savefig('./Plots/plot1.png')

    totalProgress += CONTR_PLOT
    updateProgressBar()


def createPlot2():
    global totalProgress
    combinationList, pageFaultList, mainIndex = generateStatistics_Plot2()

    # Figure Size
    _, ax = plt.subplots(figsize =(16, 9))
    
    # Horizontal Bar Plot
    bars = plt.barh(combinationList, pageFaultList) 
    bars[mainIndex].set_color('r')  
    ax.spines['right'].set_color('black')
    ax.spines['top'].set_color('black')
    ax.xaxis.set_tick_params(pad = 5)
    ax.yaxis.set_tick_params(pad = 10)
    ax.grid(b = True, color ='grey', linestyle ='-.', linewidth = 0.5, alpha = 0.2)
    ax.invert_yaxis()
    
    # Add annotation to bars
    for i in ax.patches:
        plt.text(i.get_width()+0.2, i.get_y()+0.45, str(round((i.get_width()), 2)), color ='white')

    # plt.title('Number of pagefaults for different paging and replacement methods', fontweight ="bold")
    plt.ylabel('Different combinations of paging and replacement methods')
    plt.xlabel('Number of pagefaults')
    plt.tight_layout()

    # Save figure
    plt.savefig('./Plots/plot2.png')

    totalProgress += CONTR_PLOT
    updateProgressBar()


def executeMainRequest():
    global totalProgress, CONTR_PROCESS

    os.system('g++ -I ./ simulator.cpp')

    processInfo = ['./a.out', PATH_TO_PROCESS_LIST, PATH_TO_PROCESS_TRACE, REPLACEMENT, PAGING, PAGE_SIZE, '1']

    backend   = subprocess.Popen(processInfo, stdout=subprocess.PIPE)
    dataBytes = backend.communicate()[0]
    dataStr   = dataBytes.decode('utf-8')
    data      = list(map(int,dataStr.split(' ')))

    global processCount
    global memoryRequestCount
    global pageFaults
    global pageFaultTracker

    processCount = data[0]
    memoryRequestCount = data[1]
    pageFaults = data[2]
    pageFaultTracker = data[3:]

    totalProgress += CONTR_PROCESS
    updateProgressBar()


def printData():
    global REPLACEMENT, PAGING, PAGE_SIZE
    global processCount, memoryRequestCount, pageFaults
    print(processCount, memoryRequestCount, PAGING, REPLACEMENT, PAGE_SIZE, pageFaults, end='')


def updateProgressBar():
    global progress, totalProgress
    progress['value']=totalProgress
    ProgressWin.update_idletasks()


def destroyProgressBar():
    global ProgressWin
    ProgressWin.destroy()


def main():
    updateProgressBar()
    # Arguments
    argData = sys.argv

    global PAGING, REPLACEMENT, PATH_TO_PROCESS_LIST, PATH_TO_PROCESS_TRACE, PAGE_SIZE, progress, totalProgress

    PAGING = argData[1]
    REPLACEMENT = argData[2]
    PATH_TO_PROCESS_LIST = argData[3]
    PATH_TO_PROCESS_TRACE = argData[4]
    PAGE_SIZE = argData[5]

    # Creating directory to store plots
    dir = './Plots'
    if os.path.exists(dir):
        shutil.rmtree(dir)
    os.makedirs(dir)

    # Executing all functions
    executeMainRequest()
    createPlot1()
    createPlot2()  

    totalProgress += 3
    updateProgressBar()

    printData()
    ProgressWin.destroy()


if __name__ == '__main__':

    # creating tkinter window 
    ProgressWin = Tk() 
    ProgressWin.title('Virtual memory management simulator- Processing')
    ProgressWin.config(bg = 'black')

    # Defining attributes of root window
    ProgressWin.resizable(False, False)  # This code helps to disable windows from resizing

    window_height = 150
    window_width = 500

    screen_width = ProgressWin.winfo_screenwidth()
    screen_height = ProgressWin.winfo_screenheight()

    x_cordinate = int((screen_width/2) - (window_width/2))
    y_cordinate = int((screen_height/2) - (window_height/2))

    ProgressWin.geometry("{}x{}+{}+{}".format(window_width, window_height, x_cordinate, y_cordinate))

    # Creating a main frame inside the root window
    main_frame=Frame(ProgressWin,relief=GROOVE, bg = 'black')
    main_frame.place(x=10,y=10) # Placing the frame at (10, 10)

    # Creating sub- frames
    frame1 = Frame(main_frame, padx=3, pady=3, bg = 'black')
    frame2 = Frame(main_frame, bg='white', pady=5, padx = 5)

    frame1.grid(row = 1, column = 1, padx = 5, pady = 5)
    frame2.grid(row = 2, column = 1, padx = 5, pady = (30,10))


    # Title label in sub_frame1
    label = Label(master=frame1, text="Running simulations! Please wait...", fg = '#23ff0f', font = "Verdana 15 bold", bg = 'black')
    label.pack() # Put the label into the window

    # Progress bar widget 
    progress = Progressbar(frame2, orient = HORIZONTAL, length = 450, mode = 'determinate') 
    progress.pack()

    main()

    # Run the GUI  
    ProgressWin.mainloop()
