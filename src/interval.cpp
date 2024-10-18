#include "interval.h"

const Interval Interval::empty = { infinity, -infinity };
const Interval Interval::positive = { 0.f, infinity };
const Interval Interval::negative = { -infinity, 0.f };
const Interval Interval::all = { -infinity, infinity };