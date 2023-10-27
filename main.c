/*
 * Squarefighter - initial implementation in C
 * fighting squares with nothing but squares,
 * say no to fancy models!
 *
 * using libSDL2 for graphics & input
 * using chipmunk-physics for physics :P
 *
 * (c)copyright 2023 by Gerald Wodni <gerald.wodni@gmail.com>
 * */
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chipmunk/chipmunk.h>

#define SDL_INIT_MODULES (SDL_INIT_VIDEO)
#define IMG_INIT_MODULES (IMG_INIT_JPG | IMG_INIT_PNG)

#define max(x,y) ( x<y ? y : x )
#define min(x,y) ( x<y ? x : y )

#define BULLETS 10
#define BLOCKS 20
#define PHYSICS_SPS 60

typedef struct {
    SDL_Texture *texture;
    int x;
    int y;
    int w;
    int h;
    float fx;
    float fy;

    int dynamic;
    int enabled;
    int ttlEnabled;
    float ttl;
} Entity;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    int width;
    int height;
    char *title;
    int running;
    const unsigned char *keys;
    unsigned char oldKeys[SDL_NUM_SCANCODES];

    Entity *player;
    Entity *bullets[BULLETS];
    Entity *blocks[BLOCKS];

    cpSpace space;
} Context;

void bailOut( char *message ) {
    printf( "Fatal Error: %s\nSDL:%s\n", message, SDL_GetError() );
    exit(1);
}

void initSDL( Context *context ) {
    if( SDL_Init( SDL_INIT_MODULES ) < 0 )
        bailOut( "Could not initialize SDL." );

    if( IMG_Init( IMG_INIT_MODULES ) != IMG_INIT_MODULES )
        bailOut( "Could not initialize SDL-IMG." );

    context->window = SDL_CreateWindow( context->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, context->width, context->height, 0 );
    if( context->window == NULL )
        bailOut( "Could not create Window." );

    context->renderer = SDL_CreateRenderer( context->window, -1, SDL_RENDERER_ACCELERATED );
    if( context->renderer == NULL )
        bailOut( "Could not create Renderer." );

    context->surface = SDL_GetWindowSurface( context->window );
}

SDL_Surface *loadImage( Context *context, char *imageFilename ) {
    SDL_Surface *optimizedSurface, *loadedSurface;

    loadedSurface = IMG_Load( imageFilename );
    if( !loadedSurface ) {
        printf( "Could not load image %s\n", imageFilename );
        bailOut( "Error loading essential media." );
    }

    optimizedSurface = SDL_ConvertSurface( loadedSurface, context->surface->format, 0 );
    if( !optimizedSurface ) {
        printf( "Could not convert surface %s, using original (performance impact!).", imageFilename );
        return loadedSurface;
    }

    return optimizedSurface;
}

Entity *loadEntityEx( Context *context, char *imageFilename, SDL_Texture *texture ) {
    //SDL_Surface *surface = loadImage(
    Entity *entity = calloc( 1, sizeof(Entity) );

    if( texture == NULL )
        entity->texture = IMG_LoadTexture( context->renderer, imageFilename );
    else
        entity->texture = texture;

    if( !entity->texture ) {
        printf( "Could not load texture %s\n", imageFilename );
        bailOut( "Error loading essential media." );
    }

    SDL_QueryTexture( entity->texture, NULL, NULL, &entity->w, &entity->h );

    entity->enabled = -1;
    entity->dynamic = 0;

    return entity;
}
Entity *loadEntity( Context *context, char *imageFilename ) {
    return loadEntityEx( context, imageFilename, NULL );
}

void shoot( Context *context ) {
    Entity *bullet = context->bullets[0];
    int i = 0;
    while( bullet->enabled && ++i < BULLETS )
        bullet = context->bullets[i];

    if( i == BULLETS ) {
        printf( "Out of bullets, all active\n" );
        return;
    }

    bullet->x = context->player->x + context->player->w;
    bullet->y = context->player->y + context->player->h;
    bullet->enabled = -1;
    bullet->ttlEnabled = -1;
    bullet->ttl = 10;
    bullet->dynamic = -1;
}

void updateEntity( Context *context, Entity *entity, float delta ) {
    if( !entity->enabled || !entity->dynamic )
        return;

    if( entity->ttlEnabled )
        entity->ttl -= delta;

    if( entity->ttl < 0 ) {
        entity->ttl = 0;
        entity->ttlEnabled = 0;
        entity->enabled = 0;
        return;
    }
}

void update( Context *context, float delta ) {
    if( context->keys[ SDL_SCANCODE_ESCAPE ] )
        context->running = 0;

    /* player movement */
    float playerSpeed = delta * 1000;
    if( context->keys[ SDL_SCANCODE_A ] || context->keys[ SDL_SCANCODE_LEFT ] )
        context->player->fx = max( 0, context->player->fx - playerSpeed );
    if( context->keys[ SDL_SCANCODE_D ] || context->keys[ SDL_SCANCODE_RIGHT ] )
        context->player->fx = min( context->width - context->player->w, context->player->fx + playerSpeed );

    if( context->keys[ SDL_SCANCODE_W ] || context->keys[ SDL_SCANCODE_UP ] )
        context->player->fy = max( 0, context->player->fy - playerSpeed );
    if( context->keys[ SDL_SCANCODE_S ] || context->keys[ SDL_SCANCODE_DOWN ] )
        context->player->fy = min( context->height - context->player->h, context->player->fy + playerSpeed );

    if( context->keys[ SDL_SCANCODE_RETURN ] && !context->oldKeys[ SDL_SCANCODE_RETURN ] )
        shoot( context );

    context->player->x = (int)context->player->fx;
    context->player->y = (int)context->player->fy;

    for( int i = 0; i < BULLETS; i++ )
        updateEntity( context, context->bullets[i], delta );
}

void updatePhysics( Context *context, float timeStep ) {
    for( int i = 0; i <BULLETS; i++ ) {

    }
}

SDL_Surface *image;
SDL_Texture *texture;

void drawEntity( Context *context, Entity *entity ) {
    if( !entity->enabled )
        return;
    SDL_Rect targetRect = { entity->x, entity->y, entity->w, entity->h };
    SDL_RenderCopyEx( context->renderer, entity->texture, NULL, &targetRect, 0.0, NULL, SDL_FLIP_NONE );
}

void draw( Context *context ){
    SDL_SetRenderDrawColor( context->renderer, 0xFF, 0xFF, 0x00, 0xFF );
    SDL_Rect rect = { 10, 10, 200, 100 };
    SDL_RenderFillRect( context->renderer, &rect );

    SDL_Rect targetRect = { 10, 150, 64, 64 };

    SDL_RenderCopyEx( context->renderer, texture, NULL, &targetRect, 0.0, NULL, SDL_FLIP_NONE );

    drawEntity( context, context->player );
    for( int i = 0; i < BULLETS; i++ )
        drawEntity( context, context->bullets[i] );
    for( int i = 0; i < BLOCKS; i++ )
        drawEntity( context, context->blocks[i] );
}

void render( Context *context ) {
    SDL_SetRenderDrawColor( context->renderer, 0x00, 0x80, 0x00, 0xFF );
    SDL_RenderClear( context->renderer );

    draw( context );

    SDL_RenderPresent( context->renderer );
}

int main( int argc, char **argv ) {
    Context context;
    context.running = -1;

    char *title = argv[0];
    if( strncmp( title, "./", 2 ) == 0 )
        title += 2;
    if( title[0] > 'Z' )
        title[0] -= 'a' - 'A';

    context.title = title;
    context.width = 800;
    context.height = 600;

    printf("Pre-init" );
    initSDL( &context );
    printf("Post-init" );

    image = loadImage( &context, "media/player.png" );
    texture = IMG_LoadTexture( context.renderer, "media/player.png" );


    /* initialization */
    context.player = loadEntity( &context, "media/player.png" );
    context.player->x = 200;
    context.player->y = 200;

    SDL_Texture *lastTexture = NULL;
    for( int i = 0; i < BULLETS; i++ ) {
        context.bullets[i] = loadEntityEx( &context, "media/bullet.png", lastTexture );
        context.bullets[i]->enabled = 0;
        lastTexture = context.bullets[0]->texture;
    }
    lastTexture = NULL;
    for( int i = 0; i < BLOCKS; i++ ) {
        context.blocks[i] = loadEntityEx( &context, "media/block-purple.png", lastTexture );
        lastTexture = context.blocks[0]->texture;

        context.blocks[i]->x = context.blocks[i]->w*i;
        context.blocks[i]->y = context.height - context.blocks[i]->h;
    }


    Uint64 last, now=SDL_GetPerformanceCounter(), nextPhysics = now;
    char *windowTitle = calloc( 1, strlen(context.title) + 100 );

    while( context.running )  {
        SDL_Event event;
        while( SDL_PollEvent(&event) )
            switch( event.type ) {
                case SDL_QUIT:
                    context.running = 0;
                    break;

                default:
                    break;
            }
        context.keys = SDL_GetKeyboardState(NULL);

        last = now;
        now = SDL_GetPerformanceCounter();
        float delta = (float)(now-last) / (float)SDL_GetPerformanceFrequency();
        int fps = (int)(1.0f / delta);

        sprintf( windowTitle, "%s - FPS: %d", context.title, fps );
        //SDL_SetWindowTitle( context.window, windowTitle );

        update( &context, delta );
        if( now >= nextPhysics ) {
            nextPhysics += (float)SDL_GetPerformanceFrequency() / (float)PHYSICS_SPS;
            updatePhysics( &context, 1.0f / PHYSICS_SPS );
        }
        render( &context );

        memcpy( context.oldKeys, context.keys, SDL_NUM_SCANCODES );
    }

    printf("Cleaning up" );
    free( windowTitle );

    printf("Done" );

    return 0;
}

