from tkinter import *
import tkinter.filedialog as tkFileDialog
from tkinter import messagebox
from tkinter import ttk
import tkinter as tk
import os

def select_plist():
	global plist_path
	plist_path = tkFileDialog.askopenfilename()
	if len(plist_path)> 0 and plist_path[-4:]=='.txt':
		plist_path_entry.delete(0,tk.END)
		plist_path_entry.insert(0,plist_path)
	else:
		messagebox.showinfo("Warning","Please choose a .txt file!")

def select_ptrace():
	global ptrace_path
	ptrace_path = tkFileDialog.askopenfilename()
	if len(ptrace_path)> 0 and ptrace_path[-4:]=='.txt':
		ptrace_path_entry.delete(0,tk.END)
		ptrace_path_entry.insert(0,ptrace_path)
	else:
		messagebox.showinfo("Warning","Please choose a .txt file!")

def setDefault():
	global fetch_policy
	global replacement_policy
	global page_size
	global plist_path
	global ptrace_path

	page_size = 2
	fetch_policy ='DEMAND'
	replacement_policy='FIFO'
	plist_path = './Data/plist.txt'
	ptrace_path = './Data/ptrace.txt'

	plist_path_entry.delete(0, tk.END)
	plist_path_entry.insert(0, plist_path)

	ptrace_path_entry.delete(0, tk.END)
	ptrace_path_entry.insert(0, ptrace_path)

	combobox1.current(0)
	combobox2.current(0)
	combobox3.current(1)

def submit():
	global fetch_policy
	global replacement_policy
	global page_size
	global plist_path_var
	global ptrace_path_var
	global plist_path
	global ptrace_path

	plist_path=str(plist_path_var.get())
	ptrace_path=str(ptrace_path_var.get())

	if len(plist_path)<1 or plist_path[-4:]!='.txt':
		messagebox.showinfo("Error", "Process List path invalid!")
		return
	if len(ptrace_path)<1 or ptrace_path[-4:]!='.txt':
		messagebox.showinfo("Error", "Process Trace path invalid!")
		return
	if os.path.isfile(plist_path) == False:
		messagebox.showinfo("Error", "Process List file doesn't exist!")
		return
	if os.path.isfile(ptrace_path) == False:
		messagebox.showinfo("Error", "Process Trace file doesn't exist!")
		return

	fetch_policy = str(c1.get())
	replacement_policy = str(c2.get())
	page_size = int(c3.get())

	if fetch_policy=='' or replacement_policy=='' or page_size=='':
		messagebox.showinfo("Error","All fields not field!")
		return

	print(fetch_policy, replacement_policy, plist_path, ptrace_path, page_size, end='')

	messagebox.showinfo("Info", "Successfully submitted for processing!")

	root.destroy()


root = Tk()
root.title('Virtual Memory Management Simulator')
root.resizable(False, False)
root.config(bg = 'black')

window_height = 500
window_width = 1200

screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

x_cordinate = int((screen_width/2) - (window_width/2))
y_cordinate = int((screen_height/2) - (window_height/2))

root.geometry("{}x{}+{}+{}".format(window_width, window_height, x_cordinate, y_cordinate))

frame1 = Frame(master = root, height = 100, width = 1000, pady = 10, bg = 'black')
frame2 = Frame(master = root, height = 50, width = 1000, pady = 20, bg = 'black')
frame3 = Frame(master = root, height = 50, width = 1000, pady = 20, bg = 'black')
frame4 = Frame(master = root, height = 50, width = 1000, pady = 20, bg = 'black')
frame5 = Frame(master = root, height = 50, width = 1000, pady = 20, bg = 'black')
frame6 = Frame(master = root, height = 50, width = 1000, pady = 20, bg = 'black')

frame1.pack()
frame2.pack()
frame3.pack()
frame4.pack()
frame5.pack()
frame6.pack()

title = Label(master=frame1,text='VIRTUAL MEMORY MANAGEMENT SIMULATOR',font='Script 35',fg='#23ff0f',bg='black')
title.pack()

plist_path_label = Label(master=frame2,text='Process list :',font='Script 20',fg='white',bg='black')
plist_path_label.pack(side=tk.LEFT,padx=10)

plist_path_var=StringVar()
plist_path=''
plist_path_entry = Entry(master=frame2,textvariable=plist_path_var,width=50, font='Script 20')
plist_path_entry.pack(side=tk.LEFT,padx=10)

plist_btn = Button(master=frame2,text='Browse', font='Script 20',command=select_plist)
plist_btn.pack(side=tk.LEFT,padx=10)

ptrace_path_label = Label(master=frame3,text='Process trace:',font='Script 20',fg='white',bg='black')
ptrace_path_label.pack(side=tk.LEFT,padx=10)

ptrace_path_var=StringVar()
ptrace_path=''
ptrace_path_entry = Entry(master=frame3,textvariable=ptrace_path_var,width=50, font='Script 20')
ptrace_path_entry.pack(side=tk.LEFT,padx=10)

ptrace_btn = Button(master=frame3,text='Browse', font='Script 20',command=select_ptrace)
ptrace_btn.pack(side=tk.LEFT,padx=10)


label = Label(master=frame4,text='Fetch Policy:',font='Script 20',fg='white',bg='black')
label.pack(side=tk.LEFT,padx=5)

c1 = StringVar()
combobox1 = ttk.Combobox(master=frame4,textvariable=c1,font='Script 20', width=10, state='readonly')
combobox1['values']=('DEMAND','PRE')
combobox1.current()
combobox1.pack(side=tk.LEFT,padx=10)

label = Label(master=frame4,text='Replacement Policy:',font='Script 20',fg='white',bg='black')
label.pack(side=tk.LEFT,padx=10)

c2 = StringVar()
combobox2 = ttk.Combobox(master=frame4,textvariable=c2,font='Script 20', width=10, state='readonly')
combobox2['values']=('FIFO','LRU','CLOCK')
combobox2.current()
combobox2.pack(side=tk.LEFT,padx=10)

label = Label(master=frame5,text='Page Size:',font='Script 20',fg='white',bg='black')
label.pack(side=tk.LEFT,padx=10)

c3 = IntVar()
combobox3 = ttk.Combobox(master=frame5,textvariable=c3,font='Script 20', width=10, state='readonly')
combobox3['values']=(1,2,4,8,16,32)
combobox3.current(0)
combobox3.pack(side=tk.LEFT,padx=10)

fetch_policy=''
replacement_policy=''
page_size=-1

btn = Button(master=frame6,text='Set Default',font='Script 20',command=setDefault)
btn.pack(side=tk.LEFT,padx=10)

btn = Button(master=frame6,text='Submit',font='Script 20',command=submit)
btn.pack(side=tk.LEFT,padx=10)

root.mainloop()