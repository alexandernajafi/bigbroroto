#include <processor_include.h>
#include <sysreg.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>

#include "framework.h"


void main()
{   
	dsp_init();
	dsp_start();
	/*spi_init_slave();
	spi_send();
	//spi_read();*/
	test();
	spi_read();
}
