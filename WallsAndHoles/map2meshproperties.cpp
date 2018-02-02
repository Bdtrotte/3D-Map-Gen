
#include "map2mesh.h"

// These persist throughout the program and do not have to be deleted.

M2MPropertyClass *Map2Mesh::Properties::Height = new M2MPropertyClass("height", {"baseHeight", "topHeight"}, {0, 0});
