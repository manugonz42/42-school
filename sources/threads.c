#include "philosophers.h"

/*
void	ft_print_all_memory_addresses(t_data *data)
{
	printf("data %p\n", data);
	printf("data->forks: %p\n", data->forks);
	printf("data->philos: %p\n", data->philos);
	printf("data->print: %p\n", &data->print);
	printf("data->end_mutex: %p\n", &data->end_mutex);
	printf("data->barrier: %p\n", &data->barrier);
	printf("data->waiter: %p\n", &data->waiter);
	printf("data->end: %p\n", &data->end);
	printf("data->start_time: %p\n", &data->start_time);
	printf("data->started_philos: %p\n", &data->started_philos);
	printf("data->t_waiter: %p\n", &data->t_waiter);
	int i = -1;
	while (++i < data->n_of_philos){
		printf("	*data->philos[%i] %p\n", i, &data->philos[i]);
		printf("data->philos[%i].eating %p\n", i, &data->philos[i].eating);
		printf("data->philos[%i].started %p\n", i, &data->philos[i].started);
		printf("data->philos[%i].last eat %p\n", i, &data->philos[i].last_eat);
		printf("data->forks[%i] %p\n", i, &data->forks[i]);
		printf("data->philos[%i].r_fork %p\n", i, data->philos[i].r_fork);
		printf("data->philos[%i].l_fork %p\n", i, data->philos[i].l_fork);}
}
*/

void	ft_starvation_end(t_data *data, int i)
{
	pthread_mutex_lock(&data->end_mutex);
	if (data->end == 1)
	{
		pthread_mutex_unlock(&data->end_mutex);
		pthread_mutex_unlock(&data->barrier);
		pthread_join(data->t_waiter, NULL);
		ft_clean_all(data);
		return ;
	}
	data->end = 1;
	pthread_mutex_unlock(&data->end_mutex);
	pthread_mutex_lock(&data->print);
	printf(BLUE"[%ld ms]" CYAN" Philo " GREEN"{%i} " RED"died"RESET"\n", \
		ft_get_time() - data->start_time, data->philos[i].philo_id);
	pthread_mutex_unlock(&data->print);
	pthread_mutex_unlock(&data->barrier);
	i = -1;
	while (++i < data->n_of_philos)
		pthread_join(data->philos[i].thread, NULL);
	if (data->times_teat != -1)
		pthread_join(data->t_waiter, NULL);
	ft_clean_all(data);
	return ;
}

void	ft_supervisor(t_data *data)
{
	int	i;

	if (data->times_teat != -1)
		ft_create_waiter(data);
	data->start_time = ft_get_time();
	pthread_mutex_unlock(&data->barrier);
	usleep(500);
	while (1)
	{
		i = -1;
		while (++i < data->n_of_philos)
		{
			pthread_mutex_lock(&data->barrier);
			if ((ft_get_time() - data->philos[i].last_eat) >= data->tt_die)
			{
				ft_starvation_end(data, i);
				return ;
			}
			pthread_mutex_unlock(&data->barrier);
			usleep(50);
		}
		usleep(50);
	}
}

void	ft_init_threads(t_data *data)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&data->barrier);
	while (++i < data->n_of_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, \
			&ft_barrier, (void *)&data->philos[i]) != 0)
			ft_thread_fail(data);
		data->started_philos++;
	}
	//ft_count_down(4);
	//usleep(500);
	//ft_print_all_memory_addresses(data);
	ft_supervisor(data);
}
