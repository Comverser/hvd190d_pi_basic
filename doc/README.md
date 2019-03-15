# hvd190d_pi_basic

# library generation
g++ -fPIC -c hvd190d_pi_driv.cpp
g++ -shared -o libhvd190d_pi_driv.so hvd190d_pi_driv.o -lwiringPi

# library paths 
sudo cp hvd190d_pi_driv.h /usr/include
sudo cp libhvd190d_pi_driv.so /usr/lib
sudo ldconfig

# g++ compile commands
g++ -Wall -o main main.cpp -lhvd190d_pi_driv

# optional path flags for the library linking
-I/usr/include
-L/usr/lib
