#include <string>

class ParseStatus { // Super class
	bool status;
	std::string remainingCharacters;
	std::string parsedCharacters;

	// Only for the success case

	// Only for the failed case
	std::string expectedCharacters;

	// check if two ParseStatuses are equal
	bool operator==(const ParseStatus &b) const;

	bool operator!=(const ParseStatus &b) const;
};
