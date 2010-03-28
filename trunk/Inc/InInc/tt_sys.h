#ifndef INC_TT_SYS_H__
#define INC_TT_SYS_H__


#define PSR_MODE_MASK	0x1F
#define PSR_MODE_FIQ	0x11
#define PSR_MODE_IRQ	0x12
#define PSR_IRQ_ENABLE	0x80
#define PSR_FIQ_ENABLE	0x40



extern int	g_iIRQ_disable_count;
extern bool	g_bIRQ_real_disable;




__inline bool sysIsInIRQ (void)
{
#if 1
	return false;
#else
	int tmp;
	__asm
	{
		MRS tmp, CPSR;
	}
	tmp &= PSR_MODE_MASK;
	
	if (tmp == PSR_MODE_IRQ)
		return true;
	else
		return false;
#endif
}


__inline bool sysIsIRQDisabled (void)
{
	int primask = __get_PRIMASK ();
	if ((primask & 1) == 0)
		return false;
	else
		return true;
}


__inline void sysEnableIRQ (void)
{
	if (!sysIsIRQDisabled ())
	{
		__set_PRIMASK(1);
		printf ("Not call sysDisableIRQ before sysEnableIRQ\n");
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


__inline void sysDisableIRQ (void)
{
	if (sysIsIRQDisabled ())
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
__inline void sysDumpIRQ (void)
{
	printf ("IRQ level: %d\n", g_iIRQ_disable_count);
}
#endif


#define NO_PRINTF
#if !defined NO_PRINTF
#define sysSafePrintf(...) \
do \
{ \
	sysDisableIRQ (); \
	printf(__VA_ARGS__); \
	sysEnableIRQ (); \
} while (0)
#else
#define sysSafePrintf(...)
#endif



#define ASSERT(expr) \
if (!(expr)) \
{ \
	sysSafePrintf ("Assert failed in line %d (%s):\n    %s\n", __LINE__, __FILE__, #expr); \
	sysDisableIRQ (); \
	while (1); \
}




#endif


