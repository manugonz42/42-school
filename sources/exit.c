#include "philosophers.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int i;

	i = -1;
	while(s[++i])
		ft_putchar_fd(s[i], fd);
}

void	ft_clean_all(t_data *data)
{
	int i;

	i = -1;
	while (++i < data->n_of_philos)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->waiter);
	pthread_mutex_destroy(&data->barrier);
	free(data->forks);
	free(data->philos);
}

void	ft_thread_fail(t_data *data)
{
	pthread_mutex_lock(&data->end_mutex);
	data->end = 1;
	pthread_mutex_lock(&data->end_mutex);
	pthread_mutex_unlock(&data->barrier);
	ft_usleep(500);
	ft_clean_all(data);
	ft_error_message("Error creating thread");
}

void    ft_error_message(char *s)
{
    ft_putstr_fd(RED "Error: " YELLOW, STDERR_FILENO);
    ft_putstr_fd(s, STDERR_FILENO);
    ft_putstr_fd(RESET "\n", STDERR_FILENO);
    exit(EXIT_FAILURE);
}