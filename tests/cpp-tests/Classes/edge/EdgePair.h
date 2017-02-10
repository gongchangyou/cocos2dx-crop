#pragma once
#include "Edge.h"

class EdgePair {

	// Member Variables ////////////////////////////////////////////////////////
public:
	Edge * primary;
	Edge * secondary;

	// Constructor /////////////////////////////////////////////////////////////

	EdgePair(Edge * edge1 = nullptr, Edge * edge2 = nullptr) {
		primary = edge1;
		secondary = edge2;
	}
};
