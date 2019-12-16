#pragma once
#include <inttypes.h>
class randfast {
	uint_fast64_t internal_next;
	uint64_t custom_lcg()noexcept;
	double generate()noexcept;
	double MI = 0,MA = 0;
public:
	randfast(uint64_t seed)noexcept;
	randfast(uint64_t seed, double min, double max) noexcept;
	double operator() ()noexcept;
	double operator()(double min, double max)noexcept;
	double apply()noexcept;
	void setOffset(double min, double max)noexcept;
};

