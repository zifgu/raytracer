#include "aabb.h"

const AABox AABox::empty = AABox(Interval::empty, Interval::empty, Interval::empty);
const AABox AABox::all = AABox(Interval::all, Interval::all, Interval::all);
