#ifndef INC_TT_SYS_H__
#define INC_TT_SYS_H__

#ifdef __cplusplus
extern "C" {
#endif


extern volatile int	g_iIRQ_disable_count;
extern volatile bool	g_bIRQ_real_disable;

//#define TT_SYS_NO_PRINTF


/* Implement in tt_syscall.s */
#if defined __CC_ARM
#	if __CM0_CMSIS_VERSION < 0x00020000
TT_INLINE bool tt_is_in_irq (void)
{
	register uint32_t __regIPSR          __asm("ipsr");
	return(__regIPSR);
}	
#	else
TT_INLINE bool tt_is_in_irq (void)
{
	return __get_IPSR ();
}
#endif
#elif defined __GNUC__
__attribute__((always_inline )) TT_INLINE bool tt_is_in_irq (void)
{
	uint32_t result;
	__asm__ volatile ("MRS %0, ipsr" : "=r" (result) );
	return(result);
}
#elif defined __ICCARM__
#	pragma diag_suppress=Pe940
TT_INLINE bool tt_is_in_irq (void)
{
	__asm("	MRS R0, IPSR	");
}
#endif


TT_INLINE bool tt_is_irq_disabled (void)
{
	int primask = __get_PRIMASK ();
	if ((primask & 1) == 0)
		return false;
	else
		return true;
}


TT_INLINE void tt_enable_irq (void)
{
	if (!tt_is_irq_disabled ())
	{
		__set_PRIMASK(1);
#if !defined TT_SYS_NO_PRINTF
		printf ("Not call tt_disable_irq before tt_enable_irq\n");
#endif
		while (1);
	}

	g_iIRQ_disable_count--;
	if (g_iIRQ_disable_count == 0)
	{
		if (g_bIRQ_real_disable)
		{
			__set_PRIMASK(0);
		}
	}
}


TT_INLINE void tt_disable_irq (void)
{
	if (tt_is_irq_disabled ())
	{
		if (g_iIRQ_disable_count == 0)
			g_bIRQ_real_disable = false;
	}
	else
	{
		__set_PRIMASK(1);
		if (g_iIRQ_disable_count == 0)
			g_bIRQ_real_disable = true;
	}	
	g_iIRQ_disable_count++;
}



#ifdef DEBUG_DUMP_IRQ
TT_INLINE void tt_sys_dump_irq (void)
{
#if !defined TT_SYS_NO_PRINTF
	printf ("IRQ level: %d\n", g_iIRQ_disable_count);
#endif
}
#endif



#if !defined TT_SYS_NO_PRINTF
#define tt_printf(...) \
do \
{ \
	tt_disable_irq (); \
	printf(__VA_ARGS__); \
	tt_enable_irq (); \
} while (0)
#else
#define tt_printf(...)
#endif



#define TT_ASSERT(expr) \
if (!(expr)) \
{ \
	tt_printf ("Assert failed in line %d (%s):\n    %s\n", __LINE__, __FILE__, #expr); \
	tt_disable_irq (); \
	while (1); \
}


#ifdef __cplusplus
} // extern "C"
#endif

#endif


