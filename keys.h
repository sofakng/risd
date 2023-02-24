struct key_lookup
{
    char name[20];
    int code;
};

#define KEYLIST_SIZE 100

struct key_lookup create_key (char* name, int code);
void fill_keylist();
int find_key (char* name);

extern struct key_lookup keys[KEYLIST_SIZE];