#include <scv.h>

class test : public sc_module
{
public:
    sc_port <rw_task_if> transactor;
    SC_CTOR(test) {
            SC_THREAD(main);
    }

    void main();
};

class write_constraint : virtual public scv_constraint_base
{
public:
    scv_smart_ptr <rw_task_if::write_t> write;
    SCV_CONSTRAINT_CTOR(write_constraint) {
            SCV_CONSTRAINT(write->addr() <= ram_size);
            SCV_CONSTRAINT( write->addr() != write->data());
    }
};
