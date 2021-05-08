#pragma once
#include <vector>

class ModifiableProperty {
private:
	double value;
	//multipliers
	std::vector<double> modifierList;
	//add a dictionary as well, for the use case of buffs and debuffs (so you can target-remove)

public:
	ModifiableProperty(double value);
	ModifiableProperty() : ModifiableProperty(0) {};

	double getValue();
	void addModifier(double modifier);
	void clearModifiers();
};