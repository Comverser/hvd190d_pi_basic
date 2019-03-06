#include <unistd.h> 
#include <iostream> 
#include <sstream> 
#include <fstream> 
#include <iterator> 
#include <vector> 
#include <string> 

#include <wiringPi.h>
class Hvd190d_pi
{
public:
    static void t_reset()
    {
        t_start = clock();
    }
    static inline int t_lapsed()
    {
        return (((clock() - t_start) / (float)CLOCKS_PER_SEC) * 1000000);
    }
    static inline unsigned long convert_to_spi(int ch, int v)
    {
		return 0x100000 | ( ( (0x000000 | (ch-1)) << 16) | v); 
    }
    static void write_spi(unsigned long bits)
    { 
    	digitalWrite(sync, 0); 
    	for (int i = 0; i < 24; i++) 
    		write_bit((bits >> (23 - i)) & 0x01); 
    	digitalWrite(sync, 1); 
    }
    static void write_spi(int ch, int v)
    {
        write_spi(convert_to_spi(ch, v));
    }
    static void write_trig_x(int signal)
    {
        digitalWrite(x, signal); 
    }
    static void write_trig_y(int signal)
    {
        digitalWrite(y, signal);
    }
    static void initialize()
    {
        setup_pi();
        setup_dac();
        enable_hv();
        t_start = clock();
    }
    static void terminate()
    {
        disable_hv();
    }
private:
    static clock_t t_start;
    enum Pin_spi { din = 19, sclk = 23, sync = 24 };
    enum Pin_trig { x = 38, y = 40 };
    enum Pin_misc { hv = 36 };
    
    Hvd190d_pi() {}
    static void write_bit(int bit)
    { 
    	digitalWrite(sclk, 1); 
    	digitalWrite(din, bit);
    	digitalWrite(sclk, 0); 
    }
    static void setup_pi()
    {
	    wiringPiSetupPhys(); 
    	pinMode(din, OUTPUT); 
    	pinMode(sclk, OUTPUT); 
    	pinMode(sync, OUTPUT);
        pinMode(x, OUTPUT);
        pinMode(y, OUTPUT);
    	pinMode(hv, OUTPUT);
    }
    static void setup_dac()
    {
    	write_spi(0x280001);
    	delay(50);
    	write_spi(0x380000);
    	delay(50);
    	write_spi(0x20000F);
    	delay(50);
    	write_spi(0x300000);
    	delay(50);
    }
    static void enable_hv()
    {
        digitalWrite(hv, 1);
    }
    static void disable_hv()
    {
        digitalWrite(hv, 0);
    }
};
clock_t Hvd190d_pi::t_start;

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

int main(int args_len, char * args[]) { 
    	
	vector<int> csv;
	csv = readData(args[1]); 
    // column 0 : driver output channel
    // column 1 : time in microseconds
    // column 2 : 16-bit DAC
    // column 3 : trigger signal for x
    // column 4 : trigger signal for y

    Hvd190d_pi::initialize();
	
    for (unsigned int k = 0; k < csv.size(); k += 3) 
	{ 
        if(csv[k + 0] == 0 && csv[k + 1] == 0 && csv[k + 2] == 0)
		{ 
			k = 0; 
            Hvd190d_pi::t_reset();
		}
        while ( Hvd190d_pi::t_lapsed() < csv[k + 1] );
        //Hvd190d_pi::write_trig_x();
        //Hvd190d_pi::write_trig_y();
        Hvd190d_pi::write_spi(csv[k + 0], csv[k + 2]);
	} 

    Hvd190d_pi::terminate();

	return 0; 
}
