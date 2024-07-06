NAME = pet

CXX = c++
CXXFLAGS = -std=c++17 -I./include

SOURCE =	pet.cpp

$(NAME): $(SOURCE)
	@echo "Compilation in progress"
	@$(CXX) $(CXXFLAGS) $(SOURCE) -o $(NAME)

clean:
	@echo "Delete in progress"
	@rm -rf $(NAME)

re :	clean $(NAME)

.PHONY: clean re