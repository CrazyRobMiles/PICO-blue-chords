import pcbnew

file = "C:\\Users\\rsmil\\source\\repos\\GitHub\\PICO-blue-chords\\PICO-blue-chords-pcb\\PICO-blue-chords-pcb.kicad_pcb"

board = pcbnew.LoadBoard(file)

l = []

bounds = board.GetBoundingBox()

origx = bounds.GetLeft()/ 1000000.0;
origy = bounds.GetTop()/ 1000000.0;
endx = bounds.GetRight()/ 1000000.0;
endy = bounds.GetBottom()/ 1000000.0;

l.append("origin = [{0:.2f},{1:.2f}]".format(origx,origy))
l.append("end = [{0:.2f},{1:.2f}]".format(endx,endy))

for f in board.GetFootprints():
    b = f.GetBoundingBox()
    ref = f.GetReference()
    p = f.GetPosition()
    px = (p[0] / 1000000.0)
    py = (p[1] / 1000000.0)
    height = b.GetHeight() / 1000000.0
    width = b.GetWidth() / 1000000.0
    t = '{0} = ["{0}",{1:.2f},{2:.2f},{3:.2f},{3:.2f}]'.format(ref,px,py,height,width)
    l.append(t)

l.sort()
for t in l:
    print(t)


