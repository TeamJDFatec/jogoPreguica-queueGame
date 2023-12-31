#include "raylib.h"
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define TAMANHO_BARALHO 52
#define TIPOS_CARTAS 13
#define CARTA_LARGURA 150
#define CARTA_ALTURA 200

#define OUROS_IMG "ouros.png"
#define COPAS_IMG "copas.png"

#define PONTOS_POR_FILA 10

const char *NAIPES_IMG[4] = {"img/ouros.png", "img/espada.png", "img/copas.png", "img/zap.png"};

typedef struct
{
    int numero;
    int x;
    int y;

} Baralho;

typedef struct
{
    char *valor;
    char *naipe;

} Carta;

typedef struct No {
    Carta *carta;
    struct No* proximo;
} No;


void desenhaFundo()
{

    ClearBackground(DARKGREEN);

}

void desenhaCartas(Carta carta, Texture2D naipe2D, Texture2D *logo, int posX, int posY, bool viradoPraCima)
{

    Rectangle rectangle = {posX, posY, CARTA_LARGURA, CARTA_ALTURA};
    Color cor;


    if (viradoPraCima)
    {

        if (strcmp(carta.valor, "") == 0)
            DrawRectangleLines(posX, posY, rectangle.width, rectangle.height, WHITE);
        else
        {
            DrawRectangleLines(rectangle.x - 1, rectangle.y - 1, rectangle.width + 2, rectangle.height + 2, BLACK);
            DrawRectangleRec(rectangle, WHITE);

            // Adicione uma borda
            DrawRectangleLinesEx(rectangle, 10, DARKBLUE);

        }

        if (strcmp(carta.naipe, "") != 0)
        {
            if (strcmp(carta.naipe, NAIPES_IMG[0]) == 0 || strcmp(carta.naipe, NAIPES_IMG[2]) == 0)
                cor = RED;
            else
                cor = BLACK;


            DrawTexture(naipe2D, posX + rectangle.width - 60, posY + 15, cor);
        }

        if (strcasecmp(carta.valor, "") != 0)
        {

            DrawText(carta.valor, posX + 15, posY + 15, 45, cor);
            DrawText(carta.valor, posX + (rectangle.width / 2) - 25, posY + (rectangle.height / 2) - 20, 105, cor);
        }

    }
    else
    {
        DrawRectangleLines(rectangle.x - 1, rectangle.y - 1, rectangle.width + 2, rectangle.height + 2, WHITE);
        DrawRectangleRec(rectangle, BLACK);
        DrawTexture(*logo, posX + 50, posY + 50, YELLOW);

        DrawRectangleLinesEx(rectangle, 10, YELLOW);
    }
}

void enfileirar(No** fila, Carta *carta) {
    No* aux, * novo = malloc(sizeof(No));

    if (novo) {
        novo->carta = carta;
        novo->proximo = NULL;

        if (*fila == NULL) {
            *fila = novo;
        }
        else {
            aux = *fila;

            while (aux->proximo) {
                aux = aux->proximo;
            }

            aux->proximo = novo;
        }
    }
    else {
        printf("\nErro ao alocar memoria!\n");
    }
}

No* desenfileirar(No** fila) {
    No* removido = NULL;

    if (*fila) {

        removido = *fila;
        *fila = removido->proximo;
    }

    return removido;
}

int retornaPosicaoNaipeTextura(Carta carta)
{

    if (strcmp(carta.naipe, NAIPES_IMG[0]) == 0)
        return 0;

    if (strcmp(carta.naipe, NAIPES_IMG[1]) == 0)
        return 1;

    if (strcmp(carta.naipe, NAIPES_IMG[2]) == 0)
        return 2;

    if (strcmp(carta.naipe, NAIPES_IMG[3]) == 0)
        return 3;
    else
        return -1;
}

void imprimir_fila(No** fila, Texture2D naipesTexture[], Baralho *baralho, Texture2D* logo, int limite, bool viradoPraCima) {
    No* aux = NULL;
    Texture2D naipe2D;

    int contador = 0;
    int espacoY = 0;
    int espacoX = 0;
    int tamanho_fila = 0;

    if (*fila) {
        aux = *fila;
        while(aux) {
            tamanho_fila++;
            aux = aux->proximo;
        }

        aux = *fila;
        Carta *cartasTemporarias[tamanho_fila];
        int i = 0;

        while(aux) {
            cartasTemporarias[i] = aux->carta;
            i++;
            aux = aux->proximo;
        }

        aux = *fila;
        int j = 0; //3 = ultima posi��o do vetor
        if(tamanho_fila >= limite) {
            j = limite -1;
        }
        else {
            j = tamanho_fila -1;
        }
        int aux_tamanho = j;

        while (j >= 0 && contador <= limite) {

            naipe2D = naipesTexture[retornaPosicaoNaipeTextura(*cartasTemporarias[j])];
            if(baralho->numero == 0) {
                if(aux_tamanho >= 2) {
                    desenhaCartas(*cartasTemporarias[j], naipe2D, logo, baralho->x + espacoX + 200, baralho->y + espacoY, viradoPraCima);
                }
                else if(aux_tamanho == 1) {
                    desenhaCartas(*cartasTemporarias[j], naipe2D, logo, baralho->x + espacoX + 100, baralho->y + espacoY, viradoPraCima);
                }
                else if(aux_tamanho == 0) {
                    desenhaCartas(*cartasTemporarias[j], naipe2D, logo, baralho->x + espacoX, baralho->y + espacoY, viradoPraCima);
                }
            }
            else{
                desenhaCartas(*cartasTemporarias[j], naipe2D, logo, baralho->x + espacoX, baralho->y + espacoY, viradoPraCima);
            }

            aux = aux->proximo;

            if (baralho->numero != 0)
                espacoY += 50;
            else
                espacoX -= 100;

            j--;
            contador++;
        }
    }
    else {
        printf("\n#");
    }
}

void organizarBaralho(Carta* baralho) {
    char *cartas[TIPOS_CARTAS] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Q", "J", "K"};
    int indice = 0;

    for (int i = 0; i < TIPOS_CARTAS; i++) {
        for (int j = 0; j < 4; j++) {
            baralho[indice].valor = cartas[i];
            baralho[indice].naipe = NAIPES_IMG[j];
            indice++;
        }
    }
}

void desenhaColunasCartas(Texture2D naipesTextura[], Baralho baralhos[])
{

    Carta carta;
    carta.valor = "";
    carta.naipe = "";

    int qtdeColunas = 6;
    int recuo;
    int x = baralhos[0].x;

    for (int i = 0; i < qtdeColunas; i++)
    {

        if (i == 0)
            recuo = 0;
        else
            recuo = 150;

        desenhaCartas(carta, naipesTextura[i], NULL, x, baralhos[i].y, true);

        x += i != 0 ? CARTA_LARGURA + recuo : 0;
    }

}

int buscaDado(int* lista, int valor) {
    for (int i = 0; i < TAMANHO_BARALHO; i++) {
        if (lista[i] == valor) {
            return 1;
        }
    }
    return 0;
}

void criaListaAleatoria(int* valSorteados) {
    int aleatorio;
    int jaExiste = 0;

    // Setup da matriz
    for (int i = 0; i < TAMANHO_BARALHO; i++) {
        valSorteados[i] = -1;
    }

    // Sorteia valores diferentes para cada posi��o
    for (int i = 0; i < TAMANHO_BARALHO; i++) {
        aleatorio = rand() % 52;
        jaExiste = buscaDado(valSorteados, aleatorio);

        if (jaExiste) {
            i--;
        }
        else {
            valSorteados[i] = aleatorio;
        }
    }
}

void embaralhar(No** fila, Carta* baralho) {
    int valSorteados[TAMANHO_BARALHO];
    srand(time(NULL));

    criaListaAleatoria(valSorteados);

    for (int i = 0; i < TAMANHO_BARALHO; i++) {
        enfileirar(fila, &baralho[valSorteados[i]]);
    }
}

No* ultimoValorFila(No** fila) {
    No* aux = *fila;

    while(aux && aux->proximo) {
        aux = aux->proximo;
    }

    return aux;
}


int transformaValorCharEmInteiro(char* valor)
{
    char* valoresChar[TIPOS_CARTAS] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    int valoresInt[TIPOS_CARTAS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

    for (int i = 0; i < TIPOS_CARTAS; i++)
    {
        if (strcmp(valor, valoresChar[i]) == 0)
            return valoresInt[i];
    }

    return -1;
}

int comparaValores(char* valor1, char* valor2)
{

    int valor1Int = transformaValorCharEmInteiro(valor1);
    int valor2Int = transformaValorCharEmInteiro(valor2);

    if (valor1Int == valor2Int) return 0;
    if (valor1Int < valor2Int) return -1;
    if (valor1Int > valor2Int) return 1;
}

int moverCarta(No** filaOrigem, No** filaDestino) {
    No* cartaMovida = *filaOrigem;
    No* ultimoNo = ultimoValorFila(filaDestino);


    if (cartaMovida) {
        if (!ultimoNo || (comparaValores(cartaMovida->carta->valor, ultimoNo->carta->valor) == -1 && strcmp(cartaMovida->carta->naipe, ultimoNo->carta->naipe) == 0)) {


            cartaMovida = desenfileirar(filaOrigem);
            enfileirar(filaDestino, cartaMovida->carta);
            return 1;
        } else {
            printf("\nMovimento inv�lido para mover a carta!\n");
        }
    } else {
        printf("\nFila de origem vazia. Escolha outra fila.\n");
    }
    return 0;
}

int verificaMovimentoPossivel(No** filaOrigem, No** filaDestino) {
    No* cartaMovida = *filaOrigem;
    No* ultimoNo = ultimoValorFila(filaDestino);
    if (cartaMovida) {
        if (!ultimoNo || (comparaValores(cartaMovida->carta->valor, ultimoNo->carta->valor) == -1 && strcmp(cartaMovida->carta->naipe, ultimoNo->carta->naipe) == 0)) {
            return 1;
        }
    }
    return 0;
}

int gameOver(No **filaBaralho, No **fila1, No **fila2, No **fila3, No **fila4, No **fila5) {

    No *aux0 = *filaBaralho;
    No *aux1 = *fila1;
    No *aux2 = *fila2;
    No *aux3 = *fila3;
    No *aux4 = *fila4;
    No *aux5 = *fila5;
    No *lista[6] = {aux0, aux1, aux2, aux3, aux4, aux5};

    int contaMovPossiveis = 0;

    for(int i = 0; i <= 5; i++) {
        for(int j = 0; j <= 5; j++) {
            if(i == j || j == 0) {
                continue;
            }
            else {
                contaMovPossiveis += verificaMovimentoPossivel(&lista[i], &lista[j]);
            }
        }
    }

    return contaMovPossiveis;
}

int winGame(No **filaBaralho, No **fila1, No **fila2, No **fila3, No **fila4, No **fila5) {
    No *auxBaralho = *filaBaralho;
    No *aux1 = *fila1;
    No *aux2 = *fila2;
    No *aux3 = *fila3;
    No *aux4 = *fila4;
    No *aux5 = *fila5;
    No *lista[6] = {auxBaralho, aux1, aux2, aux3, aux4, aux5};

    int tamanho_fila_baralho = 0;
    int tamanho_fila1 = 0;
    int tamanho_fila2 = 0;
    int tamanho_fila3 = 0;
    int tamanho_fila4 = 0;
    int tamanho_fila5 = 0;
    int listaTam[6] = {tamanho_fila_baralho, tamanho_fila1, tamanho_fila2, tamanho_fila3, tamanho_fila4, tamanho_fila5};

    for(int i = 0; i < 6; i++) {
        while(lista[i]) {
            listaTam[i]++;
            lista[i] = lista[i]->proximo;
        }
    }

    if(listaTam[0] == 0 && ((listaTam[1] == 13 && listaTam[2] == 13 && listaTam[3] == 13 && listaTam[4] == 13 && listaTam[5] == 0) ||
       (listaTam[1] == 13 && listaTam[2] == 13 && listaTam[3] == 13 && listaTam[4] == 0 && listaTam[5] == 13) ||
       (listaTam[1] == 13 && listaTam[2] == 13 && listaTam[3] == 0 && listaTam[4] == 13 && listaTam[5] == 13) ||
       (listaTam[1] == 13 && listaTam[2] == 0 && listaTam[3] == 13 && listaTam[4] == 13 && listaTam[5] == 13) ||
       (listaTam[1] == 0 && listaTam[2] == 13 && listaTam[3] == 13 && listaTam[4] == 13 && listaTam[5] == 13))) {
        return 0;
    }
    return 1;
}

int pontuacao(No **filaBaralho, No **fila1, No **fila2, No **fila3, No **fila4, No **fila5, int *passou1, int *passou2, int *passou3, int *passou4, int *passou5) {
    No *auxBaralho = *filaBaralho;
    No *aux1 = *fila1;
    No *aux2 = *fila2;
    No *aux3 = *fila3;
    No *aux4 = *fila4;
    No *aux5 = *fila5;
    No *lista[6] = {auxBaralho, aux1, aux2, aux3, aux4, aux5};

    int tamanho_fila_baralho = 0;
    int tamanho_fila1 = 0;
    int tamanho_fila2 = 0;
    int tamanho_fila3 = 0;
    int tamanho_fila4 = 0;
    int tamanho_fila5 = 0;
    int listaTam[6] = {tamanho_fila_baralho, tamanho_fila1, tamanho_fila2, tamanho_fila3, tamanho_fila4, tamanho_fila5};

    int pontos = 0;

    for(int i = 0; i < 6; i++) {
        while(lista[i]) {
            listaTam[i]++;
            lista[i] = lista[i]->proximo;
        }
    }

    for(int i = 0; i < 6; i++) {
        if(listaTam[i] == 13) {
            pontos++;
        }
    }

    return pontos;
}

void criaTexturaNaipes(Texture2D naipesTextura[])
{

    for (int i = 0; i < 4; i++)
    {
        naipesTextura[i] = LoadTexture(NAIPES_IMG[i]);
    }
}

void destroiTexturasNaipes(Texture2D naipesTextura[])
{
    for (int i = 0; i < 4; i++)
    {
        UnloadTexture(naipesTextura[i]);
    }
}

void pausarJogo()
{


}

Baralho *pegaBaralhoPorPosicao(Baralho baralhosPossiveis[], int posX, int posY)
{

    printf("posX = %d, posY = %d", posX, posY);

    Rectangle areaBaralho;

    for (int i = 0; i < 6; i++)
    {
        areaBaralho = (Rectangle){baralhosPossiveis[i].x, baralhosPossiveis[i].y, CARTA_LARGURA, CARTA_ALTURA};
        Vector2 ponto = {posX, posY};

        if(CheckCollisionPointRec(ponto, areaBaralho))
        {
            printf("RETORNOU\n");
            printf("baralho id = %d", baralhosPossiveis[i].numero);
            return &baralhosPossiveis[i];
        }


    }

    printf("� NULO");

    return NULL;
}

void jogo()
{

    Baralho baralhosPossiveis[6] = {

        (Baralho){0, 35, 25},
        (Baralho){1, 35, 300},
        (Baralho){2, 335, 300},
        (Baralho){3, 635, 300},
        (Baralho){4, 935, 300},
        (Baralho){5, 1235, 300}
    };


    Carta *cartaEmMovimento = NULL;
    Baralho *baralhoOrigem = NULL, *baralhoDestino = NULL;

    Texture2D naipesTextura[4];
    Texture2D logo = LoadTexture("logo100.png");
    criaTexturaNaipes(naipesTextura);

    bool jogando = true, pausado = false, movendoCarta = false;
    bool emGameOver = false;
    bool ganhou = false;
    bool exibeMensagemErro = false;
    int baralhoVazio = TAMANHO_BARALHO;

    //pontuacao
    int pontosTotais = 0;
    int passou1 = 0;
    int passou2 = 0;
    int passou3 = 0;
    int passou4 = 0;
    int passou5 = 0;

    Carta baralho[TAMANHO_BARALHO];
    organizarBaralho(baralho);

    No* filaBaralho = NULL;
    embaralhar(&filaBaralho, baralho);

    No* fila1 = NULL;
    No* fila2 = NULL;
    No* fila3 = NULL;
    No* fila4 = NULL;
    No* fila5 = NULL;

    while (!WindowShouldClose())
    {

        BeginDrawing();

            emGameOver = gameOver(&filaBaralho, &fila1, &fila2, &fila3, &fila4, &fila5) == 0;
            ganhou = winGame(&filaBaralho, &fila1, &fila2, &fila3, &fila4, &fila5) == 0;

            desenhaFundo();
            desenhaColunasCartas(naipesTextura, baralhosPossiveis);

            DrawText(TextFormat("QTD Cartas restantes: %d", baralhoVazio), 650, 25, 40, BLACK); // EXIBE CARTAS RESTANTES

            pontosTotais = pontuacao(&filaBaralho, &fila1, &fila2, &fila3, &fila4, &fila5, &passou1, &passou2, &passou3, &passou4, &passou5) * PONTOS_POR_FILA;
            DrawText(TextFormat("Pontuacao: %d", pontosTotais), 1500, 25, 50, BLUE); // EXIBE PONTUA��O

            imprimir_fila(&filaBaralho, naipesTextura, &baralhosPossiveis[0], &logo, 3, true);

            imprimir_fila(&fila1, naipesTextura, &baralhosPossiveis[1], NULL, 10, true);
            imprimir_fila(&fila2, naipesTextura, &baralhosPossiveis[2], NULL, 10, true);
            imprimir_fila(&fila3, naipesTextura, &baralhosPossiveis[3], NULL, 10, true);
            imprimir_fila(&fila4, naipesTextura, &baralhosPossiveis[4], NULL, 10, true);
            imprimir_fila(&fila5, naipesTextura, &baralhosPossiveis[5], NULL, 10, true);

            if(exibeMensagemErro) {
                DrawTextEx(LoadFont("fontes/PlatinumSignOver.ttf"), "MOVIMENTO INVALIDO", (Vector2){(GetMonitorWidth(0) / 2) - 600, (GetMonitorHeight(0) / 2)}, 50, 50, BLACK);

                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    exibeMensagemErro = false;
            }
            if (ganhou)
            {

                //DrawTextEx(LoadFont("fontes/PlatinumSignOver.ttf"), "GANHOU!", (Vector2){(GetMonitorWidth(0) / 2) - 600, (GetMonitorHeight(0) / 2) - 100}, 200, 50, BLACK);
                DrawText("GANHOU!", (GetMonitorWidth(0) / 2) - 600, (GetMonitorHeight(0) / 2) - 100, 200, RED);


                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    break;
            }
            if (emGameOver)
            {

                //DrawTextEx(LoadFont("fontes/PlatinumSignOver.ttf"), "GAME OVER!", (Vector2){(GetMonitorWidth(0) / 2) - 600, (GetMonitorHeight(0) / 2) - 100}, 200, 50, BLACK);
                DrawText("GAME OVER!", (GetMonitorWidth(0) / 2) - 600, (GetMonitorHeight(0) / 2) - 100, 200, RED);


                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    break;
            }
            else
            {


                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {

                    if (baralhoOrigem == NULL)
                        baralhoOrigem = pegaBaralhoPorPosicao(baralhosPossiveis, GetMouseX(), GetMouseY());

                    if (baralhoOrigem != NULL)
                        movendoCarta = true;
                    else
                        movendoCarta = false;

                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    baralhoDestino = pegaBaralhoPorPosicao(baralhosPossiveis, GetMouseX(), GetMouseY());
                    movendoCarta = false;
                }


                if (movendoCarta)
                {

                    if (cartaEmMovimento == NULL)
                    {
                        switch(baralhoOrigem->numero)
                        {
                            case 0: cartaEmMovimento = filaBaralho == NULL ? NULL : filaBaralho->carta; break;
                            case 1: cartaEmMovimento = fila1 == NULL ? NULL : fila1->carta; break;
                            case 2: cartaEmMovimento = fila2 == NULL ? NULL : fila2->carta; break;
                            case 3: cartaEmMovimento = fila3 == NULL ? NULL : fila3->carta; break;
                            case 4: cartaEmMovimento = fila4 == NULL ? NULL : fila4->carta; break;
                            case 5: cartaEmMovimento = fila5 == NULL ? NULL : fila5->carta; break;
                            default: break;

                        }
                    }

                    if (cartaEmMovimento == NULL)
                        printf("\nCARTA NULA\n");
                    else
                        desenhaCartas(*cartaEmMovimento, naipesTextura[retornaPosicaoNaipeTextura(*cartaEmMovimento)], &logo, GetMouseX(), GetMouseY(), true);

                }
                else
                {

                    if (baralhoDestino && cartaEmMovimento)
                    {

                            switch (baralhoOrigem->numero)
                            {
                                case 0:
                                    switch (baralhoDestino->numero)
                                    {
                                        case 1: moverCarta(&filaBaralho, &fila1) == 1 ? baralhoVazio-- : 0; break;
                                        case 2: moverCarta(&filaBaralho, &fila2) == 1 ? baralhoVazio-- : 0; break;
                                        case 3: moverCarta(&filaBaralho, &fila3) == 1 ? baralhoVazio-- : 0; break;
                                        case 4: moverCarta(&filaBaralho, &fila4) == 1 ? baralhoVazio-- : 0; break;
                                        case 5: moverCarta(&filaBaralho, &fila5) == 1 ? baralhoVazio-- : 0; break;
                                        default: exibeMensagemErro = true; break;
                                    }
                                break;
                                case 1:
                                    switch (baralhoDestino->numero)
                                    {
                                        case 2: moverCarta(&fila1, &fila2); break;
                                        case 3: moverCarta(&fila1, &fila3); break;
                                        case 4: moverCarta(&fila1, &fila4); break;
                                        case 5: moverCarta(&fila1, &fila5); break;
                                        default: exibeMensagemErro = true; break;
                                    }
                                    break;
                                    case 2:
                                        switch (baralhoDestino->numero)
                                        {
                                            case 1: moverCarta(&fila2, &fila1); break;
                                            case 3: moverCarta(&fila2, &fila3); break;
                                            case 4: moverCarta(&fila2, &fila4); break;
                                            case 5: moverCarta(&fila2, &fila5); break;
                                            default: exibeMensagemErro = true; break;
                                        }
                                        break;
                                        case 3:
                                            switch (baralhoDestino->numero)
                                            {
                                                case 1: moverCarta(&fila3, &fila1); break;
                                                case 2: moverCarta(&fila3, &fila2); break;
                                                case 4: moverCarta(&fila3, &fila4); break;
                                                case 5: moverCarta(&fila3, &fila5); break;
                                                default: exibeMensagemErro = true; break;
                                            }
                                            break;
                                            case 4:
                                                switch (baralhoDestino->numero)
                                                {
                                                    case 1: moverCarta(&fila4, &fila1); break;
                                                    case 2: moverCarta(&fila4, &fila2); break;
                                                    case 3: moverCarta(&fila4, &fila3); break;
                                                    case 5: moverCarta(&fila4, &fila5); break;
                                                    default: exibeMensagemErro = true; break;
                                                }
                                                break;
                                                case 5:
                                                    switch (baralhoDestino->numero)
                                                    {
                                                        case 1: moverCarta(&fila5, &fila1); break;
                                                        case 2: moverCarta(&fila5, &fila2); break;
                                                        case 3: moverCarta(&fila5, &fila3); break;
                                                        case 4: moverCarta(&fila5, &fila4); break;
                                                        default: exibeMensagemErro = true; break;
                                                    }
                                                    break;
                                    default:
                                        exibeMensagemErro = true; break;
                                }
                    }

                    baralhoOrigem = NULL;
                    baralhoDestino = NULL;
                    cartaEmMovimento = NULL;
                }
            }

        EndDrawing();

    }

    destroiTexturasNaipes(naipesTextura);
    UnloadTexture(logo);

    CloseWindow();
}


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    setlocale(LC_ALL, "Portuguese");

    SetTraceLogLevel(LOG_ERROR);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1500, 800, "Preguica");
    MaximizeWindow();

    SetTargetFPS(60);


    //do
    //{

        jogo();

    //} while(true);


    return 0;
}
