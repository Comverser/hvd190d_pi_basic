#include <wiringPi.h> // Raspberry Pi 
#include <unistd.h> 
#include <iostream> 
#include <sstream> 
#include <fstream> 
#include <iterator> 
#include <vector> 
#include <string> 

using namespace std; 


//////////////////// Raspberry Pi GPIO parameters //////////////////// 
const int sdiPin = 19; 
const int sclkPin = 23; 
const int syncPin = 24;
const int enable = 36;
const int trig_out_x = 38;
const int trig_out_y = 40;



#define lapsed (((clock() - start) / (float)CLOCKS_PER_SEC) * 1000000) 

//////////////////// import csv data //////////////////// 
template <class T> 
class csv_istream_iterator: public iterator<input_iterator_tag, T> 
{ 
	istream * _input; 
	char _delim; 
	string _value; 
public: 
	csv_istream_iterator( char delim = ',' ): _input( 0 ), _delim( delim ) {} 
	csv_istream_iterator( istream & in, char delim = ',' ): _input( &in ), _delim( delim ) { ++*this; } 

	const T operator *() const { 
		istringstream ss( _value ); 
		T value; 
		ss >> value; 
		return value; 
	}
 
	istream & operator ++() { 
		if( !( getline( *_input, _value, _delim ) ) ) 
		{ 
			_input = 0; 
		} 
		return *_input; 
	}
 
	bool operator !=( const csv_istream_iterator & rhs ) const { 
		return _input != rhs._input; 
	} 
}; 

template <> 
const string csv_istream_iterator<string>::operator *() const { 
	return _value; 
} 

std::vector<int> readData(char filename[]) { 
		std::vector<int> data; 
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

//////////////////// SPI with 24-bit //////////////////// 
void writeBit(int bit)
{ 
	digitalWrite(sclkPin, 1); 
	digitalWrite(sdiPin, bit);
	digitalWrite(sclkPin, 0); 
}
 
void writeBits(unsigned long bits, int length)
{ 
	digitalWrite(syncPin, 0); 
	for(int i = 0; i < length; i++) 
		writeBit((bits >> (23 - i)) & 0x01); 
	digitalWrite(syncPin, 1); 
} 

//////////////////// SPI with 24-bit for Raspberry Pi //////////////////// 
void init()
{
	digitalWrite(enable, 1);
	
	writeBits(0x280001, 24);
	delay(50);
	writeBits(0x380000, 24);
	delay(50);
	writeBits(0x20000F, 24);
	delay(50);
	writeBits(0x300000, 24);
	delay(50);
}

void end()
{
	writeBits(0x1F8000, 24);
	delay(1000);
	writeBits(0x1F0000, 24);
	delay(1000);
	digitalWrite(enable, 0);
}



int main(int args_len, char * args[]) { 
	//cout << "Starting" << endl; 
	wiringPiSetupPhys(); 
	
	pinMode(sdiPin, OUTPUT); 
	pinMode(sclkPin, OUTPUT); 
	pinMode(syncPin, OUTPUT);
	pinMode(enable, OUTPUT);
	
	std::vector<int> d; 
	d = readData(args[1]); 

	unsigned long cmd; 
	int ch; 
	int us; 
	int v; 

	init();

	clock_t start = clock(); 

	for(unsigned int k = 0; k < d.size(); k+=3) 
	{ 
		cmd = 0; 
		ch = d[k + 0]; 
		us = d[k + 1]; 
		v = d[k + 2]; 

		if(ch == 0 && us == 0 && v == 0) 
		{ 
			k = 0; 
			ch = d[k + 0]; 
			us = d[k + 1]; 
			v = d[k + 2]; 
			start = clock();
		}
 
		cmd = 0x100000 | ( ( (0x000000 | (ch-1)) << 16) | v); 
		while (lapsed < us);
		writeBits(cmd, 24); 
	} 
	
	
	end();

	return 0; 
}
