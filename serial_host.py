#%%
import serial
from pathlib import Path
from time import sleep
from datetime import datetime
#%%
# Global vars

# Directory where readings will be stored
outdir = Path("./data")

# Serial connection parameters
port = "/dev/cu.usbserial-537A0160291"
baudrate = 115200

#%%
if __name__ == "__main__":
    try:
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"Serial Connection Established!")
    except Exception as e:
        print(f"Serial Connection Failed:\n{e}")
        exit(1)
    if outdir.is_file():
        print("could not create target output directory, please move/delete file 'data in repository root.")
        exit(1)
    outdir.mkdir(exist_ok=True)

    prompt = ""
    while True:
        prompt = input("Read for how many milliseconds? (q for quit): ")
        try:
            millis = int(prompt)
        except:
            if prompt.lower().strip() in ("quit", "q", "exit"):
                exit()
            print("please enter a valid integer number of milliseconds.")
            continue

        ser.reset_input_buffer()
        ser.write(str.encode(f'R {millis * 10}'))
        # print(str.encode(f'R {millis}'))
        with (outdir / datetime.now().strftime(f"%m_%d_%y-%H_%M_%S-{millis}ms.csv")).open("wb") as f:
            # have to wait so data has time to transfer
            sleep(3 + millis / 1000)
            record_len = ser.readline()
            record_len = int(record_len)

            bytes_array = b''
            while ser.in_waiting:
                bytes_array += ser.read(1000)
                sleep(0.005)
            
            f.write(bytes_array)

    


#%%