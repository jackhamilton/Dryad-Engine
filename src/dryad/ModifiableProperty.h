#pragma once
#include <vector>

namespace dryad
{

//Basetype and modifier type respectively.
template <typename T, typename M>
class ModifiableProperty {
private:
	T value;
	//multipliers
	std::vector<M> modifierList;
	//add a dictionary as well, for the use case of buffs and debuffs (so you can target-remove)

public:
	ModifiableProperty(T value) {
		ModifiableProperty::value = value;
	};
	ModifiableProperty() {};

	T getValue() {
		T computedValue = value;
		for (M d : modifierList) {
			computedValue *= d;
		}
		return computedValue;
	};
	void addModifier(M modifier) {
		ModifiableProperty::modifierList.push_back(modifier);
	}
	void clearModifiers() {
		ModifiableProperty::modifierList.clear();
	};
};

}