# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhoffsch <rhoffsch@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/15 17:38:19 by rhoffsch          #+#    #+#              #
#    Updated: 2018/09/18 02:17:04 by rhoffsch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	SimpleGL
CC				=	clang++ -std=c++11
CFLAGS			=	-Wall -Wextra -Werror

INCLUDE			=	-I include \
					-I /Users/rhoffsch/.brew/Cellar/glfw/3.2.1/include \
					-I /Users/rhoffsch/.brew/Cellar/glew/2.1.0/include \
					-I /System/Library/Frameworks/OpenGL.framework/Versions/A/Headers/
					# -I /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers
					# -I /Developer/NVIDIA/CUDA-9.0/extras/CUPTI/include #Mac42

GLFW			=	/Users/rhoffsch/.brew/Cellar/glfw/3.2.1/lib/libglfw.dylib
GLEW			=	/Users/rhoffsch/.brew/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib

FRAMEWORKS		=	-framework OpenGL #-framework Cocoa
CC_NEEDS		=	$(INCLUDE) $(FRAMEWORKS) $(GLFW) $(GLEW)

SRC_FILE		=	main.cpp \
					blueprint.cpp \
					cam.cpp \
					glfw.cpp \
					math.cpp \
					misc.cpp \
					obj3d.cpp \
					obj3dBP.cpp \
					obj3dPG.cpp \
					program.cpp \
					skybox.cpp \
					skyboxPG.cpp \
					texture.cpp

HDR_FILE		=	simplegl.h \

SRC_DIR			=	src
OBJ_DIR			=	obj
HDR_DIR			=	include
SRC				=	$(addprefix $(SRC_DIR)/, $(SRC_FILE))
OBJ				=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILE))
HDR				=	$(addprefix $(HDR_DIR)/, $(HDR_FILE))

DEPENDS			=	$(OBJ:.o=.d)

.PHONY: all compile clean fclean pclean re

all: compile

compile:
	@mkdir -p $(OBJ_DIR)
	@$(MAKE) $(NAME)

$(NAME): $(SRC) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(CC_NEEDS)
	@# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS) $(FRAMEWORKS) $(GLFW) $(GLEW)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR) $(HDR_DIR)/%.hpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

pclean: fclean

re: fclean all
