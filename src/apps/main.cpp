#include "hvd190d_pi_wf.h" // "koc_wf_gen.h"
#include "hvd190d_pi_driv.h" // <wiringPi.h>, <iostream>

#include <thread> // std::thread() 
#include <atomic> // atomic function
#include <mutex> // mutex

//////////////////// import csv data //////////////////// 
#include <sstream> 
#include <fstream> 
#include <iterator> 
#include <vector> 
#include <string> 

std::mutex mtx;

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

void drive_hvd190d(hvd190d_pi::wf& p_wf, std::atomic<bool>& ref_is_running)
{
    p_wf.set_is_diff_on(true);
    p_wf.run_wf_differential();

    while (ref_is_running)
    {
        hvd190d_pi::t_reset(); // take A
        for (int i = 0; i < p_wf._sorted_cmd_wf.t_us.size(); i++ ) // take B, A + B = ~ 1.2 us 
        {
            while ( hvd190d_pi::t_lapsed() < p_wf._sorted_cmd_wf.t_us[i] ); // take ~1.4 us
            hvd190d_pi::write_spi(p_wf._sorted_cmd_wf.cmd_wf_p[i], p_wf._sorted_cmd_wf.cmd_wf_n[i]);
        }
    }
}

void drive_hvd190d_x(hvd190d_pi::wf& p_wf, std::atomic<bool>& ref_is_running)
{
    p_wf.set_is_diff_on(true);
    p_wf.run_wf_differential();

    while (ref_is_running)
    {
        hvd190d_pi::t_reset(); // take A
        for (int i = 0; i < p_wf._sorted_cmd_wf.t_us.size(); i++ ) // take B, A + B = ~ 1.2 us 
        {
            while ( hvd190d_pi::t_lapsed() < p_wf._sorted_cmd_wf.t_us[i] ); // take ~1.4 us
            mtx.lock();
            hvd190d_pi::write_spi(p_wf._sorted_cmd_wf.cmd_wf_p[i], p_wf._sorted_cmd_wf.cmd_wf_n[i]);
            mtx.unlock();
        }
    }
}

void drive_hvd190d_y(hvd190d_pi::wf& p_wf, std::atomic<bool>& ref_is_running)
{
    p_wf.set_is_diff_on(true);
    p_wf.run_wf_differential();

    while (ref_is_running)
    {
        hvd190d_pi::t_reset_(); // take A
        for (int i = 0; i < p_wf._sorted_cmd_wf.t_us.size(); i++ ) // take B, A + B = ~ 1.2 us 
        {
            while ( hvd190d_pi::t_lapsed_() < p_wf._sorted_cmd_wf.t_us[i] ); // take ~1.4 us
            mtx.lock();
            hvd190d_pi::write_spi(p_wf._sorted_cmd_wf.cmd_wf_p[i], p_wf._sorted_cmd_wf.cmd_wf_n[i]);
            mtx.unlock();
        }
    }
}

int menu_hvd190d()
{
    int m_user_select;
    std::cout << "(1) csv trigger mode" << std::endl;
    std::cout << "(2) csv mode" << std::endl;
    std::cout << "(3) normal trigger mode" << std::endl;
    std::cout << "(4) normal mode" << std::endl;
    std::cout << "(5) independent axis mode" << std::endl;
    std::cout << "Enter number : ";
    std::cin >> m_user_select;
    return m_user_select;
}

int main(int args_len, char * args[]) 
{
    std::atomic<bool> is_running { true };
    std::atomic<bool> is_running_ { true };
    
    hvd190d_pi::initialize();

    int user_select = menu_hvd190d();

    switch(user_select)
    {
        case 1: 
        {
            std::cout << "csv trigger mode" << std::endl;
            enum csv_col { ch, t_us, dac, trig_x, trig_y };
            // column 0 : driver output channel
            // column 1 : time in microseconds
            // column 2 : 16-bit DAC
            // column 3 : trigger signal for x
            // column 4 : trigger signal for y
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
            enum csv_col { ch, t_us, dac, trig_x, trig_y };
            // column 0 : driver output channel
            // column 1 : time in microseconds
            // column 2 : 16-bit DAC
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
            std::cout << "Development unfinished!" << std::endl;
//            wf_main.set_is_x_trig_on(true);
//            wf_main.set_is_y_trig_on(true);
//            wf_main.debug_s();
            break;
        }
        case 4:
        {
            std::cout << "normal mode" << std::endl;
            hvd190d_pi::wf wf_main;
            wf_main.set_is_x_on(true);
            wf_main.set_is_y_on(true);
            wf_main.set_param_wf(0, 25600, 100000, 2, 30, 60, 60, 0);
            wf_main.set_param_wf(1, 6000*2, 100000, 3, 6000, 60, 60, 0);

            std::thread t(drive_hvd190d, std::ref(wf_main), std::ref(is_running));

            char userinput;
            std::cout << "press 1 to exit" << std::endl;
            std::cin >> userinput;

            while (!std::cin)
            {
                std::cout << "error \n";
                std::cout << "press 1 to exit" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> userinput;
            }

            if (userinput == '1')
            {
                std::cout << "user input is 1\n";
                is_running = false;
                t.join();
            }
            else
            {
                std::cout << "user input is not 1\n";
                is_running = false;
                t.join();
            }
            break;
        }
        case 5:
        {
            std::cout << "independent axis mode" << std::endl;
            hvd190d_pi::wf wf_main;
            wf_main.set_is_x_on(true);
            wf_main.set_param_wf(0, 25600, 100000, 2, 30, 60, 60, 0);

            hvd190d_pi::wf wf_main_;
            wf_main_.set_is_y_on(true);
            wf_main_.set_param_wf(1, 25600, 100000, 2, 30, 60, 60, 0);

            std::thread t_x(drive_hvd190d_x, std::ref(wf_main), std::ref(is_running));
            std::thread t_y(drive_hvd190d_y, std::ref(wf_main_), std::ref(is_running_));

            char userinput;
            std::cout << "press 1 to exit" << std::endl;
            std::cin >> userinput;

            while (!std::cin)
            {
                std::cout << "error \n";
                std::cout << "press 1 to exit" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> userinput;
            }

            if (userinput == '1')
            {
                std::cout << "user input is 1\n";
                is_running = false;
                t_x.join();
            }
            else if (userinput == '2')
            {
                std::cout << "user input is 2\n";
                is_running_ = false;
                t_y.join();
            }
            else
            {
                std::cout << "user input is not 1 and 2\n";
                is_running = false;
                is_running_ = false;
                t_x.join();
                t_y.join();
            }
            break;
        }
        default: 
            std::cout << "Error" << std::endl;
            break;
    }

    std::cout << "terminating" << std::endl;

    hvd190d_pi::terminate();
	
    return 0; 
}
