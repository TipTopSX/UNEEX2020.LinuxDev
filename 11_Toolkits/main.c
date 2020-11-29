#include <glib.h>
#include <glib/gprintf.h>

const int STR_MAX_LEN = 255;

int cmp(gpointer a, gpointer b, gpointer hash_table) {
    return (glong) (g_hash_table_lookup((GHashTable *) hash_table, b) -
                    g_hash_table_lookup((GHashTable *) hash_table, a));
}

void print(gpointer key, gpointer hash_table) {
    g_printf("'%s': %ld\n", (gchar *) key, (glong) g_hash_table_lookup(hash_table, (gchar *) key));
}

int main(void) {
    char input[STR_MAX_LEN];
    GHashTable *hash_table = g_hash_table_new(g_int_hash, g_str_equal);

    while (fgets(input, STR_MAX_LEN, stdin)) {
        gchar **words = g_strsplit(input, " ", -1);
        for (gchar **it = words; *it; ++it) {
            for (gchar *chr = *it; *chr; ++chr) {
                if (!g_ascii_isgraph(*chr)) {
                    *chr = '\0';
                    break;
                }
            }
            if (g_strcmp0("", *it)) {
                if (g_hash_table_lookup(hash_table, *it)) {
                    g_hash_table_insert(hash_table, *it, (void *) ((glong) g_hash_table_lookup(hash_table, *it) + 1));
                } else {
                    g_hash_table_insert(hash_table, *it, (void *) (1));
                }
            }
        }
        g_free(words);
    }
    GList *final_words = g_hash_table_get_keys(hash_table);
    final_words = g_list_sort_with_data(final_words, (GCompareDataFunc) cmp, hash_table);
    g_list_foreach(final_words, print, hash_table);
    g_hash_table_destroy(hash_table);
    g_list_free(final_words);
}
