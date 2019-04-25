#include "koc_wf_gen.h"
#include <fstream> // ofstream 
#include <cmath>

//////////////////// for debug
#include <iostream>
#include <iomanip>

namespace koc
{
    // public
    wf_gen::wf_gen(int p_adc_bits, double p_vpp_top, double p_vpp_bottom, int p_fs_max, double p_fc, int p_no_repetition, waveform_mode p_waveform_mode, double p_freq, double p_amp, double p_offset, double p_phase, double p_pulse_width) : 
        adc_bits(p_adc_bits),
        vpp_top(p_vpp_top),
        vpp_bottom(p_vpp_bottom),
        fs_max(p_fs_max),
        fc(p_fc),
        no_repetition(p_no_repetition),
        _waveform_mode(p_waveform_mode),
        freq(p_freq),
        amp(p_amp),
        offset(p_offset),
        phase(p_phase),
        pulse_width(p_pulse_width),
        fs(0.0),
        fs_per_period(0),
        freq_max_eight_bits(0.0)
    {
    }

    void wf_gen::gen_wf()
    {
        double ts;
        double v;
        int fs_per_period_multiple;

        if (_waveform_mode == waveform_mode::std_sine)
        {
            gen_fs_per_period(even_odd_mode::multiples_of_four);
            fs_per_period_multiple = ( (fs_per_period==1) ? fs_per_period : fs_per_period * no_repetition );
            ts = 1.0/fs;
            freq_max_eight_bits = fs_max/256;
            wf_t.reserve(fs_per_period_multiple);
            wf_v.reserve(fs_per_period_multiple);
            auto it_t = wf_t.begin();
            auto it_v = wf_v.begin();

            for (int i = 0; i < wf_t.capacity(); i++) // generate times
            {
                wf_t.insert(it_t + i, ts*(i + 1)); // remove start-and-end point error of the algorithm
            }

            for (int i = 0; i < wf_t.capacity(); i++) // generate voltages 
            {
                v = amp*sin(2.0*M_PI*freq*wf_t[i] + to_radian(phase)) + offset;
                wf_v.insert(it_v + i, v); 
            }
        }
        else if (_waveform_mode == waveform_mode::std_triangle)
        {
            gen_fs_per_period(even_odd_mode::multiples_of_four);
            fs_per_period_multiple = ( (fs_per_period==1) ? fs_per_period : fs_per_period * no_repetition );
            ts = 1.0/fs;
            freq_max_eight_bits = fs_max/256;
            wf_t.reserve(fs_per_period_multiple);
            wf_v.reserve(fs_per_period_multiple);
            auto it_t = wf_t.begin();
            auto it_v = wf_v.begin();

            for (int i = 0; i < wf_t.capacity(); i++) // generate times
            {
                wf_t.insert(it_t + i, ts*(i + 1)); // remove start-and-end point error of the algorithm
            }

            if ( fs_per_period == 1) // DC mode
            {
                int m_period = fs_max;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) ) + (double)m_period/4.0;
                int param_norm = -1;
                int m_position = 0;

                v = (double)(m_period/2.0) - abs( ( (m_position+m_phase) % m_period ) - (double)(m_period/2.0) );
                v = ( v / (double)(m_period/4.0) ) + param_norm; // normalization
                v = v * amp + offset; // modulation 
                wf_v.insert(it_v, v); 
            }
            else
            {
                int m_period = fs_per_period;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) ) + (double)m_period/4.0;
                int param_norm = -1;

                for (int i = 0; i < wf_v.capacity(); i++) // generate voltages 
                {
                    int m_position = i + 1;

                    v = (double)(m_period/2.0) - abs( ( (m_position+m_phase) % m_period ) - (double)(m_period/2.0) );
                    v = ( v / (double)(m_period/4.0) ) + param_norm; // normalization
                    v = v * amp + offset; // modulation 
                    wf_v.insert(it_v + i, v); 
                }
            }
        }
        else if (_waveform_mode == waveform_mode::ramp)
        {
            gen_fs_per_period(even_odd_mode::even);
            fs_per_period_multiple = ( (fs_per_period==1) ? fs_per_period : fs_per_period * no_repetition );
            ts = 1.0/fs;
            freq_max_eight_bits = fs_max/256;
            wf_t.reserve(fs_per_period_multiple); // ramp function is many-to-one function
            wf_v.reserve(fs_per_period_multiple);
            auto it_t = wf_t.begin();
            auto it_v = wf_v.begin();

            for (int i = 0; i < wf_t.capacity(); i++) // generate times
            {
                wf_t.insert(it_t + i, ts*(i + 1)); // remove start-and-end point error of the algorithm
            }

            if ( fs_per_period == 1 )
            {
                int m_period = fs_max;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) ) + (double)m_period/2.0;
                int param_norm = -1;
                int m_position = 0;

                v = ( m_position + m_phase ) % m_period;
                v = ( v / (double)(m_period + param_norm) ) * 2.0 + param_norm; // normalization
                v = v * amp + offset; // modulation 
                wf_v.insert(it_v, v); 
            }
            else
            {
                int m_period = fs_per_period;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) ) + (double)m_period/2.0;
                int param_norm = -1;

                for (int i = 0; i < wf_v.capacity(); i++) // generate voltages 
                {
                    int m_position = i + 1;

                    v = ( m_position + m_phase ) % m_period;
                    v = ( v / (double)(m_period + param_norm) ) * 2.0 + param_norm; // normalization
                    v = v * amp + offset; // modulation 
                    wf_v.insert(it_v + i, v); 
                }
            }
        }
        else if (_waveform_mode == waveform_mode::std_square)
        {
            gen_fs_per_period(even_odd_mode::even);
            fs_per_period_multiple = ( (fs_per_period==1) ? fs_per_period : fs_per_period * no_repetition );
            ts = 1.0/fs;
            freq_max_eight_bits = fs_max/2;
            wf_t.reserve(fs_per_period_multiple);
            wf_v.reserve(fs_per_period_multiple);
            auto it_t = wf_t.begin();
            auto it_v = wf_v.begin();

            for (int i = 0; i < wf_t.capacity(); i++) // generate times
            {
                wf_t.insert(it_t + i, ts*(i + 1)); // remove start-and-end point error of the algorithm
            }

            if ( fs_per_period == 1 )
            {
                int m_period = fs_max;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) ) + (double)m_period/2.0;
                int param_norm = -1;
                int m_position = 0;

                v =  ( m_position + m_phase ) % m_period;
                v = ( floor( v / (double)(m_period/2) ) )*2 + param_norm; // normalization
                v = v * amp + offset; // modulation 
                wf_v.insert(it_v, v); 
            }
            else
            {
                int m_period = fs_per_period;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) ) + (double)m_period/2.0;
                int param_norm = -1;

                for (int i = 0; i < wf_v.capacity(); i++) // generate voltages 
                {
                    int m_position = i + 1;

                    v =  ( m_position + m_phase ) % m_period;
                    v = ( floor( v / (int)(m_period/2) ) )*2 + param_norm; // normalization
                    v = v * amp + offset; // modulation 
                    wf_v.insert(it_v + i, v); 
                }
            }
        }
        else if (_waveform_mode == waveform_mode::std_pulse)
        {
            gen_fs_per_period(even_odd_mode::even);
            fs_per_period_multiple = ( (fs_per_period==1) ? fs_per_period : fs_per_period * no_repetition );
            ts = 1.0/fs;
            freq_max_eight_bits = fs_max/3;
            wf_t.reserve(fs_per_period_multiple);
            wf_v.reserve(fs_per_period_multiple);
            auto it_t = wf_t.begin();
            auto it_v = wf_v.begin();

            for (int i = 0; i < wf_t.capacity(); i++) // generate times
            {
                wf_t.insert(it_t + i, ts*(i + 1)); // remove start-and-end point error of the algorithm
            }

            if ( fs_per_period == 1 )
            {
                v = 1.0;
                v = v * amp + offset; // modulation 
                wf_v.insert(it_v, v); 
            }
            else
            {
                int m_period = fs_per_period;
                int m_phase = (int)( m_period * (double)(phase/360.0) ) - (int)( m_period * (floor)(phase/360.0) );
                int lead = m_phase;
                int trail = lead + (int)( floor( pulse_width / ts ) );

                for (int i = 0; i < wf_v.capacity(); i++) // initialize vector 
                {
                    int idx = i % fs_per_period;

                    if ( trail > fs_per_period )
                    {
                        if ( (idx > lead) || (idx <= (trail - fs_per_period)) )
                        {
                            v = 1.0;
                        }
                        else
                        {
                            v = 0.0;
                        }
                    }
                    else
                    {
                        if ( (idx == 0) && (trail == fs_per_period) )
                        {
                            v = 1.0;
                        }
                        else if ( (idx > lead) && (idx <= trail) )
                        {
                            v = 1.0;
                        }
                        else
                        {
                            v = 0.0;
                        }
                    }
                    v = v * amp + offset; // modulation 
                    wf_v.insert(it_v + i, v); 
                }
            }
        }
    }

    void wf_gen::gen_wf_t_us()
    {
        unsigned long t_us;
        
        wf_t_us.reserve(wf_t.size());
        auto it_t_us= wf_t_us.begin();

        for ( int i = 0; i < wf_t_us.capacity(); i++ )
        {
            t_us = (unsigned long)( 1000000 * wf_t[i] );
            wf_t_us.insert(it_t_us + i, t_us); 
        }
    }

    void wf_gen::gen_wf_v_digital()
    {
        double v_d_norm;
        unsigned long v_d;
        int v_res = pow(2, adc_bits) - 1;

        wf_v_digital.reserve(wf_v.size());
        auto it_v_digital = wf_v_digital.begin();

        for ( int i = 0; i < wf_v_digital.capacity(); i++ )
        {
            v_d_norm = (wf_v[i]-vpp_bottom)/(vpp_top-vpp_bottom);
            if ( v_d_norm < 0 )
            {
                v_d_norm = 0;
                std::cout << "error : adc_volts()" << std::endl;
            }
            v_d = (int)( v_res * v_d_norm );
            wf_v_digital.insert(it_v_digital + i, v_d); 
        }
    }

    void wf_gen::gen_wf_trig()
    {
        wf_trig.reserve(wf_t.size());

        auto it_wf_trig = wf_trig.begin();

        for ( int i = 0; i < wf_trig.capacity(); i++ )
        {
            if ( (i % fs_per_period) < (fs_per_period/2 - 1) )
            {
                wf_trig.insert(it_wf_trig + i, true); 
            }
            else if ( (i % fs_per_period) == (fs_per_period - 1) )
            {
                wf_trig.insert(it_wf_trig + i, true); 
            }
            else
            {
                wf_trig.insert(it_wf_trig + i, false); 
            }
        }
    }

    std::vector<double> wf_gen::get_wf_t()
    {
        return wf_t;
    }

    std::vector<unsigned long> wf_gen::get_wf_t_us()
    {
        return wf_t_us;
    }

    std::vector<unsigned long> wf_gen::get_wf_v_digital()
    {
        return wf_v_digital;
    }

    std::vector<double> wf_gen::get_wf_v()
    {
        return wf_v;
    }
    
    std::vector<int> wf_gen::get_wf_trig()
    {
        return wf_trig;
    }

    void wf_gen::export_wf(analog_digital_mode p_analog_digital_mode)
    {
        std::ofstream f_wf;
        f_wf.open("wf.csv");

        if ( f_wf.is_open() )
        {
            if (p_analog_digital_mode == analog_digital_mode::digital)
            {
                for (int i = 0; i < wf_t_us.size(); i++)
                {
                    f_wf << wf_t_us[i] << ",";
                    f_wf << wf_v_digital[i] << ",\n";
                }
            }
            else
            {
                for (int i = 0; i < wf_t_us.size(); i++)
                {
                    f_wf << wf_t[i] << ",";
                    f_wf << wf_v[i] << ",\n";
                }
            }
        }
        else
        {
            std::cout << "error : export_wf()" << std::endl;
        }
        f_wf.close();

    }

    double wf_gen::get_freq_max_eight_bits()
    {
        return freq_max_eight_bits;
    }

    void wf_gen::debug()
    {
        std::cout << std::endl;
        std::cout << "//////////////////// koc_wf_gen : debug() ////////////////////" << std::endl;
        std::cout << std::endl;

        if( wf_t.empty() )
        {
            std::cout << "--> empty" << std::endl;
        }
        else
        {
            for (int i = 0; i < wf_t.size(); i++)
            {
                std::cout << i << " wf_t[" << i << "] : " << wf_t[i] << std::endl;
            }
            std::cout << std::endl;
            for (int i = 0; i < wf_v.size(); i++)
            {
                std::cout << i << " wf_v[" << i << "] : " << wf_v[i] << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            /*
            for (int i = 0; i < wf_t_us.size(); i++)
            {
                std::cout << i << " wf_t_us[" << i << "] : " << wf_t_us[i] << std::endl;
            }
            std::cout << std::endl;
            for (int i = 0; i < wf_v_digital.size(); i++)
            {
                std::cout << i << " wf_v_digital[" << i << "] : " << wf_v_digital[i] << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;
            std::cout << std::endl;
            */
            for (int i = 0; i < wf_trig.size(); i++)
            {
                std::cout << i << " wf_trig[" << i << "] : " << wf_trig[i] << std::endl;
            }
        }
        std::cout << std::endl; 
        std::cout << "////////////////////" << std::endl;
        std::cout << " - fs_max                  : " << fs_max << std::endl;
        std::cout << " - fs                      : " << std::setprecision (100) << fs << std::endl;
        std::cout << " - fs_per_period           : " << fs_per_period << std::endl;
        std::cout << " - freq_max                : " << get_freq_max_eight_bits() << std::endl;
        std::cout << " - pulse_width             : " << pulse_width<< std::endl;
        std::cout << "////////////////////" << std::endl;
        std::cout << " - wf_t.capacity()         : " << wf_t.capacity() << std::endl;
        std::cout << " - wf_t.size()             : " << wf_t.size() << std::endl;
        std::cout << " - wf_t_us.capacity()      : " << wf_t_us.capacity() << std::endl;
        std::cout << " - wf_t_us.size()          : " << wf_t_us.size() << std::endl;
        std::cout << " - wf_v.capacity()         : " << wf_v.capacity() << std::endl;
        std::cout << " - wf_v.size()             : " << wf_v.size() << std::endl;
        std::cout << " - wf_v_digital.capacity() : " << wf_v_digital.capacity() << std::endl;
        std::cout << " - wf_v_digital.size()     : " << wf_v_digital.size() << std::endl;
        std::cout << " - wf_trig.capacity() : " << wf_trig.capacity() << std::endl;
        std::cout << " - wf_trig.size()     : " << wf_trig.size() << std::endl;

        std::cout << std::endl;
        std::cout << "//////////////////// koc_wf_gen : debug() ////////////////////" << std::endl;
        std::cout << std::endl;
    }
    
    // private
    double wf_gen::to_radian(double deg) // definition of static function 
    {
        return deg*(M_PI/180);
    }

    void wf_gen::gen_fs_per_period(even_odd_mode p_even_odd_mode)
    {
        if ((freq == 0.0) || (freq*2.0 > fs_max))
        {
            fs = 1.0;
            fs_per_period = 1;
        }
        else
        {
            switch(p_even_odd_mode)
            {
                case even_odd_mode::none:
                    fs_per_period = floor( (double)fs_max/freq );
                    fs = freq * (double)fs_per_period;
                    break;
                case even_odd_mode::even:
                    if ( ( (int)(floor( (double)fs_max/freq) ) % 2 ) == 0 )
                    {
                        fs_per_period = floor( (double)fs_max/freq );
                        fs = freq * (double)fs_per_period;
                    }
                    else
                    {
                        fs_per_period = ( floor( (double)fs_max/freq ) - 1 );
                        fs = freq * (double)fs_per_period;
                    }
                    break;
                case even_odd_mode::odd:
                    if ( ( (int)(floor( (double)fs_max/freq) ) % 2 ) == 1 )
                    {
                        fs_per_period = floor( (double)fs_max/freq );
                        fs = freq * (double)fs_per_period;
                    }
                    else
                    {
                        fs_per_period = ( floor( (double)fs_max/freq ) - 1 );
                        fs = freq * (double)fs_per_period;
                    }
                    break;
                case even_odd_mode::multiples_of_four:
                    if ( freq*4.0 > fs_max )
                    {
                        fs = 1.0;
                        fs_per_period = 1;
                    }
                    else if ( ( (int)(floor( (double)fs_max/freq) ) % 4 ) == 0 )
                    {
                        fs_per_period = floor( (double)fs_max/freq );
                        fs = freq * (double)fs_per_period;
                    }
                    else
                    {
                        int rest = (int)(floor( (double)fs_max/freq) ) % 4;   
                        fs_per_period = ( floor( (double)fs_max/freq ) - rest );
                        fs = freq * (double)fs_per_period;
                    }
                    break;
                default:
                    fs = 1.0;
                    fs_per_period = 1;
                    break;
            }
        }
    }
}
