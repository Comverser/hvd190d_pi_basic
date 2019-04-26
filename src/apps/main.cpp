#include "hvd190d_pi_wf.h" // "koc_wf_gen.h"
#include "hvd190d_pi_driv.h" // <wiringPi.h>, <iostream>

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

            hvd190d_pi::wf wf_main(16,200,0,4,100000.0,koc::wf_gen::waveform_mode::std_triangle,1,100,100,0,0);
//            wf_main.set_is_x_on(true);
            wf_main.set_is_y_on(true);
            wf_main.set_is_x_trig_on(true);
//            wf_main.set_is_y_trig_on(true);
//            wf_main.set_param_wf(1, 0, 0, 0, 0, 100, 10, 0);
            wf_main.run_wf_differential();
            wf_main.debug_s();
            /*
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
*/
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
