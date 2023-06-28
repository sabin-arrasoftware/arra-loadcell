#!/bin/bash

# Step 1: Install Arduino Extension
code --install-extension ms-vscode.arduino

# Step 2: Reload Visual Studio Code
code --reload

# Step 3: Configure Arduino Extension
# Open the settings.json file for Arduino extension configuration
code ~/.config/Code/User/settings.json

# Install jq
sudo apt-get install jq

# Update the value of "arduino.useArduinoCli" to true
jq '.arduino.useArduinoCli = true' ~/.config/Code/User/settings.json > tmp_settings.json && mv tmp_settings.json ~/.config/Code/User/settings.json

# Remove the trailing comma, if any, from the last line of the file
sed -i '$s/,$//' ~/.config/Code/User/settings.json

# Update the values of "Show board config" and "Select serial port"
jq '.arduino.showBoardConfig = "Arduino Uno" | .arduino.selectSerialPort = "/dev/ttyUSB0"' ~/.config/Code/User/settings.json > tmp_settings.json && mv tmp_settings.json ~/.config/Code/User/settings.json

# Save and close settings.json

# Step 4: Add includePath in c_cpp_properties.json
c_cpp_properties_file=~/.config/Code/User/c_cpp_properties.json
include_path='"~/Arduino/libraries/**"'

if ! grep -q 'includePath' "$c_cpp_properties_file"; then
    # If includePath doesn't exist, add it under "configurations"
    jq --arg include_path "$include_path" '.configurations[].includePath += [$include_path]' "$c_cpp_properties_file" > tmp_c_cpp_properties.json && mv tmp_c_cpp_properties.json "$c_cpp_properties_file"
fi

# Save and close c_cpp_properties.json

# Step 5: Print configuration instructions
echo "Arduino Extension for Visual Studio Code configured successfully!"
echo "Connect your Arduino board to your computer via USB."
echo "Open or create an Arduino project in Visual Studio Code."

# Step 6: End of script
exit 0
