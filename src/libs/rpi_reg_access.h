#ifndef RPI_REG_ACCESS_H 
#define RPI_REG_ACCESS_H 

void gpio_init();
//void gpio_pull(int g, int mode);
void gpio_mode_out(int g);
void gpio_high(int g);
void gpio_low(int g);
void gpio_write(int g, int v);

#endif
