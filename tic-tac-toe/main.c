#include <stdio.h>
#include <stdlib.h>

#define WIN_COUNT 2
#define SIDE_LENGTH 4
#define BOARD_SIZE (SIDE_LENGTH*SIDE_LENGTH)

typedef enum player_t{
    NONE = 0,
    CROSS,
    CIRCLE,
}player_t;

typedef player_t* board_t;

void init_board(board_t *board);
player_t at(board_t board, int x, int y);
void put(board_t board, player_t player, int x, int y);
int in_bounds(int x, int y);
char player_to_char(player_t player);
player_t opossite(player_t player);
void print_board(board_t board);


typedef struct count_result_t{
    int count;
    player_t player;
}count_result_t;

//returns the maximum consecutive crosses or circles in the direction given.
void count_consecutive(board_t board, int startX, int startY, int dirX, int dirY, count_result_t *res);

typedef enum game_result_t{
    UNFINISHED,
    CIRCLE_WIN,
    CROSS_WIN,
    DRAW,
}game_result_t;

game_result_t get_game_result(board_t board);
game_result_t get_win_result(player_t player);
void print_result(game_result_t res);

int main(){
    board_t board;
    player_t turn = CIRCLE;
    game_result_t res = UNFINISHED;
    init_board(&board);
    int moveX, moveY;

    while((res = get_game_result(board)) == UNFINISHED){
        print_board(board);
        scanf("%d %d", &moveX, &moveY);
        put(board, turn, moveX, moveY);
        turn = opossite(turn);    
    }
    print_board(board);
    print_result(res);
}


void init_board(board_t *board){
    *board = (player_t*) malloc(sizeof(player_t) * BOARD_SIZE);
}

int in_bounds(int x, int y){
    return x >= 0 && x < SIDE_LENGTH && y >= 0 && y < SIDE_LENGTH;
}

player_t at(board_t board, int x, int y){
    return board[y*SIDE_LENGTH + x];
}
void put(board_t board, player_t player, int x, int y){
    board[y*SIDE_LENGTH + x] = player;
}

char player_to_char(player_t player){
    switch (player)
    {
        case NONE: return '-';
        case CROSS: return 'X';
        case CIRCLE: return 'O';
    }
}

player_t opossite(player_t player){
    return player == CROSS ? CIRCLE : CROSS;
}

game_result_t get_win_result(player_t player){
    switch (player)
    {
        case CROSS: return CROSS_WIN;
        case CIRCLE: return CIRCLE_WIN;
        default: return UNFINISHED;
    }
}

void print_result(game_result_t res){
    switch (res)
    {
        case CIRCLE_WIN: printf("Circle wins!\n"); break;
        case CROSS_WIN: printf("Cross wins!\n"); break;
        case DRAW: printf("Tie\n"); break;
    }
}

void print_board(board_t board){
    for(int y = SIDE_LENGTH-1; y >= 0; y--){
        for(int x = 0; x < SIDE_LENGTH; x++){
            printf("%c ", player_to_char(at(board, x, y)));
        }
        printf("\n");
    }
    printf("\n");
}


void count_consecutive(board_t board, int startX, int startY, int dirX, int dirY, count_result_t *res){
    int x = startX, y = startY;
    player_t current = NONE;
    int current_cout = 0;
    res->count = 0;
    res->player = NONE;
    while(in_bounds(x, y)){
        player_t at_pos = at(board, x ,y);
        if(at_pos == current){
            current_cout++;
        }else{
            if(current != NONE && current_cout >= res->count){
                res->count = current_cout;
                res->player = current;
            }
            current_cout = 1;
            current = at_pos;
        }
        x+=dirX;
        y+=dirY;
    }
    if(current != NONE && current_cout >= res->count){
        res->count = current_cout;
        res->player = current;
    }
    return;
}


game_result_t get_game_result(board_t board){
    count_result_t res;
    res.count = 0;
    res.player = NONE;

    for(int x = 0; x < SIDE_LENGTH; x++){
        count_consecutive(board, x, 0, 0, 1, &res);
        if(res.count >= WIN_COUNT)
            return get_win_result(res.player);
    }
    
    for(int y = 0; y < SIDE_LENGTH; y++){
        count_consecutive(board, 0, y, 1, 0, &res);
        if(res.count >= WIN_COUNT)
            return get_win_result(res.player);
    }

    for(int x = 0; x < SIDE_LENGTH; x++){
        count_consecutive(board, x, 0, 1, 1, &res);
        if(res.count >= WIN_COUNT)
            return get_win_result(res.player);
        count_consecutive(board, x, SIDE_LENGTH-1, 1, -1, &res);
        if(res.count >= WIN_COUNT)
            return get_win_result(res.player);
    }

    for(int y = 0; y < SIDE_LENGTH; y++){
        count_consecutive(board, 0, y, 1, 1, &res);
        if(res.count >= WIN_COUNT)
            return get_win_result(res.player);
        count_consecutive(board, 0, y, 1, -1, &res);
        if(res.count >= WIN_COUNT)
            return get_win_result(res.player);
    }

    for(int i = 0; i < BOARD_SIZE; i++)
        if(board[i] == NONE) return UNFINISHED;
    
    return DRAW;
}
