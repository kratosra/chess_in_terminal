CC ?= g++
CFLAGS ?= -Wall -Wextra -Werror -std=c++11

LDLIBS?=-lm -ltps -lSDL2 -lSDL2_ttf -lstdc++

INCLUDE_PATH = ./include

TARGET   = echecs

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin


SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

$(BINDIR)/$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_PATH)


.PHONY: all tests clean cov

all:
	make 
	make doc
	make tests

doc:
	@doxygen > /dev/null 2>&1
	@echo "Documentaion générée !"
	@xdg-open html/index.html
	
tests:
	@make
	./test-level.sh 1 ./bin/echecs && ./test-level.sh 2 ./bin/echecs && ./test-level.sh 3 ./bin/echecs && ./test-level.sh 4 ./bin/echecs
		
	

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/*.gcda
	rm -f $(OBJDIR)/*.gcno
	rm -f $(BINDIR)/$(TARGET)
	rm -r html
	
	
	
