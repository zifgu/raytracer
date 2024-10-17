#include "aabb.h"

const AABox AABox::empty = AABox(
	Interval(infinity, -infinity),
	Interval(infinity, -infinity),
	Interval(infinity, -infinity)
);
const AABox AABox::all = AABox(
	Interval(-infinity, infinity),
	Interval(-infinity, infinity),
	Interval(-infinity, infinity)
);
