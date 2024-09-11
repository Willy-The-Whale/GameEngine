#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
using namespace std;
using ComponentID = size_t;

inline ComponentID getComponentID() {

	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID getComponentID() noexcept {

	static ComponentID typeID = getComponentID();
}
