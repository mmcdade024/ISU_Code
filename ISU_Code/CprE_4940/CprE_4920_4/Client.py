import requests
import json
import os

# Configuration
FILENAME = 'C:\School\ISU\CPRE\CprE_4920_01\Grids\Coover02.txt'
ARDUINO_IP = 'http://192.168.4.1:8080' # Default IP for Arduino Access Point

def main():
    if not os.path.exists(FILENAME):
        print(f"Error: {FILENAME} not found in this directory.")
        return

    print(f"Reading {FILENAME}...")
    
    with open(FILENAME, 'r') as f:
        lines = f.readlines()

    resolution = 2.0
    map_lines = []

    # Parse the text file
    for line in lines:
        line = line.strip()
        if line.startswith("RESOLUTION:"):
            resolution = float(line.split(":")[1])
        elif line != "" and (line.startswith(".") or line.startswith("#")):
            map_lines.append(line)

    width = len(map_lines[0])
    length = len(map_lines)
    
    # Flatten the map into a single continuous string for the Arduino
    map_string = "".join(map_lines) 

    # Build the JSON payload
    payload = {
        "resolution": resolution,
        "width": width,
        "length": length,
        "map": map_string
    }

    print(f"Payload ready. Size: {width}x{length}, Resolution: {resolution}")
    print(f"Connecting to Arduino at {ARDUINO_IP}...")

    # Send the HTTP POST request to the Arduino
    try:
        response = requests.post(ARDUINO_IP, json=payload, timeout=5, proxies={"http": None, "https": None})
        print("\nResponse from Arduino:")
        print(f"Status Code: {response.status_code}")
        print(response.text)
    except requests.exceptions.RequestException as e:
        print("\nFailed to connect to Arduino. Ensure your Wi-Fi is connected to 'SaltBot_AP'.")
        print(f"Error Details: {e}")

if __name__ == "__main__":
    main()