CC		= clang++

RM		= rm -f

SRCDIR		= src
OBJDIR		= obj

CFLAGS		= -W -Werror -Wall -pedantic -Iinclude

NAMESERVER	= server

SRCSERVER	= $(wildcard src/*.cpp)

OBJSERVER	= $(SRCSERVER:%.cpp=%.o)

VPATH		= $(SRCDIR)

%.o	: %.cpp
	$(CC) -c $< $(CFLAGS) -Iinclude -o $@

all: $(NAMESERVER)

$(NAMESERVER): $(OBJSERVER)
	@$(CC) $(OBJSERVER) $(CFLAGS) -o $(NAMESERVER)
	@echo "Compiling DONE: $@"

clean:
	@echo "Cleaning compiled files"
	@$(RM) $(OBJSERVER)
	
sclean:
	@echo "Cleaning compiled files"
	@$(RM) $(OBJSERVER)
	
fclean: clean
	@echo "Cleaning executable"
	@$(RM) $(NAMESERVER)


re: fclean all sclean

.PHONY: all clean fclean re
