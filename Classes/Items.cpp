#include "Items.h"

Item::Item(int id, string name, string instruction, int amount = 1) {
	this->id = id;
	this->name = name;
	this->instruction = instruction;
	this->amount = amount;
}

int Item::addAmountBy(int num) {
	amount += num;
	return amount;
}
	
int Item::getID() {
	return id;
}

string Item::getName() {
	return name;
}

string Item::getInstruction() {
	return instruction;
}