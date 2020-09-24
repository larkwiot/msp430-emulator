CXX = clang++
CFLAGS = -std=c++2a -Wall -Werror
DEBUG = -Og -g3 -ggdb -fsanitize=address -fno-omit-frame-pointer
OPT = -Ofast

OBJS = main.o instructions.o processor.o util.o state.o

debug: CFLAGS += $(DEBUG)
debug: executable

opt: CFLAGS += $(OPT)
opt: executable

all: executable

executable: $(OBJS)
	$(CXX) $^ $(CFLAGS) -o msp430

main.o: main.cc
	$(CXX) -c $^ $(CFLAGS)

state.o: state.cc
	$(CXX) -c $^ $(CFLAGS)

instructions.o: instructions.cc
	$(CXX) -c $^ $(CFLAGS)

processor.o: processor.cc
	$(CXX) -c $^ $(CFLAGS)

util.o: util.cc
	$(CXX) -c $^ $(CFLAGS)

clean:
	rm *.o
	rm msp430