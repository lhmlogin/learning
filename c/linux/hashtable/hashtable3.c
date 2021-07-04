

    #include <stdio.h>
    #include <string.h>
    #include <glib.h>
    #include <time.h>
    #include <assert.h>


    #define TIMES 20
    static void free_data(gpointer hash_data)
    {
        g_free(hash_data);
        hash_data = NULL;
    }

    void print_key_value(gpointer key, gpointer value ,gpointer user_data)
    {
        printf("%s -------------------->%s\n",(char*)key,(char*)value);
    }

    int hash_test_1()
    {
        GHashTable* name_score = NULL;
        int ret = 0;
        name_score = g_hash_table_new(g_str_hash,g_str_equal);
        if(name_score == NULL)
        {
            fprintf(stderr, "create hash table failed\n");
            return -1;
        }

        g_hash_table_insert(name_score,"Bean","77");
        g_hash_table_insert(name_score,"Ted","79");
        g_hash_table_insert(name_score,"Lucy","87");
        g_hash_table_insert(name_score,"Jim","90");
        g_hash_table_insert(name_score,"Candy","84");

        g_hash_table_foreach(name_score,print_key_value,NULL);
        char* Bean_score = g_hash_table_lookup(name_score,"Bean");
        if(Bean_score == NULL)
        {
            fprintf(stderr,"can not found Bean Score\n");
            ret = -2;
            goto exit;
        }
        printf("Bean\'s score = %s\n",(char*)Bean_score);

        printf("modify Bean\' Score to 86\n");
        g_hash_table_replace(name_score,"Bean","86");

        Bean_score = g_hash_table_lookup(name_score,"Bean");
        if(Bean_score == NULL)
        {
            fprintf(stderr,"can not found Bean Score after modify\n");
            ret = -2;
            goto exit;

        }

        printf("Bean\'s score = %s\n",Bean_score);
    exit:
        g_hash_table_destroy(name_score);

        return ret;

    }

    int main()
    {
         hash_test_1();
        // hash_test_2();
    } 