#Modify the following line with your own serial port details
#   Currently set COM3 as serial port at 115.2kbps 8N1
#   Refer to PySerial API for other options.  One option to consider is
#   the "timeout" - allowing the program to proceed if after a defined
#   timeout period.  The default = 0, which means wait forever.

import serial
import math
import numpy as np
import open3d as o3d

s = serial.Serial('COM3', 115200, timeout = 10)
f = open("tof_radar.xyz", "w")  
arr = [90, 135, 180, 225, 270, 315,360,45]                             
print("Opening: " + s.name)

# reset the buffers of the UART port to delete the remaining data in the buffers
s.reset_output_buffer()
s.reset_input_buffer()

# wait for user's signal to start the program
input("Press Enter to start communication...")
# send the character 's' to MCU via UART
# This will signal MCU to start the transmission
s.write('s'.encode())

# recieve characters from UART of MCU
for i in range(8):
    read1 = s.readline()
    hyp = int(read1.decode())                    # read one byte
    angle = math.radians(arr[i])
    x = round(math.cos(angle) * hyp,4)
    y = round(math.sin(angle) * hyp,4)
    x = str(x)
    y = str(y)
    f.write(x)
    f.write(" ")
    f.write(y)
    f.write(" ")
    f.write("0")
    f.write("\n")   
# the encode() and decode() function are needed to convert string to bytes
# because pyserial library functions work with type "bytes"
#Read the test data in from the file we created      
f.close()  
print("Read in the prism point cloud data (pcd)")
pcd = o3d.io.read_point_cloud("tof_radar.xyz", format="xyz")

#Lets see what our point cloud data looks like numerically       
print("The PCD array:")
print(np.asarray(pcd.points))

#Lets see what our point cloud data looks like graphically       
print("Lets visualize the PCD: (spawns seperate interactive window)")
o3d.visualization.draw_geometries([pcd])

lines = [[0,1], [1,2],[2,3],[3,4],[4,5],[5,6],[6,7],[7,0]]
line_set = o3d.geometry.LineSet(points=o3d.utility.Vector3dVector(np.asarray(pcd.points)),lines=o3d.utility.Vector2iVector(lines))

#Lets see what our point cloud data with lines looks like graphically       
o3d.visualization.draw_geometries([line_set])
#close the port
print("Closing: " + s.name)
s.close()