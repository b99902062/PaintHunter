#include <cstdlib>
#include <string>

using namespace std;

class Item {

private:
	int id;
	int amount;
	string name;
	string instruction;

public:
	Item(int, string, string, int);


	int addAmountBy(int);
	int getID();
	string getName();
	string getInstruction();
};