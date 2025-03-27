# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sejjeong <sejjeong@student.42gyeongsan>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/19 16:33:48 by sejjeong          #+#    #+#              #
#    Updated: 2025/03/26 15:47:25 by sejjeong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
C++ = c++
FLAGS= -Wall -Wextra -Werror -std=c++98

RM = rm
RM_FLAGS = -rf

SRCS_DIR = .

SRCS = $(SRCS_DIR)/main.cpp \
		$(SRCS_DIR)/Server.cpp \
		$(SRCS_DIR)/Space.cpp \
		$(SRCS_DIR)/Channel.cpp \
		$(SRCS_DIR)/User.cpp \
		$(SRCS_DIR)/Lobby.cpp \
		$(SRCS_DIR)/Util.cpp


OBJS = ${SRCS:.cpp=.o}

$(NAME): $(OBJS)
		   $(C++) $(FLAGS) $^ -o $@

%.o: %.cpp
	$(C++) $(FLAGS) -c $< -o $@ 

all: $(NAME)

clean:
	$(RM) $(RM_FLAGS) $(OBJS)

fclean: clean
	$(RM) $(RM_FLAGS) $(NAME)

re: fclean all
	
.PHONY: all re clean fclean 
