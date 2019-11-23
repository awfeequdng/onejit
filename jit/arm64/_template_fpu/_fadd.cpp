#include <cstdint>

typedef union {
	float fl;
	uint32_t u;
} fnum;

fnum add(fnum a, fnum b) {
	return fnum{a.fl + b.fl};
}
void add(fnum& a, fnum& b, fnum& c) {
	c = fnum{a.fl + b.fl};
}

typedef union {
	double fl;
	uint64_t u;
} dnum;

dnum add(dnum a, dnum b) {
	return dnum{a.fl + b.fl};
}
void add(dnum& a, dnum& b, dnum& c) {
	c = dnum{a.fl + b.fl};
}


