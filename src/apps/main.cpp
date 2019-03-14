#include "hvd190d_pi_driv.h" // <wiringPi.h> <iostream>


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
	vector<int> csv;
	csv = readData(args[1]); 
    enum csv_col { ch, t_us, dac, trig_x, trig_y };
    // column 0 : driver output channel
    // column 1 : time in microseconds
    // column 2 : 16-bit DAC
    // column 3 : trigger signal for x
    // column 4 : trigger signal for y

    hvd190d_pi::initialize();
	
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
   
    std::cin.get();

    hvd190d_pi::terminate();
	
    return 0; 
}
