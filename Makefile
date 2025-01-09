C := gcc
EXE := m
FLAGS := -Wall -std=c99

poli: poli.c env.c env.h
	$(C) $(FLAGS) $^ -o $(EXE)

valu: valu.c env.c env.h
	$(C) $(FLAGS) $^ -o $(EXE)

clean:
	rm $(EXE)
