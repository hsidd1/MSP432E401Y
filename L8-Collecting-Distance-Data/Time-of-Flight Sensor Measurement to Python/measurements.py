# Implements both o3d and measurement outputs but o3d in other file worked better - run with relevant keil proj for tof I2C 
import serial
import numpy as np
import open3d as o3d
import math


s=serial.Serial('COM4', baudrate=115200,timeout=10)
print("Opening: " + s.name)

# reset the buffers of the UART port to delete the remaining data in the buffers
s.reset_output_buffer()
s.reset_input_buffer()

# wait for user's signal to start the program
input("Press Enter to start communication...")
# send the character 's' to MCU via UART
# This will signal MCU to start the transmission
s.write('s'.encode())


tot_distance=[]
zcor=[]
ycor=[]
index=0
pi=math.pi

# purpose for this loop->ignore the first 8 line which is not the data
for i in range(8):
    x = s.readline()
    print(x.decode())

while 1:
    check = s.readline()
    if(check!=''):
        break
for m in range(8):
        z = s.readline()
        print("scan time = ", m)
        # print(float(z))
        tot_distance.insert(index,round(int(float(z)))) # only output distance from microcontroller
        ycor.insert(index, round((tot_distance[index]*math.sin(pi*m/4)))) # y->sin(0,45,90,135,180,225,270,315)
        zcor.insert(index, round((tot_distance[index]*math.cos(pi*m/4)))) # z->cos(0,45,90,135,180,225,270,315)
        print("Distance(degree:"+str(m*45)+"):"+str(tot_distance[index]))
        print("(z,y):"+str(zcor[index])+", "+str(ycor[index]))
        print(" ")
        index+=1


        
# the encode() and decode() function are needed to convert string to bytes
# because pyserial library functions work with type "bytes"

#close the port
print("Closing: " + s.name)
s.close()


if __name__ == "__main__":
    #Remember the goals of modularization
    #   -- smaller problems, reuse, validation, debugging
    #To simulate the data from the sensor lets create a new file with test data 
    f = open("lab8.xyz", "w")    #create a new file for writing
        
    #Test data: Lets make a rectangular prism as a point cloud in XYZ format
    #   A simple prism would only require 8 vertices, however we
    #   will sample the prism along its x-axis a total of 10x
    #   4 vertices repeated 10x = 40 vertices
    #   This for-loop generates our test data in xyz format
    index2=0
    #for x in range(3):
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p1
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p2
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p3
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p4
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p5
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p6
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p7
    index2+=1
    f.write('{0:f} {1:f} {2:f}\n'.format(float(0),ycor[index2],zcor[index2]))    #write x,0,0 (xyz) to file as p8
    index2+=1
    f.close()   #there should now be a file containing 40 vertex coordinates                               
        
    #Read the test data in from the file we created        
    print("Read in the prism point cloud data (pcd)")
    pcd = o3d.io.read_point_cloud("lab8.xyz", format="xyz")

    #Lets see what our point cloud data looks like numerically       
    print("The PCD array:")
    print(np.asarray(pcd.points))

    #Lets see what our point cloud data looks like graphically       
    print("Lets visualize the PCD: (spawns seperate interactive window)")
    o3d.visualization.draw_geometries([pcd])

    #OK, good, but not great, lets add some lines to connect the vertices
    #   For creating a lineset we will need to tell the packahe which vertices need connected
    #   Remember each vertex actually contains one x,y,z coordinate

    #Give each vertex a unique number
    yz_slice_vertex = []
    for x in range(0,8): # 1 rotation (360 deg) -> 8 measurements
        yz_slice_vertex.append([x])

    #Define coordinates to connect lines in each yz slice        
    lines = []  
    for x in range(0,8,8):
        lines.append([yz_slice_vertex[x], yz_slice_vertex[x+1]])
        lines.append([yz_slice_vertex[x+1], yz_slice_vertex[x+2]])
        lines.append([yz_slice_vertex[x+2], yz_slice_vertex[x+3]])
        lines.append([yz_slice_vertex[x+3], yz_slice_vertex[x+4]])
        lines.append([yz_slice_vertex[x+4], yz_slice_vertex[x+5]])
        lines.append([yz_slice_vertex[x+5], yz_slice_vertex[x+6]])
        lines.append([yz_slice_vertex[x+6], yz_slice_vertex[x+7]])
        lines.append([yz_slice_vertex[x+7], yz_slice_vertex[x]])

    #Define coordinates to connect lines between current and next yz slice        
    #for x in range(0,15,8):
        #lines.append([yz_slice_vertex[x], yz_slice_vertex[x+8]])
        #lines.append([yz_slice_vertex[x+1], yz_slice_vertex[x+9]])
        #lines.append([yz_slice_vertex[x+2], yz_slice_vertex[x+10]])
        #lines.append([yz_slice_vertex[x+3], yz_slice_vertex[x+11]])
        #lines.append([yz_slice_vertex[x+4], yz_slice_vertex[x+12]])
        #lines.append([yz_slice_vertex[x+5], yz_slice_vertex[x+13]])
        #lines.append([yz_slice_vertex[x+6], yz_slice_vertex[x+14]])
        #lines.append([yz_slice_vertex[x+7], yz_slice_vertex[x+15]])

    #This line maps the lines to the 3d coordinate vertices
    line_set = o3d.geometry.LineSet(points=o3d.utility.Vector3dVector(np.asarray(pcd.points)),lines=o3d.utility.Vector2iVector(lines))

    #Lets see what our point cloud data with lines looks like graphically       
    o3d.visualization.draw_geometries([line_set])