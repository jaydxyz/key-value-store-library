#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_KEY_LENGTH 256
#define INITIAL_CAPACITY 16

typedef struct {
    char key[MAX_KEY_LENGTH];
    void* value;
} Entry;

typedef struct {
    Entry* entries;
    int capacity;
    int size;
    pthread_mutex_t lock;
} HashTable;

HashTable* create_hash_table() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->entries = (Entry*)calloc(INITIAL_CAPACITY, sizeof(Entry));
    table->capacity = INITIAL_CAPACITY;
    table->size = 0;
    pthread_mutex_init(&table->lock, NULL);
    return table;
}

void destroy_hash_table(HashTable* table) {
    free(table->entries);
    pthread_mutex_destroy(&table->lock);
    free(table);
}

unsigned int hash(const char* key) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

void resize_hash_table(HashTable* table) {
    int new_capacity = table->capacity * 2;
    Entry* new_entries = (Entry*)calloc(new_capacity, sizeof(Entry));

    for (int i = 0; i < table->capacity; i++) {
        if (table->entries[i].key[0] != '\0') {
            unsigned int index = hash(table->entries[i].key) % new_capacity;
            while (new_entries[index].key[0] != '\0')
                index = (index + 1) % new_capacity;
            new_entries[index] = table->entries[i];
        }
    }

    free(table->entries);
    table->entries = new_entries;
    table->capacity = new_capacity;
}

void put(HashTable* table, const char* key, void* value) {
    pthread_mutex_lock(&table->lock);

    if (table->size >= table->capacity * 0.75)
        resize_hash_table(table);

    unsigned int index = hash(key) % table->capacity;
    while (table->entries[index].key[0] != '\0' &&
           strcmp(table->entries[index].key, key) != 0)
        index = (index + 1) % table->capacity;

    strcpy(table->entries[index].key, key);
    table->entries[index].value = value;
    table->size++;

    pthread_mutex_unlock(&table->lock);
}

void* get(HashTable* table, const char* key) {
    pthread_mutex_lock(&table->lock);

    unsigned int index = hash(key) % table->capacity;
    while (table->entries[index].key[0] != '\0') {
        if (strcmp(table->entries[index].key, key) == 0) {
            void* value = table->entries[index].value;
            pthread_mutex_unlock(&table->lock);
            return value;
        }
        index = (index + 1) % table->capacity;
    }

    pthread_mutex_unlock(&table->lock);
    return NULL;
}

void remove_entry(HashTable* table, const char* key) {
    pthread_mutex_lock(&table->lock);

    unsigned int index = hash(key) % table->capacity;
    while (table->entries[index].key[0] != '\0') {
        if (strcmp(table->entries[index].key, key) == 0) {
            table->entries[index].key[0] = '\0';
            table->size--;
            break;
        }
        index = (index + 1) % table->capacity;
    }

    pthread_mutex_unlock(&table->lock);
}

// Example usage
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
