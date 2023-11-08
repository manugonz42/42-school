#include "../include/philosophers.h"

int	ft_check_input(int argc, char *argv[])
{
	int	i;

	if (argc == 5)
	{
		i = 0;
		while(++i < 5)
			if(!ft_isdigit(argv[i]))
				ft_error_message("All parameters must be numbers.");
		return (1);
	}
	if (argc == 6)
	{
		i = 0;
		while(++i < 6)
			if(!ft_isdigit(argv[i]))
				ft_error_message("All parameters must be numbers.");
		return (1);
	}
	ft_error_message("Wrong number of parameters, check it.");
	return (0);
}

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
	ft_init_gmutex(data);
	ft_init_forks(data);
	ft_init_philos(data);
	ft_init_threads(data);
	free(data);
    return 0;
}