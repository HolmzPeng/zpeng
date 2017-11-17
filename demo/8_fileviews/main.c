#include <views.h>

int main(int argc, char *argv[])
{
    struct views_t *views = NULL;
    char *path = "/kyo";
    int filter = 0;

    if (argc > 1)
    {
        path = argv[1];
        if (argc > 2)
            filter = atoi(argv[2]);
    }

    views = views_init(path, filter, NULL);
    if (views == NULL)
        return 0;

    views_run(views);

    views_exit(views);

    return 0;
}
