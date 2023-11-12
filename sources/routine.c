#include "philosophers.h"

int	ft_taKe_forks(t_philo *philo)
{
	if (philo->philo_id == philo->data->n_of_philos)
	{
		pthread_mutex_lock(philo->l_fork);
		if (!ft_print(philo, "has taken a FORK. (right)"))
		{
			pthread_mutex_unlock(philo->l_fork);
			return (0);
		}
		pthread_mutex_lock(philo->r_fork);
		if (!ft_print(philo, "has taken a FORK. (left)"))
		{
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
			return (0);
		}
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		if (!ft_print(philo, "has taken a FORK. (right)"))
		{
			pthread_mutex_unlock(philo->r_fork);
			return (0);
		}
		pthread_mutex_lock(philo->l_fork);
		if (!ft_print(philo, "has taken a FORK. (left)"))
		{
			pthread_mutex_unlock(philo->r_fork);
			pthread_mutex_unlock(philo->l_fork);
			return (0);
		}
	}
	return (1);
}

void	ft_check_eated(t_philo *philo)
{
	if (philo->data->times_teat != -1 && philo->satiated == 0)
	{
		philo->times_eated++;
		if (philo->times_eated >= philo->data->times_teat)
		{
			philo->satiated = 1;
			pthread_mutex_lock(&philo->data->waiter);
			philo->data->started_philos--;
			pthread_mutex_unlock(&philo->data->waiter);
		}
	}
}

void	ft_eat(t_philo *philo)
{
	if(!ft_taKe_forks(philo))
		return ;
	pthread_mutex_lock(&philo->data->barrier);
	philo->last_eat = ft_get_time();
	philo->eating = 1;
	ft_print(philo, "is EATING");
	pthread_mutex_unlock(&philo->data->barrier);
	ft_usleep(philo->data->tt_eat);
	pthread_mutex_lock(&philo->data->barrier);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->data->barrier);
	ft_check_eated(philo);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	ft_routine(t_philo *philo)
{
	if (philo->data->times_teat == 0)
		return ;
	if(philo->philo_id % 2 == 0 && philo->data->n_of_philos != 2)
		usleep(100);
	pthread_mutex_lock(&philo->data->barrier);
	philo->last_eat = ft_get_time();
	pthread_mutex_unlock(&philo->data->barrier);
	while(1)
	{
		pthread_mutex_lock(&philo->data->end_mutex);
		if (philo->data->end == 1)
		{
			pthread_mutex_unlock(&philo->data->end_mutex);
			pthread_mutex_lock(&philo->data->print);
			printf("Philo %i out.\n", philo->philo_id);
			pthread_mutex_unlock(&philo->data->print);
			return ;
		}
		pthread_mutex_unlock(&philo->data->end_mutex);
		ft_print(philo, "is THINKING");
		ft_eat(philo);
		ft_print(philo, "is SLEEPING");
		ft_usleep(philo->data->tt_sleep);
	}
}