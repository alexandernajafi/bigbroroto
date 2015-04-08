#include <processor_include.h>
#include <sysreg.h>
#include <signal.h>
#include <string.h>
#include <filter.h>
#include <stdio.h>
#include <stdlib.h>

#include "framework.h"

void spi_send(){
	
}

void main()
{   
	dsp_init();
	dsp_start();
	spi_init_slave();
}