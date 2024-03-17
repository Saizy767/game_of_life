#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int** create_place(int row, int column);
void clear_place(int** duplicate_matrix, int column);
void init_file(int** main_matrix, int row, int column);
char* generate_text_name();
int game_over(int** main_matrix, int row, int column);
void picture_round(int** main_matrix, int row, int column);
void set_dupl_next_round(int** main_matrix, int** duplicate_matrix, int row, int column);
void next_round(int** main_matrix, int** duplicate_matrix, int row, int column);
int find_neighbour(int** main_matrix, int x, int y, int row, int column);
int change_status(int** main_matrix, int x, int y, int counter_neighbour);


int main(){
    int **main_matrix, **duplicate_matrix;
    int row = 85, column = 20;

    main_matrix = create_place(row, column);
    duplicate_matrix = create_place(row, column);

    float timer_render = 0.0;

    printf("Input time of render : ");
    scanf("%f", &timer_render);

    init_file(main_matrix, row, column);
    
    while (game_over(main_matrix, row, column) == 0){
        picture_round(main_matrix, row, column);
        set_dupl_next_round(main_matrix, duplicate_matrix, row, column);
        next_round(main_matrix, duplicate_matrix, row, column);
        sleep(timer_render);
        printf("\033[H\033[J");
    }
    clear_place(main_matrix, column);
    clear_place(duplicate_matrix, column);
    return 0;
}


int** create_place(int row, int column)
{
    int **matrix =(int**)calloc(column, sizeof(int*));
    for(int y = 0; y < column; y++)
    {
        matrix[y] =(int*)calloc(row, sizeof(int));
    }
    return matrix;
}


void clear_place(int** main_matrix, int column)
{
    for (int y = 0; y < column; y++)
    {
        free(main_matrix[y]);
    }
    free(main_matrix);
}


void init_file(int** main_matrix, int row, int column){
    char* name_file = generate_text_name();
    FILE *file;
    int y = 0;
    file = fopen(name_file, "r");
    
    char currentline[85];
    
    while (fgets(currentline, sizeof(currentline), file) != NULL)
    {
        if (y < column){
            for (int x = 0; x < row; x++)
            {
                if (currentline[x] - '0' > 1 || currentline[x] - '0' < 0){
                    continue;
                }
                main_matrix[y][x] = currentline[x] - '0';
            }
            y++;
        }
    }
    fclose(file);
}


char* generate_text_name()
{
    char letter;
    int i = 0, checker = 0;
    char* text_file = (char*)calloc(sizeof(char), sizeof(char));
    printf("Input name and format file :\n");
    while(scanf("%c", &letter)){
        if (letter == '\n'){
            if(checker != 0)
            {
                break;
            }
            continue;
        }
        else{
            text_file[i] = letter;
            i++;
            checker++;
        }
    }
    return text_file;
}


void picture_round(int** main_matrix, int row, int column){
    for (int y = 0; y < column; y++){
        for(int x = 0; x < row; x++){
            if (main_matrix[y][x] == 0){
                printf("  ");
            }
            else{
                printf("* ");
            }
        }
        printf("\n");
    }
}


void set_dupl_next_round(int** main_matrix, int** duplicate_matrix, int row, int column){
    for (int y = 0; y < column; y++){
        for(int x = 0; x <= row; x++){
            int counter_neig = find_neighbour(main_matrix, x, y, row, column);
            if(change_status(main_matrix, x, y, counter_neig) == 1){
                duplicate_matrix[y][x] = main_matrix[y][x] == 0 ? 1 : 0;
            }
            else{
                duplicate_matrix[y][x] = main_matrix[y][x];
            }
        }
    }
}

void next_round(int** main_matrix, int** duplicate_matrix, int row, int column){
    for (int y = 0; y < column; y++){
        for(int x = 0; x < row; x++){
            main_matrix[y][x] = duplicate_matrix[y][x];
        }
    }
}

int find_neighbour(int** main_matrix, int x, int y, int row, int column)
{
    int counter_neighbour = 0;
    if(x < row && y < column - 1 && x > 0 && y > 0)
    {
        for (int col_y = -1; col_y <= 1; col_y++)
        {
            for (int row_x = -1; row_x <= 1; row_x++)
            {
                if (row_x == 0 && col_y == 0)
                {
                    continue;
                }
                counter_neighbour += main_matrix[y+col_y][x+row_x];
            }
        }
    }

    if(x == 0 ||
        y == column - 1||
        y == 0 ||
        x == row - 1)
    {
        for (int col_y = -1; col_y <= 1; col_y++)
        {
            for (int row_x = -1; row_x <= 1; row_x++)
            {
                if (x+row_x < 0 ||
                    x+row_x > row - 1 ||
                    y+col_y < 0 ||
                    y+col_y > column - 1||
                    (row_x == 0 && col_y == 0))
                    {
                    continue;
                    }
                counter_neighbour += main_matrix[y+col_y][x+row_x];
            }
        }
    }
    return counter_neighbour;
}

int change_status(int** main_matrix, int x, int y, int counter_neighbour)
{
    if(main_matrix[y][x] == 0)
    {
        if(counter_neighbour == 3)
        {
            return 1;
        }
        else{
            return 0;
        }
    }
    else
    {
        if(counter_neighbour == 2 || counter_neighbour == 3)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int game_over(int **main_matrix, int row, int column)
{
    int any_live = 0;
    for (int y = 0; y < column; y++)
    {
        for (int x = 0; x < row; x++)
        {
            any_live += main_matrix[y][x];
        }
    }
    if (any_live == 0)
    {
        return 1;
    }
    return 0;
}