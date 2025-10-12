#!/bin/bash

# Compile the GTK+ program
g++ dop.cpp -o dop `pkg-config --cflags --libs gtk+-3.0`

# Check if compilation was successful and then run the program
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running program..."
    ./dop
else
    echo "Compilation failed."
fi
