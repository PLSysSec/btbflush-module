TARGETS = btb-timing btb-timing-with
CFLAGS += -I. -DITERATIONS=100000000 -DTRAINING_ITERATIONS=100000000 -O0

all: $(TARGETS)

%-with: %.c
	$(CC) $(CFLAGS) -DIBPB $< -o $@

test: $(TARGETS)
	for t in $(TARGETS); do \
		./$$t; \
	done

clean:
	rm -f $(TARGETS)