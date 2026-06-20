CC = gcc
CFLAGS = -Wall -Wextra -g

PROGRAM = project
HASH_TEST = run_test_hashtable
LIST_TEST = run_test_list
LRU_TEST = run_test_lru

PROGRAM_SRC = example.c lru_cache.c hashtable.c list.c
HASH_SRC = test_hashtable.c hashtable.c
LIST_SRC = test_list.c list.c
LRU_SRC = test_lru.c lru_cache.c hashtable.c list.c

all: $(PROGRAM) $(HASH_TEST) $(LIST_TEST) $(LRU_TEST)

$(PROGRAM): $(PROGRAM_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(HASH_TEST): $(HASH_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(LIST_TEST): $(LIST_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(LRU_TEST): $(LRU_SRC)
	$(CC) $(CFLAGS) -o $@ $^

run_tests: $(HASH_TEST) $(LIST_TEST) $(LRU_TEST)
	./$(HASH_TEST)
	./$(LIST_TEST)
	./$(LRU_TEST)

test_hash: $(HASH_TEST)
	./$(HASH_TEST)

test_list: $(LIST_TEST)
	./$(LIST_TEST)

test_lru: $(LRU_TEST)
	./$(LRU_TEST)

valgrind_hash: $(HASH_TEST)
	valgrind --leak-check=full --show-leak-kinds=all ./$(HASH_TEST)

valgrind_list: $(LIST_TEST)
	valgrind --leak-check=full --show-leak-kinds=all ./$(LIST_TEST)

valgrind_lru: $(LRU_TEST)
	valgrind --leak-check=full --show-leak-kinds=all ./$(LRU_TEST)

valgrind: $(PROGRAM)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROGRAM)

clean:
	rm -f $(PROGRAM) $(HASH_TEST) $(LIST_TEST) $(LRU_TEST) *.o

.PHONY: all run_tests test_hash test_list test_lru valgrind valgrind_hash valgrind_list valgrind_lru clean
