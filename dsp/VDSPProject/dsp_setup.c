#include <processor_include.h>
#include <sysreg.h>
#include <stdio.h>

#include "framework.h"
#include "dsp_setup.h"

void spi_init() {

	int wl = 128; // WP = 01
	int  timod = 2; // TIMOD = 10
	
	int slave_setup = SPIEN | CLKPL | CPHASE | MSBF | ISSEN | wl | timod;
	memreg_write(SPICTL, slave_setup);
}
