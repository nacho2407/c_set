#ifndef NSET_H
#define NSET_H

#include <stdio.h>
#include <stdlib.h>

#define NSET_VERSION "0.0.1"

#ifndef NTREE
#define NTREE

/* nset_node color */
#define RED 1
#define BLACK 0

#define INITIAL_CAPACITY 128

#define END_OF_ITER ((void*) 0)

/* nset_iter direction */
#define FORWARD 1
#define REVERSE 0
#endif

// will be documented
#define nset_dec(name, T)							\
	typedef struct nset_node_##name						\
	{									\
		int color;							\
										\
		T data;								\
										\
		struct nset_node_##name* left;					\
		struct nset_node_##name* right;					\
	} nset_node_##name;							\
										\
	typedef struct nset_##name						\
	{									\
		nset_node_##name* root;						\
										\
		unsigned int size;						\
										\
		int (*compare)(const void*, const void*);			\
		void (*free_data)(void*);					\
	} nset_##name;								\
										\
	typedef struct nset_iter_##name						\
	{									\
		nset_node_##name** stack;					\
										\
		unsigned int current;						\
		unsigned int capacity;						\
										\
		int direction;							\
										\
		int (*compare)(const void*, const void*);			\
	} nset_iter_##name;							\
										\
	int nset_init_##name(nset_##name* s, int (*compare)(const void*, const void*), void (*free_data)(void*));\
	void nset_free_##name(nset_##name* s);					\
	void nset_insert_##name(nset_##name* s, T data);			\
	nset_iter_##name* nset_erase_##name(nset_##name* s, nset_iter_##name* i);\
	void nset_clear_##name(nset_##name* s);					\
	nset_iter_##name* nset_find_##name(const nset_##name* s, const T data);	\
	T* nset_min_##name(const nset_##name* s);				\
	T* nset_max_##name(const nset_##name* s);				\
	int nset_contains_##name(const nset_##name* s, const T data);		\
	int nset_size_##name(const nset_##name* s);				\
	int nset_empty_##name(const nset_##name* s);				\
	nset_iter_##name* nset_begin_##name(const nset_##name* s);		\
	nset_iter_##name* nset_rbegin_##name(const nset_##name* s);		\
	void nset_free_iter_##name(nset_iter_##name* i);			\
	T* nset_current_##name(nset_iter_##name* i);				\
	int nset_has_next_##name(nset_iter_##name* i);				\
	T* nset_next_##name(nset_iter_##name* i);

#define nset_foreach(name, iter, key)						\
	for(key = *nset_next_##name(iter); nset_has_next_##name(iter); key = *nset_next_##name(iter))

nset_dec(i, int)
nset_dec(u, unsigned int)
nset_dec(ll, long long int)
nset_dec(f, float)
nset_dec(d, double)
nset_dec(c, char)
nset_dec(s, char*)
nset_dec(v, void*)
#endif
