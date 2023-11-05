#include "philosophers.h"

void	ft_start_data(int argc, char *argv[], t_data *data)
{
    (void)argc;
    (void)argv;
	data->n_of_philos = ft_atoi(argv[1]);
	if (!(data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * (data->n_of_philos))))
		ft_error_message("Malloc failed");
	if (!(data->philos = (t_philo *)malloc(sizeof(t_philo) * (data->n_of_philos))))
	{
		free(data->forks);
		ft_error_message("Malloc failed");
	}
	data->tt_die = ft_atoi(argv[2]);
	data->tt_eat = ft_atoi(argv[3]);
	data->tt_sleep = ft_atoi(argv[4]);
	data->started_philos = 0;
	data->start_time = 0;
	data->end = 0;
	if (argc == 6)
	{
		data->times_teat = ft_atoi(argv[5]);
		if (pthread_mutex_init(&data->waiter, NULL))
		{
			free(data->forks);
			free(data->philos);
			ft_error_message("Mutex initialization failed.");
		}
		data->waiter = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	}
	else	
		data->times_teat = -1;
	if (pthread_mutex_init(&data->barrier, NULL))
		ft_error_message("Mutex initialization failed.");
	data->barrier = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	if (pthread_mutex_init(&data->print, NULL))
		ft_error_message("Mutex initialization failed.");
	data->print = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

void	ft_init_philos(t_data *data)
{
	int	i;

	i = -1;
	i = data->n_of_philos;
	while(--i >= 0)
	{
		data->philos[i].data = data;
		data->philos[i].philo_id = i + 1;
		data->philos[i].times_eated = 0;
		data->philos[i].satiated = 0;
		data->philos[i].eating = 0;
		data->philos[i].thread = 0;
		data->philos[i].started = 0;
		data->philos[i].last_eat = 0;
		if (i != 0)
			data->philos[i].l_fork = &data->forks[i - 1];
		else
			data->philos[i].l_fork = &data->forks[data->n_of_philos - 1];
		data->philos[i].r_fork = &data->forks[i];
	}
}

