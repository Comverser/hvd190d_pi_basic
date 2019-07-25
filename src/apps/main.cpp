#include "hvd190d_pi_wf.h" // "koc_wf_gen.h": <vector>
#include "hvd190d_pi_driv.h" // <iostream>
#include "rpi_tcp_server.h"

#include <thread> // std::thread(), std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
#include <atomic> // atomic function

#include <sstream> // <string>; std::stringstream

#include <fstream> //csv process

struct buffer
{
    std::vector<unsigned long> t_us;
    std::vector<unsigned long> cmd_wf_p;
    std::vector<unsigned long> cmd_wf_n;
};

buffer wf1, wf2, wf3, wf4, wf_buffer, wf_test;

void driv(buffer wf)
{
    hvd190d_pi::t_reset(); // take A
    for (int i = 0; i < wf.t_us.size(); i++ ) // take B, A + B = ~ 1.2 us 
    {
        while ( hvd190d_pi::t_lapsed() < wf.t_us[i] ); // take ~1.4 us
        hvd190d_pi::write_spi(wf.cmd_wf_p[i], wf.cmd_wf_n[i]);
    }
}

void driv_arb(std::vector<int> p_csv)
{
    enum csv_col { ch, t_us, dac};
    // column 0 : driver output channel
    // column 1 : time in microseconds
    // column 2 : 16-bit DAC
	
    hvd190d_pi::t_reset();
    for (unsigned int k = 0; k < p_csv.size(); k += 3) 
	{ 
        while ( hvd190d_pi::t_lapsed() < p_csv[k + t_us] );
        hvd190d_pi::write_spi(p_csv[k + ch], p_csv[k + dac]);
	} 
}

//////////////////// import csv data //////////////////// 
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

int main(int args_len, char * args[]) // release mode!
{
    // start driver
    std::cout << "on" << std::endl;
    hvd190d_pi::initialize();

    //////////////////// waveform generation 
    hvd190d_pi::wf wf_main;
    wf_main.set_is_diff_on(true);
    wf_main.set_is_x_on(true);
    wf_main.set_is_y_on(true);
    


    ////////////////////////////// characteristic test //////////////////////////////  
    // mode buffer
	wf_main.set_param_wf(0, 10, 100000, 0, 100, 0, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 10, 100000, 0, 100, 0, 60, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf_buffer.t_us = wf_main._sorted_cmd_wf.t_us;
	wf_buffer.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf_buffer.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode 1
	wf_main.set_param_wf(0, 25400, 100000, 0, 50, 100, 100, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25400, 100000, 0, 50.1, 100, 100, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf1.t_us = wf_main._sorted_cmd_wf.t_us;
	wf1.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf1.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode 2
	wf_main.set_param_wf(0, 25400, 100000, 3, 12500, 100, 100, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25400, 100000, 3, 12500.1, 100, 100, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf2.t_us = wf_main._sorted_cmd_wf.t_us;
	wf2.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf2.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode arb
    std::vector<int> wf_arb = readData(args[1]); 
    //////////////////////////////////////////////////////////////////////////////////////////////////// 


    /* ////////////////////////////// demo ////////////////////////////// 
    // mode buffer
	wf_main.set_param_wf(0, 10, 100000, 0, 100, 0, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 10, 100000, 0, 100, 0, 60, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf_buffer.t_us = wf_main._sorted_cmd_wf.t_us;
	wf_buffer.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf_buffer.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode 1
	wf_main.set_param_wf(0, 25400, 100000, 1, 0.3, -60, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25400, 100000, 0, 60.1, 60, 60, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf1.t_us = wf_main._sorted_cmd_wf.t_us;
	wf1.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf1.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode 2
	wf_main.set_param_wf(0, 25400, 100000, 0, 30.1, 60, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25400, 100000, 0, 33, 60, 60, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf2.t_us = wf_main._sorted_cmd_wf.t_us;
	wf2.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf2.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode 3
	wf_main.set_param_wf(0, 25400, 100000, 1, 10.1, 60, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25400, 100000, 0, 200, 50, 60, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf3.t_us = wf_main._sorted_cmd_wf.t_us;
	wf3.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf3.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // mode 4
	wf_main.set_param_wf(0, 25400, 100000, 1, 0.4, 60, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25400, 100000, 1, 0.1, 60, 60, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf4.t_us = wf_main._sorted_cmd_wf.t_us;
	wf4.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf4.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;
// mode arb std::vector<int> wf_arb = readData(args[1]); 
    */ //////////////////////////////////////////////////////////////////////////////////////////////////// 



    /* ////////////////////////////// test for delay compansation ////////////////////////////// 
    // mode test
    int f = 1000;
	wf_main.set_param_wf(0, 25000, 100000, 1, 13.1, 60, 60, 0, 0); // y-axis
	wf_main.set_param_wf(1, 25000, 100000, 1, f, 25, 25, 0, 0); // x-axis
	wf_main.run_wf_differential();
	wf_test.t_us = wf_main._sorted_cmd_wf.t_us;
	wf_test.cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	wf_test.cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;
    while (1)
    {
        driv(wf_test);
    }
    */ //////////////////////////////////////////////////////////////////////////////////////////////////// 
        


    //////////////////// drive 
    while(1)
    {
        // test
        driv(wf1);

        driv(wf2);

        driv_arb(wf_arb);
        // tes end
        /*
        driv(wf_buffer);
        driv(wf3);

        driv(wf_buffer);
        driv(wf4);

        driv(wf_buffer);
        driv_arb(wf_arb);
        */
    }

    // end driver
    std::cout << "Terminating" << std::endl;
    hvd190d_pi::terminate();

    return 0;

}
