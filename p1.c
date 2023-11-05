#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

int created_threads = 0;
pthread_mutex_t forkg; 

void	ft_init_forks(int i, pthread_mutex_t *forks)
{
	while(--i >= 0)
		if(pthread_mutex_init(&forks[i], NULL))
			printf("Mutex initialization failed.");
}

void    *modify(void *a)
{
    pthread_mutex_t *fork = (pthread_mutex_t *)a;
    pthread_mutex_lock(fork);
    pthread_mutex_lock(&forkg);
    printf("%i\n", created_threads);
    pthread_mutex_unlock(fork);
    pthread_mutex_unlock(&forkg);
    return NULL;
}

int main() 
{
    pthread_mutex_t fork[4];
    pthread_t       thread[4];


    pthread_mutex_init(&forkg, NULL);
    ft_init_forks(4, fork);
    pthread_mutex_lock(&fork[0]);
    for (int i = 0; i < 4; i++)
    {
        created_threads++;
        if (pthread_create(&thread[i], NULL, &modify, (void *)&fork[i]) != 0)
        {
            perror("Error creating thread");
            return 1;
        }
        usleep(100);
    }
    for (int i = 0; i < 4; i++)
    {
        if (pthread_join (thread[i], NULL) != 0)
        {
            perror("Error joining thread");
            return 1;
        }
    }
    return 0;
}