#!/bin/bash

# Update the package lists
sudo apt-get update

# Install Python3 and pip
sudo apt-get install -y python3 python3-pip

# Install tkinter for Python3
sudo apt-get install -y python3-tk

# Install pypbind11-dev for python-c++ binding
sudo apt-get install pybind11-dev

# Install pybind11
sudo pip install pybind11

# Install any other dependencies you might need
# sudo apt-get install -y <other-packages>

# Print a success message
echo "All prerequisites installed successfully!"
