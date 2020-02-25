#include <systemc.h>
#include "S1.h"
#include "S2.h"

// S1 + s2
SC_MODULE(TestModule) {
	sc_out<bool> r1, r2;
	sc_in<bool> clk;

	S1 s1;
	S2 s2;

	sc_signal<bool> toA, toB;

	SC_CTOR(TestModule) : s1("S1"), s2("S2") {
		s2.a(s1.f1);
		s2.b(s1.rep);
		sensitive << clk.pos();
	}
};