#include "philosophers.h"

void	ft_print(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->data->print);
	pthread_mutex_lock(&philo->data->end_mutex);
	if(philo->data->end == 1)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		pthread_mutex_unlock(&philo->data->print);
		return ;
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	printf(BLUE"[%ld ms]" CYAN" Philosopher " GREEN"{%i} " YELLOW"%s"RESET"\n", \
		ft_get_time() - philo->data->start_time, philo->philo_id, str);
	pthread_mutex_unlock(&philo->data->print);
}

void	ft_usleep(useconds_t time)
{
	u_int64_t	start;

	start = ft_get_time();
	while (ft_get_time() - start < time)
		usleep(time / 10);
}

u_int64_t ft_get_time(void)
{
    struct timeval tv;
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
	while(s[++i])
		if(s[i] < '0' || s[i] > '9')
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

void    ft_count_down(int seconds)
{
    int j;
	system("clear");
    printf(CYAN"Simulation starting in:\n\n");
    while (seconds-- > 1)
    {
        printf(CYAN" %d "RESET, seconds);
        ft_usleep(222);
        j = -1;
        while (++j < 3)
        {
            printf(". ");
            fflush(stdout);
            ft_usleep(222);
        }
        printf("\033[2K");
		printf("\r");
    }
    printf(GREEN"	🏁🏁🏁🏁🏁 ¡GO! 🏁🏁🏁🏁🏁\n\n"RESET);
}