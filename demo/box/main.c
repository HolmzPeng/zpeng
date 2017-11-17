#include <box.h>

/*KEYSTATE key_state;*/

int main(void)
{
    GameEngine box; 

    box_init(&box);
    
    /*SDL_SetEventFilter(event_filter);*/

    play_game(&box);

    box_exit(&box);
    
    return 0;
}
