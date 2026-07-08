NAME    = PmergeMe

CXX     = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC     = $(wildcard *.cpp src/vector/*.cpp src/deque/*.cpp)

OBJDIR  = obj
OBJ     = $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
