CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11

TARGETS = myshell custom_echo custom_wc custom_ls custom_cat custom_cp custom_mv custom_rm custom_grep

.PHONY: all
all: $(TARGETS)

myshell: shell.c
	$(CC) $(CFLAGS) -o myshell shell.c

custom_echo: custom_echo.c
	$(CC) $(CFLAGS) -o custom_echo custom_echo.c

custom_wc: custom_wc.c
	$(CC) $(CFLAGS) -o custom_wc custom_wc.c

# Target to compile custom_cat
custom_cat: custom_cat.c
	$(CC) $(CFLAGS) -o custom_cat custom_cat.c

# Target to compile custom_ls
custom_ls: custom_ls.c
	$(CC) $(CFLAGS) -o custom_ls custom_ls.c

custom_grep: custom_grep.c
	$(CC) $(CFLAGS) -o custom_grep custom_grep.c

custom_cp: custom_cp.c
	$(CC) $(CFLAGS) -o custom_cp custom_cp.c

custom_mv: custom_mv.c
	$(CC) $(CFLAGS) -o custom_mv custom_mv.c

custom_rm: custom_rm.c
	$(CC) $(CFLAGS) -o custom_rm custom_rm.c

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
	@echo "  custom_grep  Build only custom_grep"
	@echo "  custom_cp    Build only custom_cp"
	@echo "  custom_mv    Build only custom_mv"
	@echo "  custom_rm    Build only custom_rm"
	@echo "  run          Build and launch the shell"
	@echo "  clean        Remove all compiled binaries"