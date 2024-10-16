#include "interval.h"

const Interval Interval::empty = { infinity, -infinity };
const Interval Interval::positive = { 0.f, -infinity };
const Interval Interval::all = { -infinity, infinity };