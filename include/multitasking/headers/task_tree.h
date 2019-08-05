#ifndef TREE_H
#define TREE_H

#include "../../../lib/headers/stdlib.h"

typedef struct tree_node {
	void* 			value;
	struct 	tree_node 	*child;
	struct 	tree_node 	*parent;
} tree_node_t;



typedef struct tree {
	size_t 			nodes;
	tree_node_t*		root;
} tree_t;


	tree_t* 	tree_create();
	tree_node_t*	tree_node_create(void* value);	
	void 		tree_set_root(tree_t* tree, void* value);
	tree_node_t*	tree_node_insert_child(tree_t* tree, tree_node_t* parent, void* value);


#endif
