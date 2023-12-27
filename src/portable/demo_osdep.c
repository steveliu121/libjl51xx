#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "jl_types.h"

#if defined CONFIG_JL_PHY_MUTEX || defined CONFIG_JL_APB_MUTEX || defined CONFIG_JL_API_MUTEX
void *port_mutex_init(void)
{
	pthread_mutex_t *mutex;

	mutex = malloc(sizeof(pthread_mutex_t));
	if (!mutex)
		return NULL;

	pthread_mutex_init(mutex, NULL);

	return mutex;
}

void port_mutex_deinit(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

	if (mutex) {
		pthread_mutex_destroy(mutex);
		free(mutex);
	}
}

void port_mutex_lock(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

	if (mutex)
		pthread_mutex_lock(mutex);
}

void port_mutex_unlock(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

	if (mutex)
		pthread_mutex_unlock(mutex);
}
#endif

void port_ndelay(jl_uint32 ns)
{
	jl_uint32 i;
	jl_uint32 us = ns / 1000;

	for (i = 0; i < us;)
		i++;
}

void port_udelay(jl_uint32 us)
{
	usleep(us);
}
