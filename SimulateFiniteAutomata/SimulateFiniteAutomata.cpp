#include <iostream>
#include "FiniteAutomata.h"

using namespace std;

int main()
{
	// Get input.
	string FAFile = R"(.\my_FA.csv)";
	string StringToTest = "1";

	// Build a Finite Automata from file.
	FiniteAutomata myFA (FAFile);

	// Is the string in the language recognized by this FA?
	if (myFA.testString(StringToTest)) {	// Yes, send result to console.
		cout << "String is in the language." << endl;
	}
	else {	// No, send result to console.
		
		cout << "String is NOT in the language." << endl;
	}

	return 0;
}