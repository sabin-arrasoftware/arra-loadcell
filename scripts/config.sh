#!/bin/bash

# Update the package lists
sudo apt-get update

# Install Python3 and pip
sudo apt-get install -y python3 python3-pip

# Install tkinter for Python3
sudo apt-get install -y python3-tk

# Install avr-gcc, avr-libc, and avrdude for Arduino compilation and uploading
sudo apt install -y avr-gcc avr-libc avrdude

# Install pypbind11-dev for python-c++ binding
sudo apt-get install pybind11-dev

# Install any other dependencies you might need
# sudo apt-get install -y <other-packages>

# Print a success message
echo "All prerequisites installed successfully!"
