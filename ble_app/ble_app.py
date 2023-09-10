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
        uint8_value = int(hex_string[:2], 16)
        return uint8_value
    except ValueError:
        print("Error convert")

def get_data():
    ws = websocket.WebSocket()
    ws.connect("ws://raspberrypi:8080")
    command = {}
    command['command'] = 1
    command['bdaddr'] = "C8:F0:9E:9A:54:FA"
    command['handle'] = "/org/bluez/hci0/dev_C8_F0_9E_9A_54_FA/service0028/char0029"
    ws.send(json.JSONEncoder().encode(command))
    print(ws.recv())
    while True:
        data = json.loads(ws.recv())['value']
        print(hex2int(data))
        # Plot data
        def animate(i):
            int_data = hex2int(json.loads(ws.recv())['value'])
            time_points.append(i)  # Assuming you want to use an incremental time axis
            value_data.append(int_data)

            # Limit the number of points displayed if you don't want to keep all data
            max_points = 50  # Adjust as needed
            if len(time_points) > max_points:
                del time_points[0]
                del value_data[0]

            # Clear the previous plot and plot the updated data
            plt.cla()
            plt.plot(time_points, value_data, label='Data')
            plt.xlabel('Time')
            plt.ylabel('Value')
            plt.legend()

        ani = FuncAnimation(plt.gcf(), animate, interval=1000)
        plt.show()

get_data()