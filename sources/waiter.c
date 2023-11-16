#include "philosophers.h"

void	*ft_waiter_end(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->print);
	pthread_mutex_lock(&data->end_mutex);
	data->end = 1;
	pthread_mutex_unlock(&data->end_mutex);
	printf(YELLOW"All philosophers have eaten " \
		GREEN"%li" YELLOW" times."RESET"\n", data->times_teat);
	pthread_mutex_unlock(&data->print);
	pthread_mutex_unlock(&data->waiter);
	i = -1;
	while (++i < data->n_of_philos)
		pthread_join(data->philos[i].thread, NULL);
	return (NULL);
}

void	*ft_waiter(void *a)
{
	t_data	*data;

	data = (t_data *)a;
	while (1)
	{
		pthread_mutex_lock(&data->end_mutex);
		if (data->end == 1)
		{
			pthread_mutex_unlock(&data->end_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&data->end_mutex);
		pthread_mutex_lock(&data->waiter);
		if (data->started_philos == 0 || data->times_teat == 0)
			return (ft_waiter_end(data));
		pthread_mutex_unlock(&data->waiter);
		usleep(500);
	}
	return (NULL);
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
}
