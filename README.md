# Virtual Memory Management Simulator


## Overview
Virtual memory serves as an essential part of computer architectures in the modern world. As computer memory, in reality, is expensive, the need for virtual memory arises, which helps to create an illusion of the presence of a very large main memory. This project aims to implement a virtual memory manager that will facilitate the simulation of virtual memory in a way that is feasible and is easily understandable to the consumer. The objective of this project is to experiment and analyze the different combinations of paging schemes and replacement algorithms and to get the best possible combination, and plot the obtained results.


## Project structure
a. Data   			--> Contains the process list and process trace text files. (Dataset)  
b. Plots 			--> Contains the plots which are generated when the simulator is executed.  
c. Presentation and Report	--> Contains the presentaion and report for the project.  
d. driver.py			--> Driver file of the project.  
e. generateResults.py		--> Generates results and creates plots to be shown in final GUI.  
f. menuGUI.py			--> A GUI which allows user to enter input data.  
g. resultsGUI.py		--> GUI where final results and plots are displayed.  
h. simClass.h			--> Header file to store created classes.  
i. simConstants.h		--> Header file to store constants.  
j. simInput.h			--> Header file to read the process list and process trace files given as input by user.  
k. simulator.cpp		--> C++ program simulate virtual memory management.  


## Instructions
To run the simulator, follow these instructions:

a. Enter the project directory and execute the file 'driver.py':  
   python3 driver.py
   
b. Browse appropriate '.txt' files for process trace and process list. Select the page fetch policy, page replacement policy and page size for running the simulation.  
   Otherwise, click the button 'set default' which will automatically fill all the fields with the default options.

c. Click the 'submit' button.

d. Wait for the simulations to complete. Final results will be displayed after processing is complete.


## Purpose
The project 'Virtual Memory Management Simulator' has been created as a mini project for the course IT253- Operating Systems.


## Contributors
- Pratham Nayak     	(191IT241)
- Aprameya Dash     	(191IT209)
- Suyash Chintawar  	(191IT109)
