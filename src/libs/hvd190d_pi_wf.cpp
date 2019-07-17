#include "hvd190d_pi_wf.h" // "koc_wf_gen.h"
#include <cmath>
#include <iostream> // for debug
#include <bitset> // for debug : use of bitset function

namespace hvd190d_pi
{
    // public
    wf::wf(int p_adc_bits, double p_vpp_top, double p_vpp_bottom, int p_fs_max_x, double p_fc_x, koc::wf_gen::waveform_mode p_waveform_mode_x, double p_freq_x, double p_amp_x, double p_offset_x, double p_phase_x, double p_pulse_width_x, int p_fs_max_y, double p_fc_y, koc::wf_gen::waveform_mode p_waveform_mode_y, double p_freq_y, double p_amp_y, double p_offset_y, double p_phase_y, double p_pulse_width_y) :
        param_wf_x_p({p_adc_bits, p_vpp_top, p_vpp_bottom, p_fs_max_x, p_fc_x, 1, p_waveform_mode_x, p_freq_x, p_amp_x, p_offset_x, p_phase_x, p_pulse_width_x}),
        param_wf_y_p({p_adc_bits, p_vpp_top, p_vpp_bottom, p_fs_max_y, p_fc_y, 1, p_waveform_mode_y, p_freq_y, p_amp_y, p_offset_y, p_phase_y, p_pulse_width_y}),
        is_x_on(false),
        is_y_on(false),
        is_x_trig_on(false),
        is_y_trig_on(false),
        is_diff_on(false)
    {
    }

    void wf::set_is_x_on(bool p_is_x_on)
    {
        is_x_on = p_is_x_on;
    }

    void wf::set_is_y_on(bool p_is_y_on)
    {
        is_y_on = p_is_y_on;
    }

    void wf::set_is_x_trig_on(bool p_is_x_trig_on)
    {
        is_x_trig_on = p_is_x_trig_on;
    }

    void wf::set_is_y_trig_on(bool p_is_y_trig_on)
    {
        is_y_trig_on = p_is_y_trig_on;
    }

    void wf::set_is_diff_on(bool p_is_diff_on)
    {
        is_diff_on = p_is_diff_on;
    }

    void wf::set_param_wf(int xy, int p_fs_max, double p_fc, int p_waveform_mode, double p_freq, double p_amp, double p_offset, double p_phase, double p_pulse_width)
    {
        if (xy == 0)
        {
            param_wf_x_p.fs_max = p_fs_max; 
            param_wf_x_p.fc = p_fc;
            param_wf_x_p._waveform_mode = translate_waveform_mode(p_waveform_mode);
            param_wf_x_p.freq = p_freq;
            param_wf_x_p.amp = p_amp;
            param_wf_x_p.offset = p_offset;
            param_wf_x_p.phase = p_phase;
            param_wf_x_p.pulse_width = p_pulse_width;
        }
        else if (xy == 1)
        {
            param_wf_y_p.fs_max = p_fs_max; 
            param_wf_y_p.fc = p_fc;
            param_wf_y_p._waveform_mode = translate_waveform_mode(p_waveform_mode);
            param_wf_y_p.freq = p_freq;
            param_wf_y_p.amp = p_amp;
            param_wf_y_p.offset = p_offset;
            param_wf_y_p.phase = p_phase;
            param_wf_y_p.pulse_width = p_pulse_width;
        }
        else
        {
            std::cout << "error : hvd190d_pi::wf::set_param_wf()" << std::endl;
        }
    }

    void wf::run_wf_differential()
    {
        set_freq_precision();
        set_no_repetition();
        gen_wf_differential(is_x_on, param_wf_x_p, x_data_wf, is_x_trig_on);
        gen_wf_differential(is_y_on, param_wf_y_p, y_data_wf, is_y_trig_on);
        sort_wf_differential(is_x_on, is_y_on, _sorted_cmd_wf, x_data_wf, y_data_wf);
    }

    void wf::debug_s()
    {
//        debug_check_params(param_wf_x_p);
//        debug_check_params(param_wf_y_p);
//        debug_check_sorted_cmd_wf();
    }

    // private
    void wf::set_freq_precision(int p_precision_decimal_point)
    {
        precision_decimal_point = p_precision_decimal_point;
        double m = pow( 10, precision_decimal_point );
        param_wf_x_p.freq = round(param_wf_x_p.freq * m)/m;
        param_wf_y_p.freq = round(param_wf_y_p.freq * m)/m;
    }

    double wf::calc_period_gcd_freq()
    {
        int gcd = 1;
        double period_gcd;

        double m = pow(10, precision_decimal_point);
        int f1 = (int)(param_wf_x_p.freq * m);
        int f2 = (int)(param_wf_y_p.freq * m);
        if ( f1 == 0 | f2 == 0 )
        {
            f1 >= f2 ? (f2=f1) : (f1=f2);
        }

        for (int i=1; i <= f1 && i <= f2; ++i)
        {
            if ((f1 % i) == 0 && (f2 % i) == 0)
            {
                gcd = i;
            }
        }

        period_gcd = 1.0/( (double)gcd/m );

        return period_gcd;
    }

    void wf::calc_no_repetition()
    {
        double period_gcd = calc_period_gcd_freq();

        param_wf_x_p.no_repetition = round(param_wf_x_p.freq * period_gcd);
        param_wf_y_p.no_repetition = round(param_wf_y_p.freq * period_gcd);
    }

    void wf::set_no_repetition()
    {
        if ( (is_x_on == true) && (is_y_on == true) )
        {
            calc_no_repetition();
        }
        else
        {
            param_wf_x_p.no_repetition = 1;
            param_wf_y_p.no_repetition = 1;
        }
    }

    void wf::gen_wf_differential_pn(koc::wf_gen::param_wf param_wf_p, data_wf_digital_pn& ref_data_wf_digital_pn, bool p_is_trig_on)
    {
            // p
            koc::wf_gen wf_p(param_wf_p.adc_bits, param_wf_p.vpp_top, param_wf_p.vpp_bottom, param_wf_p.fs_max, param_wf_p.fc, param_wf_p.no_repetition, param_wf_p._waveform_mode, param_wf_p.freq, param_wf_p.amp, param_wf_p.offset, param_wf_p.phase, param_wf_p.pulse_width);
            wf_p.gen_wf();
            wf_p.gen_wf_t_us();
            wf_p.gen_wf_v_digital();
            if (p_is_trig_on == true)
            {
                wf_p.gen_wf_trig();
            }

            ref_data_wf_digital_pn.p.wf_t_us = wf_p.get_wf_t_us(); 
            ref_data_wf_digital_pn.p.wf_v_digital = wf_p.get_wf_v_digital();
            ref_data_wf_digital_pn.p.wf_trig = wf_p.get_wf_trig();

//            wf_p.debug();

            // n
            if (is_diff_on == true)
            {
                koc::wf_gen wf_n(param_wf_p.adc_bits, param_wf_p.vpp_top, param_wf_p.vpp_bottom, param_wf_p.fs_max, param_wf_p.fc, param_wf_p.no_repetition, param_wf_p._waveform_mode, param_wf_p.freq, -param_wf_p.amp, param_wf_p.offset, param_wf_p.phase, param_wf_p.pulse_width);
                wf_n.gen_wf();
                wf_n.gen_wf_t_us();
                wf_n.gen_wf_v_digital();

                ref_data_wf_digital_pn.n.wf_v_digital = wf_n.get_wf_v_digital();

//                wf_n.debug();
            }
            else
            {
                ref_data_wf_digital_pn.n = {};
            }
    }

    void wf::gen_wf_differential(bool p_is_on, koc::wf_gen::param_wf param_wf_p, data_wf_digital_pn& ref_data_wf_digital_pn, bool p_is_trig_on)
    {
        if ( p_is_on == true )
        {
            gen_wf_differential_pn(param_wf_p, ref_data_wf_digital_pn, p_is_trig_on);
        }
        else
        {
            ref_data_wf_digital_pn = {};
        }
    }

    void wf::sort_wf_differential_xy(sorted_cmd_wf& ref_sorted_cmd, data_wf_digital_pn& ref_x, data_wf_digital_pn& ref_y)
    {
        ref_sorted_cmd.t_us.clear();
        ref_sorted_cmd.t_us.shrink_to_fit();
        ref_sorted_cmd.cmd_wf_p.clear();
        ref_sorted_cmd.cmd_wf_p.shrink_to_fit();
        ref_sorted_cmd.cmd_wf_n.clear();
        ref_sorted_cmd.cmd_wf_n.shrink_to_fit();

        int size_wf_t_p = ref_x.p.wf_t_us.size() + ref_y.p.wf_t_us.size(); 

//        ref_sorted_cmd.trig_x.reserve(size_wf_t_p);
//        ref_sorted_cmd.trig_y.reserve(size_wf_t_p);

        ref_sorted_cmd.t_us.reserve(size_wf_t_p);
        auto it_wf_t = ref_sorted_cmd.t_us.begin();

        if (is_diff_on == false)
        {
            ref_sorted_cmd.cmd_wf_p.reserve(size_wf_t_p);

            int j = 0;
            int k = 0;
            auto it_cmd_wf_p = ref_sorted_cmd.cmd_wf_p.begin();

            for (int i = 0; i < ref_sorted_cmd.t_us.capacity(); i++)
            {
                if ( ((ref_x.p.wf_t_us[j] <= ref_y.p.wf_t_us[k]) && (j < ref_x.p.wf_t_us.size())) || k == ref_y.p.wf_t_us.size() )
                {
                    ref_sorted_cmd.t_us.insert( it_wf_t + i, ref_x.p.wf_t_us[j] );
                    ref_sorted_cmd.cmd_wf_p.insert( it_cmd_wf_p + i, convert_to_cmd_dac_quad_datum(0, ref_x.p.wf_v_digital[j]) );

                    j++;
                }
                else
                {
                    ref_sorted_cmd.t_us.insert( it_wf_t + i, ref_y.p.wf_t_us[k] );
                    ref_sorted_cmd.cmd_wf_p.insert( it_cmd_wf_p + i, convert_to_cmd_dac_quad_datum(2, ref_y.p.wf_v_digital[k]) );

                    k++;
                }
            }
        }
        else
        {
            ref_sorted_cmd.cmd_wf_p.reserve(size_wf_t_p);
            ref_sorted_cmd.cmd_wf_n.reserve(size_wf_t_p);

            int j = 0;
            int k = 0;
            auto it_cmd_wf_p = ref_sorted_cmd.cmd_wf_p.begin();
            auto it_cmd_wf_n = ref_sorted_cmd.cmd_wf_n.begin();

            for (int i = 0; i < ref_sorted_cmd.t_us.capacity(); i++)
            {
                if ( ((ref_x.p.wf_t_us[j] <= ref_y.p.wf_t_us[k]) && (j < ref_x.p.wf_t_us.size())) || k == ref_y.p.wf_t_us.size() )
                {
                    ref_sorted_cmd.t_us.insert( it_wf_t + i, ref_x.p.wf_t_us[j] );
                    ref_sorted_cmd.cmd_wf_p.insert( it_cmd_wf_p + i, convert_to_cmd_dac_quad_datum(0, ref_x.p.wf_v_digital[j]) );
                    ref_sorted_cmd.cmd_wf_n.insert( it_cmd_wf_n + i, convert_to_cmd_dac_quad_datum(1, ref_x.n.wf_v_digital[j]) );

                    j++;
                }
                else
                {
                    ref_sorted_cmd.t_us.insert( it_wf_t + i, ref_y.p.wf_t_us[k] );
                    ref_sorted_cmd.cmd_wf_p.insert( it_cmd_wf_p + i, convert_to_cmd_dac_quad_datum(2, ref_y.p.wf_v_digital[k]) );
                    ref_sorted_cmd.cmd_wf_n.insert( it_cmd_wf_n + i, convert_to_cmd_dac_quad_datum(3, ref_y.n.wf_v_digital[k]) );

                    k++;
                }
            }
        }

    }

    void wf::sort_wf_differential(bool p_is_x_on, bool p_is_y_on, sorted_cmd_wf& ref_sorted_cmd, data_wf_digital_pn& ref_x, data_wf_digital_pn& ref_y)
    {
        if ( (p_is_x_on == true) && (p_is_y_on == true) )
        {
            sort_wf_differential_xy(ref_sorted_cmd, ref_x, ref_y);

        }
        else if (p_is_x_on == true)
        {
            ref_sorted_cmd.t_us = ref_x.p.wf_t_us;
            ref_sorted_cmd.cmd_wf_p = convert_to_cmd_dac_quad_vector(0, ref_x.p.wf_v_digital);
            ref_sorted_cmd.cmd_wf_n = convert_to_cmd_dac_quad_vector(1, ref_x.n.wf_v_digital);
            ref_sorted_cmd.trig_x = ref_x.p.wf_trig;
            ref_sorted_cmd.trig_y.clear();
        }
        else if (p_is_y_on == true)
        {
            ref_sorted_cmd.t_us = ref_y.p.wf_t_us;
            ref_sorted_cmd.cmd_wf_p = convert_to_cmd_dac_quad_vector(2, ref_y.p.wf_v_digital);
            ref_sorted_cmd.cmd_wf_n = convert_to_cmd_dac_quad_vector(3, ref_y.n.wf_v_digital);
            ref_sorted_cmd.trig_x.clear();
            ref_sorted_cmd.trig_y = ref_y.p.wf_trig;
        }
        else
        {
            ref_sorted_cmd = {};
        }
    }

    std::vector<unsigned long> wf::convert_to_cmd_dac_quad_vector(int ch, std::vector<unsigned long> v_digital)
    {
        std::vector<unsigned long> buffer;
        buffer.reserve(v_digital.size());
        auto it = buffer.begin();

        for (int i = 0; i < buffer.capacity(); i++)
        {
            buffer.insert( it + i, convert_to_cmd_dac_quad_datum(ch, v_digital[i]) );
        }

        return buffer;
    }

    koc::wf_gen::waveform_mode wf::translate_waveform_mode(int int_wf_mod)
    {
        using namespace koc;

        switch (int_wf_mod)
        {
            case 0: return wf_gen::waveform_mode::std_sine;
            case 1: return wf_gen::waveform_mode::std_triangle;
            case 2: return wf_gen::waveform_mode::ramp;
            case 3: return wf_gen::waveform_mode::std_square;
            case 4: return wf_gen::waveform_mode::std_pulse;
        }
    }

    void wf::debug_check_params(koc::wf_gen::param_wf& p_ref_param_wf)
    {
        std::cout << std::endl;
        std::cout << "//////////////////// hvd190d_pi_wf : debug() ////////////////////" << std::endl;
        std::cout << std::endl;

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
        std::cout << "//////////////////// hvd190d_pi_wf : debug() ////////////////////" << std::endl;
        std::cout << std::endl;
    }

    void wf::debug_check_sorted_cmd_wf()
    {
        std::cout << "//////////////////// debug_check_sorted_cmd_wf()" << std::endl;

        int k = 0;
        for (auto i : _sorted_cmd_wf.t_us)
        {
            std::cout << "t_us[" << k++ << "] : " << i << std::endl;
        }

        k = 0;
        for (auto i : _sorted_cmd_wf.cmd_wf_p)
        {
            std::cout << "cmd_wf_p[" << k++ << "] : " << std::bitset<24>(i) << std::endl;
        }

        k = 0;
        for (auto i : _sorted_cmd_wf.cmd_wf_n)
        {
            std::cout << "cmd_wf_n[" << k++ << "] : " << std::bitset<24>(i) << std::endl;
        }

        k = 0;
        for (auto i : _sorted_cmd_wf.trig_x)
        {
            std::cout << "trig_x[" << k++ << "] : " << i << std::endl;
        }

        k = 0;
        for (auto i : _sorted_cmd_wf.trig_y)
        {
            std::cout << "trig_y[" << k++ << "] : " << i << std::endl;
        }
    }

}
