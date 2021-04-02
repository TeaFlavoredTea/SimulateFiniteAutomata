Author: Liam Guenther (Liam_Guenther@outlook.com)

 ========================================= Overview =========================================
This program reads a Finite Automata (FA) from a file definition then simulates the FA
over a hard coded input string. The program outputs to console whether the FA accepted the input string.
The program can simulate Deterministic and Non-deterministic Finite Automata. 

 ==================================== Running the program ====================================
 1) Install Visual studio.
 2) Clone the GitHub repository. 
 3) (optinal) Modify input file and string.
 4) Run the program.

 ==================================== Input File Definition ====================================
The program reads the file “my_FA.csv”, a CSV file with no whitespace.
From this file it infers the information needed to construct a FA. States in the file are identified by an integer.

The first line of the file should be a comma separated list of accept states.
Example line:
6,5,3
This example tells the program that the FA should designate state 6, state 5, and state 3 as accept states.

Every remaining line is a transition definition (an arrow in an FA diagram).
Transitions have a start state, character, and a destination state.
If the character is the “`” character, then the transition is interpreted as an epsilon transition.
Example line:
5,a,5
This example tells the program to create a transition from state 5 to state 5 for character “a”.
Example line:
1,`,2
This example tells the program to create an epsilon transition from state 1, to state 2.
