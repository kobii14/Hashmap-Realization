#include "hashmap.h"
/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    {
      return NULL;
    }
  hashmap *table = calloc(sizeof(*table), 1);
  if (table == NULL)
    {
      return NULL;
    }
  table->size = 0;
  table->capacity = HASH_MAP_INITIAL_CAP;
  table->hash_func = func;
  table->buckets = calloc(sizeof(void *), table->capacity);
  if (table->buckets == NULL)
    {
      free(table);
      return NULL;
    }
  return table;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  size_t i = 0;
  while (i < (*p_hash_map)->capacity)
    {
      vector_free(&((*p_hash_map)->buckets[i]));
      i++;
    }
  free((*p_hash_map)->buckets);
  free(*p_hash_map);
  *p_hash_map = NULL;
}
keyT get_key (const hashmap *hash_map, size_t bucket_ind, size_t data_ind)
{
  pair *a = (pair *) (hash_map->buckets[bucket_ind]->data[data_ind]);
  return a->key;
}

// This function gets a dynamically allocated hash_map and null specific part
// of it
void null_it (hashmap *hash_map, size_t start)
{
  if (start == 0)
    {
      size_t i = 0;
      for (i = 0; i < hash_map->capacity; i++)
        {
          hash_map->buckets[i] = NULL;
        }
    }
  else
    {
      for (; start < hash_map->capacity * HASH_MAP_GROWTH_FACTOR; start++)
        {
          hash_map->buckets[start] = NULL;
        }
    }
}

// This function updates a hashmap in case it needs to be rehashed
// gets a hashmap and a direction - increase for 1, otherwise to decrease the
// capacity of the hashmap.
// returns 0 if failed, 1 if succeeded
int hash_update (hashmap *hash_map, int dir)
{
  hashmap *new_hash = hashmap_alloc(hash_map->hash_func);
  if (new_hash == NULL)
    {
      return 0;
    }
  hashmap *new_hash_2 = hashmap_alloc(hash_map->hash_func);
  if (new_hash_2 == NULL)
    {
      free(new_hash->buckets);
      free(new_hash);
      return 0;
    }
  new_hash_2->size = hash_map->size;
  if (dir == 1)
    {
      new_hash->capacity = hash_map->capacity * HASH_MAP_GROWTH_FACTOR;
    }
  else
    {
      new_hash->capacity = hash_map->capacity / HASH_MAP_GROWTH_FACTOR;
    }
  new_hash->buckets = realloc(new_hash->buckets, sizeof(void*)
  * (new_hash->capacity));
  if (new_hash == NULL)
    {
      free(new_hash->buckets);
      free(new_hash);
      free(new_hash_2->buckets);
      free(new_hash_2);
      return 0;
    }
  null_it(new_hash, 0);
  size_t i = 0;
  size_t j = 0;
  for (i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] != NULL)
        {
          if (hash_map->buckets[i]->size == 0)
            {
              vector_free(&hash_map->buckets[i]);
              break;
            }
          for (j = 0; j < hash_map->buckets[i]->size; j++)
            {
              size_t index = hash_map->hash_func(get_key(hash_map, i, j))
                  & (new_hash->capacity - 1);
              if (new_hash->buckets[index] == NULL)
                {
                  new_hash->buckets[index] =
                      vector_alloc(pair_copy, pair_cmp, pair_free);
                  if (new_hash->buckets[index] == NULL)
                    {
                      hashmap_free(&new_hash);
                      free(new_hash_2->buckets);
                      free(new_hash_2);
                      return 0;
                    }
                }
              int check = vector_push_back(new_hash->buckets[index],
                                           hash_map->buckets[i]->data[j]);
              if (check == 0)
                {
                  hashmap_free(&new_hash);
                  free(new_hash_2->buckets);
                  free(new_hash_2);
                  return 0;
                }
            }
        }
    }
  new_hash_2->capacity = hash_map->capacity;
  free(new_hash_2->buckets);
  new_hash_2->buckets = hash_map->buckets;
  hashmap_free(&new_hash_2);
  hash_map->buckets = new_hash->buckets;
  hash_map->capacity = new_hash->capacity;
  free(new_hash);
  return 1;
}
//* This function returns the load factor of the vector.
//* @param vector a vector.
//* @param add - increase aor decrease the load factor in advanced
//* @return the vector's load factor, -1 if the function failed.
//*/
double pre_hashmap_get_load_factor(const hashmap *hash_map, int add)
{
  if (hash_map == NULL)
    {
      return -1;
    }
  double cap = hash_map->capacity;
  double size = hash_map->size;
  double load = ((size + add) / cap);
  return load;
}
/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL || in_pair == NULL || in_pair->key == NULL ||
  in_pair->value == NULL || hashmap_at(hash_map, in_pair->key) != NULL)
    {
      return 0;
    }
  if (pre_hashmap_get_load_factor(hash_map, 1) > HASH_MAP_MAX_LOAD_FACTOR)
    {
      hash_map->buckets = realloc(hash_map->buckets, sizeof(void*) *
      (hash_map->capacity * HASH_MAP_GROWTH_FACTOR));
      if (hash_map->buckets == NULL)
        {
          return 0;
        }
      null_it(hash_map, hash_map->capacity);
      if (hash_update(hash_map, 1) == 0)
        {
          hash_map->buckets = realloc(hash_map->buckets, sizeof(void*) *
          (hash_map->capacity / HASH_MAP_GROWTH_FACTOR));
          if (hash_map->buckets == NULL)
            {
              return 0;
            }
          return 0;
        }
    }
  size_t index = hash_map->hash_func(in_pair->key) & (hash_map->capacity - 1);
  if (hash_map->buckets[index] == NULL || ((hash_map->buckets[index] != NULL)
  && (hash_map->buckets[index]->data[0] == NULL)
  && (hash_map->buckets[index]->size > 0)))
    {
      hash_map->buckets[index] = vector_alloc(pair_copy, pair_cmp, pair_free);
      if (hash_map->buckets[index] == NULL)
        {
          return 0;
        }
    }
  int check = vector_push_back(hash_map->buckets[index], in_pair);
  if (check == 0)
    {
      return 0;
    }
  hash_map->size++;
  return 1;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise
 * (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL)
    {
      return NULL;
    }
  size_t i = 0;
  size_t j = 0;
  size_t counter = 0;
  for (i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] != NULL && hash_map->buckets[i]->size > 0)
        {
          for (j = 0; j < hash_map->buckets[i]->size; j++)
            {
              if (hash_map->buckets[i]->data[j] != NULL)
                {
                  keyT new_key = get_key(hash_map, i, j);
                  pair *a = (pair *) (hash_map->buckets[i]->data[j]);
                  if (a->key_cmp(new_key, key) == 1)
                    {
                      return a->value;
                    }
                }
            }
          counter += hash_map->buckets[i]->size;
          if (counter == hash_map->size)
            {
              return NULL;
            }
        }
    }
  return NULL;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise.
 * (if key not in map,
 * considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL || key == NULL || hashmap_at(hash_map, key) == NULL)
    {
      return 0;
    }
  size_t i = 0;
  size_t j = 0;
  if (pre_hashmap_get_load_factor(hash_map, -1) < VECTOR_MIN_LOAD_FACTOR)
    {
      if (hash_update(hash_map, -1) == 0)
        {
          return 0;
        }
    }
  for (i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] != NULL && hash_map->buckets[i]->size > 0)
        {
          for (j = 0; j < hash_map->buckets[i]->size; j++)
            {
              if (hash_map->buckets[i]->data[j] != NULL)
                {
                  pair *a = (pair *) (hash_map->buckets[i]->data[j]);
                  int compare = a->key_cmp(key, a->key);
                  if (compare == 1)
                    {
                      int check = vector_erase(hash_map->buckets[i], j);
                      if (hash_map->buckets[i]->size == 0)
                        {
                          vector_free(&hash_map->buckets[i]);
                        }
                      if (check == 0)
                        {
                          return 0;
                        }
                      hash_map->size--;
                      return 1;
                    }
                }
            }
        }
    }
  return 0;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL || hash_map->capacity < 1)
    {
      return -1;
    }
  double cap = hash_map->capacity;
  double load = (hash_map->size / cap);
  return load;
}

/**
 * This function receives a hashmap and 2 functions, the first checks
 * a condition on the keys,
 * and the seconds apply some modification on the values. The function
 * should apply the modification
 * only on the values that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char
 * is a capital letter (A-Z),
 * and val_t_func multiples the number by 2, hashmap_apply_if will change
 * the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)}, and the return
 * value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT and return
 * 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func)
{
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
      return -1;
    }
  size_t i = 0;
  size_t j = 0;
  int counter = 0;
  for (i = 0; i < hash_map->capacity; i++)
    {
      if (hash_map->buckets[i] != NULL && hash_map->buckets[i]->size > 0)
        {
          for (j = 0; j < hash_map->buckets[i]->size; j++)
            {
              pair *a = (pair *) (hash_map->buckets[i]->data[j]);
              if (keyT_func(a->key) == 1)
                {
                  counter ++;
                  valT_func(a->value);
                }
            }
        }
    }
  return counter;
}

