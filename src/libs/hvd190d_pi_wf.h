#ifndef HVD190D_PI_WF_H
#define HVD190D_PI_WF_H

#include "koc_wf_gen.h" // <vector> 

namespace hvd190d_pi
{
    class wf
    {
    public:
        wf(int p_adc_bits, double p_vpp_top, double p_vpp_bottom, int p_fs_max_x, double p_fc_x, koc::wf_gen::waveform_mode p_waveform_mode_x, double p_freq_x, double p_amp_x, double p_offset_x, double p_phase_x, double p_pulse_width_x = 0.0, int p_fs_max_y = 0, double p_fc_y = 100000.0, koc::wf_gen::waveform_mode p_waveform_mode_y = koc::wf_gen::waveform_mode::std_sine, double p_freq_y = 0.0, double p_amp_y = 0.0, double p_offset_y = 0.0, double p_phase_y = 0.0, double p_pulse_width_y = 0.0);

        int precision_decimal_point;

        void freq_precision(int p_precision_decimal_point = 1);
        double gcd_freq();

        void debug_check_params(koc::wf_gen::param_wf& p_ref_param_wf);
        void debug_s();

    private:
        double period_gcd;

        koc::wf_gen::param_wf param_wf_x_p;
        koc::wf_gen::param_wf param_wf_x_n;
        koc::wf_gen::param_wf param_wf_y_p;
        koc::wf_gen::param_wf param_wf_y_n;
        koc::wf_gen::data_wf_digital data_wf_digital_x_p;
        koc::wf_gen::data_wf_digital data_wf_digital_x_n;
        koc::wf_gen::data_wf_digital data_wf_digital_y_p;
        koc::wf_gen::data_wf_digital data_wf_digital_y_n;

    };
}

#endif
