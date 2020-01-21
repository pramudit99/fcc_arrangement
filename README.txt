The code fccint.c is a simple C script that produces a position.dat file containing the required number of coordinates which can be used as an input to the GROMACS command 'gmx insert-molecules'.

The coordinates produced will be arranged in an FCC arrangement to ensure most efficient use of space.

INPUTS:
--> Size of the simulation box (in nm): a
--> Number of molecules required : n

OUTPUT:
A file 'position.dat' with 'n' positions defined in 3D space which fit inside a cube of side length 'a'.

NOTE: Some coordinates will be commented out of the file randomly if the number of coordinates in the file exceeds n.