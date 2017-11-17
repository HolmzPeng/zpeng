#include <play.h>

int main(int argc, char *argv[])
{
    struct play_t *play = NULL;

    play = play_init(NULL);
    if (play == NULL)
        return 0;

    play_run(play);

    play_exit(play);

    return 0;
}
