CC      := gcc
CFLAGS	:= -std=c11 -ggdb -Os -Wall -Wextra -Wpedantic
LDFLAGS	:= -Llib/raylib -l:libraylib.a -lm

SRC     := src
EXE     := raytris

all: $(EXE) 

$(EXE): $(wildcard $(SRC)/*.c)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
