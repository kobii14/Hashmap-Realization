#include "test_suite.h"
#include "hash_funcs.h"
#include "test_pairs.h"

#define CAPACITY 16
#define LOW_SIZE 4
#define HIGH_CAPACITY 64
#define MID_SIZE 35
#define ASCII_A 97
#define SIZE_10 10
#define SIZE_7 7
#define SMALL_LOAD 0.125
#define LARGE_LOAD 0.75
#define SIZE_8 8
#define KEYS {'3', 'R', '8', 'W', '9', '(', ')'}
#define SIZE_5 5
#define SIZE_4 4
#define SIZE_3 3
#define SIZE_9 9
#define VALUES {0, 1, 2, 3, 4, 5, 6}
//#define INSERT_TEST "passed insert tests\n"
//#define ERASE_TEST "passed erase tests\n"
//#define AT_TEST "passed hash_map_at tests\n"
//#define LOAD_TEST "passed load tests\n"
//#define APPLY_TEST "passed apply test\n"


/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit
 * code 1.
 */
void test_hash_map_insert(void)
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert(hash_map);
  assert(hash_map->capacity == (size_t) CAPACITY);
  assert(hash_map->size == (size_t) 0);
  pair *pairs_array[LOW_SIZE];
  pair *new_pair;
  int i = 0;
  for (i = 0; i < LOW_SIZE; i++)
    {
      char char_key = (char) (i + ASCII_A);
      int int_value = i;
      new_pair = pair_alloc (&char_key, &int_value, char_key_cpy,
                             int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      assert(new_pair);
      pairs_array[i] = new_pair;
    }
  assert(hashmap_insert (NULL, pairs_array[0]) == 0);
  assert(hashmap_insert (hash_map, NULL) == 0);
  assert(hashmap_insert (hash_map, pairs_array[SIZE_3]) == 1);
  assert(hashmap_insert (hash_map, pairs_array[SIZE_3]) == 0);
  assert(hash_map->capacity == (size_t) CAPACITY);
  assert(hash_map->size == (size_t) 1);
  for (i = 0; i < LOW_SIZE; i++)
    {
      pair_free ((void **) &pairs_array[i]);
    }
  hashmap_free (&hash_map);


  pair *pairs_array_2[MID_SIZE];
  pair *new_pair_2;
  for (i = 0; i < MID_SIZE; i++)
    {
      char char_key = (char) (i + ASCII_A);
      int int_value = i;
      new_pair_2 = pair_alloc (&char_key, &int_value, char_key_cpy,
                               int_value_cpy,
                               char_key_cmp, int_value_cmp, char_key_free,
                               int_value_free);
      assert(new_pair_2);
      pairs_array_2[i] = new_pair_2;
    }
  hashmap *hash_map_2 = hashmap_alloc (hash_char);
  assert(hash_map_2);
  for (i = 0; i < MID_SIZE; i++)
    {
      assert(hashmap_insert (hash_map_2, pairs_array_2[i]) == 1);
    }
  assert(hash_map_2->size == (size_t) MID_SIZE);
  assert(hash_map_2->capacity == (size_t) HIGH_CAPACITY);
  for (i = 0; i < MID_SIZE; i++)
    {
      pair_free ((void **) &pairs_array_2[i]);
    }
  hashmap_free (&hash_map_2);
  //printf(INSERT_TEST);
}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void)
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert(hash_map);
  pair *pairs_array[LOW_SIZE];
  pair *new_pair;
  int i = 0;
  for (i = 0; i < LOW_SIZE; i++)
    {
      char char_key = (char) (i + ASCII_A);
      int int_value = i;
      new_pair = pair_alloc (&char_key, &int_value, char_key_cpy,
                             int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      assert(new_pair);
      pairs_array[i] = new_pair;
    }
  assert(hashmap_at(NULL, pairs_array[0]->key) == 0);
  assert(hashmap_at(hash_map, NULL) == 0);
  for (i = 0; i < SIZE_3; i++)
    {
      assert(hashmap_insert (hash_map, pairs_array[i]) == 1);
      assert(hashmap_at(hash_map, pairs_array[i]->key) != NULL);
    }
  assert(hashmap_at(hash_map, pairs_array[SIZE_3]->key) == NULL);
  for (i = 0; i < LOW_SIZE; i++)
    {
      pair_free ((void **) &pairs_array[i]);
    }
  hashmap_free (&hash_map);
  //printf(AT_TEST);
}


/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void)
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert(hash_map);
  assert(hash_map->capacity == (size_t) CAPACITY);
  assert(hash_map->size == (size_t) 0);
  pair *pairs_array[SIZE_10];
  pair *new_pair;
  int i = 0;
  for (i = 0; i < SIZE_10; i++)
    {
      char char_key = (char) (i + ASCII_A);
      int int_value = i;
      new_pair = pair_alloc (&char_key, &int_value, char_key_cpy,
                             int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      assert(new_pair);
      pairs_array[i] = new_pair;
    }
  for (i = 0; i < SIZE_9; i++)
    {
      assert(hashmap_insert (hash_map, pairs_array[i]) == 1);
    }
  assert(hash_map->capacity == (size_t) CAPACITY);
  assert(hash_map->size == (size_t) SIZE_9);
  assert(hashmap_erase(NULL, pairs_array[0]->key) == 0);
  assert(hashmap_erase(hash_map, NULL) == 0);
  assert(hashmap_erase(hash_map, pairs_array[SIZE_9]->key) == 0);
  assert(hashmap_erase(hash_map, pairs_array[SIZE_8]->key) == 1);
  assert(hashmap_erase(hash_map, pairs_array[SIZE_8]->key) == 0);
  assert(hashmap_erase(hash_map, pairs_array[SIZE_7]->key) == 1);
  for (i = 0; i < SIZE_10; i++)
    {
      pair_free ((void **) &pairs_array[i]);
    }
  hashmap_free (&hash_map);

  hashmap *hash_map_2 = hashmap_alloc (hash_char);
  assert(hash_map_2);
  pair *pairs_array_2[SIZE_5];
  pair *new_pair_2;
  for (i = 0; i < LOW_SIZE; i++)
    {
      char char_key = (char) (i + ASCII_A);
      int int_value = i;
      new_pair_2 = pair_alloc (&char_key, &int_value, char_key_cpy,
                               int_value_cpy,
                               char_key_cmp, int_value_cmp, char_key_free,
                               int_value_free);
      assert(new_pair_2);
      pairs_array_2[i] = new_pair_2;
    }
  for (i = 0; i < LOW_SIZE; i++)
    {
      assert(hashmap_insert (hash_map_2, pairs_array_2[i]) == 1);
    }
  assert(hash_map_2->size == (size_t) LOW_SIZE);
  assert(hash_map_2->capacity == (size_t) CAPACITY);
  assert(hashmap_erase(hash_map_2, pairs_array_2[SIZE_3]->key) == 1);
  assert(hash_map_2->size == (size_t) SIZE_3);
  assert(hash_map_2->capacity == (size_t) SIZE_8);
  char char_key = (char) (SIZE_5 + ASCII_A);
  int int_value = SIZE_5;
  new_pair_2 = pair_alloc (&char_key, &int_value, char_key_cpy, int_value_cpy,
                           char_key_cmp, int_value_cmp, char_key_free,
                           int_value_free);
  assert(new_pair_2);
  pairs_array_2[LOW_SIZE] = new_pair_2;
  assert(hashmap_insert(hash_map_2, pairs_array_2[SIZE_3]) == 1);
  assert(hash_map_2->size == (size_t) LOW_SIZE);
  assert(hash_map_2->capacity == (size_t) SIZE_8);

  for (i = 0; i < SIZE_5; i++)
    {
      pair_free ((void **) &pairs_array_2[i]);
    }
  hashmap_free (&hash_map_2);
  //printf(ERASE_TEST);
}
/**
 * This function checks the hashmap_get_load_factor function of
 * the hashmap library.
 * If hashmap_get_load_factor fails at some points, the
 * functions exits with exit code 1.
 */
void test_hash_map_get_load_factor(void)
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert(hash_map);
  assert(hashmap_get_load_factor(NULL) == -1);
  hash_map->capacity = (size_t) 0;
  assert(hashmap_get_load_factor(hash_map) == -1);
  hash_map->capacity = (size_t) CAPACITY;
  hash_map->size = (size_t) 2;
  assert(hashmap_get_load_factor(hash_map) == SMALL_LOAD);
  hash_map->size = (size_t) 12;
  assert(hashmap_get_load_factor(hash_map) == LARGE_LOAD);
  assert(hash_map->capacity == CAPACITY);
  hashmap_free (&hash_map);
  //printf(LOAD_TEST);
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits
 * with exit code 1.
 */
void test_hash_map_apply_if()
{
  hashmap *hash_map = hashmap_alloc (hash_char);
  assert(hash_map);
  char keys_array[SIZE_7] = KEYS;
  int values[SIZE_7] = VALUES;
  pair *pairs_array[SIZE_7];
  pair *new_pair;
  int i = 0;
  for (i = 0; i < SIZE_7; i++)
    {

      new_pair = pair_alloc (&keys_array[i], &values[i], char_key_cpy,
                             int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      assert(new_pair);
      pairs_array[i] = new_pair;
    }
  for (i = 0; i < SIZE_7; i++)
    {
      assert(hashmap_insert (hash_map, pairs_array[i]) == 1);
    }
  assert(hashmap_apply_if(NULL, is_digit, double_value) == -1);
  assert(hashmap_apply_if(hash_map, NULL, double_value) == -1);
  assert(hashmap_apply_if(hash_map, is_digit, NULL) == -1);
  assert(hashmap_apply_if(hash_map, is_digit, double_value) == SIZE_3);
  assert(*(int *) hashmap_at(hash_map, &keys_array[0]) == 0);
  assert(*(int *) hashmap_at(hash_map, &keys_array[2]) == SIZE_4);
  assert(*(int *) hashmap_at(hash_map, &keys_array[SIZE_4]) == SIZE_8);
  assert(*(int *) hashmap_at(hash_map, &keys_array[1]) == 1);
  assert(*(int *) hashmap_at(hash_map, &keys_array[SIZE_3]) == SIZE_3);
  for (i = 0; i < SIZE_7; i++)
    {
      pair_free ((void **) &pairs_array[i]);
    }
  hashmap_free(&hash_map);
  //printf (APPLY_TEST);
}

//int main ()
//{
//  test_hash_map_insert();
//  test_hash_map_at();
//  test_hash_map_erase();
//  test_hash_map_get_load_factor();
//  test_hash_map_apply_if();
//}