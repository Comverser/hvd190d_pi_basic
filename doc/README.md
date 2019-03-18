# hvd190d_pi_basic

## -------------------- library generation --------------------
### shared library generation in default mode
g++ -fPIC -c hvd190d_pi_driv.cpp  
g++ -shared -o libhvd190d_pi_driv.so hvd190d_pi_driv.o -lwiringPi  
### optional shared library generation in debug mode
g++ -c -g -Wall -Wextra -Werror -fPIC hvd190d_pi_driv.cpp -o hvd190d_pi_driv_debug.o  
g++ -shared -o libhvd190d_pi_driv_debug.so hvd190d_pi_driv_debug.o -lwiringPi  
### optional shared library generation in release mode
g++ -c -O2 -DNDEBUG -fpic hvd190d_pi_driv.cpp -o hvd190d_pi_driv_release.o  
g++ -shared -o libhvd190d_pi_driv_release.so hvd190d_pi_driv_release.o -lwiringPi  

## -------------------- library linking -------------------- 
### copy library to system path
sudo cp hvd190d_pi_driv.h /usr/include  
sudo cp libhvd190d_pi_driv.so /usr/lib  
sudo ldconfig  

## -------------------- main program --------------------
### g++ compile commands
g++ -o main main.cpp -lhvd190d_pi_driv  

## -------------------- notes --------------------
### optional path flags for the library linking
-I/usr/include  
-L/usr/lib  
