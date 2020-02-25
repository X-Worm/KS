#include <systemc.h>

// r1 = a + b
// r2 = номер варіанту = 5
const int variant = 5;

SC_MODULE(S2) {
	sc_out<bool> r1, r2;
	sc_in<bool>a, b, clk;

	void mainFunc() {
		r1.write(a.read() + b.read());
		r2.write(variant);
	}

	SC_CTOR(S2) {
		SC_METHOD(mainFunc);  
		sensitive << clk.pos();
	}
};