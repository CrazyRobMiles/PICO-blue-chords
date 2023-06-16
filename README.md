# PICO-blue-chords

![Image of left and right handed versions of the keyboard](images/keyboards.jpg)
This is a chord keyboard that uses the MicroWriter chord system. It uses the Bluetooth to connect and runs on a Raspberry Pi PICO-W. It is described in the HackSpace magazine. 
## Note
This is a work in progress. The software works correctly and the PCB and the case designs are now complete but these updated designs have not been constructed. The author knows of no reason why they won't work, but he didn't know of any reasons why the prototype wouldn't work until he tried to use it...

## Using a chord keyboard
You can find instructions on how to use the keyboard and the chord shapes in the doc folder. 
## Installing the software on your PICO from the UF2 Images
You can install the software by following these steps:
1. Press down the boot button on your PICO-W.
2. Connect your PICO-W to a usb port on your computer. 
3. The PICO-W should show up as a external drive.
4. The UF2 folder holds images for left and right handed use. Drag the left or right handed uf2 file onto the drive. 
5. The PICO-W will reboot running the software.
## Printed Circuit Board Designs
![Image of the keyboard PCB](images/pcb.jpg)
The PCB designs are in the folder PICO-blue-chords-pcb. This folder also contains the partsdump.py file which can be used to extract the footprint positions for use in the case design software.
## Case 
To create a custom case design you use the PICO-blue-chords-case-builder.FCMacro macro which is in the cases folder. Load this into FreeCAD and select left or right handed and the specific key positions. The macro contains the following statements:
```
# right handed flag
right_handed = True 
if right_handed:
	keyHoles = [Little_5,Ring_5, Middle_2, Index_1, Thumb_1,Control_2]
	displayHole = Brd2
else:
      keyHoles = [Little_3,Ring_2, Middle_1, Index_2, Thumb_5,Control_5]
      displayHole = Brd1
```
Set the value of right_handed to False if you want a left-handed keyboard. Change the numbers for each key to match the position that you want for that key hole. The higher the number the farther down the keyboard the key hole will be placed. You can see the key numbers on the PCB design above. Run the macro and then export the resulting design as an STL file for printing. 
## Building the code
You use CMAKE to build the application from source. If you load the project with Visual Studio Code, make sure that you have the CMake Tools extension installed. The file CMakeLists.txt is used to create the makefiles for the build. Line 10 of this file sets the path to the SDK:
```
set(PICO_SDK_PATH "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1/pico-sdk")
```
This works on my Windows system. You may need to change this when building the software on your machine. Note that you don't need to build the software yourself to use it. You can just install the image files which will be in the build folder which is created by the build process. 

## Origins
![Image of Microwriter Agenda](images/agenda.jpg)
The keyboard is based on the Cykey chord design by Cy Enfield which was used as the basis of the Microwriter and Microwriter AgendA devices developed by him and Chris Rainey. You can find out more about these devices [here](http://www.computinghistory.org.uk/det/5794/Microwriter-MW4/), [here](https://www.microsoft.com/buxtoncollection/detail.aspx?id=5) and [here](https://mindmachine.co.uk/book/A/Inp-Outp/Microwriter01.html). 

If you want to buy an official Microwriter keyboard you can get one [here](https://sites.google.com/site/cykeybellaire/home). 

Have fun!

Rob Miles: [blog](https://www.robmiles.com)


