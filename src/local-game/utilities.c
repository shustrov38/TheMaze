#include "utilities.h"


int Draw_rectangle(SDL_Surface *dest_surface, size_t block_type, size_t x, size_t y, size_t w, size_t h) {
    SDL_Rect rect;

    if (dest_surface == NULL) return -1;

    rect.x = x;
    rect.y = y;
    rect.w = w + 1;
    rect.h = h + 1;
    SDL_FillRect(dest_surface, &rect, SDL_MapRGB(dest_surface->format, BORDER_COLOR_R, BORDER_COLOR_G, BORDER_COLOR_B));

    rect.x = x + 1;
    rect.y = y + 1;
    rect.w = w - 1;
    rect.h = h - 1;

    if (GOLD_BLOCK == block_type)
        SDL_FillRect(dest_surface, &rect, SDL_MapRGB(dest_surface->format, GOLD_COLOR_R, GOLD_COLOR_G, GOLD_COLOR_B));
    if (GRAY_BLOCK == block_type)
        SDL_FillRect(dest_surface, &rect, SDL_MapRGB(dest_surface->format, GRAY_COLOR_R, GRAY_COLOR_G, GRAY_COLOR_B));
    if (BLUE_BLOCK == block_type)
        SDL_FillRect(dest_surface, &rect, SDL_MapRGB(dest_surface->format, BLUE_COLOR_R, BLUE_COLOR_G, BLUE_COLOR_B));

    dest_surface = NULL;
    return 0;
}

int Init_window(const char *title, int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return -1;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if (window == NULL) return -1;
    screen = SDL_GetWindowSurface(window);

    return 0;
}

SDL_Surface *Get_screen(void) {
    return screen;
}

SDL_Window *Get_window(void) {
    return window;
}

void CleanUp_window(void) {
    if (NULL != window) SDL_DestroyWindow(window);
    window = NULL;
}

void CleanUp_screen(void) {
    if (NULL != screen) SDL_FreeSurface(screen);
    screen = NULL;
}

int Update_window(void) {
    if (NULL == window) return -1;
    else return SDL_UpdateWindowSurface(window);
}

int Update_window_rect(int x, int y, unsigned int w, unsigned int h) {
    SDL_Rect rect;

    if (window == NULL) return -1;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return SDL_UpdateWindowSurfaceRects(window, &rect, 1);
}


SDL_Surface **initICONS() {
    SDL_Surface **ICONS = (SDL_Surface **) malloc(15 * sizeof(SDL_Surface *));

    const int ICON_SIZE = 21;

    ICONS[0] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_anti-mage.png"), ICON_SIZE, ICON_SIZE);

    ICONS[1] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_bristleback.png"), ICON_SIZE, ICON_SIZE);

    ICONS[2] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_invoker.png"), ICON_SIZE, ICON_SIZE);

    ICONS[3] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_juggernaut.png"), ICON_SIZE, ICON_SIZE);

    ICONS[4] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_mk.png"), ICON_SIZE, ICON_SIZE);

    ICONS[5] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_morph.png"), ICON_SIZE, ICON_SIZE);

    ICONS[6] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_phantom-assasin.png"), ICON_SIZE, ICON_SIZE);

    ICONS[7] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_pidor.png"), ICON_SIZE, ICON_SIZE);

    ICONS[8] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_queen-of-pain.png"), ICON_SIZE, ICON_SIZE);

    ICONS[9] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_rudge.png"), ICON_SIZE, ICON_SIZE);

    ICONS[10] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_shadow-fiend.png"), ICON_SIZE, ICON_SIZE);

    ICONS[11] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_templar_assassin.png"), ICON_SIZE, ICON_SIZE);

    ICONS[12] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_void-spirit.png"), ICON_SIZE, ICON_SIZE);

    ICONS[13] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_zeus.png"), ICON_SIZE, ICON_SIZE);

    ICONS[14] = ScaleSurface(Load_img(
            "../../../src/local-game/Textures/hero_windranger.png"), ICON_SIZE, ICON_SIZE);

    return ICONS;
}


SDL_Surface *Load_img(const char *filename) {
    /*The img that's loaded*/
    SDL_Surface *loadedImage = NULL;

    /*Initialize PNG loading*/
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }

    /*Load the img using SDL_image*/
    loadedImage = IMG_Load(filename);

    /*If the image loaded*/
    if (loadedImage == NULL)
        printf("Error opening image %s\n", filename);

    /*Return the optimized image*/
    return loadedImage;
}

static SDL_Surface *ScaledCopy(SDL_Surface *src, SDL_Rect *dstSize) {
    SDL_Surface *scaledCopy = SDL_CreateRGBSurface(0,
                                                   dstSize->w, dstSize->h,
                                                   src->format->BitsPerPixel,
                                                   src->format->Rmask, src->format->Gmask, src->format->Bmask,
                                                   src->format->Amask);

    // Get the old mode
    SDL_BlendMode oldBlendMode;
    SDL_GetSurfaceBlendMode(src, &oldBlendMode);

    // Set the new mode so copying the source won't change the source
    SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);

    // Do the copy
    if (SDL_BlitScaled(src, NULL, scaledCopy, dstSize) != 0) {

    }

    // Restore the original blending mode
    SDL_SetSurfaceBlendMode(src, oldBlendMode);
    return scaledCopy;
}

SDL_Surface *ScaleSurface(SDL_Surface *image, size_t width, size_t height) {
    SDL_Surface *scaled_background = SDL_CreateRGBSurface(0,
                                                          width, height,
                                                          image->format->BitsPerPixel,
                                                          image->format->Rmask, image->format->Gmask,
                                                          image->format->Bmask,
                                                          image->format->Amask);

    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = width;
    stretchRect.h = height;
    SDL_BlitScaled(image, NULL, scaled_background, &stretchRect);

    return scaled_background;
}

int Draw_image(SDL_Surface *dest_surface, SDL_Surface *src_surface, size_t x, size_t y) {
    SDL_Rect dest_rect;

    if (dest_surface == NULL || src_surface == NULL) return -1;

    dest_rect.w = 0;
    dest_rect.h = 0;
    dest_rect.x = x;
    dest_rect.y = y;

    SDL_BlitSurface(src_surface, NULL, dest_surface, &dest_rect);

    return 0;
}

void WriteText(int x, int y, char text[100], int sz, int r, int g, int b) {
    SDL_Color clr; // Тип для цвета. 4 числа — R, G, B и A, соответственно.
    clr.r = r;
    clr.g = g;  // Зададим параметры цвета
    clr.b = b;
    TTF_Font *fnt = TTF_OpenFont("../../../src/local-game/Fonts/impact.ttf",
                                 sz); // Загружаем шрифт по заданному адресу размером sz
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_Surface *TextSurface = TTF_RenderText_Blended(fnt, text,
                                                      clr); // Переносим на поверхность текст с заданным шрифтом и цветом
    SDL_BlitSurface(TextSurface, NULL, screen, &dest);
    SDL_FreeSurface(TextSurface); // Освобождаем память уже ненужной поверхности
    TTF_CloseFont(fnt); // Закрываем шрифт
}