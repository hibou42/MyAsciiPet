NAME = pet

#***** Flags *****#

CXX = c++
CXXFLAGS = -std=c++17 -I./include

#***** Sources / Objs *****#

SRCS =	pet.cpp

OBJ_DIR			= obj
OBJS			= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

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
	@rm -rf $(NAME)

re: clean $(NAME)

.PHONY: clean fclean re