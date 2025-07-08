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


def nrf_receiver_thread():
    global data_queue,sleep_sent
    last_received_time = time.time()
    while True:
        try:
            if sleep_sent:
                time.sleep(60)
            if uart_nrf.any():
                line = uart_nrf.readline()
                if line:
                    data_str = line.decode().strip()
                    print("Received:", data_str)
                    log_to_file(f"Received from NRF: {data_str}")
                    parsed = parse_data(data_str)
                    if parsed:
                        with queue_lock:
                            data_queue.append(parsed)
                    last_received_time = time.time()

            if (time.time() - last_received_time > 60):
                print("No data from NRF for 10 seconds. Restarting...")
                log_to_file("No data from NRF for 10 seconds. Restarting ESP32.")
                uart_nrf.write("reset\r\n")
                machine.reset()

            time.sleep(0.2)
        except Exception as e:
            print("Receiver thread error:", e)
            log_to_file(f"Receiver thread error: {e}")

def sim7600_sender_thread():
    global data_queue,sleep_sent
    while True:
        try:
            if sleep_sent:
                time.sleep(60);
            with queue_lock:
                if data_queue:
                    data = data_queue.pop(0)
                else:
                    data = None

            if data:
                print("Sending queued data to SIM7600:", data)
                log_to_file(f"Dequeued data for SIM7600: {data}")
                send_to_sim7600(data)

            time.sleep(1)
        except Exception as e:
            print("Sender thread error:", e)
            log_to_file(f"Sender thread error: {e}")
            time.sleep(2)

def sim7600_sleep():
    print("Putting SIM7600 to sleep...")
    log_to_file("Putting SIM7600 to sleep")
    send_at("AT+CSCLK=1")  # Enable automatic sleep
    dtr_pin.value(1)       # High = sleep
    log_to_file("SIM7600 should now be in sleep mode.")

def sim7600_wake():
    print("Waking SIM7600 up...")
    log_to_file("Waking SIM7600 up")
    dtr_pin.value(0)       # Low = wake
    send_at("AT+CSCLK=0")  # Disable sleep to be sure
    log_to_file("SIM7600 should now be awake.")


def main():
    global sleep_sent,dtr_pin
    print("System initialized.")
    log_to_file("System initialized.")
    led = Pin(2, Pin.OUT)
    led.value(not led.value())
    time.sleep(2)

    last_sleep_command_time = time.time()
    sleep_sent = False
    
    dtr_pin = Pin(4, Pin.OUT)  # Use appropriate GPIO pin
    dtr_pin.value(0)
    #send_at("AT+CSCLK=0")  # Wake by default
    # Start threads
    _thread.start_new_thread(nrf_receiver_thread, ())
    _thread.start_new_thread(sim7600_sender_thread, ())

    # Main thread handles sleep/wake toggling
    while True:
        try:
            if time.time() - last_sleep_command_time > 120:
                if not sleep_sent:
                    uart_nrf.write("mode s\r\n")
                    print("Sent to NRF: mode s")
                    log_to_file("Sent to NRF: mode s")
                    sim7600_sleep()
                    sleep_sent = True
                else:
                    uart_nrf.write("mode rsdn\r\n")
                    print("Sent to NRF: mode rsdn")
                    log_to_file("Sent to NRF: mode rsdn")
                    sim7600_wake() 
                    sleep_sent = False
                last_sleep_command_time = time.time()

            time.sleep(1)
        except Exception as e:
            print("Main loop error:", e)
            log_to_file(f"Main loop error: {e}")
            time.sleep(3)
main()




