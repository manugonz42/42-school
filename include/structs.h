#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_data	t_data;

typedef struct s_philo
{
	t_data			*data;
	int				started;
	int				times_eated;
	int				satiated;
	int				eating;
	int				philo_id;
	u_int64_t		last_eat;
	pthread_t		thread;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
}	t_philo;

typedef struct s_data
{
	int				started_philos;	
	int				n_of_philos;
	int				end;
	u_int64_t		tt_die;
	u_int64_t		start_time;
	u_int64_t		tt_eat;
	u_int64_t		tt_sleep;
	int64_t			times_teat;
	pthread_mutex_t	*forks;
	pthread_mutex_t	waiter;
	pthread_mutex_t	barrier;
	pthread_mutex_t	print;
	pthread_mutex_t	end_mutex;
	pthread_t		t_waiter;
	t_philo			*philos;
}	t_data;

#endif