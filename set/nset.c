#include "nset.h"

#define nset_def(name, T)							\
        static nset_node_##name* get_node_##name(T data);			\
        static void free_node_##name(nset_node_##name* n, void (*free_data)(void*));\
        static int is_red_##name(const nset_node_##name* n);			\
        static nset_node_##name* rotate_left_##name(nset_node_##name* n);	\
        static void change_color_##name(nset_node_##name* first, nset_node_##name* second);\
        static nset_node_##name* lean_left_##name(nset_node_##name* n);		\
        static nset_node_##name* rotate_right_##name(nset_node_##name* n);	\
        static nset_node_##name* lean_right_##name(nset_node_##name* n);	\
        static nset_node_##name* flip_color_##name(nset_node_##name* n);	\
        static nset_node_##name* adjust_##name(nset_node_##name* n);		\
        static nset_node_##name* insert_node_##name(nset_node_##name* n, nset_node_##name* new_node, int (*compare)(const void*, const void*));\
        static nset_node_##name* move_red_left_##name(nset_node_##name* n);	\
        static nset_node_##name* move_red_right_##name(nset_node_##name* n);	\
        static nset_node_##name* find_node_##name(nset_node_##name* n, const T target, int (*compare)(const void*, const void*));\
        static nset_node_##name* get_min_##name(nset_node_##name* n);		\
        static nset_node_##name* erase_min_##name(nset_node_##name* n, void (*free_data)(void*));\
        static nset_node_##name* erase_node_##name(nset_node_##name* n, const T target, int (*compare)(const void*, const void*), void (*free_data)(void*));\
        static nset_node_##name* get_max_##name(nset_node_##name* n);		\
        static void resize_stack_##name(nset_iter_##name* i);			\
        static void insert_stack_##name(nset_iter_##name* i, nset_node_##name* root, nset_node_##name* n);\
										\
        static int insert_flag_##name = 0;					\
        static int erase_flag_##name = 0;					\
										\
        int nset_init_##name(nset_##name* s, int (*compare)(const void*, const void*), void (*free_data)(void*))\
        {									\
                if(!s)                                                          \
                        return 0;                                               \
                                                                                \
                s->root = NULL;		                                        \
		s->size = 0;							\
                s->compare = compare;						\
                s->free_data = free_data;					\
										\
                return 1;							\
        }									\
										\
        void nset_free_##name(nset_##name* s)					\
        {									\
                if(!s || s->size == 0)						\
                        return;							\
										\
                free_node_##name(s->root, s->free_data);			\
										\
                free(s);							\
        }									\
										\
        void nset_insert_##name(nset_##name* s, T data)				\
        {									\
                if(!s)                                                          \
                        return;                                                 \
                                                                                \
                nset_node_##name* new_node = get_node_##name(data);		\
										\
                if(!new_node)							\
                        return;							\
										\
                s->root = insert_node_##name(s->root, new_node, s->compare);	\
                s->root->color = BLACK;						\
										\
                if(insert_flag_##name) {					\
                        s->size++;						\
										\
                        insert_flag_##name = 0;					\
                }								\
        }									\
										\
        nset_iter_##name* nset_erase_##name(nset_##name* s, nset_iter_##name* i)\
        {									\
                if(!s || !i)							\
                        return i;						\
                                                                                \
                if(s->size == 0)						\
                        return i;						\
										\
                T* target = nset_next_##name(i);				\
										\
                if(!target)					                \
                        return i;						\
										\
                s->root = erase_node_##name(s->root, *target, s->compare, s->free_data);\
										\
                if(s->root)							\
                        s->root->color = BLACK;					\
										\
                if(erase_flag_##name) {						\
                        s->size--;						\
										\
                        erase_flag_##name = 0;					\
                }								\
                                                                                \
                nset_iter_##name* ret = END_OF_ITER;				\
                                                                                \
                T* new_begin = nset_current_##name(i);                          \
                if(new_begin) {				                        \
                        ret = nset_find_##name(s, *nset_current_##name(i));     \
                        ret->direction = i->direction;	                        \
                }				                                \
										\
                nset_free_iter_##name(i);					\
										\
                return ret;							\
        }									\
										\
        void nset_clear_##name(nset_##name* s)					\
        {									\
                if(!s)								\
                        return;							\
                                                                                \
                if(s->size == 0)						\
                        return;							\
										\
                free_node_##name(s->root, s->free_data);			\
                s->root = NULL;							\
                s->size = 0;							\
        }									\
										\
        nset_iter_##name* nset_find_##name(const nset_##name* s, const T data)	\
        {									\
                if(!s || s->size == 0)						\
                        return END_OF_ITER;					\
                                                                                \
                nset_node_##name* n = find_node_##name(s->root, data, s->compare);\
										\
                if(!n)								\
                        return END_OF_ITER;					\
										\
                nset_iter_##name* i = (nset_iter_##name*) malloc(sizeof(nset_iter_##name));\
										\
                if(!i) {							\
                        perror("iter allocation failed");			\
										\
                        return NULL;						\
                }								\
										\
                i->capacity = INITIAL_CAPACITY;					\
                i->stack = (nset_node_##name**) malloc(sizeof(nset_node_##name*) * INITIAL_CAPACITY);\
										\
                if(!i->stack) {							\
                        perror("iter stack allocation failed");			\
										\
                        free(i);						\
										\
                        return NULL;						\
                }								\
										\
                i->current = 0;							\
                i->direction = FORWARD;						\
                i->compare = s->compare;					\
										\
                insert_stack_##name(i, s->root, n);				\
										\
                return i;							\
        }									\
										\
        T* nset_min_##name(const nset_##name* s)				\
        {									\
                if(!s || s->size == 0)						\
                        return NULL;						\
                else								\
                        return &get_min_##name(s->root)->data;			\
        }									\
										\
        T* nset_max_##name(const nset_##name* s)				\
        {									\
                if(!s || s->size == 0)						\
                        return NULL;						\
                else								\
                        return &get_max_##name(s->root)->data;			\
        }									\
										\
        int nset_contains_##name(const nset_##name* s, const T data)		\
        {									\
                return s && find_node_##name(s->root, data, s->compare) != NULL;\
        }									\
										\
        int nset_size_##name(const nset_##name* s)				\
        {									\
                return s ? s->size : 0;						\
        }									\
										\
        int nset_empty_##name(const nset_##name* s)				\
        {									\
                return s && (s->size == 0);					\
        }									\
										\
        nset_iter_##name* nset_begin_##name(const nset_##name* s)		\
        {									\
                if(!s)                                                          \
                        return NULL;                                            \
                                                                                \
                nset_iter_##name* i = (nset_iter_##name*) malloc(sizeof(nset_iter_##name));\
										\
                if(!i) {							\
                        perror("iter allocation failed");			\
										\
                        return NULL;						\
                }								\
										\
                i->capacity = INITIAL_CAPACITY;					\
                i->stack = (nset_node_##name**) malloc(sizeof(nset_node_##name*) * INITIAL_CAPACITY);\
										\
                if(!i->stack) {							\
                        perror("iter stack allocation failed");			\
										\
                        free(i);						\
										\
                        return NULL;						\
                }								\
										\
                i->current = 0;							\
                i->direction = FORWARD;						\
                i->compare = s->compare;					\
										\
                if(s->root)							\
                        insert_stack_##name(i, s->root, get_min_##name(s->root));\
										\
                return i;							\
        }									\
										\
        nset_iter_##name* nset_rbegin_##name(const nset_##name* s)		\
        {									\
                if(!s)                                                          \
                        return NULL;                                            \
                                                                                \
                nset_iter_##name* i = (nset_iter_##name*) malloc(sizeof(nset_iter_##name));\
										\
                if(!i) {							\
                        perror("iter allocation failed");			\
										\
                        return NULL;						\
                }								\
										\
                i->capacity = INITIAL_CAPACITY;					\
                i->stack = (nset_node_##name**) malloc(sizeof(nset_node_##name*) * INITIAL_CAPACITY);\
										\
                if(!i->stack) {							\
                        perror("iter stack allocation failed");			\
										\
                        free(i);						\
										\
                        return NULL;						\
                }								\
										\
                i->current = 0;							\
                i->direction = REVERSE;						\
                i->compare = s->compare;					\
										\
                if(s->root)							\
                        insert_stack_##name(i, s->root, get_max_##name(s->root));\
										\
                return i;							\
        }									\
										\
        void nset_free_iter_##name(nset_iter_##name* i)				\
        {									\
                if(!i)                                                          \
                        return;                                                 \
                                                                                \
                free(i->stack);							\
										\
                free(i);							\
        }									\
										\
        T* nset_current_##name(nset_iter_##name* i)				\
        {									\
                if(!i)                                                          \
                        return NULL;                                            \
                                                                                \
                if(i->current == 0)						\
                        return END_OF_ITER;					\
										\
                return &i->stack[i->current]->data;				\
        }									\
                                                                                \
        int nset_has_next_##name(nset_iter_##name* i)                           \
        {                                                                       \
                return i && i->current != 0;                                    \
        }                                                                       \
										\
        T* nset_next_##name(nset_iter_##name* i)				\
        {									\
                if(!i)                                                          \
                        return NULL;                                            \
                                                                                \
                if(i->current == 0)						\
                        return END_OF_ITER;					\
										\
                nset_node_##name* current_node = i->stack[i->current--];	\
										\
                if(i->direction == FORWARD && current_node->right)		\
                        insert_stack_##name(i, current_node->right, get_min_##name(current_node->right));\
                else if(i->direction == REVERSE && current_node->left)		\
                        insert_stack_##name(i, current_node->left, get_max_##name(current_node->left));\
										\
                return &current_node->data;					\
        }									\
										\
        static nset_node_##name* get_node_##name(T data)			\
        {									\
                nset_node_##name* n = (nset_node_##name*) calloc(sizeof(nset_node_##name), 1);\
										\
                if(!n) {							\
                        perror("node allocation failed");			\
										\
                        return NULL;						\
                }								\
										\
                n->color = RED;							\
                n->data = data;							\
										\
                return n;							\
        }									\
										\
        static void free_node_##name(nset_node_##name* n, void (*free_data)(void*))\
        {									\
                if(n->left)							\
                        free_node_##name(n->left, free_data);			\
										\
                if(n->right)							\
                        free_node_##name(n->right, free_data);			\
										\
                if(free_data)							\
                        free_data(&n->data);					\
										\
                free(n);							\
        }									\
										\
        static int is_red_##name(const nset_node_##name* n)			\
        {									\
                return n && n->color;						\
        }									\
										\
        static nset_node_##name* rotate_left_##name(nset_node_##name* n)	\
        {									\
                if(!n->right)						\
                        return n;						\
										\
                nset_node_##name* new_root = n->right;				\
										\
                n->right = new_root->left;					\
                new_root->left = n;						\
										\
                return new_root;						\
        }									\
										\
        static void change_color_##name(nset_node_##name* first, nset_node_##name* second)\
        {									\
                if(!first || !second)						\
                        return;							\
										\
                int temp = first->color;					\
                first->color = second->color;					\
                second->color = temp;						\
        }									\
										\
        static nset_node_##name* lean_left_##name(nset_node_##name* n)		\
        {									\
                change_color_##name(n, n->right);				\
                return rotate_left_##name(n);					\
        }									\
										\
        static nset_node_##name* rotate_right_##name(nset_node_##name* n)	\
        {									\
                if(!n->left)						\
                        return n;						\
										\
                nset_node_##name* new_root = n->left;				\
										\
                n->left = new_root->right;					\
                new_root->right = n;						\
										\
                return new_root;						\
        }									\
										\
        static nset_node_##name* lean_right_##name(nset_node_##name* n)		\
        {									\
                change_color_##name(n, n->left);				\
                return rotate_right_##name(n);					\
        }									\
										\
        static nset_node_##name* flip_color_##name(nset_node_##name* n)		\
        {									\
                n->color = !n->color;						\
										\
                if(n->left)							\
                        n->left->color = !n->left->color;			\
										\
                if(n->right)							\
                        n->right->color = !n->right->color;			\
										\
                return n;							\
        }									\
										\
        static nset_node_##name* adjust_##name(nset_node_##name* n)		\
        {									\
                if(is_red_##name(n->right)) {					\
                        if(is_red_##name(n->left))				\
                                return flip_color_##name(n);			\
                        else							\
                                return lean_left_##name(n);			\
                } else if(is_red_##name(n->left) && is_red_##name(n->left->left))\
                        return flip_color_##name(lean_right_##name(n));		\
                else								\
                        return n;						\
        }									\
										\
        static nset_node_##name* insert_node_##name(nset_node_##name* n, nset_node_##name* new_node, int (*compare)(const void*, const void*))\
        {									\
                if(!n) {							\
                        insert_flag_##name = 1;					\
										\
                        return new_node;					\
                }								\
										\
                if(compare ? compare(&n->data, &new_node->data) : n->data < new_node->data)\
                        n->right = insert_node_##name(n->right, new_node, compare);\
                else if(compare ? compare(&new_node->data, &n->data) : new_node->data < n->data)\
                        n->left = insert_node_##name(n->left, new_node, compare);\
                else								\
                        n->data = new_node->data;				\
										\
                return adjust_##name(n);					\
        }									\
										\
        static nset_node_##name* move_red_left_##name(nset_node_##name* n)	\
        {									\
                n->color = BLACK;						\
                n->left->color = RED;						\
										\
                if(is_red_##name(n->right->left)) {				\
                        n->right = rotate_right_##name(n->right);		\
                        n = rotate_left_##name(n);				\
                } else								\
                        n->right->color = RED;					\
										\
                return n;							\
        }									\
										\
        static nset_node_##name* move_red_right_##name(nset_node_##name* n)	\
        {									\
                n->color = BLACK;						\
                n->right->color = RED;						\
										\
                if(is_red_##name(n->left->left)) {				\
                        n = rotate_right_##name(n);				\
                        n->color = RED;						\
                        n->left->color = BLACK;					\
                } else								\
                        n->left->color = RED;					\
										\
                return n;							\
        }									\
										\
        static nset_node_##name* find_node_##name(nset_node_##name* n, const T target, int (*compare)(const void*, const void*))\
        {									\
                if(!n)								\
                        return n;						\
										\
                if(compare ? compare(&n->data, &target) : n->data < target)	\
                        return find_node_##name(n->right, target, compare);	\
                else if(compare ? compare(&target, &n->data) : target < n->data)\
                        return find_node_##name(n->left, target, compare);	\
                else								\
                        return n;						\
        }									\
										\
        static nset_node_##name* get_min_##name(nset_node_##name* n)		\
        {									\
                if(!n->left)							\
                        return n;						\
                else								\
                        return get_min_##name(n->left);				\
        }									\
										\
        static nset_node_##name* erase_min_##name(nset_node_##name* n, void (*free_data)(void*))\
        {									\
                if(!n->left) {							\
                        erase_flag_##name = 1;					\
										\
                        return NULL;						\
                }								\
										\
                /* If there are no red nodes on the way down, move a red node to the left */\
                if(!is_red_##name(n->left) && !is_red_##name(n->left->left))	\
                        n = move_red_left_##name(n);				\
										\
                nset_node_##name* temp = erase_min_##name(n->left, free_data);	\
										\
                if(!temp) {							\
                        free_node_##name(n->left, free_data);		        \
                        n->left = NULL;                                         \
                }                                                               \
										\
                return adjust_##name(n);					\
        }									\
										\
        static nset_node_##name* erase_node_##name(nset_node_##name* n, const T target, int (*compare)(const void*, const void*), void (*free_data)(void*))\
        {									\
                if(compare ? compare(&target, &n->data) : target < n->data) {	\
                        if(!n->left)						\
                                return n;					\
										\
                        if(!is_red_##name(n->left) && !is_red_##name(n->left->left))\
                                n = move_red_left_##name(n);			\
										\
                        nset_node_##name* temp = erase_node_##name(n->left, target, compare, free_data);\
										\
                        if(!temp && compare ? compare(&n->left->data, &target) : n->left->data < target == 0) {\
                                free_node_##name(n->left, free_data);	        \
                                n->left = NULL;                                 \
                        }                                                       \
                } else {							\
                        /* If there is a red node on the left while going down, move it to the right */\
                        if(is_red_##name(n->left))				\
                                n = lean_right_##name(n);			\
										\
                        if(!n->right) {						\
                                /* If the target is a leaf node,		\
										\
                                   For your information, the case where a red node is on the left was handled above.\
                                   There is no case where only a black node is on the left */\
                                if(!(compare ? compare(&n->data, &target) : n->data < target)) {\
                                        erase_flag_##name = 1;			\
                                        					\
                                        return NULL;				\
                                } else						\
                                        return n;				\
                        }							\
										\
                        /* If there are no red nodes on the way down, move a red node to the left */\
                        if(!is_red_##name(n->right) && !is_red_##name(n->right->left))\
                                n = move_red_right_##name(n);			\
										\
                        /* If the target is not a leaf node, */			\
                        if(!(compare ? compare(&n->data, &target) : n->data < target)) {\
                                nset_node_##name* successor = get_min_##name(n->right);\
                                n->data = successor->data;			\
										\
                                nset_node_##name* temp = erase_min_##name(n->right, free_data);\
										\
                                if(!temp) {                                     \
                                        free_node_##name(n->right, free_data);  \
                                        n->right = NULL;                        \
                                }                                               \
                        } else {						\
                                nset_node_##name* temp = erase_node_##name(n->right, target, compare, free_data);\
										\
                                if(!temp && !(compare ? compare(&n->right->data, &target) : n->right->data < target)\
                                && !(compare ? compare(&target, &n->right->data) : target < n->right->data)) {\
                                        free_node_##name(n->right, free_data);  \
                                        n->right = NULL;                        \
                                }                                               \
                        }							\
                }								\
										\
                return adjust_##name(n);					\
        }									\
										\
        static nset_node_##name* get_max_##name(nset_node_##name* n)		\
        {									\
                if(!n->right)							\
                        return n;						\
                else								\
                        return get_max_##name(n->right);			\
        }									\
										\
        static void resize_stack_##name(nset_iter_##name* i)			\
        {									\
                if(i->current == i->capacity) {					\
                        i->capacity *= 2;					\
										\
                        nset_node_##name** resized_stack = (nset_node_##name**) realloc(i->stack, sizeof(nset_node_##name*) * i->capacity);\
										\
                        if(!resized_stack) {					\
                                perror("iter stack reallocation failed");	\
										\
                                i->capacity /= 2;				\
										\
                                return;						\
                        }							\
										\
                        i->stack = resized_stack;				\
                } else if(i->capacity > INITIAL_CAPACITY && i->current < i->capacity / 4) {\
                        i->capacity /= 2;					\
										\
                        nset_node_##name** resized_stack = (nset_node_##name**) realloc(i->stack, sizeof(nset_node_##name*) * i->capacity);\
										\
                        if(!resized_stack) {					\
                                perror("iter stack reallocation failed");	\
										\
                                i->capacity *= 2;				\
										\
                                return;						\
                        }							\
										\
                        i->stack = resized_stack;				\
                }								\
        }									\
										\
        static void insert_stack_##name(nset_iter_##name* i, nset_node_##name* root, nset_node_##name* n)\
        {									\
                i->stack[++i->current] = root;					\
										\
                for(; ; ) {							\
                resize_stack_##name(i);						\
										\
                        if(i->compare ? i->compare(&i->stack[i->current]->data, &n->data) : i->stack[i->current]->data < n->data)\
                                i->stack[++i->current] = i->stack[i->current]->right;\
                        else if(i->compare ? i->compare(&n->data, &i->stack[i->current]->data) : n->data < i->stack[i->current]->data)\
                                i->stack[++i->current] = i->stack[i->current]->left;\
                        else							\
                                break;						\
                }								\
	}

nset_def(i, int)
nset_def(ll, long long int)
nset_def(f, float)
nset_def(d, double)
nset_def(c, char)
nset_def(s, char*)
nset_def(v, void*)
