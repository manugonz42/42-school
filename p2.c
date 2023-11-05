# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <fcntl.h>
# include <stdint.h>

void ft_print(char *str) {
    printf("[ms] Philosopher %s\n", str);
}

int main() {
    ft_print("time elapsed "\033[30m\"philosopher 1\"\033[0m\" is eating");
    return 0;
}