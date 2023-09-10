import websocket
import json
import struct
from matplotlib.animation import FuncAnimation
import matplotlib.pyplot as plt
from itertools import count

time_points = []
value_data = []


def hex2int(hex_string):
    try:
        result = (int(hex_string[6:8], 16) << 16) | (int(hex_string[8:10], 16) << 8) | int(hex_string[10:12], 16)
        result = result << 12
        result = result >> 12
        result = ((result) / 1048576 * 200 - 50)
        return result
    except ValueError:
        print("Error convert")



last_value = None
def get_data():
    ws = websocket.WebSocket()
    ws.connect("ws://raspberrypi:8080")
    command = {}
    command['command'] = 1
    command['bdaddr'] = "60:A4:23:D4:5C:9C"
    command['handle'] = "/org/bluez/hci0/dev_60_A4_23_D4_5C_9C/service0001/char0002"
    ws.send(json.JSONEncoder().encode(command))
    print(ws.recv())
    while True:
        data = json.loads(ws.recv())['value']
        print(hex2int(data))
        # Plot data
        def animate(i):
            data = json.loads(ws.recv())['value']
            int_data = hex2int(data)
            global last_value
            last_value = None
            if last_value != int_data:
                print(int_data, json.loads(ws.recv())['value'])
                time_points.append(i)  # Assuming you want to use an incremental time axis
                value_data.append(int_data)

                # Limit the number of points displayed if you don't want to keep all data
                max_points = 5000000  # Adjust as needed
                if len(time_points) > max_points:
                    del time_points[0]
                    del value_data[0]

                # Clear the previous plot and plot the updated data
                plt.cla()
                plt.plot(time_points, value_data, label='Data')
                plt.xlabel('Time')
                plt.ylabel('Value')
                plt.legend()
                last_value = int_data

        ani = FuncAnimation(plt.gcf(), animate, interval=1500)
        plt.show()

get_data()
