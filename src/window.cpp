#include "window.h"
#include <string.h>

Window::Window()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log( "SDL could not initialize! SDL error: %s\n", SDL_GetError() );
    }else{
        if(window = SDL_CreateWindow("GB Emulator", 160, 144, 0); window = nullptr)
        {
            SDL_Log( "Window could not be created! SDL error: %s\n", SDL_GetError() );
        }else
        {
            windowSurface = SDL_GetWindowSurface(window);
        }
        
    }

    //The event data
    SDL_Event e;
    SDL_zero( e );
    
    //The main loop
    while( quit == false )
    {
        //Get event data
        while( SDL_PollEvent( &e ) == true )
        {
            //If event is quit type
            if( e.type == SDL_EVENT_QUIT )
            {
                //End the main loop
                quit = true;
            }
        }

        //Fill the surface white
        SDL_FillSurfaceRect( windowSurface, nullptr, SDL_MapSurfaceRGB( windowSurface, 0xFF, 0xFF, 0xFF ) );
    
        //Update the surface
        SDL_UpdateWindowSurface( window );
    } 
        

}