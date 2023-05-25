from pcbnew import *

#file = "C:\\Users\\rsmil\\pcbs\\keyboard\\flippedkeyboard.kicad_pcb"
#file = "C:\\Users\\rsmil\\OneDrive\\2023 Year\\HackSpace\\Bluetooth Chord Keyboard\\pcb\\flippedkeyboard\\flippedkeyboard.kicad_pcb"
file = "C:\\Users\\rsmil\\source\\repos\\GitHub\\PICO-blue-chords\\PICO-blue-chords-pcb\\PICO-blue-chords-pcb.kicad_pcb"

board = LoadBoard(file)

bounds = board.GetBoundingBox()

x = 4100
y = 1400

columns = [
   ["Control",5,163],
   ["Thumb",5,163],
   ["Index",2,0],
   ["Middle",2,0],
   ["Ring",5,163],
   ["Little",5,163]]

x_spacing = 800.0
y_spacing = 676

col_x = x + (x_spacing/2.0)
col_y = y + (y_spacing/2.0)

for col in columns:
   name = col[0]
   size = col[1]
   startY = col[2]
   print(name)
   for no in range(1,size+1):
      ref = "{0}_{1}".format(name,no)
      print("   ",ref)
      component = board.FindFootprintByReference(ref)
      comp_x = col_x
      comp_y = col_y+startY+(no*y_spacing)
      component.SetPosition(VECTOR2I(FromMils(comp_x), FromMils(comp_y)))
   col_x = col_x + x_spacing 

Refresh() 




for f in board.GetFootprints():
    b = f.GetBoundingBox()
    ref = f.GetReference()
    p = f.GetPosition()
    px = (p.x / 1000000.0)-x
    py = (p.y / 1000000.0)-y
    height = b.GetHeight() / 1000000.0
    width = b.GetWidth() / 1000000.0
    t = '    makeKey("{0}",{1:.2f},{2:.2f},{3:.2f},{3:.2f})'.format(ref,px,py,height,width)
    l.append(t)

l.sort()
for t in l:
    print(t)

