/// <summary>
/// DISCLAIMER:
/// Randfast is a c++ implementation by DVSProductions of the algorithm described here:
/// https://stackoverflow.com/a/29485641/4203189
/// </summary>
#include "randfast.h"
#include <float.h>

#define SHR_BIT (64 - (DBL_MANT_DIG-1))
uint64_t randfast::custom_lcg()noexcept {
	return internal_next = internal_next * 2862933555777941757ULL + 3037000493ULL;
}

randfast::randfast(uint64_t seed) noexcept {
	internal_next = seed;
}

randfast::randfast(uint64_t seed, double min, double max) noexcept :randfast(seed) {
	setOffset(min, max);
}

double randfast::generate()noexcept {
	union {
		double f; uint64_t i;
	}u;
	u.f = 1.0;
	u.i = u.i | (custom_lcg() >> SHR_BIT);
	return u.f - 1.0;
}

double randfast::operator()()noexcept {
	return generate();
}

double randfast::operator()(double min, double max) noexcept {
	return generate() * (max - min) + min;
}

double randfast::apply() noexcept {
	return generate() * (MA) + MI;
}

void randfast::setOffset(double min, double max) noexcept {
	MI = min;
	MA = max - min;
}