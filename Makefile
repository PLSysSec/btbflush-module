TARGETS = btb-timing
CFLAGS += -I.

all: $(TARGETS)

test: $(TARGETS)
	for t in $(TARGETS); do \
		echo $$t:; \
		./$$t; \
	done


clean:
	rm -f $(TARGETS)