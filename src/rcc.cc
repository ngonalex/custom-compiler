
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
	std::string oops = "Some foolish mistake";
	std::cout << "Hello world of " << argc << " arguments!" << std::endl;
	std::cout << oops << std::endl;
	return 0;
}
