# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rhoffsch <rhoffsch@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/15 17:38:19 by rhoffsch          #+#    #+#              #
#    Updated: 2019/11/15 10:45:55 by rhoffsch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	SimpleGL
CC				=	g++ -std=c++11
CFLAGS			=	-Wall -Wextra -Werror -MMD -g #-Wpadded
TEST_FLAGS		=	-DUNIT_TESTS=false


INCLUDE			=	-I ${GLEW_DIR}/include \
					-I ${GLFW_DIR}/include \
					-I include
					#-I /System/Library/Frameworks/OpenGL.framework/Versions/A/Headers/
					# -I /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers
					# -I /Developer/NVIDIA/CUDA-9.0/extras/CUPTI/include #Mac42

GLEW			=	${GLEW_DIR}/lib/libGLEW.dylib
GLFW			=	${GLFW_DIR}/lib/libglfw.dylib

FRAMEWORKS		=	-framework OpenGL #-framework Cocoa
CC_NEEDS		=	$(FRAMEWORKS) $(GLFW) $(GLEW)

SRC_FILE		=	main.cpp \
					blueprint.cpp \
					cam.cpp \
					glfw.cpp \
					math.cpp \
					misc.cpp \
					properties.cpp \
					object.cpp \
					obj3d.cpp \
					obj3dBP.cpp \
					obj3dPG.cpp \
					program.cpp \
					skybox.cpp \
					skyboxPG.cpp \
					texture.cpp \
					behavior.cpp \
					behaviormanaged.cpp \
					transformBH.cpp \
					fps.cpp \
					gamemanager.cpp

HDR_FILE		=	simplegl.h

SRC_DIR			=	src
OBJ_DIR			=	obj
HDR_DIR			=	include
SRC				=	$(addprefix $(SRC_DIR)/, $(SRC_FILE))
OBJ				=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILE))
DPD				=	$(patsubst %.cpp, $(OBJ_DIR)/%.d, $(SRC_FILE))
HDR				=	$(addprefix $(HDR_DIR)/, $(HDR_FILE))

DEPENDS			=	$(OBJ:.o=.d)
UT_MAKEFILE		=	Makefile.ut

.PHONY: all clean fclean pclean re tests run

release:
	@make -j all

all: $(NAME)
	@echo $(NAME) > .gitignore

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(CC_NEEDS)
	@# $(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS) $(FRAMEWORKS) $(GLFW) $(GLEW)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile | $(OBJ_DIR)
	$(CC) $(TEST_FLAGS) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

pclean: fclean
	@$(MAKE) pclean -f $(UT_MAKEFILE)

re:
	@make fclean
	@make release

tests:
	make run -f $(UT_MAKEFILE)

run: all
	@echo ""
	./$(NAME)
	@echo ""
	@echo "exitcode: $$?"

-include $(DPD)
