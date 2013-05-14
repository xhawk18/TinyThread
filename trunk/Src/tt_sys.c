#include "../Inc/tt_thread.h"

volatile int g_iIRQ_disable_count = 0;
volatile bool g_bIRQ_real_disable = 0;

