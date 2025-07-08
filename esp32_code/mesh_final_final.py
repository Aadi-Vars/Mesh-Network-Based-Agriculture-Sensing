import machine
import _thread
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

data_queue = []
queue_lock = _thread.allocate_lock()
GOOGLE_SCRIPT_URL = "http://mesh.pythonanywhere.com"
#LOG_FILE = "log.txt"

'''
def sync_time_from_sim7600():
    try:
        response = send_at("AT+CCLK?", delay=2)
        decoded = response.decode()
        match = re.search(r'\+CCLK: "(\d+)/(\d+)/(\d+),(\d+):(\d+):(\d+)', decoded)
        if match:
            year = int("20" + match.group(1))
            month = int(match.group(2))
            day = int(match.group(3))
            hour = int(match.group(4))
            minute = int(match.group(5))
            second = int(match.group(6))
            tm = (year, month, day, 0, hour, minute, second, 0)
            machine.RTC().datetime((tm[0], tm[1], tm[2], tm[3], tm[4], tm[5], tm[6], tm[7]))
            #log_to_file(f"RTC synced from SIM7600: {tm}")
            print("RTC synced from SIM7600:", tm)
        else:
            print("Failed to parse SIM7600 time")
            #log_to_file("Failed to parse SIM7600 time")
    except Exception as e:
        print("Error syncing time from SIM7600:", e)
        #log_to_file(f"Error syncing time from SIM7600: {e}")

def log_to_file(message):
    timestamp = "{:04}-{:02}-{:02} {:02}:{:02}:{:02}".format(*localtime()[:6])
    try:
        with open(LOG_FILE, "a") as f:
            f.write(f"[{timestamp}] {message}\n")
    except Exception as e:
        print("Failed to write to log file:", e)

'''
        
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
        #log_to_file(f"Error parsing data: {e}")
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
    #log_to_file(f"AT Command: {command} => Response: {decoded}")
    return response

def send_to_sim7600_batch(batch):
    try:
        payload = ""
        for data in batch:
            payload += f"{data['sample1']},{data['sample2']},{data['sample3']},{data['node_name']}|"
        payload = payload.rstrip("|")
        url = f"{GOOGLE_SCRIPT_URL}/?batch={payload}"

        send_at('AT+HTTPINIT')
        send_at(f'AT+HTTPPARA="URL","{url}"')
        send_at('AT+HTTPACTION=0', delay=5)
        send_at('AT+HTTPTERM')

    except Exception as e:
        print("SIM7600 batch send error:", e)

def nrf_receiver_thread():
    global data_queue,sleep_sent
    last_received_time = time.time()
    while True:
        try:
            if not sleep_sent:

                if uart_nrf.any():
                    line = uart_nrf.readline()
                    if line:
                        data_str = line.decode().strip()
                        print("Received:", data_str)
                        #log_to_file(f"Received from NRF: {data_str}")
                        parsed = parse_data(data_str)
                        if parsed:
                            with queue_lock:
                                data_queue.append(parsed)
                        last_received_time = time.time()

                if (time.time() - last_received_time > 300): #5 minutes
                    print("No data from NRF for 10 seconds. Restarting...")
                    #log_to_file("No data from NRF for 10 seconds. Restarting ESP32.")
                    uart_nrf.write("reset\r\n")
                    machine.reset()

            time.sleep(0.2)
        except Exception as e:
            print("Receiver thread error:", e)
            #log_to_file(f"Receiver thread error: {e}")

def sim7600_sender_thread():
    global data_queue,sleep_sent
    while True:
        try:
            if not sleep_sent:
               
                with queue_lock:
                    batch = []
                    while data_queue and len(batch) < 5:
                        batch.append(data_queue.pop(0))
                        
                if batch:
                    #print("Sending batch to SIM7600:", batch)
                    send_to_sim7600_batch(batch)

            time.sleep(1)
        except Exception as e:
            print("Sender thread error:", e)
            #log_to_file(f"Sender thread error: {e}")
            time.sleep(2)

def sim7600_sleep():
    print("Putting SIM7600 to sleep...")
    #log_to_file("Putting SIM7600 to sleep")
    send_at("AT+CSCLK=1")  # Enable automatic sleep
    dtr_pin.value(1)       # High = sleep
    #log_to_file("SIM7600 should now be in sleep mode.")

def sim7600_wake():
    print("Waking SIM7600 up...")
    #log_to_file("Waking SIM7600 up")
    dtr_pin.value(0)       # Low = wake
    send_at("AT+CSCLK=0")  # Disable sleep to be sure
    #log_to_file("SIM7600 should now be awake.")


def main():
    global sleep_sent,dtr_pin
    print("System initialized.")
    #log_to_file("System initialized.")
    led = Pin(2, Pin.OUT)
    led.value(not led.value())
    time.sleep(2)

   
    
    dtr_pin = Pin(4, Pin.OUT)  # Use appropriate GPIO pin
    dtr_pin.value(0)
    send_at("AT+CSCLK=0")
    #send_at("AT+CSCLK=0")  # Wake by default
    # Start threads
    
    #sync_time_from_sim7600()
    last_sleep_command_time = time.time()
    sleep_sent = False
    _thread.start_new_thread(nrf_receiver_thread, ())
    _thread.start_new_thread(sim7600_sender_thread, ())

    # Main thread handles sleep/wake toggling
    while True:
        try:
            if time.time() - last_sleep_command_time > 900:
                if not sleep_sent:
                    uart_nrf.write("mode s\r\n")
                    print("Sent to NRF: mode s")
                    #log_to_file("Sent to NRF: mode s")
                    sim7600_sleep()
                    sleep_sent = True
                else:
                    uart_nrf.write("mode rsdn\r\n")
                    print("Sent to NRF: mode rsdn")
                    #log_to_file("Sent to NRF: mode rsdn")
                    sim7600_wake() 
                    sleep_sent = False
                last_sleep_command_time = time.time()

            time.sleep(1)
        except Exception as e:
            print("Main loop error:", e)
            machine.reset()
            #log_to_file(f"Main loop error: {e}")
            time.sleep(3)
main()





