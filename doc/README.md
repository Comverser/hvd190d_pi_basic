# hvd190d_pi_basic

# library generation
g++ -fPIC -c hvd190d_pi_driv.cpp
g++ -shared -o libhvd190d_pi_driv.so hvd190d_pi_driv.o -lwiringPi

# library linking
sudo cp hvd190d_pi_driv.h /usr/include
sudo cp libhvd190d_pi_driv.so /usr/lib
sudo ldconfig

# g++ compile commands
g++ -c main.cpp
g++ -o main main.o /usr/lib/libhvd190d_pi_driv.so
