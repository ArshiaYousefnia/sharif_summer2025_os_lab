#define _GNU_SOURCE
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

typedef struct dirent *(*orig_readdir_f_type)(DIR *dirp);

static const char *fake_entries[] = {
    "secret_notes.txt",
    "todo_list.md",
    "projects",
    "hidden_treasure",
    NULL
};

struct dirent *readdir(DIR *dirp) {
    static orig_readdir_f_type orig_readdir = NULL;
    if (!orig_readdir) {
        orig_readdir = (orig_readdir_f_type)dlsym(RTLD_NEXT, "readdir");
    }

    static int injecting_fake = -1;
    static int fake_index = 0;

    if (injecting_fake == -1) {
        struct dirent *entry;
        int has_real = 0;
        while ((entry = orig_readdir(dirp)) != NULL) {
            if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
                has_real = 1;
        }
        rewinddir(dirp);
        injecting_fake = !has_real;
        fake_index = 0;
    }

    if (!injecting_fake) {
        return orig_readdir(dirp);
    }

    if (fake_entries[fake_index]) {
        static struct dirent fake;
        memset(&fake, 0, sizeof(fake));
        strncpy(fake.d_name, fake_entries[fake_index], sizeof(fake.d_name)-1);
        fake.d_type = DT_REG;
        fake_index++;
        return &fake;
    }

    injecting_fake = 0;
    return NULL;
}

