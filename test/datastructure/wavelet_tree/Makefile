NAME		= wavelet_tree_test.out
CXX			= g++
CXXFLAGS	= -std=c++11 -Wall -Wextra -O2 -I../../..

SRCS		= main.cpp
OBJS		= $(SRCS:.cpp=.o)

run: $(NAME)
	./$(NAME)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all run clean fclean re
