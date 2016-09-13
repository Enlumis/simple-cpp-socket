CC		= clang++

RM		= rm -f

SRCDIR		= src
OBJDIR		= obj

CFLAGS		= -W -Werror -Wall -pedantic -Iinclude

NAMESERVER	= serveur

SRCSERVER	= $(wildcard src/*.cpp)

OBJSERVER	= $(SRCSERVER:%.cpp=%.o)

VPATH		= $(SRCDIR)

%.o	: %.cpp
	@echo "Compiling Server Source : \"$<\".."
	@$(CC) -c $< $(CFLAGS) -Iinclude -o $@

all: $(NAMESERVER)

$(NAMESERVER): $(OBJSERVER)
	@$(CC) $(OBJSERVER) $(CFLAGS) -o $(NAMESERVER)
	@echo "Compiling DONE: $@"

clean:
	@echo -n "Removing Server Objects.. "
	@$(RM) $(OBJSERVER)
	@echo " --[DONE]"

fclean: clean
	@echo "Removing $(NAMESERVER)"
	@$(RM) $(NAMESERVER)

re: fclean all

.PHONY: all clean fclean re
