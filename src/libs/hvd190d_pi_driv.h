#ifndef HVD190D_PI_DRIV_H
#define HVD190D_PI_DRIV_H

#include <wiringPi.h>
#include <iostream> // for the inline functions

namespace hvd190d_pi
{
    int const delay_loop_pi_ns = 1500; // while loop delay - spi delay

    extern clock_t t_start;
    inline int t_lapsed()
    {
        return (((clock() - t_start) / (float)CLOCKS_PER_SEC) * 1000000);
    }
    inline unsigned long convert_to_spi(int ch, int v)
    {
        return 0x100000 | ( ( (0x000000 | (ch-1)) << 16) | v); 
    }

    void t_reset();
    void write_spi(unsigned long bits);
    void write_spi(unsigned long bits_p, unsigned long bits_n);
    void write_spi(int ch, int v);
    void write_spi(int ch_p, int v_p, int ch_n, int v_n);
    void write_trig_x(int signal);
    void write_trig_y(int signal);
    void initialize();
    void terminate();
}

#endif
