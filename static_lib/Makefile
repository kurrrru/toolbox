NAME = toolbox.a
SRCS = $(shell find ./src -type f -name '*.cpp')
OBJS = $(SRCS:.cpp=.o)

.DEFAULT_GOAL := all

list: $(SRCS)
	@echo $(SRCS)
	@echo $(OBJS)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.cpp
	g++ -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
