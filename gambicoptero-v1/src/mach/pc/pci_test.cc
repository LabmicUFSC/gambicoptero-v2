// EPOS PC PCI Test Program

// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.

#include <utility/ostream.h>
#include <pci.h>

__USING_SYS

int main()
{
    OStream cout;

    cout << "PC_PCI test\n";

    PC_PCI pci;

    for(int bus = 0; bus <= Traits<PC_PCI>::MAX_BUS; bus++) {
	for(int dev_fn = 0; dev_fn <= Traits<PC_PCI>::MAX_DEV_FN; dev_fn++) {
	    PC_PCI::Header hdr;
	    
	    pci.header(PC_PCI::Locator(bus, dev_fn), &hdr);
	    if(hdr) {
		cout << (int)bus << ":" << (dev_fn >> 3) << "." << (dev_fn & 7)
		     << " Class " << hdr.class_id << " " << hdr.vendor_id
		     << ":" << hdr.device_id
		     << " (rev " << (int)hdr.revision_id
		     << ")";
		for(int i = 0; i < PC_PCI::Region::N; i++)
		    if(hdr.region[i].size)
			cout << " #" << i 
			     << "(" << (void *)hdr.region[i].phy_addr
			     << "," << (void *)hdr.region[i].size << ")";
		cout << "\n";
	    }
	}
    }

    PC_PCI::Locator loc = pci.scan(32902, 28945, 0);
	
    PC_PCI::Header hdr;
    pci.header(loc, &hdr);
    if(hdr) {
	cout << hdr.locator.bus << ":" << (hdr.locator.dev_fn >> 3)
	     << "." << (hdr.locator.dev_fn & 7)
	     << " Class " << hdr.class_id << " " << hdr.vendor_id
	     << ":" << hdr.device_id 
	     << " (rev " << (int)hdr.revision_id
	     << ")";
	for(int i = 0; i < 6; i++)
	    if(hdr.region[i].size)
		cout << " #" << i 
		     << "(" << (void *)hdr.region[i].phy_addr
		     << "," << (void *)hdr.region[i].size << ")";
	cout << "\n";
    }

    cout << "Finish!\n";

    return 0;
}
