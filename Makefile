TARGETS = btb-timing btb-timing-with
CFLAGS += -I.

all: $(TARGETS)

%-with: %.c
	$(CC) $(CFLAGS) -DIBPB $< -o $@

test: $(TARGETS)
	for t in $(TARGETS); do \
		./$$t; \
	done

clean:
	rm -f $(TARGETS)