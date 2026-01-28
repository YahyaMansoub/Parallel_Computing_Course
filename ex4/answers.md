## Exercise 4 — Memory Management and Debugging with Valgrind

### Objective
Use Valgrind (Memcheck) to detect memory leaks in a C program that allocates, initializes, prints, and duplicates an array, then fix the leaks and verify the fix.

---

## 1) Program behavior (what the code does)

### `allocate_array(size)`
- Allocates `size * sizeof(int)` bytes on the heap using `malloc`.
- Checks if `malloc` returned `NULL` (allocation failure). If so, prints an error and exits.
- Returns a pointer (`int*`) to the allocated array.

### `initialize_array(arr, size)`
- Fills the allocated array with deterministic values:
  - `arr[i] = i * 10`
- For `SIZE = 5`, the array becomes: `0, 10, 20, 30, 40`.

### `print_array(arr, size)`
- Prints all array elements.

### `duplicate_array(arr, size)`
- Allocates a new heap array (`copy`) of the same size.
- Copies the bytes from `arr` into `copy` using `memcpy`.
- Returns the new pointer (`copy`), so now we have **two separate heap allocations**:
  - `array`
  - `array_copy`

### `free_memory(arr)`
- Should release heap memory to avoid leaks (`free(arr)`).

### `main()`
- Allocates `array`, initializes, prints.
- Creates `array_copy` via `duplicate_array`, prints it.
- Frees both arrays to avoid leaks.
- Exits cleanly.

---

## 2) What Valgrind checks
Valgrind Memcheck tracks heap allocations (`malloc`) and deallocations (`free`), and reports:
- **definitely lost**: leaked memory with no remaining pointers
- **indirectly lost**: leaked memory referenced only by leaked blocks
- **still reachable**: memory not freed but still pointed to at exit

---

## 3) Fix applied
The original exercise intentionally contained a leak (missing `free`).  
To fix:
- Implemented `free_memory(int *arr)` as `free(arr);`
- Called `free_memory(array_copy);` as well as `free_memory(array);`

This ensures every `malloc` has a matching `free`.

---

## 4) Compilation and execution commands

### Compile with debug symbols
```bash
gcc -g -o memory_debug memory_debug.c
