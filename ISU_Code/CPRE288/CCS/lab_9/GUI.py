from tkinter import*

# Create the main application window
root = Tk()


clicked = 0


def myClick():

    global clicked

    clicked += 1

    if (clicked < 4):
        myLabel = Label(root, text="Dam you clicked the button") # Create a label with specific text
    elif (clicked < 8):
        myLabel = Label(root, text="Stop clicking the button")
    else:
        myLabel = Label(root, text="Great you broke it")
        clicked = 0

    myLabel.grid(row=3+clicked, column=5)


myLabel = Label(root, text="Hello World!")
myLabe2 = Label(root, text="My Name Is Mikhail McDade")

 # use padx/y to change the buttons size
 # if you want a function to exicute if pressed do command=<function name>
 # fg changes the font color and bg change the back color ex: fg = "blue"
myButton = Button(root, text="Dont press", command=myClick, fg="blue", bg="#000000")


# these dont have to be here they can be at the end of Button ex: Button(...).grid(...)
myLabel.grid(row=0, column=5)
myLabe2.grid(row=1, column=5)

myButton.grid(row=3, column=5)


# Start the main event loop, waiting for user interactions
# tells python to run the Tkinter event loop (sizes the window to be as small as it can be)
# listens for events such as buttion clicks or keypresses and blocks any code that comes 
# after it from running until you close the window where you called the method
# without this the program will not run
root.mainloop()
