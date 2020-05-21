TARGETS = btb-timing btb-timing-with
ITERATIONS=10000000
CFLAGS += -I. -DITERATIONS=$(ITERATIONS) -DTRAINING_ITERATIONS=10000000 -O1
NUM_JUMPS=1 10 20 50 100

all: $(TARGETS)

%-with: %.c
	$(CC) $(CFLAGS) -DIBPB $< -o $@

test: $(TARGETS)
	echo  "$(ITERATIONS) \t\t jumps \t sum \t\t avg"
	for j in $(NUM_JUMPS); do \
		for t in $(TARGETS); do \
			./$$t $$j; \
		done \
	done

clean:
	rm -f $(TARGETS)