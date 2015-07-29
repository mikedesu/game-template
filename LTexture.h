#ifndef _LTEXTURE_H
#define _LTEXTURE_H
#include <string>
#include <SDL2/SDL.h>
#include <SDL_image/SDL_image.h>
#include <SDL_ttf/SDL_ttf.h>
class LTexture {
    public:
        //Initializes variables
        LTexture();
        //Deallocates memory
        ~LTexture();
        //Loads image at specified path
        bool loadFromFile( std::string path );
        //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        //Deallocates texture
        void free();
        //Renders texture at given point
        void render( int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );
        //Set blend mode
        void setBlendMode( SDL_BlendMode blending );
        //Set alpha modulation
        void setAlpha( Uint8 alpha );
        //Gets image dimensions
        int getWidth();
        int getHeight();
        //Set the renderer
        void setRenderer( SDL_Renderer *renderer );
        //Set the font
        void setFont(TTF_Font *font);
        //Get the renderer
        SDL_Renderer *getRenderer();
        //Get the font
        TTF_Font *getFont();
    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        //The renderer
        SDL_Renderer *gRenderer;
        //The font (if used)
        TTF_Font *gFont;
        //Image dimensions
        int mWidth;
        int mHeight;
};
#endif

