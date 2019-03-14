#include "driv.h" // <wiringPi.h> <iostream>

int main()
{ 
    hvd190d_pi::initialize();
	
    hvd190d_pi::write_spi(1,50000,2,30000);

    std::cin.get();

    hvd190d_pi::terminate();

	return 0; 
}
