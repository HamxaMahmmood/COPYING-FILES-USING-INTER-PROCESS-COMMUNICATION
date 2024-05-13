import tkinter as tk
import tkinter.messagebox as tmsg
import os



def get_folder_files(folder):
    directory_path = os.path.abspath(folder)
    files = os.listdir(directory_path)
    return files

def update_folder1_files():
    global folder1_files
    folder1_files = get_folder_files("folder1")
    listbox1.delete(0, tk.END)
    for file in folder1_files:
        listbox1.insert(tk.END, file)
    return
def update_folder2_files():
    global folder2_files
    folder2_files = get_folder_files("folder2")
    listbox2.delete(0, tk.END)
    for file in folder2_files:
        listbox2.insert(tk.END, file)
    return


def copy_one_file(source,destination,filename):
    r1, w1 = os.pipe()
    r2, w2 = os.pipe()
    r3, w3 = os.pipe()
    
    # Fork a child process
    pid = os.fork()
    
    if pid > 0:  # Parent process
        os.close(r1)  # Close the read end of the first pipe in the parent
        os.close(r2)
        os.close(r3)# Close the read end of the second pipe in the parent
        w1 = os.fdopen(w1, 'w')  # Open the write end of the first pipe in write mode
        w2 = os.fdopen(w2, 'w')
        w3 = os.fdopen(w3, 'w')# Open the write end of the second pipe in write mode
        w1.write(source)  # Write the first message to the first pipe
        w1.close()  # Close the write end of the first pipe
        w2.write(destination)  # Write the second message to the second pipe
        w2.close()
        w3.write(filename)
        w3.close()# Close the write end of the second pipe
        
       
        os.wait()
    elif pid == 0:  # Child process
        os.close(w1)  # Close the write end of the first pipe in the child
        os.close(w2)
        os.close(w3)# Close the write end of the second pipe in the child
        os.dup2(r1, 0)  # Redirect stdin to the read end of the first pipe
        os.dup2(r2, 3)
        os.dup2(r3, 4)# Redirect a new file descriptor (3) to the read end of the second pipe
        os.execl("./onefile", "./onefile")  # Execute the C program
    else:
        print("Fork failed")
    if destination == "folder2/":
        update_folder2_files()
    else:
        update_folder1_files()
    
    return

def copy_from_folder1():
    selected_indices = listbox1.curselection()
    if len(selected_indices) == 0:
        tmsg.showerror("ERROR", "No Files Selected!")
        return
    selected_entries = [folder1_files[i] for i in selected_indices]
    selected_entries.reverse();
    # Create pipes for folder names
     # Create two pipes
    while len(selected_entries) != 0:
        if len(selected_entries) == 1:
            copy_one_file("folder1/","folder2/",selected_entries.pop())
            tmsg.showinfo("SUCCESS","Files copied Successfully")
            return
        r1, w1 = os.pipe()
        r2, w2 = os.pipe()
        r_folder1, w_folder1 = os.pipe()
        r_folder2, w_folder2 = os.pipe()
        
        # Fork a child process
        pid = os.fork()
        
        if pid > 0:  # Parent process
            os.close(r1)  # Close the read end of the first pipe in the parent
            os.close(r2)  # Close the read end of the second pipe in the parent
            os.close(r_folder1)
            os.close(r_folder2)
            w1 = os.fdopen(w1, 'w')  # Open the write end of the first pipe in write mode
            w2 = os.fdopen(w2, 'w')  # Open the write end of the second pipe in write mode
            w1.write(selected_entries.pop())  # Write the first message to the first pipe
            w1.close()  # Close the write end of the first pipe
            w2.write(selected_entries.pop())  # Write the second message to the second pipe
            w2.close()  # Close the write end of the second pipe
            
            w_folder1 = os.fdopen(w_folder1, 'w')
            w_folder1.write("folder1/")
            w_folder1.close()
            
            w_folder2 = os.fdopen(w_folder2, 'w')
            w_folder2.write("folder2/")
            w_folder2.close()
            os.wait()
        elif pid == 0:  # Child process
            os.close(w1)  # Close the write end of the first pipe in the child
            os.close(w2)  # Close the write end of the second pipe in the child
            os.dup2(r1, 0)  # Redirect stdin to the read end of the first pipe
            os.dup2(r2, 3)  # Redirect a new file descriptor (3) to the read end of the second pipe
            os.close(w_folder1)
            os.close(w_folder2)
            os.dup2(r_folder1, 4)  
            os.dup2(r_folder2, 5) 
            os.execl("./backend", "./backend")  # Execute the C program
        else:
            print("Fork failed")
    update_folder2_files()
    tmsg.showinfo("SUCCESS","Files copied Successfully")
    return


def copy_from_folder2():
    selected_indices = listbox2.curselection()
    if len(selected_indices) == 0:
        tmsg.showerror("ERROR", "No Files Selected!")
        return
    selected_entries = [folder2_files[i] for i in selected_indices]
    selected_entries.reverse();
    # Create pipes for folder names
     # Create two pipes
    while len(selected_entries) != 0:
        if len(selected_entries) == 1:
            copy_one_file("folder2/","folder1/",selected_entries.pop())
            tmsg.showinfo("SUCCESS","Files copied Successfully") 
            return
        r1, w1 = os.pipe()
        r2, w2 = os.pipe()
        r_folder1, w_folder1 = os.pipe()
        r_folder2, w_folder2 = os.pipe()
        
        # Fork a child process
        pid = os.fork()
        
        if pid > 0:  # Parent process
            os.close(r1)  # Close the read end of the first pipe in the parent
            os.close(r2)  # Close the read end of the second pipe in the parent
            os.close(r_folder1)
            os.close(r_folder2)
            w1 = os.fdopen(w1, 'w')  # Open the write end of the first pipe in write mode
            w2 = os.fdopen(w2, 'w')  # Open the write end of the second pipe in write mode
            w1.write(selected_entries.pop())  # Write the first message to the first pipe
            w1.close()  # Close the write end of the first pipe
            w2.write(selected_entries.pop())  # Write the second message to the second pipe
            w2.close()  # Close the write end of the second pipe
            
            w_folder1 = os.fdopen(w_folder1, 'w')
            w_folder1.write("folder2/")
            w_folder1.close()
            
            w_folder2 = os.fdopen(w_folder2, 'w')
            w_folder2.write("folder1/")
            w_folder2.close()
            os.wait()
        elif pid == 0:  # Child process
            os.close(w1)  # Close the write end of the first pipe in the child
            os.close(w2)  # Close the write end of the second pipe in the child
            os.dup2(r1, 0)  # Redirect stdin to the read end of the first pipe
            os.dup2(r2, 3)  # Redirect a new file descriptor (3) to the read end of the second pipe
            os.close(w_folder1)
            os.close(w_folder2)
            os.dup2(r_folder1, 4)  
            os.dup2(r_folder2, 5) 
            os.execl("./backend", "./backend")  # Execute the C program
        else:
            print("Fork failed")
    update_folder1_files()
    tmsg.showinfo("SUCCESS","Files copied Successfully")
    return








    




root = tk.Tk()
root.config(bg="black")
root.title("Multi-Selection Dropdown Example")
root.minsize(1500,900)

tk.Label(root, text="AN INTER-PROCESS COMMUNICATOR",fg="beige",bg="black",height=3,width=150,font=("Helvetica", 30, "bold"),highlightthickness=3,highlightcolor="beige").pack(side=tk.TOP)

tk.Label(root, text="FOLDER 1 CONTENT",fg="beige",bg="black",font=("Helvetica", 15, "bold"),highlightthickness=2,highlightcolor="beige",padx=10,pady=5,width=43).place(x=200,y=260)

tk.Label(root, text="FOLDER 2 CONTENT",fg="beige",bg="black",font=("Helvetica", 15, "bold"),highlightthickness=2,highlightcolor="beige",padx=10,pady=5,width=43).place(x=1222,y=260)

# FOLDER 1 LISTBOX
listbox_frame1 = tk.Frame(root, height=50 , width=60)
listbox_frame1.pack(side=tk.LEFT, padx=200, pady=100)
scrollbar1 = tk.Scrollbar(listbox_frame1, orient=tk.VERTICAL,bg="black")
listbox1 = tk.Listbox(listbox_frame1, selectmode=tk.MULTIPLE, yscrollcommand=scrollbar1.set, height=30, width=60,bg="beige")
scrollbar1.config(command=listbox1.yview)
scrollbar1.pack(side=tk.RIGHT, fill=tk.Y)
listbox1.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

tk.Button(root, text="COPY TO FOLDER 2", command=copy_from_folder1, bg="beige",height=2,width=15,font=("helvetica",12,'bold')).place(x = 878,y = 700)




# FOLDER 2 LISTBOX

listbox_frame2 = tk.Frame(root , height=50, width=60)
listbox_frame2.pack(side=tk.RIGHT, padx=200, pady=100)
scrollbar2 = tk.Scrollbar(listbox_frame2, orient=tk.VERTICAL,bg="black")
listbox2 = tk.Listbox(listbox_frame2, selectmode=tk.MULTIPLE, yscrollcommand=scrollbar2.set , height=30, width=60,bg="beige")
scrollbar2.config(command=listbox2.yview)
scrollbar2.pack(side=tk.RIGHT, fill=tk.Y)
listbox2.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)

tk.Button(root, text="COPY TO FOLDER 1", command=copy_from_folder2,bg="beige",height=2,width=15,font=("helvetica",12,'bold')).place(x = 878,y = 625)



# Add text inside the frame

label = tk.Label(root, text="INSTRUCTIONS:\nSelect Any file by clicking on it and then press\n the COPY TO button to paste the file into\n the other folder, You can also chose multiple\n files at a time.", bg="black",highlightthickness=2, highlightbackground="beige",fg="beige")
label.place(x=800,y=350)





update_folder1_files()
update_folder2_files()



root.mainloop()
