#include <systemc.h>

// X = 5 - номер варіанту
// Y = K + V = 75 + 86 = 161 - сума ASCII першої літери Пр і Імя

const int X = 5;
const int Y = 161;

SC_MODULE(S1) {
	sc_out<bool> f1, rep;
	sc_in<bool> clk;

	void mainFunc() {
		f1.write(X < Y ? X-Y : Y - X);
		rep.write(Y > 0 ? Y : X);
	}

	SC_CTOR(S1) {
		SC_METHOD(mainFunc);  
		sensitive << clk.pos();
	}
};