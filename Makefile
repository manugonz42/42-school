.SILENT:

TARGET = philo

# Archives
SOURCEDIR = sources/
SOURCES = main.c utils.c exit.c routine.c starting.c threads.c
SOURCES := $(addprefix $(SOURCEDIR), $(SOURCES))

#Libraries
LIBS = include/

# Colors
CYAN = \033[0;36m
RESET = \033[0M
BLACK = \033[0;30m
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
WHITE = \033[0;37m

# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I$(LIBS)

# Objets
OBJECTS = $(SOURCES:.c=.o)

# Rules
all: $(TARGET)
#clang -fsanitize=thread
$(TARGET): $(OBJECTS)
	@echo "$(CYAN)--Compiling $(BLUE)philo$(RESET)"
	$(CC) -o $(TARGET) $(OBJECTS)
	@echo "$(CYAN)--Ready$(WHITE)"

# Cleaning
clean:
	@echo "$(MAGENTA)--Deleting .o archives$(RESET)"
	@rm -f $(OBJECTS) 

fclean: clean
	@echo "$(MAGENTA)--Deleting .a archives$(RESET)"
	@rm -f $(TARGET)
re:	fclean
	@echo "$(BLUE)--Remaking all...$(RESET)"
	@$(MAKE) -s all
