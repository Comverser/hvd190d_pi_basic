#include "hvd190d_pi_wf.h" // "koc_wf_gen.h"
//#include "hvd190d_pi_driv.h" // <wiringPi.h>, <iostream>
#include <cmath>
#include <iostream> // for debug

namespace hvd190d_pi
{
    // public
    wf::wf(int p_adc_bits, double p_vpp_top, double p_vpp_bottom, int p_fs_max_x, double p_fc_x, koc::wf_gen::waveform_mode p_waveform_mode_x, double p_freq_x, double p_amp_x, double p_offset_x, double p_phase_x, double p_pulse_width_x, int p_fs_max_y, double p_fc_y, koc::wf_gen::waveform_mode p_waveform_mode_y, double p_freq_y, double p_amp_y, double p_offset_y, double p_phase_y, double p_pulse_width_y) :
        param_wf_x_p({p_adc_bits, p_vpp_top, p_vpp_bottom, p_fs_max_x, p_fc_x, 1, p_waveform_mode_x, p_freq_x, p_amp_x, p_offset_x, p_phase_x, p_pulse_width_x}),
        param_wf_x_n({p_adc_bits, p_vpp_top, p_vpp_bottom, p_fs_max_x, p_fc_x, 1, p_waveform_mode_x, p_freq_x, -p_amp_x, p_offset_x, p_phase_x, p_pulse_width_x}),
        param_wf_y_p({p_adc_bits, p_vpp_top, p_vpp_bottom, p_fs_max_y, p_fc_y, 1, p_waveform_mode_y, p_freq_y, p_amp_y, p_offset_y, p_phase_y, p_pulse_width_y}),
        param_wf_y_n({p_adc_bits, p_vpp_top, p_vpp_bottom, p_fs_max_y, p_fc_y, 1, p_waveform_mode_y, p_freq_y, -p_amp_y, p_offset_y, p_phase_y, p_pulse_width_y})
    {
    }

    void wf::freq_precision(int p_precision_decimal_point)
    {
        precision_decimal_point = p_precision_decimal_point;
        double m = pow( 10, precision_decimal_point );
        param_wf_x_p.freq = round(param_wf_x_p.freq * m)/m;
        param_wf_y_p.freq = round(param_wf_y_p.freq * m)/m;
        std::cout << " -> freq1 : " << param_wf_x_p.freq << std::endl;
        std::cout << " -> freq2 : " << param_wf_y_p.freq << std::endl;
    }

    double wf::gcd_freq()
    {
        double m = pow( 10, precision_decimal_point );
        int f1 = (int)(param_wf_x_p.freq * m);
        int f2 = (int)(param_wf_y_p.freq * m);
        if ( f1 == 0 | f2 == 0 )
        {
            f1 >= f2 ? (f2=f1) : (f1=f2);
        }
        std::cout << " -> f1 : " << f1 << std::endl;
        std::cout << " -> f2 : " << f2 << std::endl;

        int gcd = 1;

        for (int i=1; i <= f1 && i <= f2; ++i)
        {
            if ((f1 % i) == 0 && (f2 % i) == 0)
            {
                gcd = i;
            }
        }
        std::cout << " -> gcd : " << gcd/m << std::endl;
        std::cout << " -> period_gcd : " << 1.0/(gcd/m) << std::endl;
        return 1.0/( (double)gcd/m );
    }

    // private
    void wf::debug_check_params(koc::wf_gen::param_wf& p_ref_param_wf)
    {
        static int i;
        std::cout << "//////////////////// No. " << i << std::endl;
        i++;
        std::cout << "adc_bits      : " << p_ref_param_wf.adc_bits << std::endl;
        std::cout << "vpp_top       : " << p_ref_param_wf.vpp_top << std::endl;
        std::cout << "vpp_bottm     : " << p_ref_param_wf.vpp_bottom << std::endl;
        std::cout << "fs_max        : " << p_ref_param_wf.fs_max << std::endl;
        std::cout << "fc            : " << p_ref_param_wf.fc << std::endl;
        std::cout << "no_repetition : " << p_ref_param_wf.no_repetition << std::endl;

        if (param_wf_x_p._waveform_mode == koc::wf_gen::waveform_mode::std_triangle)
        {
            std::cout << "waveform_mode : match" << std::endl;
        }
        else
        {
            std::cout << "waveform_mode : not match" << std::endl;
        }

        
        std::cout << "freq          : " << p_ref_param_wf.freq << std::endl;
        std::cout << "amp           : " << p_ref_param_wf.amp << std::endl;
        std::cout << "offset        : " << p_ref_param_wf.offset << std::endl;
        std::cout << "phase         : " << p_ref_param_wf.phase << std::endl;
        std::cout << "width         : " << p_ref_param_wf.pulse_width << std::endl;
        std::cout << std::endl;
    }

    void wf::debug_s()
    {
        debug_check_params(param_wf_x_p);
        debug_check_params(param_wf_x_n);
        debug_check_params(param_wf_y_p);
        debug_check_params(param_wf_y_n);
    }

}
/*
            // x_p
            param_wf x_p;
            x_p.adc_bits = 16;
            x_p.vpp_top = 200;
            x_p.vpp_bottom = 0;
            x_p.fs_max = 25000;
            x_p.fc = 0;
            x_p.no_repetition = 1;
            x_p._waveform_mode = koc::wf_gen::waveform_mode::std_triangle;
            x_p.freq = 100;
            x_p.amp = 100;
            x_p.offset = 100;
            x_p.phase = 0;
            x_p.pulse_width = 0;

            koc::wf_gen wf_x_p(x_p.adc_bits, x_p.vpp_top, x_p.vpp_bottom, x_p.fs_max, x_p.fc, x_p.no_repetition, x_p._waveform_mode, x_p.freq, x_p.amp, x_p.offset, x_p.phase, x_p.pulse_width);
            wf_x_p.gen_wf();
            wf_x_p.gen_wf_t_us();
            wf_x_p.gen_wf_v_digital();
            wf_x_p.gen_wf_trig();

            x_p.wf_t_us = wf_x_p.get_wf_t_us(); 
            x_p.wf_v_digital = wf_x_p.get_wf_v_digital();
            x_p.wf_trig = wf_x_p.get_wf_trig();

            // x_n
            param_wf x_n = x_p;
            reverse_sign_wf_amp(x_n);

            koc::wf_gen wf_x_n(x_n.adc_bits, x_n.vpp_top, x_n.vpp_bottom, x_n.fs_max, x_n.fc, x_n.no_repetition, x_n._waveform_mode, x_n.freq, x_n.amp, x_n.offset, x_n.phase, x_n.pulse_width);
            wf_x_n.gen_wf();
            wf_x_n.gen_wf_t_us();
            wf_x_n.gen_wf_v_digital();

            x_n.wf_t_us = wf_x_n.get_wf_t_us(); 
            x_n.wf_v_digital = wf_x_n.get_wf_v_digital();
            x_n.wf_trig = wf_x_n.get_wf_trig();

            // y_p
            param_wf y_p;
            y_p.adc_bits = 16;
            y_p.vpp_top = 200;
            y_p.vpp_bottom = 0;
            y_p.fs_max = 4;
            y_p.fc = 0;
            y_p.no_repetition = 1;
            y_p._waveform_mode = koc::wf_gen::waveform_mode::std_square;
            y_p.freq = 2;
            y_p.amp = 10;
            y_p.offset = 10;
            y_p.phase = 0;
            y_p.pulse_width = 0;

            koc::wf_gen wf_y_p(y_p.adc_bits, y_p.vpp_top, y_p.vpp_bottom, y_p.fs_max, y_p.fc, y_p.no_repetition, y_p._waveform_mode, y_p.freq, y_p.amp, y_p.offset, y_p.phase, y_p.pulse_width);
            wf_y_p.gen_wf();
            wf_y_p.gen_wf_t_us();
            wf_y_p.gen_wf_v_digital();
            wf_y_p.gen_wf_trig();

            y_p.wf_t_us = wf_y_p.get_wf_t_us(); 
            y_p.wf_v_digital = wf_y_p.get_wf_v_digital();
            y_p.wf_trig = wf_y_p.get_wf_trig();

            // y_n
            param_wf y_n = y_p;
            reverse_sign_wf_amp(y_n);

            koc::wf_gen wf_y_n(y_n.adc_bits, y_n.vpp_top, y_n.vpp_bottom, y_n.fs_max, y_n.fc, y_n.no_repetition, y_n._waveform_mode, y_n.freq, y_n.amp, y_n.offset, y_n.phase, y_n.pulse_width);
            wf_y_n.gen_wf();
            wf_y_n.gen_wf_t_us();
            wf_y_n.gen_wf_v_digital();

            y_n.wf_t_us = wf_y_n.get_wf_t_us(); 
            y_n.wf_v_digital = wf_y_n.get_wf_v_digital();
            y_n.wf_trig = wf_y_n.get_wf_trig();
*/
// loop delay compensation
//
