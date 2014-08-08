#include <cstdlib>
#include <string>

using namespace std;

class Pet {

private:
	int id;
	string name;

public:
	Pet(int, string);

	int getID();
	string getName();
};