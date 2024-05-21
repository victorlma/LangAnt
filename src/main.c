#include "stdlib.h"
#include "raylib.h"
#include "raymath.h"

#define MAX_ANT 255


typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    DIRECTIONS
} DIRECTION;

Vector2 dirVectors[] = 
{
        [UP] = (Vector2) {0, -1},
        [RIGHT] = (Vector2) {1, 0},
        [DOWN] = (Vector2) {0, 1},
        [LEFT] = (Vector2) {-1, 0},
};

typedef struct
{
    int active;
    Vector2 pos;
    int direction;

    Color fgcolor;
} Ant;

typedef struct
{

    int dimen;
    unsigned int *data;
} Grid;

typedef struct
{
    Color defbgc;
    Color deffgc;
    Color antcolor;
    Vector2 startpos;

    int antsnum;
    Ant ants[MAX_ANT];


    int sqrsize;

    Grid    grid;

} State;

State st = {0};

int checkGrid(Grid *grid, int x, int y)
{
    return grid->data[
        (y*grid->dimen) + x
    ];
}

void flipGrid(Grid *grid, int x, int y, Color color)
{
    unsigned int *tile = &grid->data[(y * grid->dimen) + x];

    if (*tile > 0) {
        *tile = 0;
    }
    else {
        *tile = ColorToInt(color);
    }
}

void processAnts(Ant *ants, int n)
{

    for (int i=0; i < n; ++i) {


        if (ants[i].pos.x < 0) ants[i].pos.x = st.grid.dimen -1;
        if (ants[i].pos.y < 0) ants[i].pos.y = st.grid.dimen -1;


        if (ants[i].pos.x > st.grid.dimen -1) ants[i].pos.x = 0;
        if (ants[i].pos.y > st.grid.dimen -1) ants[i].pos.y = 0;

        if (ants[i].active) {
            if (checkGrid(&st.grid, ants[i].pos.x, ants[i].pos.y)) {

                flipGrid(&st.grid, ants[i].pos.x, ants[i].pos.y, ants[i].fgcolor);

                ants[i].direction -= 1;
                if (ants[i].direction < UP) ants[i].direction = LEFT;
                if (ants[i].direction > LEFT) ants[i].direction = UP;
                ants[i].pos = Vector2Add(ants[i].pos, dirVectors[ants[i].direction]);
            }

            else {
                flipGrid(&st.grid, ants[i].pos.x, ants[i].pos.y, ants[i].fgcolor);

                ants[i].direction += 1;
                if (ants[i].direction < UP) ants[i].direction = LEFT;
                if (ants[i].direction > LEFT) ants[i].direction = UP;
                ants[i].pos = Vector2Add(ants[i].pos, dirVectors[ants[i].direction]);

            }

        }
        else {
            ants[i].active = 1;
        }

        if (ants[i].pos.x < 0) ants[i].pos.x = st.grid.dimen -1;
        if (ants[i].pos.y < 0) ants[i].pos.y = st.grid.dimen -1;


        if (ants[i].pos.x > st.grid.dimen -1) ants[i].pos.x = 0;
        if (ants[i].pos.y > st.grid.dimen -1) ants[i].pos.y = 0;


    }
}


void drawAnts(Ant *ants, int n)
{

    for (int i=0; i < n; ++i)
    {
        Vector2 antRec = (Vector2) {
            .x = ants[i].pos.x * st.sqrsize,
            .y = ants[i].pos.y * st.sqrsize,
        };
        DrawRectangleV(antRec, 
                           (Vector2) {st.sqrsize, st.sqrsize}, 
                        st.antcolor);

    }
}
void drawGrid(Grid *grid)
{
    for (int y=0; y < grid->dimen; ++y) {

        for (int x=0; x < grid->dimen; ++x) {

            int color = checkGrid(grid, x, y);

            Vector2 gridrec = (Vector2) {
                .x = x * st.sqrsize,
                .y = y * st.sqrsize,
            };
        
            if (color) {
                DrawRectangleV(gridrec, (Vector2) {st.sqrsize, st.sqrsize}, GetColor(color));
            }
            else {
                DrawRectangleV(gridrec, (Vector2) {st.sqrsize, st.sqrsize}, st.defbgc);
            }
        }

    }

}
int main(void)
{

st = (State) {
        .defbgc = GRAY,
        .deffgc = BLACK,
        .antcolor = RED,

        .grid.dimen = 50,
        .sqrsize = 10,
        .antsnum = 1,

        .startpos = (Vector2) {290, 50},
    };

    st.ants[0]  = (Ant) {
        .active = 0,
        .direction = DOWN,
        .fgcolor = st.deffgc,
        .pos = st.startpos
    };

    st.grid.data = (unsigned int *) malloc(sizeof(unsigned int) * st.grid.dimen * st.grid.dimen);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "LangAnt");

    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        processAnts(st.ants, st.antsnum);
        drawGrid(&st.grid);
        drawAnts(st.ants, st.antsnum);
        EndDrawing();
    }

    free(st.grid.data);
    CloseWindow();

    return 0;
}
