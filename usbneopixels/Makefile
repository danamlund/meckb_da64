all: rainbow status

%: %.c
	gcc -Wall -lm -O2 -pipe -std=c99 $< -o $@


clean:
	rm -f rainbow status
