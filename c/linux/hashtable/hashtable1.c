#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node {
    char key[32];
    char value[32];
}Node;

static void displayhash(gpointer key, gpointer value, gpointer user_data) {
    g_printf("user_data:%s\n",user_data);
    g_printf("key:%s value:%s\n",key,value);
}

static void destroy_key(gpointer hash_data) {
    g_printf("destroy key:%s\n",hash_data);
    //因为我这里的键是数组形式不是指针所以不用释放内存。我就直接清空吧
/*    free(hash_data);
    hash_data = NULL;*/
    memset(hash_data,0,sizeof(hash_data));
}

static void destroy_value(gpointer hash_data) {
    g_printf("destroy value:%s\n",hash_data);
    //因为我这里的值是数组形式不是指针所以不用释放内存。我就直接清空吧
/*    free(hash_data);
    hash_data = NULL;*/
    memset(hash_data,0,sizeof(hash_data));
}

/*用来创建每个节点。每个键值对都需要有自己的内存*/
Node *create_node(char *key,char * value) {
    Node *node = NULL;
    node = malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }

    memset(node,0,sizeof(Node));
    strcpy(node->key,key);
    strcpy(node->value,value);

    return node;
}

int main()
{
    char buff[32] = {0};
    static GHashTable *g_hash = NULL;
    
    g_hash = g_hash_table_new_full(g_str_hash, g_int_equal, destroy_key, destroy_value);
    
    Node *node = create_node("name","xcy");
    if(node != NULL)
        g_hash_table_insert(g_hash, &node->key, &node->value);
    Node *node1 = create_node("age","18");
    if(node1 != NULL)
        g_hash_table_insert(g_hash, &node1->key, &node1->value);
    Node *node2 = create_node("sex","man");
    if(node2 != NULL)
        g_hash_table_insert(g_hash, &node2->key, &node2->value);
    Node *node3 = create_node("id","00001");
    if(node3 != NULL)
        g_hash_table_insert(g_hash, &node3->key, &node3->value);


    memcpy(buff,"this is parm",12);
    if(NULL != g_hash) {
        g_hash_table_foreach(g_hash, displayhash, buff);
    }
    

    g_printf("------------------free hashtable------------------------\n");

    g_hash_table_destroy(g_hash);
}
