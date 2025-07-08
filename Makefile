CC      := gcc
CFLAGS	:= -std=c11 -ggdb -Wall -Wextra -Wpedantic
LDFLAGS	:= -Lraylib -l:libraylib.a -lm -lX11 -lpthread -lGL -lrt

SRC     := src
EXE     := raytris

all: $(EXE) 

$(EXE): $(wildcard $(SRC)/*.c)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
