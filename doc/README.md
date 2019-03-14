# hvd190d_pi_basic

# library generation
g++ -fPIC -c hvd190d_pi_driv.cpp
g++ -shared -o libhvd190d_pi_driv.so hvd190d_pi_driv.o -lwiringPi
cp libhvd190d_pi_driv.so /usr/lib/
