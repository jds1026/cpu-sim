IDIR =./include
COMPILE=g++-10
CPPFLAGS=-I$(IDIR)

ODIR=obj
SDIR=./src

LIBS=-lm

_DEPS = clock.h cpu.h process_queue.h simulation_statistics.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = clock.cpp cpu.cpp main.cpp process_queue.cpp
OBJ = $(patsubst %,$(SDIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(COMPILE) -c -o $@ $< $(CPPFLAGS)

cpu-sim: $(OBJ)
	$(COMPILE) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f cpu-sim
