#include "vector.h"
#include <stdio.h>
/**
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element stored in the
 * vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements stored
 * in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *vector_alloc(vector_elem_cpy elem_copy_func,
                     vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func)
{
  vector *v = calloc(sizeof(*v), 1);
  if (v == NULL)
    {
      return NULL;
    }
  if ((elem_copy_func == NULL)|| (elem_cmp_func == NULL) ||
  (elem_free_func == NULL))
    {
      free(v);
      return NULL;
    }
  v->capacity = VECTOR_INITIAL_CAP;
  v->size = 0;
  v->data = calloc(sizeof(void*), v->capacity);
  if (v->data == NULL)
    {
      free(v);
      return NULL;
    }
  v->elem_copy_func = elem_copy_func;
  v->elem_cmp_func = elem_cmp_func;
  v->elem_free_func = elem_free_func;
  return v;
}
/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free(vector **p_vector)
{
  if (*p_vector == NULL)
    {
      return;
    }
  size_t i = 0;
  for (i = 0; i < (*p_vector)->size; i++)
    {
      (*p_vector)->elem_free_func(&(*p_vector)->data[i]);
    }
  free((*p_vector)->data);
  free(*p_vector);
  *p_vector = NULL;
}
/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists (the element itself,
 * not a copy of it),
 * NULL otherwise.
 */
void *vector_at(const vector *vector, size_t ind)
{
  int a = ((int) ind);
  if (vector == NULL || vector->data == NULL || ind > vector->size - 1 ||
  a < 0 || vector->data[ind] == NULL)
    {
      return NULL;
    }
  return vector->data[ind];
}
/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector
 * ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find(const vector *vector, const void *value)
{
  if (vector == NULL || vector->data == NULL || vector->elem_cmp_func == NULL)
    {
      return -1;
    }
  size_t i = 0;
  while (vector->data[i] != NULL)
    {
      if (vector->elem_cmp_func(vector->data[i], value) == 1)
        {
          return i;
        }
      i ++;
    }
  return -1;
}
//* This function returns the load factor of the vector.
//* @param vector a vector.
//* @param add - increase aor decrease the load factor in advanced
//* @return the vector's load factor, -1 if the function failed.
//*/
double pre_vector_get_load_factor(const vector *vector, int add)
{
  if (vector == NULL)
    {
      return -1;
    }
  double cap = vector->capacity;
  double size = vector->size;
  double load = ((size + add) / cap);
  return load;
}
/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back(vector *vector, const void *value)
{
  if (vector == NULL || value == NULL)
    {
      return 0;
    }
  size_t cap = vector->capacity;
  if (pre_vector_get_load_factor(vector, 1) > VECTOR_MAX_LOAD_FACTOR)
    {
      vector->data = realloc(vector->data, sizeof(void*) *
      (vector->capacity * VECTOR_GROWTH_FACTOR));
      if (vector->data == NULL)
        {
          return 0;
        }
      vector->capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
    }
  vector->data[vector->size] = vector->elem_copy_func(value);
  if (vector->data[vector->size] == NULL)
    {
      if (cap != vector->capacity)
        {
          vector->capacity = vector->capacity / VECTOR_GROWTH_FACTOR;
        }
      return 0;
    }
  vector->size++;
  return 1;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor(const vector *vector)
{
  if (vector == NULL)
    {
      return -1;
    }
  double cap = vector->capacity;
  double load = (vector->size / cap);
  return load;
}
/**
 * Removes the element at the given index from the vector.
 * alters the indices of the remaining
 * elements so that there are no empty indices in the range
 * [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase(vector *vector, size_t ind)
{
  int a = ((int) ind);
  if (vector == NULL || vector->data == NULL ||
  ind > vector->size - 1 || a < 0 || vector->data[ind] == NULL)
    {
      return 0;
    }
  if (pre_vector_get_load_factor(vector, -1) < VECTOR_MIN_LOAD_FACTOR)
    {
      vector->data = realloc(vector->data, sizeof(void*) *
      (vector->capacity / VECTOR_GROWTH_FACTOR));
      if (vector->data == NULL)
        {
          return 0;
        }
      vector->capacity = vector->capacity / VECTOR_GROWTH_FACTOR;
    }
  vector->elem_free_func(&vector->data[ind]);

  size_t i = 0;
  for (i = ind; i < vector->size - 1; i++)
    {
      vector->data[i] = vector->data[i+1];
    }
  vector->data[vector->size - 1] = NULL;
  vector->size--;
  return 1;
}
/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear(vector *vector)
{
  if (vector == NULL || vector->data == NULL)
    {
      return;
    }
  size_t i = 0;
  size_t size = vector->size;
  for (i = 0; i < size; i++)
    {
      vector_erase (vector, 0);
    }
}

