#include <string>

class ParseStatus {
	bool status;
	std::string remainingCharacters;

	// check if two ParseStatuses are equal
	bool operator==(const ParseStatus &b) const;

	bool operator!=(const ParseStatus &b) const;
};