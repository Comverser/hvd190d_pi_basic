#include "hvd190d_pi_driv.h" // <iostream>
#include "rpi_reg_access.h"

namespace hvd190d_pi
{
    // private
    enum pin_spi { din = 10, sclk = 11, sync = 8 };
    enum pin_trig { x = 20, y = 21 };
    enum pin_misc { hv = 16 };

    static void write_bit(int bit)
    { 
        gpio_high(sclk);
        gpio_write(din, bit);
        gpio_low(sclk);
    }

    static void setup_pi()
    {
        gpio_init();
        gpio_mode_out(din);
        gpio_mode_out(sclk);
        gpio_mode_out(sync);
        gpio_mode_out(x);
        gpio_mode_out(y);
        gpio_mode_out(hv);
    }

    static void setup_dac()
    {
        write_spi(0x280001); // full reset
        write_spi(0x20000F); // power-down mode: all DAC channels power-up
    }

    static void power_down_dac()
    {
        write_spi(0x20001F); // power-down mode: 1 kohm to GND
    }

    static void enable_hv()
    {
        gpio_high(hv);
    }

    static void disable_hv()
    {
        gpio_low(hv);
    }
    
    // public
    clock_t t_start = 0;
    clock_t t_start_ = 0;

    void t_reset()
    {
        t_start = clock();
    }

    void t_reset_()
    {
        t_start_ = clock();
    }

    void write_spi(unsigned long bits)
    { 
        gpio_low(sync);
        for (int i = 0; i < 24; i++) 
        {
            write_bit((bits >> (23 - i)) & 0x01); 
        }
        gpio_high(sync);
    }

    void write_spi(unsigned long bits_p, unsigned long bits_n)
    { 
        gpio_low(sync);
        for (int i = 0; i < 24; i++) 
        {
            write_bit((bits_p >> (23 - i)) & 0x01); 
        }
        gpio_high(sync);
        gpio_low(sync);
        for (int i = 0; i < 24; i++) 
        {
            write_bit((bits_n >> (23 - i)) & 0x01); 
        }
        gpio_high(sync);
    }

    void write_spi(int ch, unsigned long v_digital)
    {
        write_spi(convert_to_spi(ch, v_digital));
    }

    void write_spi(int ch_p, unsigned long v_digital_p, int ch_n, unsigned long v_digital_n)
    {
        write_spi(convert_to_spi(ch_p, v_digital_p));
        write_spi(convert_to_spi(ch_n, v_digital_n));
    }

    void write_trig_x(int signal)
    {
        gpio_write(x, signal);
    }

    void write_trig_y(int signal)
    {
        gpio_write(y, signal);
    }

    void initialize()
    {
        setup_pi();
        setup_dac();
        enable_hv();
    }

    void terminate()
    {
        disable_hv();
//        power_down_dac();
    }

}
