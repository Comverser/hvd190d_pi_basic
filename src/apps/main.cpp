#include "hvd190d_pi_wf.h" // "koc_wf_gen.h"
#include "hvd190d_pi_driv.h" // <wiringPi.h>, <iostream>
#include "rpi_tcp_server.h"

#include <thread> // std::thread(), std::this_thread::sleep_for
#include <chrono> // std::chrono::seconds
#include <atomic> // atomic function
//#include <mutex> // mutex

#include <vector>
//#include <string.h> // strncmp(), etc 

std::vector<unsigned long> t_us;
std::vector<unsigned long> cmd_wf_p;
std::vector<unsigned long> cmd_wf_n;

//std::mutex mtx;

void drive_hvd190d(std::atomic<bool>& ref_is_running, std::atomic<bool>& ref_is_on_thread_driv)
{
    while (ref_is_on_thread_driv)
    {
//        std::lock_guard<std::mutex> lock(mtx);
//        mtx.lock();
        while (ref_is_running)
        {
            hvd190d_pi::t_reset(); // take A
            for (int i = 0; i < t_us.size(); i++ ) // take B, A + B = ~ 1.2 us 
            {
                while ( hvd190d_pi::t_lapsed() < t_us[i] ); // take ~1.4 us
                hvd190d_pi::write_spi(cmd_wf_p[i], cmd_wf_n[i]);
            }
        }
//        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int args_len, char * args[]) 
{
    // start driver
    std::cout << "on" << std::endl;
    hvd190d_pi::initialize();

    // create waveform class
    hvd190d_pi::wf wf_main;
    wf_main.set_is_diff_on(true);
    wf_main.set_is_x_on(true);
    wf_main.set_is_y_on(true);
	wf_main.set_param_wf(0, 25400, 100000, 1, 1, 60, 60, 0);
	wf_main.set_param_wf(1, 25400, 100000, 3, 1, 30, 30, 0);
	wf_main.run_wf_differential();
	t_us = wf_main._sorted_cmd_wf.t_us;
	cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
	cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;

    // create thread
    std::atomic<bool> is_running { false }; 
    std::atomic<bool> is_on_thread_driv { true }; 
    std::thread t_driv;
    // start thread
    t_driv = std::thread(drive_hvd190d, std::ref(is_running), std::ref(is_on_thread_driv));
//    t_driv = std::thread(drive_hvd190d, std::ref(wf_main), std::ref(is_running), std::ref(is_on_thread_driv));

////////////////////////////////////////

    // start server
    rpi::init_tcp_server();
    while ( rpi::data_tcp[0] != '4' ) 
    {
        rpi::run_tcp_server();
        while ( rpi::data_tcp[0] != '3' && rpi::data_tcp[0] != '4' ) 
        {
            rpi::get_data_tcp_server();
            if (rpi::data_tcp[0] >= '0' && rpi::data_tcp[0] <= '4')
            {
                rpi::send_data_tcp_server((unsigned char *)"OK");
                if (rpi::data_tcp[0] == '0') // stop 
                {
                    std::cout << "Stop" << std::endl;
					is_running = false;
                }
                else if (rpi::data_tcp[0] == '1') // run
                {
                    std::cout << "Start" << std::endl;
					is_running = true;
                }
                else if (rpi::data_tcp[0] == '2') // setting
                {
                    std::cout << "Setting" << std::endl;
					wf_main.set_param_wf(0, 25400, 100000, 3, 1, 100, 100, 0);
					wf_main.set_param_wf(1, 25400, 100000, 3, 1, 30, 30, 0);
					wf_main.run_wf_differential();
					t_us = wf_main._sorted_cmd_wf.t_us;
					cmd_wf_p = wf_main._sorted_cmd_wf.cmd_wf_p;
					cmd_wf_n = wf_main._sorted_cmd_wf.cmd_wf_n;
                }
            }
            else
            {
                rpi::send_data_tcp_server((unsigned char *)"INVALID");
            }
        }

		// end tcp server
		rpi::terminate_tcp_server();

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

////////////////////////////////////////

    // terminate therad
    is_running = false;
    is_on_thread_driv= false;
    t_driv.join();

    // end driver
    std::cout << "Terminating" << std::endl;
    hvd190d_pi::terminate();

    return 0;

}
