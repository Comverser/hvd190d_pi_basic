#ifndef KOC_WF_GEN_H
#define KOC_WF_GEN_H

#include <vector>

namespace koc
{
    class wf_gen
    {
    public:
        enum class waveform_mode
        { 
            std_sine,
            std_triangle,
            ramp,
            std_square,
            std_pulse
        };

        enum class analog_digital_mode
        { 
            analog,
            digital 
        };

        struct param_wf
        {
            int adc_bits;
            double vpp_top;
            double vpp_bottom;
            int fs_max; 
            double fc;
            int no_repetition;
            waveform_mode _waveform_mode;
            double freq;
            double amp;
            double offset;
            double phase;
            double pulse_width;
        };

        struct data_wf_digital
        {
            std::vector<unsigned long> wf_t_us;
            std::vector<unsigned long> wf_v_digital;
            std::vector<int> wf_trig;
        };

        wf_gen(int p_adc_bits, double p_vpp_top, double p_vpp_bottom, int p_fs_max, double p_fc, int p_no_repetition, waveform_mode p_waveform_mode, double p_freq, double p_amp, double p_offset, double p_phase, double p_pulse_width = 0.0);
        void gen_wf();
        void gen_wf_t_us();
        void gen_wf_v_digital();
        void gen_wf_trig();
        std::vector<double> get_wf_t();
        std::vector<unsigned long> get_wf_t_us();
        std::vector<double> get_wf_v();
        std::vector<unsigned long> get_wf_v_digital();
        std::vector<int> get_wf_trig();
        void export_wf(analog_digital_mode p_analog_digital_mode);
        double get_freq_max_eight_bits();
        void debug();
    private:
        int adc_bits;
        double vpp_top;
        double vpp_bottom;
        int fs_max; 
        double fc;
        int no_repetition;
        waveform_mode _waveform_mode;
        double freq;
        double amp;
        double offset;
        double phase;
        double pulse_width;
        double fs; 
        int fs_per_period;
        double freq_max_eight_bits;
        int loop_delay;
        std::vector<double> wf_t;
        std::vector<unsigned long> wf_t_us;
        std::vector<double> wf_v;
        std::vector<unsigned long> wf_v_digital;
        std::vector<int> wf_trig;

        enum class even_odd_mode 
        { 
            none,
            even,
            odd,
            multiples_of_four 
        };

        static double to_radian(double deg);
        void gen_fs_per_period(even_odd_mode p_even_odd_mode);
    };
}

#endif
