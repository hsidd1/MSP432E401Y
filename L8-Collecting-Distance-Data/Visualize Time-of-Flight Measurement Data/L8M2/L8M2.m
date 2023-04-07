data = input("Enter Sensor Data: ")
xpoints = data.*[0 cos(pi/4) 1 cos(pi/4) 0 -cos(pi/4) -1 -cos(pi/4)];
ypoints = data.*[1 sin(pi/4) 0 -sin(pi/4) -1 -sin(pi/4) 0 sin(pi/4)];

plot(xpoints, ypoints, 'LineStyle', 'none', 'Marker','.')
xlim([-3000 3000])
ylim([-3000 3000])