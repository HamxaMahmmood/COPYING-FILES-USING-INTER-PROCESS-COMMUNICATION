
Multi-Process File Copying Application:

Overview:
The Multi-Process File Copying Application is a software solution designed to facilitate the efficient copying of files between two folders using inter-process communication (IPC) mechanisms. The application consists of a frontend developed in Python's Tkinter library and a backend implemented in the C programming language.

 Features:
- Graphical User Interface (GUI) built with Tkinter for easy interaction.
- Display of folder contents in separate listboxes for Folder 1 and Folder 2.
- File copying functionality between Folder 1 and Folder 2 with a single click.
- Seamless communication between frontend and backend using pipes.
- Multithreading in the backend for concurrent file read/write operations.
- Clear user instructions provided within the interface.

 Installation:
1. Ensure Python (version 3.x) is installed on your system.
2. Compile the C backend source code using a C compiler (e.g., GCC).
   
   gcc backend.c -o backend
   
3. Ensure executable permissions are set for the compiled backend file.
   
   chmod +x backend
   
   
Usage:
1. Run the Python script `GUI.py` to launch the application.
   
   python3 GUI.py
   
2. Select files from Folder 1 or Folder 2 by clicking on them.
3. Click the "COPY TO FOLDER 2" button to copy selected files from Folder 1 to Folder 2.
4. Click the "COPY TO FOLDER 1" button to copy selected files from Folder 2 to Folder 1.
5. Follow on-screen instructions for file selection and copying.

Dependencies:
- Python (version 3.x)
- Tkinter library (usually included with Python installations)
- C compiler (e.g., GCC) for compiling the backend source code

Notes:
- Ensure proper permissions are set for folder access and file operations.
- Handle errors and exceptions gracefully to enhance user experience.
- Refer to the project documentation for detailed information on the application's architecture and implementation.

Credits:
- Developed by:
  -Hamza Mahmood (413603)
  -Abdul Rehman (408651)

