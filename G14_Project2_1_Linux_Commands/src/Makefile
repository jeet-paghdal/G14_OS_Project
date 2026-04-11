

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11


TARGETS = myshell custom_echo


.PHONY: all
all: $(TARGETS)


myshell: shell.c
	$(CC) $(CFLAGS) -o myshell shell.c


custom_echo: custom_echo.c
	$(CC) $(CFLAGS) -o custom_echo custom_echo.c


.PHONY: run
run: all
	./myshell


.PHONY: clean
clean:
	rm -f $(TARGETS)


.PHONY: help
help:
	@echo "Targets:"
	@echo "  all          Build all binaries (default)"
	@echo "  myshell      Build only the shell"
	@echo "  custom_echo  Build only custom_echo"
	@echo "  run          Build and launch the shell"
	@echo "  clean        Remove all compiled binaries"