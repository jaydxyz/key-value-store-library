# In-Memory Key-Value Store Library

This project is a high-performance, multi-threaded, in-memory key-value store library implemented in C. It provides a simple and efficient way to store and retrieve key-value pairs in memory.

## Features

- Thread-safe hash table implementation for efficient key-value storage and retrieval.
- Support for multiple concurrent read and write operations using fine-grained locking.
- Efficient memory management, including dynamic resizing of the hash table and handling of hash collisions.
- Support for storing string values (can be extended to support other data types).
- Clean and well-documented API for easy integration into other projects.

## Usage

To use the key-value store library in your project, include the `kvstore.c` file in your source code and compile it along with your project files.

Here's an example of how to use the library:

```c
#include "kvstore.c"

int main() {
    HashTable* table = create_hash_table();

    put(table, "key1", "value1");
    put(table, "key2", "value2");
    put(table, "key3", "value3");

    printf("Value for key1: %s\n", (char*)get(table, "key1"));
    printf("Value for key2: %s\n", (char*)get(table, "key2"));
    printf("Value for key3: %s\n", (char*)get(table, "key3"));

    remove_entry(table, "key2");
    printf("Value for key2 after removal: %s\n", (char*)get(table, "key2"));

    destroy_hash_table(table);

    return 0;
}
```

## API Functions

- `HashTable* create_hash_table()`: Creates a new hash table and returns a pointer to it.
- `void destroy_hash_table(HashTable* table)`: Destroys the hash table and frees the allocated memory.
- `void put(HashTable* table, const char* key, void* value)`: Inserts a key-value pair into the hash table.
- `void* get(HashTable* table, const char* key)`: Retrieves the value associated with a given key from the hash table.
- `void remove_entry(HashTable* table, const char* key)`: Removes a key-value pair from the hash table.

## Performance

The key-value store library is designed for high performance and scalability. It uses a hash table data structure with linear probing for collision resolution. The hash table dynamically resizes when the load factor exceeds a threshold to maintain efficient operations.

The library supports multiple concurrent read and write operations using fine-grained locking with pthread mutexes. This allows for efficient parallel access to the key-value store.

## Future Enhancements

- Support for additional data types as values (e.g., integers, floats, custom structs).
- Comprehensive unit tests to ensure correctness and thread safety.
- Performance benchmarks to showcase the efficiency of the implementation.
- Improved error handling and resource management.
- Optimization of the hashing function and collision resolution mechanism.

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request on the project's GitHub repository.

## License

This project is licensed under the [MIT License](LICENSE).
