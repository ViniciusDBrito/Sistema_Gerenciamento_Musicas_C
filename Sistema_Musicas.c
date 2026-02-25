#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structs Artista
typedef struct artista {
    int id;
    char nome[200];
    char genero[200];
} artista;

typedef struct artista_no {
    artista *artista;
    struct artista_no *prox;
} artista_no;

//Structs Musica
typedef struct musica {
    int id;
    char titulo[200];
    int id_artista;
    int duracao;
} musica;

//Nó AVL
typedef struct musica_no {
    musica *musica;
    struct musica_no *esq;
    struct musica_no *dir;
    int altura;
} musica_no;

//Struct Playlist circular 
typedef struct playlist_no {
    musica *musica;
    struct playlist_no *prox;
} playlist_no;

//Struct Conjunto de playlists
typedef struct ConjuntoPlaylists {
    int id;
    char nome[200];
    playlist_no *musicas;
    struct ConjuntoPlaylists *prox;
} ConjuntoPlaylists;


//Protótipos
artista_no* cria_artista();
void insere_artista(artista_no *ini, int id, char nome[], char genero[]);
void imprime_todos_artistas(artista_no *ini);
void remove_artista_e_musicas(artista_no *artistas, musica_no *raiz, ConjuntoPlaylists *playlists, int id_artista);

int altura(musica_no *no);
int max(int a, int b);
int fator(musica_no *no);
musica_no* rotacaoDireita(musica_no *y);
musica_no* rotacaoEsquerda(musica_no *x);
musica_no* menorDireita(musica_no *no);

musica_no* insere_musica_avl(musica_no *raiz, int id, char titulo[], int id_artista, int duracao);
musica_no* remove_musica_avl(musica_no *raiz, ConjuntoPlaylists *playlists, int id);
musica* busca_musica_por_id_avl(musica_no *raiz, int id);
void imprime_todas_musicas_avl(musica_no *raiz, artista_no *ini);
void imprime_musicas_artista_avl(musica_no *raiz, int id_artista);

playlist_no* cria_playlist();
void monta_playlist_circ(playlist_no **iniPlaylist, musica_no *raiz, char *listaIds);
ConjuntoPlaylists* cria_CPlaylists();
void monta_CPlaylists(ConjuntoPlaylists *ini, int id, char nome[], playlist_no *musicas);
void shuffle_playlist(playlist_no *playlist);
void imprime_playlist(ConjuntoPlaylists *p);
void remove_musicaPlaylist(ConjuntoPlaylists *iniP, int id);

int menuPrincipal();
int menuArtistas();
int menuMusicas();
int menuPlaylists();


//Implementação Artistas
artista_no* cria_artista() {
    artista_no* cabeca = malloc(sizeof(artista_no));
    cabeca->prox = NULL;
    return cabeca;
}

void insere_artista(artista_no *ini, int id, char nome[], char genero[]) {
    artista_no* novo = malloc(sizeof(artista_no));
    novo->artista = malloc(sizeof(artista));
    novo->artista->id = id;
    strcpy(novo->artista->nome, nome);
    strcpy(novo->artista->genero, genero);
    novo->prox = ini->prox;
    ini->prox = novo;
}

void imprime_todos_artistas(artista_no *ini) {
    artista_no *p = ini->prox;
    int cont = 1;
    while (p) {
        printf("\n---------- Artista %d -------------", cont);
        printf("\nId-Artista: %d ", p->artista->id);
        printf("\nNome: %s ", p->artista->nome);
        printf("\nGênero: %s \n", p->artista->genero);
        cont++;
        p = p->prox;
    }
}


//Implementação Musica com AVL
int altura(musica_no *no) {
    return (no == NULL ? 0 : no->altura);
}

int max(int a, int b) {
    return (a > b ? a : b);
}

int fator(musica_no *no) {
    if (no == NULL) return 0;
    return altura(no->esq) - altura(no->dir);
}


musica_no* rotacaoDireita(musica_no *y) {
    musica_no *x = y->esq;
    musica_no *t2 = x->dir;

    x->dir = y;
    y->esq = t2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

musica_no* rotacaoEsquerda(musica_no *x) {
    musica_no *y = x->dir;
    musica_no *t2 = y->esq;

    y->esq = x;
    x->dir = t2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}


musica_no* insere_musica_avl(musica_no *raiz, int id, char titulo[],
                              int id_artista, int duracao)
{
    if (raiz == NULL) {
        musica_no *novo = malloc(sizeof(musica_no));
        novo->musica = malloc(sizeof(musica));
        novo->musica->id = id;
        strcpy(novo->musica->titulo, titulo);
        novo->musica->id_artista = id_artista;
        novo->musica->duracao = duracao;

        novo->esq = novo->dir = NULL;
        novo->altura = 1;

        return novo;
    }

    if (id < raiz->musica->id)
        raiz->esq = insere_musica_avl(raiz->esq, id, titulo, id_artista, duracao);
    else if (id > raiz->musica->id)
        raiz->dir = insere_musica_avl(raiz->dir, id, titulo, id_artista, duracao);
    else {
        printf("\nID já existe.\n");
        return raiz;
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int fb = fator(raiz);

    if (fb > 1 && id < raiz->esq->musica->id)
        return rotacaoDireita(raiz);

    if (fb < -1 && id > raiz->dir->musica->id)
        return rotacaoEsquerda(raiz);

    if (fb > 1 && id > raiz->esq->musica->id) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && id < raiz->dir->musica->id) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

musica_no* menorDireita(musica_no *no) {
    musica_no *atual = no;
    while (atual && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

musica_no* remove_musica_avl(musica_no *raiz, ConjuntoPlaylists *playlists, int id)
{
    if (raiz == NULL)
        return raiz;

    if (id < raiz->musica->id)
        raiz->esq = remove_musica_avl(raiz->esq, playlists, id);

    else if (id > raiz->musica->id)
        raiz->dir = remove_musica_avl(raiz->dir, playlists, id);

    else {
        remove_musicaPlaylist(playlists, id);

        if (raiz->esq == NULL || raiz->dir == NULL) {
            musica_no *temp = raiz->esq ? raiz->esq : raiz->dir;

            free(raiz->musica);
            free(raiz);

            return temp;
        }

        musica_no *temp = menorDireita(raiz->dir);

        raiz->musica->id = temp->musica->id;
        strcpy(raiz->musica->titulo, temp->musica->titulo);
        raiz->musica->id_artista = temp->musica->id_artista;
        raiz->musica->duracao = temp->musica->duracao;

        raiz->dir = remove_musica_avl(raiz->dir, playlists, temp->musica->id);
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int fb = fator(raiz);

    if (fb > 1 && fator(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    if (fb > 1 && fator(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && fator(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    if (fb < -1 && fator(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

void coletar_ids_remocao(musica_no *n, int idArtista, int **ids, int *qtd, int *capacidade) {
    if (!n) return;

    coletar_ids_remocao(n->esq, idArtista, ids, qtd, capacidade);

    if (n->musica && n->musica->id_artista == idArtista) {
        if (*qtd == *capacidade) {
            *capacidade *= 2;
            int *novo = realloc(*ids, sizeof(int) * (*capacidade));
            if (!novo) {
                free(*ids);
                perror("realloc");
                exit(1);
            }
            *ids = novo;
        }
        (*ids)[(*qtd)++] = n->musica->id;
    }

    coletar_ids_remocao(n->dir, idArtista, ids, qtd, capacidade);
}

musica_no* remove_musicas_por_artista_avl(
        musica_no *raiz,
        ConjuntoPlaylists *playlists,
        int idArtista) {

    if (!raiz) return raiz;

    int capacidade = 50;
    int qtd = 0;
    int *ids = malloc(sizeof(int) * capacidade);

    if (!ids) {
        perror("malloc");
        return raiz;
    }

    coletar_ids_remocao(raiz, idArtista, &ids, &qtd, &capacidade);

    if (qtd == 0) {
        free(ids);
        return raiz;
    }

    for (int i = 0; i < qtd; i++) {
        raiz = remove_musica_avl(raiz, playlists, ids[i]);
    }

    free(ids);
    return raiz;
}



musica* busca_musica_por_id_avl(musica_no *raiz, int id) {
    while (raiz != NULL) {
        if (id == raiz->musica->id)
            return raiz->musica;

        if (id < raiz->musica->id)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }
    return NULL;
}


void imprime_todas_musicas_avl(musica_no *raiz, artista_no *ini) {
    if (raiz == NULL) return;

    imprime_todas_musicas_avl(raiz->esq, ini);

    printf("\n---------- Música -------------");
    printf("\nID: %d", raiz->musica->id);
    printf("\nTítulo: %s", raiz->musica->titulo);

    artista_no *p = ini->prox;
    while (p && p->artista->id != raiz->musica->id_artista)
        p = p->prox;

    printf("\nArtista: %s", p ? p->artista->nome : "(não encontrado)");

    int d = raiz->musica->duracao;
    printf("\nDuração: %02d:%02d:%02d\n", d/3600, (d%3600)/60, d%60);

    imprime_todas_musicas_avl(raiz->dir, ini);
}

void imprime_musicas_artista_avl(musica_no *raiz, int id_artista) {
    if (raiz == NULL) return;

    imprime_musicas_artista_avl(raiz->esq, id_artista);

    if (raiz->musica->id_artista == id_artista)
        printf("\n - %s", raiz->musica->titulo);

    imprime_musicas_artista_avl(raiz->dir, id_artista);
}


//Implementação Playlist
playlist_no* cria_playlist() {
    playlist_no* cabeca = malloc(sizeof(playlist_no));
    cabeca->prox = cabeca;
    cabeca->musica = NULL;
    return cabeca;
}


void monta_playlist_circ(playlist_no **iniPlaylist, musica_no *raiz, char *listaIds) {
    int vetor[50];
    int qtd = 0;

    char *token = strtok(listaIds, " ");
    while (token && qtd < 50) {
        vetor[qtd++] = atoi(token);
        token = strtok(NULL, " ");
    }

    if (*iniPlaylist == NULL) {
        *iniPlaylist = cria_playlist();
    }

    playlist_no *cabeca = *iniPlaylist;

    playlist_no *ultimo = cabeca;
    while (ultimo->prox != cabeca) {
        ultimo = ultimo->prox;
    }

    for (int i = 0; i < qtd; i++) {
        musica *m = busca_musica_por_id_avl(raiz, vetor[i]);
        if (!m) {
            printf("\nMusica com ID %d nao encontrada, ignorada.", vetor[i]);
            continue;
        }

        playlist_no *novo = malloc(sizeof(playlist_no));
        novo->musica = m;

        novo->prox = cabeca;
        ultimo->prox = novo;
        ultimo = novo;
    }

    printf("\nPlaylist montada com sucesso!\n");
}


ConjuntoPlaylists* cria_CPlaylists(){
    ConjuntoPlaylists *cabeca = malloc(sizeof(ConjuntoPlaylists));
    cabeca->prox = NULL;
    return cabeca;
}

void monta_CPlaylists(ConjuntoPlaylists *ini, int id, char nome[], playlist_no *musicas){
    ConjuntoPlaylists *novo = malloc(sizeof(ConjuntoPlaylists));
    novo->id = id;
    strcpy(novo->nome, nome);
    novo->musicas = musicas;
    novo->prox = ini->prox;
    ini->prox = novo;
}

void shuffle_playlist(playlist_no *playlist) {
    if (!playlist) return;
    playlist_no *cabeca = playlist;

    if (cabeca->prox == cabeca) return;

    int qtd = 0;
    playlist_no *p = cabeca->prox;
    while (p != cabeca) {
        qtd++;
        p = p->prox;
    }
    if (qtd <= 1) return;

    musica **vet = malloc(sizeof(musica*) * qtd);
    p = cabeca->prox;
    for (int i = 0; i < qtd; i++) {
        vet[i] = p->musica;
        p = p->prox;
    }

    srand((unsigned) time(NULL));
    for (int i = qtd - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        musica *temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }

    p = cabeca->prox;
    for (int i = 0; i < qtd; i++) {
        p->musica = vet[i];
        p = p->prox;
    }

    free(vet);
}


void imprime_playlist(ConjuntoPlaylists *p) {
    if (!p->musicas) {
        printf("\nPlaylist vazia.\n");
        return;
    }

    printf("\n---- Playlist: %s  (ID: %d) ----\n", p->nome, p->id);

    if (!p->musicas || p->musicas->prox == p->musicas) {
        printf("\nPlaylist vazia.\n");
        return;
    }

    playlist_no *m = p->musicas->prox;
    while (m != p->musicas) {
        printf(" - %s\n", m->musica->titulo);
        m = m->prox;
    }
}

void remove_musicaPlaylist(ConjuntoPlaylists *listaPlaylists, int idMusica) {
    if (!listaPlaylists) return;

    ConjuntoPlaylists *pl = listaPlaylists->prox;

    while (pl) {
        playlist_no *cabeca = pl->musicas;

        if (!cabeca || cabeca->prox == cabeca) {
            pl = pl->prox;
            continue;
        }

        playlist_no *ant = cabeca;
        playlist_no *at = cabeca->prox;

        while (at != cabeca) {
            if (at->musica && at->musica->id == idMusica) {
                ant->prox = at->prox;
                free(at);
                at = ant->prox;
                continue;
            }
            ant = at;
            at = at->prox;
        }

        pl = pl->prox;
    }
}



void remove_artista_por_id(artista_no *artistas, int id_artista) {
    artista_no *ant = artistas;
    artista_no *atual = artistas->prox;
    while (atual) {
        if (atual->artista->id == id_artista) {
            ant->prox = atual->prox;
            free(atual->artista);
            free(atual);
            return;
        }
        ant = atual;
        atual = atual->prox;
    }
}

//Menus
int menuPrincipal() {
    int opcao;
    printf("\n==== MENU PRINCIPAL ====\n");
    printf("1 - Artistas\n");
    printf("2 - Musicas\n");
    printf("3 - Playlists\n");
    printf("4 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

int menuArtistas() {
    int opcao;
    printf("\n==== MENU ARTISTAS ====\n");
    printf("1 - Inserir artista\n");
    printf("2 - Listar todos os artistas\n");
    printf("3 - Remover artista e suas musicas\n");
    printf("4 - Voltar\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

int menuMusicas() {
    int opcao;
    printf("\n==== MENU MUSICAS ====\n");
    printf("1 - Inserir musica\n");
    printf("2 - Listar todas musicas\n");
    printf("3 - Listar musicas de um artista\n");
    printf("4 - Remover musica\n");
    printf("5 - Voltar\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

int menuPlaylists() {
    int opcao;
    printf("\n==== MENU PLAYLISTS ====\n");
    printf("1 - Criar playlist\n");
    printf("2 - Montar playlist\n");
    printf("3 - Listar playlist\n");
    printf("4 - Shuffle playlist\n");
    printf("5 - Voltar\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    return opcao;
}

int main() {
    artista_no *lista_artistas = cria_artista();

    musica_no *raiz_musicas = NULL;

    ConjuntoPlaylists *conj_playlists = cria_CPlaylists();

    int opcao, opcaoSub;

    do {
        opcao = menuPrincipal();
        switch(opcao) {
            case 1:
                do {
                    opcaoSub = menuArtistas();
                    switch(opcaoSub) {
                        case 1: {
                            int id;
                            char nome[200], genero[200];
                            printf("ID do artista: ");
                            scanf("%d", &id);
                            printf("Nome do artista: ");
                            scanf(" %[^\n]", nome);
                            printf("Genero do artista: ");
                            scanf(" %[^\n]", genero);
                            insere_artista(lista_artistas, id, nome, genero);
                            printf("\nArtista inserido com sucesso!\n");
                            break;
                        }
                        case 2:
                            imprime_todos_artistas(lista_artistas);
                            break;
                        case 3: {
                            int id;
                            printf("ID do artista a remover: ");
                            scanf("%d", &id);

                            raiz_musicas = remove_musicas_por_artista_avl(raiz_musicas, conj_playlists, id);

                            remove_artista_por_id(lista_artistas, id);

                            printf("\nArtista e suas musicas removidos.\n");
                            break;
                        }
                    }
                } while(opcaoSub != 4);
                break;

            case 2:
                do {
                    opcaoSub = menuMusicas();
                    switch(opcaoSub) {
                        case 1: {
                            int id, id_artista, duracao;
                            char titulo[200];
                            printf("ID da musica: ");
                            scanf("%d", &id);
                            printf("Titulo: ");
                            scanf(" %[^\n]", titulo);
                            printf("ID do artista: ");
                            scanf("%d", &id_artista);
                            printf("Duracao em segundos: ");
                            scanf("%d", &duracao);

                            raiz_musicas = insere_musica_avl(raiz_musicas, id, titulo, id_artista, duracao);
                            printf("\nMusica inserida com sucesso!\n");
                            break;
                        }
                        case 2:
                            if (raiz_musicas == NULL) {
                                printf("\nNenhuma música cadastrada.\n");
                            } else {
                                imprime_todas_musicas_avl(raiz_musicas, lista_artistas);
                            }
                            break;
                        case 3: {
                            int id_artista;
                            printf("ID do artista: ");
                            scanf("%d", &id_artista);
                            printf("\nTodas as músicas do artista do ID: %d\n", id_artista);
                            imprime_musicas_artista_avl(raiz_musicas, id_artista);
                            printf("\n");
                            break;
                        }
                        case 4: {
                            int id;
                            printf("ID da musica a remover: ");
                            scanf("%d", &id);

                            raiz_musicas = remove_musica_avl(raiz_musicas, conj_playlists, id);
                            printf("\nMusica removida.\n");
                            break;
                        }
                    }
                } while(opcaoSub != 5);
                break;

            case 3:
                do {
                    opcaoSub = menuPlaylists();
                    switch(opcaoSub) {
                        case 1: {
                            int id;
                            char nome[200];
                            printf("ID da playlist: ");
                            scanf("%d", &id);
                            printf("Nome da playlist: ");
                            scanf(" %[^\n]", nome);
                            playlist_no *nova = cria_playlist();
                            monta_CPlaylists(conj_playlists, id, nome, nova);
                            printf("\nPlaylist criada!\n");
                            break;
                        }
                        case 2: {
                            int id_playlist;
                            char ids[200];
                            printf("ID da playlist: ");
                            scanf("%d", &id_playlist);
                            printf("IDs das musicas separados por espaco: ");
                            scanf(" %[^\n]", ids);
                            ConjuntoPlaylists *p = conj_playlists->prox;
                            while(p && p->id != id_playlist){
                                p = p->prox;
                            }
                            if (p) monta_playlist_circ(&p->musicas, raiz_musicas, ids);
                            else printf("\nPlaylist nao encontrada!\n");
                            break;
                        }
                        case 3: {
                            int id_playlist;
                            printf("ID da playlist: ");
                            scanf("%d", &id_playlist);
                            ConjuntoPlaylists *p = conj_playlists->prox;
                            while(p && p->id != id_playlist){
                                p = p->prox;
                            }
                            if(p) imprime_playlist(p);
                            else printf("\nPlaylist nao encontrada!\n");
                            break;
                        }
                        case 4: {
                            int id_playlist;
                            printf("ID da playlist: ");
                            scanf("%d", &id_playlist);
                            ConjuntoPlaylists *p = conj_playlists->prox;
                            while(p && p->id != id_playlist){
                                p = p->prox;
                            }
                            if(p) shuffle_playlist(p->musicas);
                            else printf("\nPlaylist nao encontrada!\n");
                            break;
                        }
                    }
                } while(opcaoSub != 5);
                break;
        }
    } while(opcao != 4);

    return 0;
}
