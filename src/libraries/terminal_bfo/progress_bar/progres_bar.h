//
// Created by maximus on 19.04.23.
//

#ifndef C_VERSION_PROGRES_BAR_H
#define C_VERSION_PROGRES_BAR_H

#include <time.h>

typedef struct progress_bar {
    time_t start_time;
    int bar_length;
    int number_of_all_elements;
    int this_number;
} progress_bar;

progress_bar create_bar(int number_of_all_elements);

void bar_step(progress_bar *bar);

void delete_bar(progress_bar *bar);

#endif //C_VERSION_PROGRES_BAR_H
