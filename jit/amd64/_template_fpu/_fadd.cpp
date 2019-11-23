#include <cstdint>

typedef union {
	float fl;
	uint32_t u;
} fnum;

fnum add(fnum a, fnum b) {
	return fnum{a.fl + b.fl};
}
fnum add(fnum& a, fnum& b) {
	return fnum{a.fl + b.fl};
}

typedef union {
	double fl;
	uint64_t u;
} dnum;

dnum add(dnum a, dnum b) {
	return dnum{a.fl + b.fl};
}
dnum add(dnum& a, dnum& b) {
	return dnum{a.fl + b.fl};
}


