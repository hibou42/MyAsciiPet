NAME	= asciiPet

#***** Flags *****#

CXX		= c++
CXXFLAGS = -std=c++17 -I./include

#***** Sources / Objs *****#

SRCS	=	main.cpp \
			srcs/game_logic.cpp \
			srcs/input_handler.cpp \
			srcs/display_manager.cpp

OBJ_DIR	= obj
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

#***** Compilation *****#

all : $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@echo "Compilation in progress"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) :	${OBJS}
	@${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

clean:
	@echo "Delete in progress"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean $(NAME)

test: all
	@echo "Saves will be delete"
	@rm -rf saves
	@echo "Let's run !!!"
	@./$(NAME)

TEST: fclean all 
	@echo "Saves will be delete"
	@rm -rf saves
	@echo "Let's run !!!"
	@./$(NAME)

.PHONY: clean fclean re test TEST