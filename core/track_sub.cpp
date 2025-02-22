#include "tree.h"


tree tree::operator[](const int& id) {
    if (id >= size()) return *this;
    tree returntree(this, rects[id]);
    return returntree;
}
