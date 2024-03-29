# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rubennijhuis <rubennijhuis@student.coda      +#+                      #
#                                                    +#+                       #
#    Created: 2022/02/02 20:54:00 by rubennijhui   #+#    #+#                  #
#    Updated: 2023/09/26 21:57:51 by dyeboa        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

#======= General =======#
CC			=	g++

SRC_DIR		=	src
OBJS_DIR	=	objs
BIN_DIR		=	bin
INCLUDE_DIR	=	include
EXEC_NAME	=	Webserv_dev
ASSET		= 	config/test.conf
NAME		=	$(BIN_DIR)/$(EXEC_NAME)
OUTPUT		=	$(BIN_DIR)/$(EXEC_NAME)

#======== SRCS =========#

INC			=	-I $(INCLUDE_DIR)

SRCS		=	CgiResponse.cpp \
				Client.cpp \
				Config.cpp \
				main.cpp \
				MimeTypes.cpp \
				ParseConfig.cpp \
				ParseConfigUtil.cpp \
				ParseVariables.cpp \
				Path.cpp \
				Request.cpp \
				Response.cpp \
				ResponseDirectory.cpp \
				ResponseUploadDelete.cpp \
				Server.cpp \
				ServerControl.cpp \
				Settings.cpp \
				utils.cpp

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

re: fclean all ./$(OUTPUT) $(ASSET)

.PHONY: run all clean fclean re