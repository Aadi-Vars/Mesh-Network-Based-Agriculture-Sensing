import machine
import time
import re
from machine import UART, Pin
import os
from time import localtime
time.sleep(10)
# Setup UARTs
print("Started")
uart_nrf = UART(2, baudrate=115200, tx=Pin(26), rx=Pin(27))  # UART for nRF52840
uart_nrf.init(115200, bits=8, parity=None, stop=1)
uart_sim = UART(1, baudrate=115200, tx=Pin(17), rx=Pin(16))  # UART for SIM7600
uart_sim.init(115200, bits=8, parity=None, stop=1)


GOOGLE_SCRIPT_URL = "http://mesh.pythonanywhere.com"
LOG_FILE = "log.txt"

def log_to_file(message):
    timestamp = "{:04}-{:02}-{:02} {:02}:{:02}:{:02}".format(*localtime()[:6])
    try:
        with open(LOG_FILE, "a") as f:
            f.write(f"[{timestamp}] {message}\n")
    except Exception as e:
        print("Failed to write to log file:", e)
        
def parse_data(data_str):
    try:
        pattern = r'1234:(\d+):(\d+):(\d+)::(\w+)'
        match = re.search(pattern, data_str)
        if match:
            return {
                'sample1': match.group(1),
                'sample2': match.group(2),
                'sample3': match.group(3),
                'node_name': match.group(4)
            }
        return None
    except Exception as e:
        print(f"Error parsing data: {e}")
        log_to_file(f"Error parsing data: {e}")
        return None

def send_at(command, delay=1, timeout=3000):
    uart_sim.write((command + '\r\n').encode())
    time.sleep(delay)
    t = time.ticks_ms()
    response = b""
    while time.ticks_diff(time.ticks_ms(), t) < timeout:
        if uart_sim.any():
            response += uart_sim.read()
    print(response.decode().strip())
    decoded = response.decode().strip()
    log_to_file(f"AT Command: {command} => Response: {decoded}")
    return response

def send_to_sim7600(data):
    try:
        url = f"{GOOGLE_SCRIPT_URL}/?hum={data['sample1']}&humi={data['sample2']}&humii={data['sample3']}&name={data['node_name']}"
        log_to_file(f"Sending to SIM7600: {url}")
        #send_at('AT+HTTPTERM')  # Always reset first
        send_at('AT+HTTPINIT')
        #send_at('AT+HTTPPARA="CID",1"')
        send_at(f'AT+HTTPPARA="URL","{url}"')
        response = send_at('AT+HTTPACTION=0', delay=5)  # 0 = HTTP GET
       
        send_at('AT+HTTPTERM')

    except Exception as e:
        print("SIM7600 send error:", e)
        log_to_file(f"SIM7600 send error: {e}")

def main():
    print("Waiting for data from nRF52840...")
    log_to_file("System initialized. Waiting for data from nRF52840...")
    led=Pin(2,Pin.OUT)
    led.value(not led.value())
    time.sleep(2)
    last_received_time = time.time()
    while True:
        try:
             if uart_nrf.any():
                 line = uart_nrf.readline()
                 if line:
                    data_str = line.decode().strip()
                    #data_str="1234:34:445:23::test"
                    print("Received:", data_str)
                    log_to_file(f"Received from NRF: {data_str}")
                    parsed = parse_data(data_str)
                    if parsed:
                        send_to_sim7600(parsed)
                    last_received_time = time.time()
              # Check if 10 seconds have passed without receiving data
             if time.time() - last_received_time > 10:
                print("No data from NRF for 10 seconds. Restarting...")
                log_to_file("No data from NRF for 10 seconds. Restarting ESP32.")
                machine.reset()  # Soft restart

             time.sleep(2)
        except Exception as e:
            print("Loop error:", e)
            log_to_file(f"Main loop error: {e}")
            time.sleep(5)

main()


