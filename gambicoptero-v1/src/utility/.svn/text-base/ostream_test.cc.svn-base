// EPOS OStream Utility Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>

__USING_SYS;

int main()
{
    OStream cout;

    cout << "OStream test\n";
    cout << "This is a char:\t\t\t" << 'A' << "\n";
    cout << "This is a negative char:\t" << '\377' << "\n";
    cout << "This is an unsigned char:\t" << 'A' << "\n";
    cout << "This is an int:\t\t\t" 
	 << (1 << sizeof(int) * 8 - 1) - 1 << "\n" 
	 << "\t\t\t\t" << hex << (1 << sizeof(int) * 8 - 1) - 1 << "(hex)\n"
	 << "\t\t\t\t" << dec << (1 << sizeof(int) * 8 - 1) - 1 << "(dec)\n"
	 << "\t\t\t\t" << oct << (1 << sizeof(int) * 8 - 1) - 1 << "(oct)\n"
	 << "\t\t\t\t" << bin << (1 << sizeof(int) * 8 - 1) - 1 << "(bin) " <<  endl;
    cout << "This is a negative int:\t\t" 
	 << (1 << sizeof(int) * 8 - 1) << "\n" 
	 << "\t\t\t\t" << hex << (1 << sizeof(int) * 8 - 1) << "(hex)\n"
	 << "\t\t\t\t" << dec << (1 << sizeof(int) * 8 - 1) << "(dec)\n"
	 << "\t\t\t\t" << oct << (1 << sizeof(int) * 8 - 1) << "(oct)\n"
	 << "\t\t\t\t" << bin << (1 << sizeof(int) * 8 - 1) << "(bin) " <<  endl;
    cout << "This is a string:\t\t" << "string" << "\n";
    cout << "This is a pointer:\t\t" << &cout << "\n";

    return 0;
}
