
# ---------------SET THESE VARIABLES-----------------
O_FOLDER = obj/
SRC_FOLDER = ./
CFLAGS = $(shell pkg-config --cflags sdl2) -I./
LIBS = $(shell pkg-config --libs sdl2)
OUT_NAME = SDL_FontCache.a
# ---------------------------------------------------

CC = clang $(CFLAGS)

nullstring =
space = $(nullstring) #End

SRC_FILES = $(shell find $(SRC_FOLDER) -type f -name '*.c')
SRC_NAMES = $(patsubst $(SRC_FOLDER)%.c,%, $(SRC_FILES))


define MAKE_CPP
$(TARGET):				$(SRC_FOLDER)$(PRE).c
						$(CC) -c $(SRC_FOLDER)$(PRE).c -o $(SRC_FOLDER)$(PRE).o
						$(eval x=$(O_FOLDER)$(PRE).o)
						$(eval x1=$(subst /,$(space),$(x)))
						$(eval x=$(filter-out $(lastword $(x1)), $(x1)))
						$(eval x1=$(subst $(space),/,$(x)))
						mkdir -p $(x1)
						mv -f $(SRC_FOLDER)$(PRE).o $(x1)
endef

O_PATHS = $(foreach o_file, $(SRC_NAMES), $(O_FOLDER)$(o_file).o)
$(OUT_NAME):			$(O_PATHS)
						ar -crs $(OUT_NAME) $(O_PATHS)

clean:					$(O_FOLDER)
						rm -rf $(O_FOLDER)

PRE = $(name)
TARGET = $(O_FOLDER)$(name).o

$(foreach name, $(SRC_NAMES), \
	$(eval $(MAKE_CPP)))

