# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zaiicko <meskrabe@student.s19.be>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/29 23:59:36 by zaiicko           #+#    #+#              #
#    Updated: 2025/04/14 02:10:17 by zaiicko          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
READLINE_PATH = $(shell brew --prefix readline)
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INC_DIR = inc
INC = -I$(READLINE_PATH)/include -I$(INC_DIR) -I$(LIBFT_DIR)
LDFLAGS = -L$(READLINE_PATH)/lib -lreadline -lhistory
SRC_DIR = src
OBJ_DIR = obj
RM = rm -rf

SRCS =	main.c\
	input_handler.c\
	signal_handler.c\
	utils.c\
	ast.c\
	lexer/lexer.c\
	lexer/lexer_utils.c\
	lexer/lexer_utils2.c\

SRC = $(addprefix $(SRC_DIR)/, $(SRCS))

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: logo $(NAME)

$(NAME):	$(OBJ) $(LIBFT)
		@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(LIBFT):
		@make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		@mkdir -p $(dir $@)
		@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJ_DIR):
		@mkdir -p $(OBJ_DIR)

clean:
			@$(RM) $(OBJ_DIR)
			@make clean -C $(LIBFT_DIR)

fclean: clean
		@$(RM) $(NAME)
		@make fclean -C $(LIBFT_DIR)

re:		fclean all

logo:
	@echo "\033[31m"
	@echo "                                                                         "
	@echo "                 _|            _|            _|                  _|  _|  "
	@sleep 0.01
	@echo " _|_|_|  _|_|        _|_|_|          _|_|_|  _|_|_|      _|_|    _|  _|  "
	@sleep 0.01
	@echo " _|    _|    _|  _|  _|    _|  _|  _|_|      _|    _|  _|_|_|_|  _|  _|  "
	@sleep 0.01
	@echo " _|    _|    _|  _|  _|    _|  _|      _|_|  _|    _|  _|        _|  _|  "
	@sleep 0.01
	@echo " _|    _|    _|  _|  _|    _|  _|  _|_|_|    _|    _|    _|_|_|  _|  _|  "
	@sleep 0.01
	@echo "                                                                         "
	@sleep 0.01
	@echo "                                                                         "

.PHONY:	fclean re all clean logo
