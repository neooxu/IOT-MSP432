

#include "mx_hal.h"
#include "cmsis_ccs.h"

/**
 * \brief Disable interrupts, enter critical section
 */
void atomic_enter_critical(hal_atomic_t volatile *atomic)
{
	*atomic = __get_PRIMASK();
	__disable_irq();
	__DMB();
}

/**
 * \brief Exit atomic section
 */
void atomic_leave_critical(hal_atomic_t volatile *atomic)
{
	__DMB();
	__set_PRIMASK(*atomic);
}
