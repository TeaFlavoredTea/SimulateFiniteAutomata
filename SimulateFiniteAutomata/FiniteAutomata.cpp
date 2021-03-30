#include "FiniteAutomata.h"

FiniteAutomata::FiniteAutomata() { }

FiniteAutomata::FiniteAutomata(const std::string& FADefinitionFile)
{
	// Get all lines from file and store in a 2D jagged array of strings
	const std::vector<std::vector<std::string>> parsedFile = getLinesFromFile(FADefinitionFile);

	// Get accept states from first line of the parsed file.
	try {
		int stateID;
		for (auto stateNumString = parsedFile[0].begin(); stateNumString != parsedFile[0].end(); stateNumString++) {
			stateID = stoi(*stateNumString);
			auto acceptStateItr = this->states.emplace(stateID, std::unique_ptr<State>(new State(stateID))).first;
			this->acceptStates.emplace(acceptStateItr->second.get());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Unable to parse accept states on line 1 of file: " << FADefinitionFile
			<< ". Expected comma separated integers" << std::endl;
		throw e;
	}

	// Get transitions from remaining lines of the file.
	int lineNumber = 2;
	try {
		for (auto currentLine = parsedFile.begin() + 1; currentLine != parsedFile.end(); currentLine++) {
			
			// Parse values from line in file
			int startStateID = std::stoi(currentLine->at(0));
			char character = currentLine->at(1).at(0);	// Potential Issue: if there are more than one characters in this string.
			int endStateID = std::stoi(currentLine->at(2));

			// Add start state and end state (protected against duplicate additions). Also get iterators to each.
			auto startStateItr = this->states.emplace(startStateID, std::unique_ptr<State>(new State(startStateID))).first;
			auto endStateItr = this->states.emplace(endStateID, std::unique_ptr<State>(new State(endStateID))).first;
			
			// Add the transition to the start state.
			startStateItr->second->addTransition(character, endStateItr->second.get());

		}
		lineNumber++;
	}
	catch (const std::exception& e) {
		std::cerr << "Unable to parse transitions on line " << std::to_string(lineNumber)
			<< " of file: " << FADefinitionFile << ". Expected lines of the form \"integer,character,integer\"" 
			<< std::endl;
		throw e;
	}
}

bool FiniteAutomata::testString(const std::string& inputString)
{
	std::unordered_set<State*> activeStates;	// Stores the states to simulate a FA running from.

	// Does this FA have the entry state (state ID = 1)?
	auto entryState = this->states.find(1);
	if (entryState == this->states.end()) {	 // No, this FA cannot be run.
		throw std::invalid_argument("Cannot run a FA that does not have the entry state (state with ID = 1)\n");
	}
	else {	// Yes, add the entry state to activeStates.
		activeStates.emplace(entryState->second.get());
	}
	
	// Run the FA over the input string.
	for (char currentChar : inputString) {
		
		// Record states reached after the FA does transitions.
		std::unordered_set<State*> newActiveStates;	// After the FA reads the character.

		// Make sure this is not the '`' character used for epsilon transitions.
		if (currentChar == '`')
			throw std::invalid_argument("Illegal \'`\' character in string: " + inputString + "\n");

		
		// Merge activeStates with states reachable by epsilon transitions.
		std::unordered_set<State*> epsilonReachable = this->getEpsilonTransitions(activeStates);
		activeStates.insert(epsilonReachable.begin(), epsilonReachable.end());

		// Add states reachable from active states by currentChar-transitions to newActiveStates.
		for (State* statePtr : activeStates) {
			for (State* reachableState : statePtr->getTransitons(currentChar)) {
				newActiveStates.emplace(reachableState);
			}
		}

		// Update activeStates.
		activeStates = std::move(newActiveStates);	// Destructive copy since newActiveStates goes out of scope at the end of this loop.

		// If the FA has no active states then reject the string.
		if (activeStates.empty())
			return false;
	}	// END for loop

	// Do one last round of epsilon transitions after the string has ended.
	std::unordered_set<State*> lastEpsilonReachable = this->getEpsilonTransitions(activeStates);
	activeStates.insert(lastEpsilonReachable.begin(), lastEpsilonReachable.end());

	// Did the FA has finish in an accept state?
	for (State* statePtr : activeStates) {
		if (acceptStates.find(statePtr) != acceptStates.end()) {	// Yes, at least one active state was an accept state.
			return true;
		}
	}
	return false;	// No, none of the active states were also accept states.
}

const std::vector<std::vector<std::string>> FiniteAutomata::getLinesFromFile(const std::string& fileName)
{
	// Open File.
	std::ifstream inFile;
	inFile.open(fileName);
	if (!inFile.is_open())
		throw std::invalid_argument("cannot open file : " + fileName + "\n");

	// Get data from file.
	std::string line;
	std::vector<std::vector<std::string>> parsedFile;	// A jagged 2D array of strings.
	while (getline(inFile, line)) {
		std::stringstream strStream(line);
		std::vector<std::string> splitLine;
		std::string word;

		// Break this line in the file into a vector of strings separated by ','.
		while (getline(strStream, word, ',')) {
			splitLine.push_back(word);
		}
		parsedFile.push_back(splitLine);
	}
	inFile.close();

	return parsedFile;
}

// A depth first search of states reachable by epsilon (`) transitions. Done from every active state.
std::unordered_set<State*> FiniteAutomata::getEpsilonTransitions(const std::unordered_set<State*>& activeStates)
{
	std::unordered_set<State*> visited;  // Used to remember what states have been reached over an epsilon transition.
	for (State* startState : activeStates) {
		
		// Check all states that can be 'seen'
		std::vector<State*> reachable = startState->getTransitons('`');  // Initially 'seen' states.
		while (!reachable.empty()) {

			// Go to one of the states that was 'seen'. 
			State* stateToVisit = reachable.back();
			reachable.pop_back();

			// State must not have been visited in a search, and must not be currently occupied by the FA.
			if (visited.find(stateToVisit) == visited.end() && activeStates.find(stateToVisit) == activeStates.end()) {
				visited.emplace(stateToVisit); 	// Mark the state as visited.

				// Can any states be 'seen' from this newly visited state?
				const std::vector<State*>& newlyReachable = stateToVisit->getTransitons('`');
				if (!newlyReachable.empty()) {	// Yes, append those states
					reachable.insert(reachable.end(), newlyReachable.begin(), newlyReachable.end()); // Copy in new states that can be 'seen'.
				}
			}
		}
	}

	return visited;
}