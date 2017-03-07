import hyperion, time, sys

# Get the parameters
fps       = int(hyperion.args.get('fps', 5.0))
frames    = int(hyperion.args.get('frames', 2))
pixels    = int(hyperion.args.get('pixels', 1))
pixelData = list(hyperion.args.get('pixelData', (0,0,0,255,0,0)))

# checks
if len(pixelData) == 0 or len(pixelData) != frames*pixels*3:
	print("invalid size of pixelData")
	sys.exit(1)

# Initialize
sleepTime = 1.0 / fps
ledData = []
for i in range(frames):
	framePos  = i*pixels*3
	frameData = pixelData[framePos:(framePos+pixels*3)]
	framePixels = len(frameData)/3
	p = 0
	ledData.append(bytearray())
	for j in range(hyperion.ledCount):
		if p>=framePixels: p=0
		ledData[i] += bytearray((frameData[3*p],frameData[3*p+1],frameData[3*p+2]))
		p += 1

# Start the write data loop
currentFrame = 0
while not hyperion.abort():
	hyperion.setColor(ledData[currentFrame])
	time.sleep(sleepTime)
	currentFrame += 1
	if currentFrame >= frames:
		currentFrame = 0
