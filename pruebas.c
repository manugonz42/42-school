#include "include/philosophers.h"

int glob = 0;
pthread_mutex_t barrier;

void	ft_start_data(int argc, char *argv[], t_data *data)
{
	data->n_of_philos = 4;
	data->tt_die = 800;
	data->tt_eat = 200;
	data->tt_sleep = 200;
	data->printd = 0;
	data->started_philos = 0;
	data->end = 0;
	data->times_teat = 3;
}

void	ft_supervisor(t_data *data)
{
	int	i;

	while(data->end == 0)
	{
		i = -1;
		while(++i < data->n_of_philos)
		{
			if(INANITION)
			{
				data->end = 1;
				printf(RESET"philo %i ha muerto, last_eat: %ld, time from l_e: %ld.\n", i, data->philos[i].last_eat, ft_get_time() - data->philos[i].last_eat);
				return ;
			}
		}
	}
}

u_int64_t ft_get_time(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0)
    {
        perror("gettimeofday failed");
        exit(1);
    }
    return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / (uint64_t)1000));
}

void	ft_init_forks(t_data *data)
{
	int	i;

	i = -1;
	while(++i < data->n_of_philos)
		if(pthread_mutex_init(&data->forks[i], NULL))
			ft_error_message("Mutex initialization failed.");
}

void	ft_init_philos(t_data *data)
{
	int	i;

	i = data->n_of_philos;
	while(--i >= 0)
	{
		data->philos[i].philo_id = i + 1;
		if (i != 0)
			data->philos[i].l_fork = &data->forks[i - 1];
		else
			data->philos[i].l_fork = &data->forks[data->n_of_philos - 1];
		data->philos[i].r_fork = &data->forks[i];
	}
}

void	ft_routine(t_philo *philo)
{
	int i = 0;
	while(i++ < 4)
	{
		if (philo->data->end == 1)
			return;
		printf(BLUE"philo " YELLOW"{%i}" BLUE" is " YELLOW "THINKING.\n", philo->philo_id);
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->l_fork);
		if (philo->data->end == 1)
		{
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
			return ;
		}
		printf(BLUE"philo " GREEN"{%i}" BLUE" is " GREEN "EATING.\n", philo->philo_id);
		printf("without eat: %ld\n", ft_get_time() - philo->last_eat);
		philo->eating = 1;
		usleep(philo->data->tt_eat * 1000);
		philo->eating = 0;
		philo->last_eat = ft_get_time();
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		if (philo->data->end == 1)
			return ;
		printf(BLUE"philo " MAGENTA"{%i}" BLUE" is " MAGENTA "SLEEPING.\n", philo->philo_id);
		usleep(philo->data->tt_sleep * 1000);
	}
}

void *modify(void *a)
{
    t_philo *philo = (t_philo *)a;
    pthread_mutex_lock(&barrier);
    while(philo->data->started_philos < philo->data->n_of_philos)
	{
		usleep(1);
	}
    pthread_mutex_unlock(&barrier);
	glob++;
	ft_routine(philo);
    return NULL;
}

int main(int argc, char *argv[])
{

    int i;
    t_data *data = (t_data *)malloc(sizeof(t_data));
    if (data == NULL)
    {
        perror("malloc failed");
        return 1;
    }
	data->start_time = ft_get_time(); // !!!!!!!!cambiar ubicacion;
	ft_start_data(argc, argv, data);
	ft_init_forks(data);
	ft_init_philos(data);
	pthread_mutex_init(&barrier, NULL);
	i = -1;
    while (++i < 6)
    {
        data->philos[i].data = data; 
        if (pthread_create(&data->philos[i].thread, NULL, &modify, (void *)&data->philos[i]) != 0)
        {
            perror("Error creating thread");
            return 1;
        }
		data->started_philos++;
    }
	//pthread_mutex_lock(&data->forks[2]);
	i = -1;
	while(++i < data->n_of_philos)
		data->philos[i].last_eat = ft_get_time();
	printf("esperando 5\n");
	ft_supervisor(data);
    for (int i = 0; i < 6; i++)
    {
        if (pthread_join(data->philos[i].thread, NULL) != 0)
        {
            perror("Error joining thread");
            return 1;
        }
    }
	printf("sigue\n");
    free(data);
    return 0;
}
