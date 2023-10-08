# Compilation targets
PROG1 = prog1
PROG2 = prog2
PROG1_O1 = prog1_O1
PROG1_O2 = prog1_O2
PROG1_O3 = prog1_O3
PROG1_PROF = prog1_prof
PROG1_O3_PROF = prog1_O3_prof

# Object files
OBJS_PROG1 = prog1.o
OBJS_PROG2 = prog2.o

# Compiler and linker
CC = gcc
LD = gcc

# Compiling and linking flags
FLAGS = -Wall
LDFLAGS =
$(PROG1):         CFLAGS = $(FLAGS)
$(PROG2):         CFLAGS = $(FLAGS)
$(PROG1_O1):      CFLAGS = $(FLAGS) -O1
$(PROG1_O2):      CFLAGS = $(FLAGS) -O1
$(PROG1_O3):      CFLAGS = $(FLAGS) -O1
$(PROG1_PROF):    CFLAGS = $(FLAGS) -pg
$(PROG1_PROF):    LDFLAGS = -pg
$(PROG1_O3_PROF): CFLAGS = $(FLAGS) -O3 -pg
$(PROG1_O3_PROF): LDFLAGS = -pg


# Compiling rules
$(PROG1): $(OBJS_PROG1)

$(PROG2): $(OBJS_PROG2)

$(PROG1_O1): $(OBJS_PROG1)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG1_O2): $(OBJS_PROG1)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG1_O3): $(OBJS_PROG1)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG1_PROF): $(OBJS_PROG1)
	$(CC) $(LDFLAGS) $^ -o $@

$(PROG1_O3_PROF): $(OBJS_PROG1)
	$(CC) $(LDFLAGS) $^ -o $@

# Cleaning
clean :
	rm -f $(OBJS_PROG1) $(OBJS_PROG2)
