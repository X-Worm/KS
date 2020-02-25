#include <systemc.h>
#include <string>
#include <iostream>
#include <iomanip>
#include "TestModule.h"

using namespace std;

SC_MODULE(mon)
{
	sc_in<bool> A, B, F;
	sc_in<bool> Clk;
	void monitor()
	{
		cout << setw(10) << "Time";
		cout << setw(2) << "A";
		cout << setw(2) << "B";
		cout << setw(2) << "F" << endl;
		while (true)
		{
			cout << setw(10) << sc_time_stamp();
			cout << setw(2) << A.read();
			cout << setw(2) << B.read();
			cout << setw(2) << F.read() << endl;
			wait(); // wait for 1 clock cycle
		}
	}
	SC_CTOR(mon)
	{
		SC_THREAD(monitor);
		sensitive << Clk.pos();
	}
};

struct test_module : sc_module {
	SC_HAS_PROCESS(test_module);

	test_module(::sc_core::sc_module_name) {
		SC_THREAD(test_thread);
	}

	sc_signal<std::string>  message{ "message" };

	void test_thread() {
		message.write("Hello world!");
		wait(1, SC_NS);
		cout << message.read() << endl;
		sc_stop();
	}
};




SC_MODULE(nand2)          // declare nand2 sc_module
{
	sc_in<bool> A, B;       // input signal ports
	sc_out<bool> F;         // output signal ports

	void do_nand2()         // a C++ function
	{
		F.write(!(A.read() && B.read()));
	}

	SC_CTOR(nand2)          // constructor for nand2
	{
		SC_METHOD(do_nand2);  // register do_nand2 with kernel
		sensitive << A << B;  // sensitivity list
	}
};


SC_MODULE(exor2)
{
	sc_in<bool> A, B;
	sc_out<bool> F;

	nand2 n1, n2, n3, n4;

	sc_signal<bool> S1, S2, S3;

	SC_CTOR(exor2) : n1("N1"), n2("N2"), n3("N3"), n4("N4")
	{
		n1.A(A);
		n1.B(B);
		n1.F(S1);

		n2.A(A);
		n2.B(S1);
		n2.F(S2);

		n3.A(S1);
		n3.B(B);
		n3.F(S3);

		n4.A(S2);
		n4.B(S3);
		n4.F(F);
	}
};

SC_MODULE(stim)
{
	sc_out<bool> A, B;
	sc_in<bool> Clk;

	void StimGen()
	{
		A.write(false);
		B.write(false);
		wait();
		A.write(false);
		B.write(true);
		wait();
		A.write(true);
		B.write(false);
		wait();
		A.write(true);
		B.write(true);
		wait();
		sc_stop();
	}
	SC_CTOR(stim)
	{
		SC_THREAD(StimGen);
		sensitive << Clk.pos();
	}
};



//int sc_main(int argc, char* argv[])
//{
//	sc_signal<bool> ASig, BSig, FSig;
//	sc_clock TestClk("TestClock", 10, SC_NS, 0.5);
//	stim Stim1("Stimulus");
//	Stim1.A(ASig);
//	Stim1.B(BSig);
//	Stim1.Clk(TestClk);
//	exor2 DUT("exor2");
//	DUT.A(ASig);
//	DUT.B(BSig);
//	DUT.F(FSig);
//	mon Monitor1("Monitor");
//	Monitor1.A(ASig);
//	Monitor1.B(BSig);
//	Monitor1.F(FSig);
//	Monitor1.Clk(TestClk);
//	sc_start(); // run forever
//	return 0;
//}


int sc_main(int argc, char* argv[])
{
	sc_signal<bool> ASig, BSig, FSig, FiSig;
	sc_clock TestClk("TestClock", 10, SC_NS, 0.5, 1, SC_NS);
	stim Stim1("Stimulus");
	Stim1.A(ASig);
	Stim1.B(BSig);
	Stim1.Clk(TestClk);
	
	//	exor2 DUT("exor2");
	//DUT.A(ASig);
	//DUT.B(BSig);
	//DUT.F(FSig);
	TestModule DUT("testMod");
	DUT.clk(TestClk);
	DUT.r1(FSig);
	DUT.r2(FiSig);
	mon Monitor1("Monitor");
	Monitor1.A(ASig);
	Monitor1.B(BSig);
	Monitor1.F(FSig);
	
	Monitor1.Clk(TestClk);
		sc_trace_file* Tf;
	Tf = sc_create_vcd_trace_file("traces");
	//((sc_trace_file*)Tf)->sc_set_vcd_time_unit(-9);
	sc_trace(Tf, ASig, "A");
	sc_trace(Tf, BSig, "B");
	sc_trace(Tf, FSig, "F");
	sc_trace(Tf, FiSig, "Fi");

	sc_start(); // run forever
	sc_close_vcd_trace_file(Tf);
	return 0;
}
