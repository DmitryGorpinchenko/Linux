#include <stdio.h>
#include <dlfcn.h>

void *init(const char *libname, const char *fname);

int (*f)(int);

int main(int argc, char *argv[])
{
    void *hdl = init(argv[1], argv[2]);

    if (hdl == NULL) {
        printf("ERROR: Plug-in has not been loaded correctly!\n");
        return -1;
    }

    int n;
    sscanf(argv[3], "%d", &n);
    printf("%d\n", f(n));

    dlclose(hdl);
    return 0;
}

void *init(const char *libname, const char *fname)
{
    void *hdl = dlopen(libname, RTLD_LAZY);

    if (hdl == NULL) {
        return NULL;
    }

    f = dlsym(hdl, fname);

    if (f == NULL) {
        dlclose(hdl);
        return NULL;
    }
    return hdl;
}

