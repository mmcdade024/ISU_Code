import requests
import json
import time

ESP32_IP = "http://192.168.1.XXX/gps" 

def send_dummy_data():
    # The exact datatypes you requested
    payload = {
        "lat_int": 42,
        "lat_frac": 15,
        "lon_int": -93,
        "lon_frac": 64
    }

    headers = {'Content-Type': 'application/json'}

    print(f"Pushing JSON to {ESP32_IP}...")
    
    try:
        response = requests.post(ESP32_IP, data=json.dumps(payload), headers=headers, timeout=5)
        print(f"Status Code: {response.status_code}")
        print(f"ESP32 Replied: {response.text}")
    except requests.exceptions.RequestException as e:
        print(f"Connection Failed! Make sure the IP is correct and you are on the same WiFi. Error: {e}")

if __name__ == "__main__":
    send_dummy_data()