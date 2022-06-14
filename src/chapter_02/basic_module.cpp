#include <systemc>

SC_MODULE(MinimalSystemCModule) {
    sc_core::sc_in_clk clk_pi;

    void main_method(void);

    SC_CTOR(MinimalSystemCModule) {
        SC_METHOD(main_method);
        sensitive << clk_pi.neg();
        dont_initialize();
    }
};


void MinimalSystemCModule::main_method(void) {
    sc_core::sc_stop();
}

int sc_main(int argc, char *argv[]) {
    const sc_core::sc_time t_PERIOD(8, sc_core::SC_NS);

    sc_core::sc_clock clk("clk", t_PERIOD);
    MinimalSystemCModule systemc("MinimalSystemCModule");
    systemc.clk_pi(clk);
    sc_core::sc_start();
    return 0;
}
