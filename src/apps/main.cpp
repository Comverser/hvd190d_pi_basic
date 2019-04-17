#include "hvd190d_pi_driv.h" // <wiringPi.h> <iostream>
#include "koc_wf_gen.h" // <vector>

#include <typeinfo> // debug : typeid().name()

            struct param_wf
            {
                int adc_bits;
                double vpp_top;
                double vpp_bottom;
                int fs_max; 
                double fc;
                int no_repetition;
                koc::wf_gen::waveform_mode _waveform_mode;
                double freq;
                double amp;
                double offset;
                double phase;
                double pulse_width;
                std::vector<unsigned long> wf_t_us;
                std::vector<unsigned long> wf_v_digital;
                std::vector<int> wf_trig;
            };

            void reverse_sign_wf_amp(param_wf& p_param_wf)
            {
                p_param_wf.amp = -p_param_wf.amp;
            }

//////////////////// import csv data //////////////////// 
#include <sstream> 
#include <fstream> 
#include <iterator> 
#include <vector> 
#include <string> 

using namespace std; 
template <class T> 
class csv_istream_iterator: public iterator<input_iterator_tag, T> 
{ 
	istream * _input; 
	char _delim; 
	string _value; 
public: 
	csv_istream_iterator( char delim = ',' ): _input( 0 ), _delim( delim ) {} 
	csv_istream_iterator( istream & in, char delim = ',' ): _input( &in ), _delim( delim ) { ++*this; } 

	const T operator *() const 
    { 
		istringstream ss( _value ); 
		T value; 
		ss >> value; 
		return value; 
	}
 
	istream & operator ++() 
    { 
		if( !( getline( *_input, _value, _delim ) ) ) 
		{ 
			_input = 0; 
		} 
		return *_input; 
	}
 
	bool operator !=( const csv_istream_iterator & rhs ) const 
    { 
		return _input != rhs._input; 
	} 
}; 

template <> 
const string csv_istream_iterator<string>::operator *() const 
{ 
	return _value; 
} 

vector<int> readData(char filename[]) 
{ 
    vector<int> data; 
    { // test for integers 
        ifstream fin( filename ); 
    	if( fin ) 
    	{ 
    		copy( csv_istream_iterator<int>( fin ), 
    			csv_istream_iterator<int>(), 
    			back_inserter(data) );
			fin.close(); 
		} 
	} 
    return data; 
} 


int main(int args_len, char * args[]) 
{
    int user_select;
    enum csv_col { ch, t_us, dac, trig_x, trig_y };
    // column 0 : driver output channel
    // column 1 : time in microseconds
    // column 2 : 16-bit DAC
    // column 3 : trigger signal for x
    // column 4 : trigger signal for y
    
    hvd190d_pi::initialize();

    std::cout << "(1) csv trigger mode" << std::endl;
    std::cout << "(2) csv mode" << std::endl;
    std::cout << "(3) normal mode" << std::endl;
    std::cout << "Enter number : ";
    std::cin >> user_select;

    switch(user_select)
    {
        case 1: 
        {
            std::cout << "csv trigger mode" << std::endl;
            vector<int> csv;
            csv = readData(args[1]); 
            
            hvd190d_pi::t_reset();
            for (unsigned int k = 0; k < csv.size(); k += 5) 
            { 
                if(csv[k + ch] == 0 && csv[k + t_us] == 0 && csv[k + dac] == 0)
                { 
                    k = 0; 
                    hvd190d_pi::t_reset();
                }
                while ( hvd190d_pi::t_lapsed() < csv[k + t_us] );
                hvd190d_pi::write_trig_x(csv[k + trig_x]);
                hvd190d_pi::write_trig_y(csv[k + trig_y]);
                hvd190d_pi::write_spi(csv[k + ch], csv[k + dac]);
            } 
            break;
        }
        case 2: 
        {
            std::cout << "csv mode" << std::endl;
            vector<int> csv;
            csv = readData(args[1]); 
            
            hvd190d_pi::t_reset();
            for (unsigned int k = 0; k < csv.size(); k += 3) 
            { 
                if(csv[k + ch] == 0 && csv[k + t_us] == 0 && csv[k + dac] == 0)
                { 
                    k = 0; 
                    hvd190d_pi::t_reset();
                }
                while ( hvd190d_pi::t_lapsed() < csv[k + t_us] );
                hvd190d_pi::write_spi(csv[k + ch], csv[k + dac]);
            } 
            break;
        }
        case 3:
        {
            std::cout << "normal trigger mode" << std::endl;

            // x_p
            param_wf x_p;
            x_p.adc_bits = 16;
            x_p.vpp_top = 200;
            x_p.vpp_bottom = 0;
            x_p.fs_max = 50000;
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
//            wf_x_p.debug_s();
//            wf_x_p.export_wf(koc::wf_gen::analog_digital_mode::digital);

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

            // y_n
            param_wf y_n = y_p;
            reverse_sign_wf_amp(y_n);

            koc::wf_gen wf_y_n(y_n.adc_bits, y_n.vpp_top, y_n.vpp_bottom, y_n.fs_max, y_n.fc, y_n.no_repetition, y_n._waveform_mode, y_n.freq, y_n.amp, y_n.offset, y_n.phase, y_n.pulse_width);
            wf_y_n.gen_wf();
            wf_y_n.gen_wf_t_us();
            wf_y_n.gen_wf_v_digital();

            // drive 
            while (1)
            {
                hvd190d_pi::t_reset(); // take A
                for (int i = 0; i < x_p.wf_t_us.size() ; i++ ) // take B, A + B = ~ 1.2 us 
                {
                    while ( hvd190d_pi::t_lapsed() < x_p.wf_t_us[i] ); // take ~1.4 us
                    hvd190d_pi::write_trig_y(x_p.wf_trig[i]); // take ~0.22 us
                    hvd190d_pi::write_spi(1, x_p.wf_v_digital[i]); // take ~3 us
                    hvd190d_pi::write_spi(2, x_n.wf_v_digital[i]); // take ~3 us
                }
            }

            break;
        }
        case 4:
        {
            std::cout << "normal mode" << std::endl;
            break;
        }
        default: 
            std::cout << "Error" << std::endl;
            break;
    }

    hvd190d_pi::terminate();
	
    return 0; 
}
