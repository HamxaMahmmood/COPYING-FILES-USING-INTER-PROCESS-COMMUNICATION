import tkinter as tk
import tkinter.messagebox as tmsg
import os


def copy_from_folder1():
    selected_indices = listbox1.curselection()
    if len(selected_indices) == 0:
        tmsg.showerror("ERROR", "Files not selected!")
        return
    selected_entries = [folder1_files[i] for i in selected_indices]
     # Create two pipes
    r1, w1 = os.pipe()
    r2, w2 = os.pipe()
    
    # Fork a child process
    pid = os.fork()
    
    if pid > 0:  # Parent process
        os.close(r1)  # Close the read end of the first pipe in the parent
        os.close(r2)  # Close the read end of the second pipe in the parent
        w1 = os.fdopen(w1, 'w')  # Open the write end of the first pipe in write mode
        w2 = os.fdopen(w2, 'w')  # Open the write end of the second pipe in write mode
        w1.write(selected_entries[0])  # Write the first message to the first pipe
        w1.close()  # Close the write end of the first pipe
        w2.write(selected_entries[0])  # Write the second message to the second pipe
        w2.close()  # Close the write end of the second pipe
        os.wait()
    elif pid == 0:  # Child process
        os.close(w1)  # Close the write end of the first pipe in the child
        os.close(w2)  # Close the write end of the second pipe in the child
        os.dup2(r1, 0)  # Redirect stdin to the read end of the first pipe
        os.dup2(r2, 3)  # Redirect a new file descriptor (3) to the read end of the second pipe
        os.execl("./test", "./test")  # Execute the C program
    else:
        print("Fork failed")

def copy_from_folder2():
    return

def get_folder1_files():
    directory_path = os.path.abspath("folder1")
    files = os.listdir(directory_path)
    return files

def get_folder2_files():
    directory_path = os.path.abspath("folder2")
    files = os.listdir(directory_path)
    return files

def update_folder1_files(files):
    listbox1.delete(0, tk.END)
    for file in files:
        listbox1.insert(tk.END, file)
    return
def update_folder2_files(files):
    listbox2.delete(0, tk.END)
    for file in files:
        listbox2.insert(tk.END, file)
    return


global folder1_files
global folder2_files




    




root = tk.Tk()
root.config(bg="black")
root.title("Multi-Selection Dropdown Example")

# FOLDER 1 LISTBOX
listbox_frame1 = tk.Frame(root)
listbox_frame1.pack(side=tk.LEFT, padx=5, pady=50)
scrollbar1 = tk.Scrollbar(listbox_frame1, orient=tk.VERTICAL)
listbox1 = tk.Listbox(listbox_frame1, selectmode=tk.MULTIPLE, yscrollcommand=scrollbar1.set)
scrollbar1.config(command=listbox1.yview)
scrollbar1.pack(side=tk.RIGHT, fill=tk.Y)
listbox1.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

button = tk.Button(root, text="Copy to folder2", command=copy_from_folder1)
button.place(x = 0,y = 600)




# FOLDER 2 LISTBOX

listbox_frame2 = tk.Frame(root)
listbox_frame2.pack(side=tk.RIGHT, padx=5, pady=50)
scrollbar2 = tk.Scrollbar(listbox_frame2, orient=tk.VERTICAL)
listbox2 = tk.Listbox(listbox_frame2, selectmode=tk.MULTIPLE, yscrollcommand=scrollbar2.set)
scrollbar2.config(command=listbox2.yview)
scrollbar2.pack(side=tk.RIGHT, fill=tk.Y)
listbox2.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)






folder1_files = get_folder1_files()
folder2_files = get_folder2_files()
update_folder1_files(folder1_files)
update_folder2_files(folder2_files)

root.mainloop()
