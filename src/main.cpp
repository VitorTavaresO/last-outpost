#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* args[]) {
    // Inicializa a SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar a SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Cria a janela
    SDL_Window* window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Erro ao criar a janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Cria o renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Erro ao criar o renderizador: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Define a cor de fundo (preto)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Define a cor do quadrado (vermelho)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Define o retângulo do quadrado
    SDL_Rect rect;
    rect.x = (SCREEN_WIDTH / 2) - 50;
    rect.y = (SCREEN_HEIGHT / 2) - 50;
    rect.w = 100;
    rect.h = 100;

    // Renderiza o quadrado
    SDL_RenderFillRect(renderer, &rect);

    // Atualiza a tela
    SDL_RenderPresent(renderer);

    // Espera 5 segundos
    SDL_Delay(5000);

    // Limpa e finaliza a SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}