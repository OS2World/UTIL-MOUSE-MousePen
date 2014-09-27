
This is a little program I wrote for a University Project.  It uses a
backpropogation neural network to recognize characters. I have only
trained the network with Numeric characters(0-9), but the other characters
can be created using HRNNUI.EXE, and added to the NUM.H file. You will need
GNU C/C++ to recompile BPNN.CPP if you make any changes to the training sets.

USING THE NEURAL NETWORK FOR CHARACTER RECOGNITION
--------------------------------------------------

The file HRNNUI.EXE is a PM Program which lets the user input characters
on an 8x8 grid, pressing the START button will use the grid as the input
to the neural network. The LEFT Mouse Button to ENTERS a point on the GRID,
and DOUBLE_CLICKING the LEFT Mouse Button to REMOVE a point from the GRID.
When the START Button is pressed the output of the Neural Network is printed
above the Grid i.e. Char ?. A single beep indicates that the character was
recognized, a double beeb indicateds that the character wasn't recognized
properly.


To Start the PM Program type HRNNUI <Weight filename>

e.g to use the weight file that came with the archive type.

HRNNUI 1000a.wts


The Grid can be written to a file by pressing the SAVE Button.
The file INPUTS.DAT will then contain a 1-Dimensional Array on the GRID.
This can be used to create characters for training the network.

TRAINING THE NETWORK
--------------------

If you look at the training sets in the file NUM.H, you will see that the
"input" arrary is just a array of 30, 64 character strings. Where a 1 is
a ON pixel and a 0 if an off pixel.

The "correctout" array is the desired output of the particular input, it is
also a 30-by-64 character string array.  Where the position of the 1 is the
position of the output character in the string below.

"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

e.g the desired output

"000000000010000000000000000000000000" is :-
           ^
           |
"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
           ^
           |
           |_______
                  |
>  the character 'A'

If you need any more information send me some mail at the address below.

rha@snoopy.apana.org.au

You are free to use the source code in any of your programs, if you find
it useful. I hope it encourages people to give OS/2 programming a Go!


This is listing of the files that should be in the archive.

1000A.WTS   -   Binary file which contains the Network weights.
BPNN.CPP    -   Source file for Network Training.
BPNN.EXE    -   Network Training executable, this was used to create 1000a.WTS
HRNNUI.C    -   Source file for PM Interface.
hrnnui.exe  -   Executable of PM Interface for Network
NUM.H       -   Include file containing training sets, used with BPNN.CPP

