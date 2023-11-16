#include "philosophers.h"

int	ft_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->end_mutex);
	if (philo->data->end == 1)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		pthread_mutex_unlock(&philo->data->print);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	printf(YELLOW"[%ld ms]" CYAN" Philo " GREEN"{%i} " YELLOW"%s"RESET"\n", \
		ft_get_time() - philo->data->start_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print);
	return (1);
}

void	ft_usleep(useconds_t time)
{
	u_int64_t	start;

	start = ft_get_time();
	while (ft_get_time() - start < time)
		usleep(time / 10);
}

u_int64_t	ft_get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		ft_error_message("Error getting time.");
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / (uint64_t)1000));
}

int	ft_isdigit(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = -1;
	while (s[++i])
		if (s[i] < '0' || s[i] > '9')
			return (0);
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int	cont;
	int	neg;
	int	res;

	res = 0;
	cont = 0;
	while ((nptr[cont] > 8 && nptr[cont] < 14) || nptr[cont] == 32)
		cont++;
	if (nptr[cont] == 45)
		neg = -1;
	else
		neg = 1;
	if (nptr[cont] == '+' || nptr[cont] == '-')
		cont++;
	while (nptr[cont] >= '0' && nptr[cont] <= '9')
	{
		res = (res * 10) + (nptr[cont] - 48);
		cont++;
	}
	return (res * neg);
}
