#include "philosophers.h"

void	*ft_waiter(void *a)
{
	t_data *data;
	data = (t_data *)a;
	while(1)
	{
		pthread_mutex_lock(&data->end_mutex);
		if (data->end == 1)
		{
			pthread_mutex_unlock(&data->end_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&data->end_mutex);
		pthread_mutex_lock(&data->waiter);
		if (data->started_philos == 0 || data->times_teat == 0)
		{
			pthread_mutex_lock(&data->print);
			pthread_mutex_lock(&data->end_mutex);
			data->end = 1;
			pthread_mutex_unlock(&data->end_mutex);
			printf(YELLOW"All philosophers have eaten " \
				GREEN"%li" YELLOW" times."RESET"\n", data->times_teat);
			pthread_mutex_unlock(&data->print);
			pthread_mutex_unlock(&data->waiter);
			printf("1\n");
			ft_usleep(500);
			printf("2\n");
			ft_clean_all(data);
			printf("3\n");
			return NULL;
		}
		pthread_mutex_unlock(&data->waiter);
		usleep(500);
	}
	return NULL;
}

void	ft_create_waiter(t_data *data)
{
	if (pthread_create(&data->t_waiter, NULL, &ft_waiter, (void *)data) != 0)
	{	
		pthread_mutex_lock(&data->end_mutex);
		data->end = 1;
		pthread_mutex_unlock(&data->end_mutex);
		ft_usleep(500);
		ft_clean_all(data);
		ft_error_message("Error creating thread");
	}
	if (pthread_detach(data->t_waiter) != 0)
	{
		pthread_mutex_lock(&data->end_mutex);
		data->end = 1;
		pthread_mutex_unlock(&data->end_mutex);
		ft_usleep(500);
		ft_clean_all(data);
		ft_error_message("Error detaching thread");
	}
}

void	ft_supervisor(t_data *data)
{
	int	i;

	if (data->times_teat != -1)
		ft_create_waiter(data);
	data->start_time = ft_get_time();
	pthread_mutex_unlock(&data->barrier);
	usleep(500);
	while(1)
	{
		pthread_mutex_lock(&data->end_mutex);
		if (data->end == 1)
		{
			pthread_mutex_unlock(&data->end_mutex);
			ft_usleep(500);
			return ;
		}
		pthread_mutex_unlock(&data->end_mutex);
		i = -1;
		while(++i < data->n_of_philos)
		{
			pthread_mutex_lock(&data->barrier);
			if(INANITION)
			{
				pthread_mutex_lock(&data->end_mutex);
				if (data->end == 1)
				{
					pthread_mutex_unlock(&data->end_mutex);
					pthread_mutex_unlock(&data->barrier);
					ft_usleep(500);
					return ;
				}
				data->end = 1;
				pthread_mutex_unlock(&data->end_mutex);
				printf(BLUE"[%ld ms]" CYAN" Philosopher " GREEN"{%i} " RED"died"RESET"\n", \
					ft_get_time() - data->start_time, data->philos[i].philo_id);
				pthread_mutex_unlock(&data->barrier);
				ft_usleep(500);
				ft_clean_all(data);
				return ;
			}
			pthread_mutex_unlock(&data->barrier);
			usleep(500);
		}
		ft_usleep(500);
	}
}

void    ft_init_threads(t_data *data)
{
    int	i;
    i = -1;
	pthread_mutex_lock(&data->barrier);
    while(++i < data->n_of_philos)
    {
        if (pthread_create(&data->philos[i].thread, NULL, &ft_barrier, (void *)&data->philos[i]) != 0)
			ft_thread_fail(data);
        if (pthread_detach(data->philos[i].thread) != 0)
			ft_thread_fail(data);
		data->started_philos++;
    }
	
    //ft_count_down(4);
	ft_usleep(500);
    ft_supervisor(data);
}

void	ft_init_forks(t_data *data)
{
	int	i;
	int	x;

	i = -1;
	while(++i < data->n_of_philos)
	{
		if(pthread_mutex_init(&data->forks[i], NULL))
		{
			x = 0;
			while(x < i)
				pthread_mutex_destroy(&data->forks[x++]);
			free(data->forks);
			free(data->philos);
			ft_error_message("Mutex initialization failed.");
		}
		data->forks[i] = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	}
}