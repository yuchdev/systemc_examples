#include <systemc>

class Adder : public sc_core::sc_module
{
public:
    SC_CTOR(Adder)
    {
    }
private:
    // Inputs
    sc_core::sc_in<bool> in_a;
    sc_core::sc_in<bool> in_b;
    sc_core::sc_in<bool> in_c;

    // Outputs
    sc_core::sc_out<bool> sum;
    sc_core::sc_out<bool> carry;

    // Wires
    sc_core::sc_signal<bool> ab_wire;
    sc_core::sc_signal<bool> ac_wire;
    sc_core::sc_signal<bool> bc_wire;

    // Gates
    sc_core::Xor xor_gate1;
    sc_core::And and_gate_ab;
    sc_core::And and_gate_ac;
    sc_core::And and_gate_bc;
    sc_core::Or or_gate1;
};
