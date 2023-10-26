/*
 * Squarefighter - initial implementation in C
 * using libSDL2 for graphics & input
 * using chipmunk-physics for physics :P
 *
 * (c)copyright 2023 by Gerald Wodni <gerald.wodni@gmail.com>
 * */
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define SDL_INIT_MODULES (SDL_INIT_VIDEO)

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    int width;
    int height;
    char *title;
} Context;

void bailOut( char *message ) {
    printf( "Fatal Error: %s\nSDL:%s\n", message, SDL_GetError() );
    exit(1);
}

void initSDL( Context *context ) {
    if( SDL_Init( SDL_INIT_MODULES ) < 0 )
        bailOut( "Could not initialize SDL." );

    context->window = SDL_CreateWindow( context->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, context->width, context->height, 0 );
    if( context->window == NULL )
        bailOut( "Could not create Window." );
    
    context->renderer = SDL_CreateRenderer( context->window, -1, SDL_RENDERER_ACCELERATED );
    if( context->renderer == NULL )
        bailOut( "Could not create Renderer." );

    context->surface = SDL_GetWindowSurface( context->window );
    SDL_SetRenderDrawColor( context->renderer, 0x00, 0x80, 0x00, 0xFF );
}

void update( Context *context ) {
}

void render( Context *context ) {
}

int main( int argc, char **argv ) {
    Context context;

    char *title = argv[0];
    if( strncmp( title, "./", 2 ) == 0 )
        title += 2;
    if( title[0] > 'A' )
        title[0] -= 'a' - 'A';

    context.title = title;
    context.width = 800;
    context.height = 600;

    printf("Pre-init" );
    initSDL( &context );
    printf("Post-init" );
    SDL_Delay( 1000 );
    printf("Done" );
    return 0;
}

