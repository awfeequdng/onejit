#include <cstddef>

float get(float *addr, size_t offset) {
	return addr[offset];
}
void set(float *addr, size_t offset, float value) {
	addr[offset] = value;
}
float get_c1(float *addr) {
	return addr[1];
}
float get_c2(float *addr) {
	return addr[32];
}
float get_c3(float *addr) {
	return addr[4095];
}
void set_c1(float *addr, float value) {
	addr[1] = value;
}
void set_c2(float *addr, float value) {
	addr[32] = value;
}
void set_c3(float *addr, float value) {
	addr[4095] = value;
}
void set_c4(float *addr, float value) {
	addr[8191] = value;
}




double get(double *addr, size_t offset) {
	return addr[offset];
}
void set(double *addr, size_t offset, double value) {
	addr[offset] = value;
}
double get_c1(double *addr) {
	return addr[1];
}
double get_c2(double *addr) {
	return addr[32];
}
double get_c3(double *addr) {
	return addr[4095];
}
void set_c1(double *addr, double value) {
	addr[1] = value;
}
void set_c2(double *addr, double value) {
	addr[32] = value;
}
void set_c3(double *addr, double value) {
	addr[4095] = value;
}
void set_c4(double *addr, double value) {
	addr[8191] = value;
}




