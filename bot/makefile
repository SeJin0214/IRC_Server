# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/11 17:30:52 by sejjeong          #+#    #+#              #
#    Updated: 2025/04/11 17:42:33 by sejjeong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sejjeong <sejjeong@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 16:33:48 by sejjeong          #+#    #+#              #
#    Updated: 2025/04/11 13:40:24 by sejjeong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = bot
C++ = c++
FLAGS= -Wall -Wextra -Werror -std=c++98 -g

RM = rm
RM_FLAGS = -rf

SRCS_DIR = ..
CORE_DIR = $(SRCS_DIR)/core
COMMAND_DIR = $(SRCS_DIR)/command
PARSER_DIR = $(SRCS_DIR)/parser
BOT_DIR = .
INCLUDE = -I $(CORE_DIR) -I $(COMMAND_DIR) -I $(PARSER_DIR)

SRCS = $(BOT_DIR)/main.cpp \
		$(BOT_DIR)/Bot.cpp \
		$(PARSER_DIR)/Parser.cpp


OBJS = ${SRCS:.cpp=.o}


$(NAME): $(OBJS)
		   $(C++) $(FLAGS) $(INCLUDE) $^ -o $@

%.o: %.cpp
	$(C++) $(FLAGS) $(INCLUDE) -c $< -o $@ 

all: $(NAME)

clean:
	$(RM) $(RM_FLAGS) $(OBJS)

fclean: clean
	$(RM) $(RM_FLAGS) $(NAME)

re: fclean all
	
.PHONY: all re clean fclean 
