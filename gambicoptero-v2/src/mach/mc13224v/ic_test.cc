// EPOS MC13224V_IC Test Program

#include <ic.h>
#include <cpu.h>
#include <machine.h>
#include <alarm.h>

__USING_SYS

typedef Machine::IO IO;

OStream cout;

void kbi_handler()
{
    CPU::out32(IO::CRM_STATUS, CPU::in32(IO::CRM_STATUS)  //Needs to clear flag
        | (4 << 4) //EXT_WU_EVT
    );
    cout << "KBI handled\n";
}

int main()
{
    cout << "MC13224V_IC test\n";

    cout << "Testing KBI... Hit the push_button to test.\n";

    IC::int_vector(IC::IRQ_CRM, kbi_handler);
    IC::enable(IC::IRQ_CRM);

    CPU::out32(IO::GPIO_PAD_DIR0, CPU::in32(IO::GPIO_PAD_DIR0)
            & ~(1 << 28) // GPIO_28 as input
    );
    CPU::out32(IO::GPIO_PAD_PU_SEL0, CPU::in32(IO::GPIO_PAD_PU_SEL0)
            | (1 << 28) // GPIO_28 has a pull-up resistor
    );
    CPU::out32(IO::GPIO_PAD_PU_EN0, CPU::in32(IO::GPIO_PAD_PU_EN0)
            | (1 << 28) // GPIO_28' pull-up resistor is enabled
    );
    CPU::out32(IO::GPIO_FUNC_SEL1, CPU::in32(IO::GPIO_FUNC_SEL1)
            & ~(3 << 24) // GPIO_28 is at default function
    );
    CPU::out32(IO::GPIO_DATA_SEL0, CPU::in32(IO::GPIO_DATA_SEL0)
            & ~(1 << 28) // GPIO_28 data comes from the pad
    );
    CPU::out32(IO::CRM_WU_CNTL, CPU::in32(IO::CRM_WU_CNTL)
            | (0x4 << 4)  // EXT_WU_EN
            | (0x4 << 8)  // EXT_WU_EDGE
            | (0x4 << 12) // EXT_WU_POL
            | (0x4 << 20) // EXT_WU_IEN
    );

    CPU::int_enable();

    while(true)
    {
        cout << "Waiting...\n";
        Alarm::delay(1000000);
    }

    return 0;
}
