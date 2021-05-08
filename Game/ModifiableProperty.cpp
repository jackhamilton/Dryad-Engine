#include "ModifiableProperty.h"

ModifiableProperty::ModifiableProperty(double value) {
	ModifiableProperty::value = value;
}

double ModifiableProperty::getValue() {
	double computedValue = value;
	for (double d : modifierList) {
		computedValue *= d;
	}
	return computedValue;
}

void ModifiableProperty::addModifier(double modifier) {
	ModifiableProperty::modifierList.push_back(modifier);
}

void ModifiableProperty::clearModifiers() {
	ModifiableProperty::modifierList.clear();
}