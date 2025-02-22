#include <stdint.h>
#include <signal.h>

#include <random>
#include "verilated_vcd_c.h"
#include "VDecaTopLevelSim.h"

using namespace std;

template<typename T>
class ClockManager
{
  public:
   ClockManager(VerilatedVcdC *tfp,T* top ){}
   void advance(void)
   {
     
     


   };
  
  protected:

    

  VerilatedVcdC *m_tfp;
  T* m_top;
   


  vluint64_t m_time = 0; 
};



static bool done;

vluint64_t main_time = 0;       // Current simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.

double sc_time_stamp () {       // Called by $time in Verilog
  return main_time;           // converts to double, to match
  // what SystemC does
}

void INThandler(int signal)
{
	printf("\nCaught ctrl-c\n");
	done = true;
}

/*
typedef struct {
  bool last_value;
} gpio_context_t;

void do_gpio(gpio_context_t *context, bool gpio) {
  if (context->last_value != gpio) {
    context->last_value = gpio;
    printf("%lu output q is %s\n", main_time, gpio ? "ON" : "OFF");
  }
}
*/
/*
typedef struct {
  uint8_t state;
  char ch;
  uint32_t baud_t;
  vluint64_t last_update;
} uart_context_t;

void uart_init(uart_context_t *context, uint32_t baud_rate) {
  context->baud_t = 1000*1000*1000/baud_rate;
  context->state = 0;
}

void do_uart(uart_context_t *context, bool rx) {
  if (context->state == 0) {
    if (rx)
      context->state++;
  }
  else if (context->state == 1) {
    if (!rx) {
      context->last_update = main_time + context->baud_t/2;
      context->state++;
    }
  }
  else if(context->state == 2) {
    if (main_time > context->last_update) {
      context->last_update += context->baud_t;
      context->ch = 0;
      context->state++;
    }
  }
  else if (context->state < 11) {
    if (main_time > context->last_update) {
      context->last_update += context->baud_t;
      context->ch |= rx << (context->state-3);
      context->state++;
    }
  }
  else {
    if (main_time > context->last_update) {
      context->last_update += context->baud_t;
      putchar(context->ch);
      context->state=1;
    }
  }
}
*/

int main(int argc, char **argv/*, char **env*/)
{
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<vluint64_t> distrib(-10,10);
  //vluint64_t clock_jitter; 	
  vluint64_t sample_time = 0;
	uint32_t insn = 0;
	uint32_t ex_pc = 0;
	//int baud_rate = 0;
  vluint64_t clock_count = 0; 

//	gpio_context_t gpio_context;
//	uart_context_t uart_context;
	Verilated::commandArgs(argc, argv);

	VDecaTopLevelSim* top = new VDecaTopLevelSim;

/*
	const char *arg = Verilated::commandArgsPlusMatch("uart_baudrate=");
	if (arg[0]) {
	  baud_rate = atoi(arg+15);
	  if (baud_rate) {
	    uart_init(&uart_context, baud_rate);
	  }
	}
*/
	VerilatedVcdC * tfp = 0;
	const char *vcd = Verilated::commandArgsPlusMatch("vcd=");
	if (vcd[0]) {
	  Verilated::traceEverOn(true);
	  tfp = new VerilatedVcdC;
	  top->trace (tfp, 99);
	  tfp->open ("trace.vcd");
	}

	signal(SIGINT, INThandler);

	vluint64_t timeout = 0;
	const char *arg_timeout = Verilated::commandArgsPlusMatch("timeout=");
	if (arg_timeout[0])
	  timeout = atoi(arg_timeout+9);
    vluint64_t key1_toggle_time = 100000;
	vluint64_t vcd_start = 0;
	const char *arg_vcd_start = Verilated::commandArgsPlusMatch("vcd_start=");
	if (arg_vcd_start[0])
	  vcd_start = atoi(arg_vcd_start+11);

    top->uart_0_rx = 1;
  //  top->uart_1_rx = 1; 
   // top->SEN_SDO = 0;
	bool dump = false;
	top->DDR3_CLK_50 = 1;
	top->i_clk = 1;
//	top->USB_CLKIN = 1;
//	top->USB_DATA_i = 0xA5;
	top->key1 = 0;
	top->SW0 = 0;
	top->SW1 = 0;
	//bool q = top->q;
	while (!(done || Verilated::gotFinish())) {
	  if (tfp && !dump && (main_time > vcd_start)) {
	    dump = true;
	  }
	  top->i_rst = main_time < 200;
	 // if( (clock_count % 3) == 0)
 // if( (clock_count%17) == 0 )
   //  if( (clock_count % 15) == 0)
//	  { 
	 	top->i_clk = !top->i_clk;
	 // 	top->USB_CLKIN = !top->USB_CLKIN;
		top->DDR3_CLK_50 = !top->DDR3_CLK_50;
	 
 		top->eval();
	  	if (dump)
	    tfp->dump(main_time);
//	  }
    


	    /*
	  if (baud_rate)
	    do_uart(&uart_context, top->q);
	  else
	    do_gpio(&gpio_context, top->q);
*/ 
      if(main_time > key1_toggle_time)
	  { 
         key1_toggle_time += 10000000;
        top->key1 = (top->key1+1) % 2;  
	//	top->SW0 =  (top->SW0+1) % 2;
	//	top->SW1 = (top->SW1+1) % 2;    
	  }   
	  if (timeout && (main_time >= timeout)) {
	    printf("Timeout: Exiting at time %lu\n", main_time);
	    done = true;
	  }
       main_time+=(1);  
       
	  {
		 
		//    top->DDR3_CLK_50 = !top->DDR3_CLK_50;
      	 	
	//  	 top->eval();
//		if (dump)
//	    tfp->dump(main_time);
	  }
	
	  main_time+=(9);

      clock_count++;
	}
	if (tfp)
	  tfp->close();
	exit(0);
}
