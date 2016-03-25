CC = g++ -std=c++11
SRCS = ./*.cpp
OBS = ./*.o

single_cycle: SRC
	$(CC) -o $@ $(OBS)

SRC: $(SRCS)
	$(CC) -c $(SRCS)

clean: $(OBS)
	rm -f $(OBS)  
