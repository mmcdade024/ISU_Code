# Author: Mikhail McDade
# Date: 12/03/2024
# Description: Simple client code for collecting data from the CyBot with GUI functionality.

import serial
import time
import socket
import threading
import os
import subprocess
from tkinter import *

def socket_thread():

    global Last_command_Label
    global gui_send_message 
    global text_box

    # paths for file operations
    absolute_path = os.path.dirname(__file__)
    relative_path = "./"
    full_path = os.path.join(absolute_path, relative_path)
    filename = 'Data.txt'

    # TCP Socket Configuration
    HOST = "192.168.1.1"
    PORT = 288
    cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    cybot_socket.connect((HOST, PORT))

    cybot = cybot_socket.makefile("rbw", buffering=0)

    send_message = "Hello\n"
    gui_send_message = "wait\n"

    cybot.write(send_message.encode())
    print("Sent to server: " + send_message)

    while send_message != 'quit\n':

        try:
            command_display = "Last Command Sent:\t" + send_message
            Last_command_Label.config(text = command_display)

            if (send_message == "M\n") or (send_message == "m\n"):

                print("Requested Sensor Scan from CyBot:\n")
                rx_message = bytearray(1)

                with open(full_path + filename, 'w') as file_object:
                    while rx_message.decode() != "\r\n":
                        rx_message = cybot.readline()
                        file_object.write(rx_message.decode())
                        print(rx_message.decode())

                file_object.close()

                subprocess.run(["python", "CyBot-Plot-Sensor-Scan-Values-From-File.py"])

            
            else:

                print("Waiting for server reply\n")
                rx_message = cybot.readline()      # Wait for a message, readline expects message to end with "\n"
                print("Got a message from server: " + rx_message.decode() + "\n") # Convert message from bytes to String (i.e., decode)

                if rx_message.decode() == "white\n":
                    text_box.insert(END, "I'm on white moving back\n")

                if rx_message.decode() == "black\n":
                    text_box.insert(END, "I'm on black moving back\n")
                
                if rx_message.decode() == "left\n":
                    text_box.insert(END, " Left Bump Detected! Moving backward...\n")

                if rx_message.decode() == "right\n":
                    text_box.insert(END, " Right Bump Detected! Moving backward...\n")

            while gui_send_message == "wait\n": 
                time.sleep(.1)  # Sleep for .1 seconds

            send_message = gui_send_message

            gui_send_message = "wait\n"  # Reset gui command message request to wait                        

            cybot.write(send_message.encode()) # Convert String to bytes (i.e., encode), and send data to the server

        except Exception as e:
            print(f"Error sending command: {e}")

    print("Client exiting, and closing file descriptor, and/or network socket\n")
    time.sleep(2) # Sleep for 2 seconds
    cybot.close() # Close file object associated with the socket or UART
    cybot_socket.close()  # Close the socket (NOTE: comment out if using UART interface, only use for network socket option)

    # Function to send a message to the CyBot
    
def sendQuit():
    global gui_send_message # Command that the GUI has requested be sent to the Cybot
    global root  # Main GUI window
    
    gui_send_message = "quit\n"   # Update the message for the Client to send
    time.sleep(1)
    root.destroy() # Exit GUI

def sendScan():
    global gui_send_message

    gui_send_message = "m\n"

def sendForward():
    global gui_send_message

    gui_send_message = "w\n"

def sendLeft():
    global gui_send_message

    gui_send_message = "a\n"

def sendBack():
    global gui_send_message

    gui_send_message = "s\n"

def sendRight():
    global gui_send_message

    gui_send_message = "d\n"

def main():

    # GUI Setup
    global root
    root = Tk()
    root.title('288 Final Proj GUI')
    root.config(bg='#9FD996')

    global Last_command_Label  # Made global so that Client function (socket_thread) can modify
    Last_command_Label = Label(root, text="Last Command Sent: ") 
    Last_command_Label.grid(row=0, column=0, columnspan=3, padx=5, pady=5)

    my_thread = threading.Thread(target=socket_thread) # Creat the thread
    my_thread.start() # Start the thread


    Button(
            root,
            text="W",
            fg="white",
            bg="#000000",
            font=("Helvetica", 14),
            command=lambda: sendForward()
        ).grid(row=1, column=2, padx=5, pady=5)

    Button(
            root,
            text="A",
            fg="white",
            bg="#000000",
            font=("Helvetica", 14),
            command=lambda: sendLeft()
        ).grid(row=2, column=1, padx=5, pady=5)

    Button(
            root,
            text="S",
            fg="white",
            bg="#000000",
            font=("Helvetica", 14),
            command=lambda: sendBack()
        ).grid(row=3, column=2, padx=5, pady=5)

    Button(
            root,
            text="D",
            fg="white",
            bg="#000000",
            font=("Helvetica", 14),
            command=lambda: sendRight()
        ).grid(row=2, column=3, padx=5, pady=5)

    Button(
            root,
            text="Scan",
            fg="white",
            bg="#000000",
            font=("Helvetica", 14),
            command=lambda: sendScan()
        ).grid(row=4, column=2, padx=5, pady=5)

    Button(
            root,
            text="QUIT",
            fg="white",
            bg="#000000",
            font=("Helvetica", 14),
            command=lambda: sendQuit()
        ).grid(row=6, column=2, padx=5, pady=5)        


    #create_button("w", 1, 2)
    #create_button("a", 2, 1)
    #create_button("s", 3, 2)
    #create_button("d", 2, 3)

    global text_box
    text_box = Text(root, height=5, width=40) 
    text_box.grid(row=5, column=1, columnspan=3, padx=5, pady=5)

    root.mainloop()

main()
