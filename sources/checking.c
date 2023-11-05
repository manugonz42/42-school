#include "philosophers.h"

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