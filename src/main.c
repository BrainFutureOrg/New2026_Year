#include<stdio.h>
#include<termios.h>
#include"libraries/terminal_bfo/colors_bfo/colors.h"
#include"libraries/terminal_bfo/terminal_funcs.h"
#include<sys/ioctl.h>
#include <unistd.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    }
    while (res && errno == EINTR);

    return res;
}

char img[]=
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"0000000000001111000011110000000011110000000000000000111100001111000000000000"
"1111111111111111000011110000000011110000111111111111111100001111111111111111"
"1111111111111111000011110000000011110000111111111111111100001111111111111111"
"1111000000000000000011110000000011110000111100000000000000001111000000001111"
"1111000000000000000011110000000011110000111100000000000000001111000000001111"
"1111000000000000000011110000000011110000111100000000000000001111000000001111"
"1111000000000000000011110000000011110000111100000000000000001111000000001111"
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
"1111111111111111000011111111111111110000111111111111111100001111111111111111"
;// in case 2026 label is needed

int img_w = 76,img_h=14;// dimensions of 2026 label

char* snow_symbols = "*.";
int snow_symbols_len = 2;

void fill_bg(struct winsize size)
{
        
    terminal_goto_xy(1, 1);
    color_to_rgb_background(40,50,60);
    for (int i = 0; i < size.ws_row; i++)
    {
        for (int j = 0; j < size.ws_col; j++)
        {
            printf(" ");
        }
    }
}

struct snow_list_element{
    double x,y,v_x,v_y;
    char c, moving, r, g, b;
    struct snow_list_element* next;
};

struct snow_list_head{
    struct snow_list_element *first, *last;
    //unsigned int count;
};

void snow_list_add(struct snow_list_head *snow_list, double x, double y, double v_x, double v_y, char c, char moving, char r, char g, char b){
    struct snow_list_element *element = malloc(sizeof(struct snow_list_element));
    element->x=x;
    element->y=y;
    element->v_x=v_x;
    element->v_y=v_y;
    element->c=c;
    element->moving=moving;
    element->next=NULL;
    element->r=r;
    element->g=g;
    element->b=b;
    if(snow_list->first){
        snow_list->last->next = element;
        snow_list->last = element;
    }
    else{
        snow_list->first=snow_list->last=element;
    }
}

void draw_process_snow(struct snow_list_head* snow_list, struct winsize size){
    struct snow_list_element* snowflake = snow_list->first;
    
    while(snowflake){
        color_to_rgb_foreground(snowflake->r,snowflake->g,snowflake->b);
        int x_i = (int)snowflake->x, y_i = (int)snowflake->y;
        int x=x_i+size.ws_col/2, y=y_i+size.ws_row/2;
        if(x<=0){
            x+=size.ws_col;
            snowflake->x+=size.ws_col;
        }
        else if(x>size.ws_col){
            x-=size.ws_col;
            snowflake->x-=size.ws_col;
        }
        if(y<=0){
            y+=size.ws_row;
            snowflake->y+=size.ws_row;
        }
        else if(y>size.ws_row){
            y-=size.ws_row;
            snowflake->y-=size.ws_row;
        }
        terminal_goto_xy(x,y);
        putchar(snowflake->c);
        if(snowflake->moving){
            int x_2 = x_i + img_w/2, y_2 =y_i+img_h/2;
            if(x_2>=0&&x_2<img_w&&y_2>=0&&y_2<img_h&&img[x_2+y_2*img_w]=='1'&&rand()<RAND_MAX/10){
                snowflake->moving=0;
                img[x_2+y_2*img_w]='0';
            }
            else{
                snowflake->x+=snowflake->v_x;
                snowflake->y+=snowflake->v_y;
            }
        }
        snowflake = snowflake->next;
    }
}

void generate_snow(struct snow_list_head *snow_list, int x_start, int x_end, int y_start, int y_end, int amount){
    for(int i=0; i<amount; i++){
        int rg = 200+rand()%56;
        snow_list_add(snow_list, x_start+(x_end-x_start)*(rand()/(double)RAND_MAX), y_start+(y_end-y_start)*(rand()/(double)RAND_MAX), -1+2*(rand()/(double)RAND_MAX), 1.5*(rand()/(double)RAND_MAX), snow_symbols[rand()%snow_symbols_len], 1, rg, rg, 250+rand()%6);
    }
}

int main(){
    terminal_invisible_cursor;
    struct winsize prev_size;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &prev_size);
    struct snow_list_head snow = {NULL};
    int amount = (int)(prev_size.ws_row*prev_size.ws_col*0.1);
    generate_snow(&snow, -prev_size.ws_col/2, prev_size.ws_col-prev_size.ws_col/2, -prev_size.ws_row/2, prev_size.ws_row-prev_size.ws_row/2, amount);
    for(int i=0; i<1000000; i++){
        struct winsize size;
        fill_bg(size);
        draw_process_snow(&snow, size);
        ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
        fflush(stdout); // do not forget to fflush after drawing
        msleep(100);
        prev_size=size;
    }
    terminal_visible_cursor;
}
