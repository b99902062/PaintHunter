#include <vector>
#include <string>
#include "Items.h"
#include "Pets.h"

using namespace std;

class UserData {

private:
	int id;
	string name;

	int storyProgress;
	vector<Item> itemsList;
	vector<Pet> petList;

public:
	UserData(void);
	~UserData(void);
};

