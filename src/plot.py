import numpy as np
import matplotlib.pyplot as plt

with open("lipfeatures12.txt") as f:
    content = f.readlines()
numFrames = len(content)
#print numFrames

invalid_sec1 = 0
invalid_sec2 = 0
invalid2_sec1 = 0
invalid2_sec2 = 0

num     = [0 for i in range(numFrames)]
frame   = [0 for i in range(numFrames)]
leftX   = [0 for i in range(numFrames)]
leftY   = [0 for i in range(numFrames)]
upperX  = [0 for i in range(numFrames)]
upperY  = [0 for i in range(numFrames)]
rightX  = [0 for i in range(numFrames)]
rightY  = [0 for i in range(numFrames)]
lowerX  = [0 for i in range(numFrames)]
lowerY  = [0 for i in range(numFrames)]
upperX2 = [0 for i in range(numFrames)]
upperY2 = [0 for i in range(numFrames)]
lowerX2 = [0 for i in range(numFrames)]
lowerY2 = [0 for i in range(numFrames)]


for i in range(numFrames):
	crtLine = content[i].split(",")
	num[i] = crtLine[0]
	frame[i] = crtLine[1]
	leftX[i] = crtLine[2]
	leftY[i] = crtLine[3]
	upperX[i] = crtLine[4]
	upperY[i] = crtLine[5]
	rightX[i] = crtLine[6]
	rightY[i] = crtLine[7]
	lowerX[i] = crtLine[8]
	lowerY[i] = crtLine[9]
	upperX2[i] = crtLine[10]
	upperY2[i] = crtLine[11]
	lowerX2[i] = crtLine[12]
	lowerY2[i] = crtLine[13]

#process list into numpy array
leftX = np.array(leftX)
leftX = leftX.astype(np.float)
leftY = np.array(leftY)
leftY = leftY.astype(np.float)
upperX = np.array(upperX)
upperX = upperX.astype(np.float)
upperY = np.array(upperY)
upperY = upperY.astype(np.float)
rightX = np.array(rightX)
rightX = rightX.astype(np.float)
rightY = np.array(rightY)
rightY = rightY.astype(np.float)
lowerX = np.array(lowerX)
lowerX = lowerX.astype(np.float)
lowerY = np.array(lowerY)
lowerY = lowerY.astype(np.float)
upperX2 = np.array(upperX2)
upperX2 = upperX2.astype(np.float)
upperY2 = np.array(upperY2)
upperY2 = upperY2.astype(np.float)
lowerX2 = np.array(lowerX2)
lowerX2 = lowerX2.astype(np.float)
lowerY2 = np.array(lowerY2)
lowerY2 = lowerY2.astype(np.float)


dis = [0 for i in range(numFrames)]
for i in range(numFrames):
	if lowerY2[i] - upperY2[i] < 0:
		dis[i] = 0
		if num[i] == 1:
			invalid_sec1 += 1
		else:
			invalid_sec2 += 1
	else:
		dis[i] = lowerY2[i] - upperY2[i]

dis2 = [0 for i in range(numFrames)]
for i in range(numFrames):
	if lowerY[i] - upperY[i] < 0:
		dis2[i] = 0
		if num[i] == 1:
			invalid2_sec1 += 1
		else:
			invalid2_sec2 += 1
	else:
		dis2[i] = lowerY[i] - upperY[i]

#average value
section = (int)(num[numFrames-1])
sumDis = [0 for i in range(section)]
sumF = [0 for i in range(section)]
sumDis2 = [0 for i in range(section)]
sumF2 = [0 for i in range(section)]

for i in range(section):
	sumDis[i] = 0
	sumF[i] = 0
	sumDis2[i] = 0
	sumF2[i] = 0

sumDis = np.array(sumDis)
sumDis = sumDis.astype(np.float)
sumF = np.array(sumF)
sumF = sumF.astype(np.float)
dis = np.array(dis)
dis = dis.astype(np.float)

sumDis2 = np.array(sumDis2)
sumDis2 = sumDis2.astype(np.float)
sumF2 = np.array(sumF2)
sumF2 = sumF2.astype(np.float)
dis2 = np.array(dis2)
dis2 = dis2.astype(np.float)

for i in range(numFrames):
	sumDis[(int)(num[i])-1] += dis[i]
	sumF[(int)(num[i])-1] += 1
	sumDis2[(int)(num[i])-1] += dis2[i]
	

sumF2[0] = sumF[0] - invalid2_sec1
sumF2[1] = sumF[1] - invalid2_sec2
sumF[0] = sumF[0] - invalid_sec1
sumF[1] = sumF[1] - invalid_sec2

for i in range(section):
	print("innerlip distance in section %d = %f" % ((i+1), (sumDis[i]/sumF[i])))
	print("outerlip distance in section %d = %f" % ((i+1), (sumDis2[i]/sumF2[i])))

plt.plot(frame, dis, label='innerlip distance')
plt.plot(frame, dis2, label='outerlip distance')
plt.xlabel('frame #')
plt.ylabel('distance')
plt.title('distance between lips')
plt.legend(loc='upper left')
plt.grid(True)
plt.savefig("plot12.png")
plt.show()







