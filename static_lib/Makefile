NAME = toolbox
SRCS = $(shell find ../src -type f -name '*.hpp')
OBJS = $(SRCS:.hpp=.o)

.DEFAULT_GOAL := all

list: $(SRCS)
	@echo $(SRCS)
	@echo $(OBJS)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

%.o: %.hpp
	g++ -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean

re: fclean all

.PHONY: all clean fclean re
