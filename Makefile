WARNING = -Wall -Wshadow -pedantic
ERROR = -Wvla
GCC = gcc -O3 -std=c99 $(WARNING) $(ERROR)
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --verbose


SRCS = main.c binary_search_tree.c huffman_coding.c huffman_file_IO.c 
OBJS = $(SRCS:%.c=%.o)

pa2: $(OBJS)
	$(GCC) $(TESTFLAGS) $(OBJS) -o pa2

.c.o:
	$(GCC) $(TESTFLAGS) -c $*.c

testmem: pa2
	$(VAL) ./pa2 examples/original/binary1 outputs/binary1.count outputs/binary1.tree outputs/binary1.code outputs/binary1.hbt

testAll: testEmpty test1 test2 test3 test4 test5

testEmpty: pa2
	./pa2 examples/original/empty outputs/empty.count outputs/empty.tree outputs/empty.code outputs/empty.hbt
	diff examples/count/empty.count outputs/empty.count
	diff examples/code/empty.code outputs/empty.code
	diff examples/tree/empty.tree outputs/empty.tree
	diff examples/compressed/empty.hbt outputs/empty.hbt

test1: pa2
	./pa2 examples/original/lorum outputs/lorum.count outputs/lorum.tree outputs/lorum.code outputs/lorum.hbt
	diff examples/count/lorum.count outputs/lorum.count
	diff examples/code/lorum.code outputs/lorum.code
	diff examples/tree/lorum.tree outputs/lorum.tree
	diff examples/compressed/lorum.hbt outputs/lorum.hbt

test2: pa2
	./pa2 examples/original/woods outputs/woods.count outputs/woods.tree outputs/woods.code outputs/woods.hbt
	diff examples/count/woods.count outputs/woods.count
	diff examples/code/woods.code outputs/woods.code
	diff examples/tree/woods.tree outputs/woods.tree
	diff examples/compressed/woods.hbt outputs/woods.hbt
test3: pa2
	./pa2 examples/original/stone outputs/stone.count outputs/stone.tree outputs/stone.code outputs/stone.hbt
	diff examples/count/stone.count outputs/stone.count
	diff examples/code/stone.code outputs/stone.code
	diff examples/tree/stone.tree outputs/stone.tree
	diff examples/compressed/stone.hbt outputs/stone.hbt

test4: pa2
	./pa2 examples/original/gophers outputs/gophers.count outputs/gophers.tree outputs/gophers.code outputs/gophers.hbt
	diff examples/count/gophers.count outputs/gophers.count
	diff examples/code/gophers.code outputs/gophers.code
	diff pa2_examples/tree/gophers.tree outputs/gophers.tree
	diff examples/compressed/gophers.hbt outputs/gophers.hbt

test5: pa2
	./pa2 examples/original/binary1 outputs/binary1.count outputs/binary1.tree outputs/binary1.code outputs/binary1.hbt
	diff examples/count/binary1.count outputs/binary1.count
	diff examples/code/binary1.code outputs/binary1.code
	diff examples/tree/binary1.tree outputs/binary1.tree
	diff examples/compressed/binary1.hbt outputs/binary1.hbt
clean:
	rm -f pa2 *.o output?
