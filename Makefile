SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
CC = gcc

CFLAGS = -O3 -Wall -Wextra -I$(INCDIR)

TARGET = $(BINDIR)/laGrange

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
