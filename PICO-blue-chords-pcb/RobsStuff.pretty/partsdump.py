import pcbnew

file = "C:\\Users\\rsmil\\OneDrive\\2023 Year\\HackSpace\\Bluetooth Chord Keyboard\\pcb\\flippedkeyboard\\flippedkeyboard.kicad_pcb"

board = pcbnew.LoadBoard(file)

o = board.GetOrigin()
x = o.x / 1000000.0
y = o.y / 1000000.0

l = []

for f in board.GetFootprints():
    b = f.GetBoundingBox()
    ref = f.GetReference()
    p = f.GetPosition()
    px = (p[0] / 1000000.0)-x
    py = (p[1] / 1000000.0)-y
    height = b.GetHeight() / 1000000.0
    width = b.GetWidth() / 1000000.0
    print(ref)
    t = '["{0}",{1:.2f},{2:.2f},{3:.2f},{3:.2f}]'.format(ref,px,py,height,width)
    l.append(t)

l.sort()
for t in l:
    print(t,",",end="")


