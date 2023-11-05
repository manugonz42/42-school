#include "../include/philosophers.h"

void	*ft_barrier(void *x)
{
	t_philo *philo;

	philo = (t_philo *)x;
	philo->started = 1;
	pthread_mutex_lock(&philo->data->barrier);
	pthread_mutex_unlock(&philo->data->barrier);
	ft_routine(philo);
	return (NULL);
}

int	main(int argc, char *argv[])
{
    t_data *data;
	
	ft_check_input(argc, argv);
	data = (t_data *)malloc(sizeof(t_data));
    if (data == NULL)
    {
        ft_error_message("malloc failed");
        return 1;
    }
	ft_start_data(argc, argv, data);
	ft_init_forks(data);
	ft_init_philos(data);
	ft_init_threads(data);
	free(data);
    return 0;
}