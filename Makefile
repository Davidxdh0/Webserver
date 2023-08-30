# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rubennijhuis <rubennijhuis@student.coda      +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/02 20:54:00 by rubennijhui   #+#    #+#                  #
#    Updated: 2023/06/08 13:53:52 by rubennijhui   ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#======= General =======#
CC			=	g++

SRC_DIR		=	src
OBJS_DIR	=	objs
BIN_DIR		=	bin
INCLUDE_DIR	=	include
EXEC_NAME	=	Webserv
NAME		=	$(BIN_DIR)/$(EXEC_NAME)
OUTPUT		=	$(BIN_DIR)/$(EXEC_NAME)

#======== SRCS =========#

INC			=	-I $(INCLUDE_DIR)

SRCS		=	main.cpp \
				Config.cpp \
				Client.cpp \
				Server.cpp \
				Path.cpp \
				ServerControl.cpp\
				utils.cpp \
				Request.cpp \
				Response.cpp \
				Settings.cpp \

OBJS		=	$(addprefix $(OBJS_DIR)/,$(SRCS:.cpp=.o))

#== Compilation stuff ==#

CFLAGS		=	-Wall -Wextra -Werror -std=c++98 $(INC)

#======= Rules =========#

objs/%.o:src/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) -c $(CFLAGS) -o $@ $^
	@echo "🔨 Compiling: $^"

all: $(NAME)

run: $(NAME)
	./$(OUTPUT) $(ASSET)

$(NAME):$(OBJS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT)
	@echo "Finished creating: $@"

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: run all clean fclean re