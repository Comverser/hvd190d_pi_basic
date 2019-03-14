#ifndef HVD190D_PI_WRITE_SPI_DRIV_H
#define HVD190D_PI_WRITE_SPI_DRIV_H

#include <wiringPi.h>
#include <iostream> // For the inline functions

namespace hvd190d_pi
{
    int const delay_loop_pi_ns = 1500;

    extern clock_t t_start;
    inline int t_lapsed()
    {
        return (((clock() - t_start) / (float)CLOCKS_PER_SEC) * 1000000);
    }
    inline unsigned long convert_to_spi(int ch, int v)
    {
        return 0x100000 | ( ( (0x000000 | (ch-1)) << 16) | v); 
    }

    enum Pin_spi { din = 19, sclk = 23, sync = 24 };
    enum Pin_trig { x = 38, y = 40 };
    enum Pin_misc { hv = 36 };
    void t_reset();
    void write_spi(unsigned long bits);
    void write_spi(unsigned long bits_p, unsigned long bits_n);
    void write_spi(int ch, int v);
    void write_spi(int ch_p, int v_p, int ch_n, int v_n);
    void write_trig_x(int signal);
    void write_trig_y(int signal);
    void initialize();
    void terminate();
    
    void write_bit(int bit);
    void setup_pi();
    void setup_dac();
    void enable_hv();
    void disable_hv();
}

#endif
