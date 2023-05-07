#include "pic.h"
#include "io.h"

void pic_init()
{
	// set up cascading mode
	outb(PIC_MASTER_CMD, 0x11);
	outb(PIC_SLAVE_CMD,  0x11);
	// Setup master's vector offset
	outb(PIC_MASTER_DATA, 0x20);
	// Tell the slave its vector offset
	outb(PIC_SLAVE_DATA, 0x28);
	// Tell the master that he has a slave (sounds funny)
	outb(PIC_MASTER_DATA, 4);
	outb(PIC_SLAVE_DATA, 2);
	// Enabled 8086 mode (default is 8080)
	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);

	outb(PIC_MASTER_DATA, 0x0);
	outb(PIC_SLAVE_DATA, 0x0);
}

void pci_eoi(uint8_t irq)
{
    // check if IRQ came from the Slave PIC and if so issue the cmd to both chips
    if (irq >= 8)
        outb(PIC_SLAVE_CMD, 0x20);
	outb(PIC_MASTER_CMD, 0x20);
}