// ============================================================
// Хеш-таблица с цепочечным разрешением коллизий
// ============================================================
// Ключи: целочисленные (int)
// Значения: указатели на void (void*) — можно хранить любые данные
// Размер таблицы всегда округляется до простого числа
// Автоматическое рехеширование при загрузке > 0.75
//
// ВНИМАНИЕ: данная версия НЕ освобождает value при удалении узла.
// Это сделано для LRU-кеша, где памятью управляет список.
// ============================================================

#include <stdio.h>
#include <stdlib.h>

// Узел цепочки хеш-таблицы
struct HashNode_t {
    int key;                    // ключ
    void* value;                // значение (указатель на данные)
    struct HashNode_t* next;    // следующий узел в цепочке
};

// Структура хеш-таблицы
struct HashTable_t {
    struct HashNode_t** buckets;  // массив корзин
    int size;                     // размер таблицы
    int count;                    // количество элементов
    float load_factor;            // коэффициент загрузки
};

// Проверка на простое число
static int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Следующее простое число
static int next_prime(int n) {
    while (!is_prime(n)) n++;
    return n;
}

// Хеш-функция для целочисленных ключей
static int hash_int(int key, int table_size) {
    return (key < 0 ? -key : key) % table_size;
}

// Создание узла хеш-таблицы
static struct HashNode_t* create_hash_node(int key, void* value) {
    struct HashNode_t* node = (struct HashNode_t*)malloc(sizeof(struct HashNode_t));
    if (!node) return NULL;
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}

// Освобождение узла (НЕ освобождает value)
static void free_hash_node(struct HashNode_t* node) {
    if (!node) return;
    free(node);
}

// Создание хеш-таблицы
struct HashTable_t* hash_table_create(int size) {
    if (size <= 0) {
        fprintf(stderr, "Error: size must be > 0\n");
        return NULL;
    }

    int prime_size = next_prime(size);
    struct HashTable_t* ht = (struct HashTable_t*)malloc(sizeof(struct HashTable_t));
    if (!ht) {
        fprintf(stderr, "Error: failed to allocate memory for hash table\n");
        return NULL;
    }

    ht->buckets = (struct HashNode_t**)calloc((size_t)prime_size, sizeof(struct HashNode_t*));
    if (!ht->buckets) {
        fprintf(stderr, "Error: failed to allocate memory for buckets\n");
        free(ht);
        return NULL;
    }

    ht->size = prime_size;
    ht->count = 0;
    ht->load_factor = 0.75f;

    return ht;
}

// Освобождение хеш-таблицы (НЕ освобождает value)
void hash_table_free(struct HashTable_t* ht) {
    if (!ht) return;

    for (int i = 0; i < ht->size; i++) {
        struct HashNode_t* current = ht->buckets[i];
        while (current) {
            struct HashNode_t* next = current->next;
            free_hash_node(current);
            current = next;
        }
    }

    free(ht->buckets);
    free(ht);
}

// Рехеширование (увеличение размера вдвое)
void hash_table_resize(struct HashTable_t* ht) {
    if (!ht) return;

    int new_size = next_prime(ht->size * 2);

    struct HashNode_t** old_buckets = ht->buckets;
    int old_size = ht->size;

    struct HashNode_t** new_buckets = (struct HashNode_t**)calloc((size_t)new_size, sizeof(struct HashNode_t*));
    if (!new_buckets) {
        fprintf(stderr, "Error: failed to allocate memory for resize\n");
        return;
    }

    for (int i = 0; i < old_size; i++) {
        struct HashNode_t* current = old_buckets[i];
        while (current) {
            struct HashNode_t* next = current->next;

            int new_index = hash_int(current->key, new_size);

            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;

            current = next;
        }
    }

    ht->buckets = new_buckets;
    ht->size = new_size;

    free(old_buckets);
}

// Вставка или обновление значения по ключу
// При обновлении старое значение освобождается
void hash_table_put(struct HashTable_t* ht, int key, void* value) {
    if (!ht) {
        fprintf(stderr, "Error: hash table is NULL\n");
        return;
    }
    if (!value) {
        fprintf(stderr, "Warning: attempt to insert NULL for key %d\n", key);
        return;
    }

    // Автоматическое рехеширование при переполнении
    if ((float)ht->count / (float)ht->size > ht->load_factor) {
        hash_table_resize(ht);
    }

    int index = hash_int(key, ht->size);
    struct HashNode_t* current = ht->buckets[index];

    // Поиск ключа в цепочке
    while (current) {
        if (current->key == key) {
            // Ключ уже есть — обновляем значение
            free(current->value);
            current->value = value;
            return;
        }
        current = current->next;
    }

    // Ключа нет — создаём новый узел и добавляем в начало цепочки
    struct HashNode_t* new_node = create_hash_node(key, value);
    if (!new_node) {
        fprintf(stderr, "Error: failed to allocate memory for key %d\n", key);
        free(value);
        return;
    }

    new_node->next = ht->buckets[index];
    ht->buckets[index] = new_node;
    ht->count++;
}

// Поиск значения по ключу
void* hash_table_get(struct HashTable_t* ht, int key) {
    if (!ht) return NULL;

    int index = hash_int(key, ht->size);
    struct HashNode_t* current = ht->buckets[index];

    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

// Удаление элемента по ключу
// НЕ освобождает value
int hash_table_remove(struct HashTable_t* ht, int key) {
    if (!ht) return 0;

    int index = hash_int(key, ht->size);
    struct HashNode_t* current = ht->buckets[index];
    struct HashNode_t* prev = NULL;

    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                ht->buckets[index] = current->next;
            }
            free_hash_node(current);
            ht->count--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

// Очистка всех элементов (НЕ освобождает value)
void hash_table_clear(struct HashTable_t* ht) {
    if (!ht) return;

    for (int i = 0; i < ht->size; i++) {
        struct HashNode_t* current = ht->buckets[i];
        while (current) {
            struct HashNode_t* next = current->next;
            free_hash_node(current);
            current = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
}

// Получение количества элементов
int hash_table_count(struct HashTable_t* ht) {
    return ht ? ht->count : 0;
}

// Получение размера таблицы
int hash_table_size(struct HashTable_t* ht) {
    return ht ? ht->size : 0;
}

// Отладочный вывод содержимого
void hash_table_print(struct HashTable_t* ht) {
    if (!ht) {
        printf("NULL\n");
        return;
    }

    float load = (float)ht->count / (float)ht->size;

    printf("size=%d, count=%d, load=%.2f\n", ht->size, ht->count, load);

    for (int i = 0; i < ht->size; i++) {
        if (ht->buckets[i]) {
            printf("[%d]: ", i);
            struct HashNode_t* current = ht->buckets[i];
            while (current) {
                printf("%d ", current->key);
                if (current->next) printf("-> ");
                current = current->next;
            }
            printf("\n");
        }
    }
}
