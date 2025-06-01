
NAME = $(notdir $(CURDIR))_test.out

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror
LDFLAGS = -lm

.DEFAULT_GOAL := run

s:
	echo $(NAME)

run: $(NAME)
	./$(NAME)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run
