SHELL   = /bin/sh
CC      = g++
INSTALL = install
EXEC    = battleships

# Sources
SRCS_0 = gun.cpp screen.cpp bot.cpp outstream.cpp state.cpp output.cpp
SRCS = $(SRCS_0) main.cpp
HDRS = $(SRCS_0:.cpp=.h)
OBJS = $(SRCS:.cpp=.o)

EXECS = $(EXEC)

CFLAGS += -Wall -O2 -g
LDLIBS += -lm -lncurses


.PHONY: all clean

# Build
all: $(EXECS)

clean:
	-rm -f $(OBJS) $(EXECS)

%.o: %.cpp $(HDRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

$(EXEC): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(EXEC)
