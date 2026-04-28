# In-Memory Document Search Index

A modular in-memory document indexing and retrieval engine written in C, developed for the Data Structures and Algorithms (SDA) course at UPB (Faculty of Automatic Control and Computers).

The engine organizes document records by keywords, allowing fast prefix/keyword searches using a Trie and ranked Top-K retrieval using a Binary Max-Heap.

## Architecture & Data Structures

The system is built around three core data structures:

1. **Trie (Prefix Tree)** (`trie.c`, `trie.h`):
   - Indexes 26-ary lowercase English keywords (`a-z`).
   - Each word node stores a reference list containing pointers to matching documents.
   - Lookups run in `O(L)` time, where `L` is the length of the keyword, independent of total document count.
   - Prunes empty branches when keywords are unlinked (`trie_remove`).

2. **Binary Max-Heap** (`heap.c`, `heap.h`):
   - Array-backed priority queue with automatic capacity doubling.
   - Takes a custom comparator function (`compare_files`: score descending, ID ascending).
   - Used during `TOPK` queries to retrieve the highest-scoring documents without sorting the entire dataset.

3. **Circular Doubly Linked List with Sentinel** (`list.c`, `list.h`):
   - Used for the main document registry and for keyword reference lists.
   - The sentinel node avoids edge-case handling for empty lists or single-element updates.

## Supported Commands

The program reads instructions from `indexare.in` and writes results to `indexare.out`:

- `ADD <id> <score> <n_keywords> <kw1> <kw2> ...`: Registers a document. Prints `OK` or `EXISTS`.
- `DEL <id>`: Deletes a document and its references. Prints `OK` or `NOT FOUND`.
- `ADDKW <id> <n_keywords> <kw1> ...`: Adds keywords to an existing document.
- `DELKW <id> <n_keywords> <kw1> ...`: Removes keywords from a document.
- `FIND <word>`: Returns matching document count and IDs sorted alphabetically, or `EMPTY`.
- `TOPK <word> <k>`: Returns the top `k` highest-scoring documents containing the word.
- `PRINT`: Traverses the Trie and prints all keywords with their document lists in alphabetical order.

## Memory Management

Every allocated block (`malloc`, `realloc`, `strdup`) is paired with an explicit cleanup function (`free_system`, `trie_free`, `heap_free`, `free_list`).

Tested with Valgrind:
```bash
valgrind --leak-check=full --show-leak-kinds=all ./search_index
```
Result: **0 errors, 0 memory leaks**.

## File Structure

```text
.
├── main.c                   # Entry point and file I/O handling
├── system.c / system.h      # System initialization and memory cleanup
├── commands.c / commands.h  # Command parsing and logic
├── trie.c / trie.h          # Generic Trie implementation
├── heap.c / heap.h          # Binary Max-Heap implementation
├── list.c / list.h          # Doubly linked list with sentinel
├── indexare.in.example      # Sample input file
├── Makefile
└── README.md
```

## How to Build and Run

**1. Compile:**
```bash
make build
```

**2. Run with sample input:**
```bash
cp indexare.in.example indexare.in
./search_index
cat indexare.out
```

**3. Clean build artifacts:**
```bash
make clean
```

## Author

- Rares Spinu
- GitHub: [Rareshh](https://github.com/Rareshh)
- Student at Faculty of Automatic Control and Computers, POLITEHNICA Bucharest
