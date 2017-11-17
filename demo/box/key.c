#include <box.h>

int event_filter(const SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym)
            {
                case SDLK_LEFT:
                    key_state.left = 1;
                    break;
                case SDLK_RIGHT:
                    key_state.right = 1;
                    break;
                case SDLK_UP:
                    if (kye_state.rotate)
                        key_state.rotate = 0;
                    else
                        key_state.rotate = 1;
                    break;
                case SDLK_DOWN:
                    key_state.down = 1;
                    break;
                case SDLK_ESCAPE:
                    key_state.quit = 1;
                    break;
                case SDLK_SPACE:
                    if (key_state.pause)
                        key_state.pause = 0;
                    else
                        key_state.pause = 1;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (event->key.keysym.sym)
            {
                case SDLK_LEFT:
                    key_state.left = 0;
                    break;
                case SDLK_RIGHT:
                    key_state.right = 0;
                    break;
                case SDLK_DOWN:
                    key_state.down = 0;
                    break;
                default:
                    break;
            }
            break;
        case SDL_QUIT:
            key_state.quit = 1;
            break;
        default:
            return 1;
    }

    return 0;
}
