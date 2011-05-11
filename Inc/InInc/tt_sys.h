#ifndef INC_TT_SYS_H__
#define INC_TT_SYS_H__

#ifdef __cplusplus
extern "C" {
#endif


extern int	g_iIRQ_disable_count;
extern bool	g_bIRQ_real_disable;

#define TT_SYS_NO_PRINTF


/* Implement in tt_syscall.s */
bool sysIsInIRQ (void);


__INLINE bool sysIsIRQDisabled (void)
{
	int primask = __get_PRIMASK ();
	if ((primask & 1) == 0)
		return false;
	else
		return true;
}


__INLINE void sysEnableIRQ (void)
{
	if (!sysIsIRQDisabled ())
	{
		__set_PRIMASK(1);
#if !defined TT_SYS_NO_PRINTF
		printf ("Not call sysDisableIRQ before sysEnableIRQ\n");
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


__INLINE void sysDisableIRQ (void)
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
__INLINE void sysDumpIRQ (void)
{
#if !defined TT_SYS_NO_PRINTF
	printf ("IRQ level: %d\n", g_iIRQ_disable_count);
#endif
}
#endif



#if !defined TT_SYS_NO_PRINTF
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


#ifdef __cplusplus
} // extern "C"
#endif

#endif


