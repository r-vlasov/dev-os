#include "../headers/process_tree.h"

/* */

tree_t* 	tree_create()
{
	tree_t* res = kmalloc(sizeof(tree_t));
	res->nodes = 0;
	res->root = NULL;
	return res;
}

/* */
tree_node_t*	tree_node_create(void* value)
{
	tree_node_t* res = kmalloc(sizeof(tree_node_t));
	res->value = value;
	res->child = NULL;
	res->parent = NULL;
	return res;
}
/* */

void 		tree_set_root(tree_t* tree, void* value)
{
	tree_node_t* root = tree_node_create(value);
	tree->root = root;
	tree->nodes = 1;
}


//tree_node_insert_child(process_tree, parent->tree_entry, node);

tree_node_t*	tree_node_insert_child(tree_t* tree, tree_node_t* parent, void* value)
{
	tree_node_t* res = tree_node_create(value);
	res->parent = parent;
	if (parent->child == NULL)
	{
		parent->child = list_create();
		list_set_head(parent->child, res);
	}
	else
	{
		list_add(parent->child, res);
	}
	tree->nodes++;
	return res;
}
