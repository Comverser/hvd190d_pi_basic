#ifndef HVD190D_PI_WF_H
#define HVD190D_PI_WF_H

#include "koc_wf_gen.h" // <vector> 

namespace hvd190d_pi
{
    class wf
    {
    public:
        wf(int p_adc_bits = 16, double p_vpp_top = 200.0, double p_vpp_bottom = 0.0, 
            int p_fs_max_x = 0, double p_fc_x = 0.0, koc::wf_gen::waveform_mode p_waveform_mode_x = koc::wf_gen::waveform_mode::std_sine, double p_freq_x = 0.0, double p_amp_x = 0.0, double p_offset_x = 0.0, double p_phase_x = 0.0, double p_pulse_width_x = 0.0, 
            int p_fs_max_y = 0, double p_fc_y = 0.0, koc::wf_gen::waveform_mode p_waveform_mode_y = koc::wf_gen::waveform_mode::std_sine, double p_freq_y = 0.0, double p_amp_y = 0.0, double p_offset_y = 0.0, double p_phase_y = 0.0, double p_pulse_width_y = 0.0);

        bool is_x_on;
        bool is_y_on;
        bool is_x_trig_on;
        bool is_y_trig_on;
        bool is_diff_on;

        struct sorted_cmd_wf
        {
            std::vector<unsigned long> t_us;
            std::vector<unsigned long> cmd_wf_p;
            std::vector<unsigned long> cmd_wf_n;
            std::vector<int> trig_x;
            std::vector<int> trig_y;
        };
        sorted_cmd_wf _sorted_cmd_wf;

        void set_is_x_on(bool p_is_x_on);
        void set_is_y_on(bool p_is_y_on);
        void set_is_x_trig_on(bool p_is_x_trig_on);
        void set_is_y_trig_on(bool p_is_y_trig_on);
        void set_is_diff_on(bool p_is_diff_on);
        void set_param_wf(int xy, int p_fs_max, double p_fc, int p_waveform_mode, double p_freq, double p_amp, double p_offset, double p_phase, double p_pulse_width = 0);

//        void drive_mems();
//        void soft_start_end();
        void run_wf_differential();

        void debug_s();

    private:
        int precision_decimal_point;

        koc::wf_gen::param_wf param_wf_x_p;
        koc::wf_gen::param_wf param_wf_y_p;

        struct data_wf_digital_pn
        {
            koc::wf_gen::data_wf_digital p;
            koc::wf_gen::data_wf_digital n;
        };
        data_wf_digital_pn x_data_wf;
        data_wf_digital_pn y_data_wf;

        void set_freq_precision(int p_precision_decimal_point = 1);
        double calc_period_gcd_freq();
        void calc_no_repetition();
        void set_no_repetition();
        void gen_wf_differential_pn(koc::wf_gen::param_wf param_wf_p, data_wf_digital_pn& ref_data_wf_digital_pn, bool p_is_trig_on);
        void gen_wf_differential(bool p_is_on, koc::wf_gen::param_wf param_wf_p, data_wf_digital_pn& ref_data_wf_digital_pn, bool p_is_trig_on);
        void sort_wf_differential_xy(sorted_cmd_wf& ref_sorted_cmd, data_wf_digital_pn& ref_x, data_wf_digital_pn& ref_y);
        void sort_wf_differential(bool p_is_x_on, bool p_is_y_on, sorted_cmd_wf& ref_sorted_cmd, data_wf_digital_pn& ref_x, data_wf_digital_pn& ref_y);

        inline unsigned long convert_to_cmd_dac_quad_datum(int ch, unsigned long v_digital) const
        {
            return 0x180000 | ( ( (0x000000 | (ch)) << 16) | v_digital); // channel number starts from zero
        }

        std::vector<unsigned long> convert_to_cmd_dac_quad_vector(int ch, std::vector<unsigned long> v_digital);

        koc::wf_gen::waveform_mode translate_waveform_mode(int int_wf_mod);

        void debug_check_params(koc::wf_gen::param_wf& p_ref_param_wf);
        void debug_check_sorted_cmd_wf();
    };
}

#endif
