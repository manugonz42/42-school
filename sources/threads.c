#include "philosophers.h"

void	*ft_waiter(void *a)
{
	t_data *data;

	data = (t_data *)a;
	while(data->end == 0)
	{

		pthread_mutex_lock(&data->waiter);
		if (data->started_philos == 0 || data->times_teat == 0)
		{
			pthread_mutex_lock(&data->print);
			data->end = 1;
			printf(YELLOW"All philosophers have eaten " \
				GREEN"%li" YELLOW" times."RESET"\n", data->times_teat);
			pthread_mutex_unlock(&data->print);
			pthread_mutex_unlock(&data->waiter);
			usleep(500);
			ft_clean_all(data);
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
		data->end = 1;
		ft_usleep(500);
		ft_clean_all(data);
		ft_error_message("Error creating thread");
	}
	if (pthread_detach(data->t_waiter) != 0)
	{
		data->end = 1;
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
	while(data->end == 0)
	{
		i = -1;
		while(++i < data->n_of_philos)
		{
			pthread_mutex_lock(&data->barrier);
			if(INANITION)
			{
				data->end = 1;
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
        {
			data->end = 1;
			pthread_mutex_unlock(&data->barrier);
			ft_usleep(500);
			ft_clean_all(data);
			ft_error_message("Error creating thread");
		}
        if (pthread_detach(data->philos[i].thread) != 0)
        {
			data->end = 1;
			pthread_mutex_unlock(&data->barrier);
			ft_usleep(500);
			ft_clean_all(data);
			ft_error_message("Error creating thread");
		}
		data->started_philos++;
    }
	i = -1;
	while(++i < data->n_of_philos)
		if (data->philos[i].started == 0)
			i = -1;
    //ft_count_down(4);
    ft_supervisor(data);
}

void	ft_init_forks(t_data *data)
{
	int	i;
	int	x;

	i = -1;
	if (pthread_mutex_init(&data->barrier, NULL))
	{
		free(data->forks);
		free(data->philos);
		ft_error_message("Mutex initialization failed.");
	}
	data->barrier = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	if (pthread_mutex_init(&data->print, NULL))
	{
		free(data->forks);
		free(data->philos);
		ft_error_message("Mutex initialization failed.");
	}
	data->print = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
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