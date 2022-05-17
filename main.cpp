#include <allegro.h>
#include <string>
#include <stdio.h>
#include <fstream>

volatile long hspeed = 0;
void ihspeed(){
    hspeed ++;
}
END_OF_FUNCTION(ihspeed);
LOCK_VARIABLE(hspeed);
LOCK_FUNCTION(ihspeed);

int size_c_x = 4, size_c_y = 4, _speed = 0, _rand = 10, MAX_cells_number = 1, buttons_power = 1;
const int size_x = 275 /* 1100 */, size_y = 180;
bool epilepsy_mode = false;

int they_live[9], they_are_born[9], color_R = 241, color_G = 80, color_B = 192;

void assign_variables(long & they_live_r, long & they_are_born_r){
    int i;

    they_live_r = 0;
    they_are_born_r = 0;
    for(i = 1; i < 9; i ++){
        if(rand() % 2 == 1){
            they_live_r *= 10;
            they_live_r += i;
        }
    }
    for(i = 1; i < 9; i ++){
        if(rand() % 2 == 1){
            they_are_born_r *= 10;
            they_are_born_r += i;
        }
    }
    if(rand() % 2 == 1){
        if(they_live_r == 0)
            they_live_r = -1;
        else
            they_live_r *= 10;
    }
    if(rand() % 2 == 1){
        if(they_are_born_r == 0)
            they_are_born_r = -1;
        else
            they_are_born_r *= 10;
    }

    long they_live_r_ = they_live_r;
    long they_are_born_r_ = they_are_born_r;
    for(i = 0; i < 9; i ++){
        they_live[i] = -1;
        they_are_born[i] = -1;
    }
    if(they_live_r_ > -1){
        i = 0;
        do{
            they_live[i] = they_live_r_ % 10;
            they_live_r_ /= 10;
            i ++;
        }while(they_live_r_ > 0);
    }
    if(they_are_born_r_ > -1){
        i = 0;
        do{
            they_are_born[i] = they_are_born_r_ % 10;
            they_are_born_r_ /= 10;
            i ++;
        }while(they_are_born_r_ > 0);
    }
}

void random_life_arrangement(bool cells[][size_y]){
    int i, j, k;
    for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            cells[i][j] = 0;
        }
    }
    i = 0;
    while(i < MAX_cells_number){
        j = rand() % size_x;
        k = rand() % size_y;
        if(cells[j][k] == 0){
            i ++;
        }
        cells[j][k] = 1;
    }
    /*for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            if(rand() % _rand == 0){
                cells[i][j] = rand() % 2;
                cells_count ++;
                if(MAX_cells_number == cells_count) return;
            }
        }
    }*/
}

void draw_cells(BITMAP * cells_backg, bool cells[][size_y]){
    int i, j;
    for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            if(cells[i][j] == 1){
                if(!epilepsy_mode) rectfill(cells_backg, i * size_c_x, j * size_c_y, i * size_c_x + size_c_x - 1, j * size_c_y + size_c_x - 1, makecol(color_R, color_G, color_B));
                else rectfill(cells_backg, i * size_c_x, j * size_c_y, i * size_c_x + size_c_x - 1, j * size_c_y + size_c_x - 1, makecol(rand()%150+50, rand()%150+50, rand()%150+50));
                //rectfill(cells_backg, i * size_c_x, j * size_c_y, i * size_c_x + size_c_x - 1, j * size_c_y + size_c_x - 1, makecol(150, 150, 150));
            }
        }
    }
}

void update_cells(bool cells[][size_y], bool cells_updated[][size_y]){
    int number_of_neighbours, i, j, k;
    for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            cells_updated[i][j] = 0;
        }
    }
    for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            number_of_neighbours = 0;
            if(i - 1 > -1 && j - 1 > -1){
                if(cells[i-1][j-1]) number_of_neighbours ++;
            }
            if(j - 1 > -1){
                if(cells[i][j-1]) number_of_neighbours ++;
            }
            if(i + 1 < size_x && j - 1 > -1){
                if(cells[i+1][j-1]) number_of_neighbours ++;
            }
            if(i + 1 < size_x){
                if(cells[i+1][j]) number_of_neighbours ++;
            }
            if(i + 1 < size_x && j + 1 < size_y){
                if(cells[i+1][j+1]) number_of_neighbours ++;
            }
            if(j + 1 < size_y){
                if(cells[i][j+1]) number_of_neighbours ++;
            }
            if(i - 1 > -1 && j + 1 < size_y){
                if(cells[i-1][j+1]) number_of_neighbours ++;
            }
            if(i - 1 > -1){
                if(cells[i-1][j]) number_of_neighbours ++;
            }

            if(cells[i][j] == 0){
                for(k = 0; k < 9; k ++){
                    if(number_of_neighbours == they_are_born[k])
                        cells_updated[i][j] = 1;
                }
            }
            if(cells[i][j] == 1){
                for(k = 0; k < 9; k ++)
                    if(number_of_neighbours == they_live[k])
                        cells_updated[i][j] = 1;
            }
        }
    }
    for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            cells[i][j] = cells_updated[i][j];
        }
    }
}

bool coll_cursor(short x,short y,short x_s,short y_s){
    if(mouse_x>=x&&mouse_x<=x+x_s&&mouse_y>=y&&mouse_y<=y+y_s)
        return true;
    return false;
}
class click{
public:
    bool mp;
    short basex;
    short basey;
    void Do_Click();
    bool Check_Click(short x,short y,short sizex,short sizey);
    void End_Click();
};
void click::Do_Click(){
    if(mouse_b==1&&mp==0){
        mp=1;
        basex=mouse_x;
        basey=mouse_y;
    }
}
bool click::Check_Click(short x,short y,short sizex,short sizey){
    if(mouse_b==0 && mp==1 && coll_cursor(x, y, sizex, sizey) && basex >= x && basex <= x + sizex && basey >= y && basey <=y + sizey){
        return true;
    }
    return false;
}
void click::End_Click(){
    if(mouse_b == 0 && mp == 1)
        mp = 0;
}

void save_code(long they_live_r, long they_are_born_r){
    std::fstream save_file;
    save_file.open("codes.txt", std::ios::app);
    save_file << std::endl;
    save_file << they_live_r << "/" << they_are_born_r;
    save_file.close();
}

void control(click * mouse, int & actions_in_second, bool cells[][size_y], long & they_live_r, long & they_are_born_r, short & entering_code){
    if(mouse->Check_Click(1104, 14, 167, 25))
        random_life_arrangement(cells);
    if(mouse->Check_Click(1104, 49, 167, 25))
        assign_variables(they_live_r, they_are_born_r);
    if(mouse->Check_Click(1104, 84, 167, 25))
        save_code(they_live_r, they_are_born_r);
    if(mouse->Check_Click(1104, 650, 167, 25)){
        color_R = rand() % 100 + 5;
        color_G = rand() % 100 + 5;
        color_B = rand() % 100 + 5;
    }
    if(mouse->Check_Click(1104, 680, 167, 25)){
        if(epilepsy_mode)
            epilepsy_mode = false;
        else epilepsy_mode = true;
    }

    int me = 122;
    for(int i = 0; i < 5; i ++){
        if(mouse->Check_Click(1105, 267 + i * 47 + me, 40, 40)){
            if(i == 0) MAX_cells_number ++;
            if(i == 1) MAX_cells_number += 5;
            if(i == 2) MAX_cells_number += 50;
            if(i == 3) MAX_cells_number += 500;
            if(i == 4) MAX_cells_number += 10000;
        }
        if(mouse->Check_Click(1235, 267 + i * 47 + me, 40, 40)){
            if(i == 0) MAX_cells_number --;
            if(i == 1) MAX_cells_number -= 5;
            if(i == 2) MAX_cells_number -= 50;
            if(i == 3) MAX_cells_number -= 500;
            if(i == 4) MAX_cells_number -= 10000;
        }
    }
    if(MAX_cells_number > size_x * size_y)
        MAX_cells_number = size_x * size_y;
    if(MAX_cells_number < 0)
        MAX_cells_number = 0;

    if(mouse->Check_Click(1105, 150 + me, 40, 40) && _speed < 50){
        actions_in_second = 0;
        _speed ++;
    }

    if(mouse->Check_Click(1235, 150 + me, 40, 40) && _speed > 0){
        actions_in_second = 0;
        _speed --;
    }
}

void draw_buttons(BITMAP * backg, BITMAP * buttons, FONT * font_1){
    int r = 255, g = 255, b = 255;
    if(mouse_b == 1 && coll_cursor(1104, 14, 167, 25)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 14, 167, 25)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Reset life", 1110, 15, makecol(r, g, b), -1);
    rect(backg, 1104, 14, 1104 + 167, 14 + 25, makecol(r, g, b));
    rect(backg, 1105, 15, 1105 + 165, 15 + 23, makecol(r, g, b));

    r = 255; g = 255; b = 255;
    if(mouse_b == 1 && coll_cursor(1104, 49, 167, 25)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 49, 167, 25)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Generate code", 1110, 50, makecol(r, g, b), -1);
    rect(backg, 1104, 49, 1104 + 167, 49 + 25, makecol(r, g, b));
    rect(backg, 1105, 50, 1105 + 165, 50 + 23, makecol(r, g, b));

    r = 255; g = 255; b = 255;
    if(mouse_b == 1 && coll_cursor(1104, 84, 167, 25)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 84, 167, 25)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Save code", 1110, 85, makecol(r, g, b), -1);
    rect(backg, 1104, 84, 1104 + 167, 84 + 25, makecol(r, g, b));
    rect(backg, 1105, 85, 1105 + 165, 85 + 23, makecol(r, g, b));

    int me = 122;
    if(mouse_b == 1 && coll_cursor(1105, 150 + me, 40, 40))
        blit(buttons, backg, 80, 0, 1105, 150 + me, 40, 40);
    else if(coll_cursor(1105, 150 + me, 40, 40))
        blit(buttons, backg, 40, 0, 1105, 150 + me, 40, 40);
    else
        blit(buttons, backg, 0, 0, 1105, 150 + me, 40, 40);

    if(mouse_b == 1 && coll_cursor(1235, 150 + me, 40, 40))
        blit(buttons, backg, 80, 40, 1235, 150 + me, 40, 40);
    else if(coll_cursor(1235, 150 + me, 40, 40))
        blit(buttons, backg, 40, 40, 1235, 150 + me, 40, 40);
    else
        blit(buttons, backg, 0, 40, 1235, 150 + me, 40, 40);

    int i = 0;
    for(i = 0; i < 5; i ++){
        if(mouse_b == 1 && coll_cursor(1105, 267 + i * 47 + me, 40, 40))
            blit(buttons, backg, 80, 0, 1105, 267 + i * 47 + me, 40, 40);
        else if(coll_cursor(1105, 267 + i * 47 + me, 40, 40))
            blit(buttons, backg, 40, 0, 1105, 267 + i * 47 + me, 40, 40);
        else
            blit(buttons, backg, 0, 0, 1105, 267 + i * 47 + me, 40, 40);

        if(mouse_b == 1 && coll_cursor(1235, 267 + i * 47 + me, 40, 40))
            blit(buttons, backg, 80, 40, 1235, 267 + i * 47 + me, 40, 40);
        else if(coll_cursor(1235, 267 + i * 47 + me, 40, 40))
            blit(buttons, backg, 40, 40, 1235, 267 + i * 47 + me, 40, 40);
        else
            blit(buttons, backg, 0, 40, 1235, 267 + i * 47 + me, 40, 40);
    }

    r = 255; g = 255; b = 255;
    if(mouse_b == 1 && coll_cursor(1104, 650, 167, 25)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 650, 167, 25)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Generate color", 1108, 645, makecol(r, g, b), -1);
    rect(backg, 1104, 644, 1104 + 167, 644 + 25, makecol(r, g, b));
    rect(backg, 1105, 645, 1105 + 165, 645 + 23, makecol(r, g, b));

    r = 255; g = 255; b = 255;
    if(mouse_b == 1 && coll_cursor(1104, 680, 167, 25)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 680, 167, 25)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Active epilepsy", 1108, 680, makecol(r, g, b), -1);
    rect(backg, 1104, 679, 1104 + 167, 679 + 25, makecol(r, g, b));
    rect(backg, 1105, 680, 1105 + 165, 680 + 23, makecol(r, g, b));
}
void draw_text(BITMAP * backg, FONT * font_1, long they_live_r, long they_are_born_r, short & entering_code){
    char alive_cells_count_text[10];
    int me = 122;

    hline(backg, 1100,  me, 1280, makecol(255, 255, 255));

    int r = 255, g = 255, b = 255;
    if(entering_code == 1){
        r = 181; g = 230; b = 29;
    }
    else if(mouse_b == 1 && coll_cursor(1104, 5 + me, 167, 45)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 5 + me, 167, 45)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Code Survival", 1105, 5 + me, makecol(r, g, b), -1);
    sprintf(alive_cells_count_text, "%ld", they_live_r);
    if(they_live_r > -1) textout_ex(backg, font_1, alive_cells_count_text, 1105, 30 + me, makecol(r, g, b), -1);

    r = 255; g = 255; b = 255;
    if(entering_code == 2){
        r = 181; g = 230; b = 29;
    }
    else if(mouse_b == 1 && coll_cursor(1104, 60 + me, 167, 45)){
        r = 31; g = 160; b = 70;
    }
    else if(coll_cursor(1104, 60 + me, 167, 45)){
        r = 77; g = 221; b = 120;
    }
    textout_ex(backg, font_1, "Code Birth", 1105, 60 + me, makecol(r, g, b), -1);
    sprintf(alive_cells_count_text, "%ld", they_are_born_r);
    if(they_are_born_r > -1) textout_ex(backg, font_1, alive_cells_count_text, 1105, 85 + me, makecol(r, g, b), -1);

    hline(backg, 1100, 115 + me, 1280, makecol(255, 255, 255));

    textout_ex(backg, font_1, "Simulation speed", 1103, 120 + me, makecol(255, 255, 255), -1);
    sprintf(alive_cells_count_text, "%d", _speed);
    textout_ex(backg, font_1, alive_cells_count_text, 1155, 157 + me, makecol(255, 255, 255), -1);
    textout_ex(backg, font_1, "/50", 1187, 157 + me, makecol(255, 255, 255), -1);

    hline(backg, 1100, 203 + me, 1280, makecol(255, 255, 255));

    textout_ex(backg, font_1, "Quantity of life", 1103, 208 + me, makecol(255, 255, 255), -1);
    sprintf(alive_cells_count_text, "%d", MAX_cells_number);
    textout_ex(backg, font_1, alive_cells_count_text, 1103, 235 + me, makecol(255, 255, 255), -1);

    textout_ex(backg, font_1, "+/-1", 1172, 277 + me, makecol(255, 255, 255), -1);
    textout_ex(backg, font_1, "+/-5", 1172, 324 + me, makecol(255, 255, 255), -1);
    textout_ex(backg, font_1, "+/-50", 1166, 371 + me, makecol(255, 255, 255), -1);
    textout_ex(backg, font_1, "+/-500", 1160, 418 + me, makecol(255, 255, 255), -1);
    textout_ex(backg, font_1, "+/-10000", 1150, 465 + me, makecol(255, 255, 255), -1);

    hline(backg, 1100, 508 + me, 1280, makecol(255, 255, 255));

    /*for(int i = 0; i < 9; i ++){
        sprintf(alive_cells_count_text, "%d", they_live[i]);
        textout_ex(backg, font_1, alive_cells_count_text, 1103 + i * 18, 500 + me, makecol(255, 255, 255), -1);
        sprintf(alive_cells_count_text, "%d", they_are_born[i]);
        textout_ex(backg, font_1, alive_cells_count_text, 1103 + i * 18, 520 + me, makecol(255, 255, 255), -1);
    }*/
}

int key_to_number(){
    if(key[KEY_0]){
        key[KEY_0] = false;
        return 0;
    }
    else if(key[KEY_1]){
        key[KEY_1] = false;
        return 1;
    }
    else if(key[KEY_2]){
        key[KEY_2] = false;
        return 2;
    }
    else if(key[KEY_3]){
        key[KEY_3] = false;
        return 3;
    }
    else if(key[KEY_4]){
        key[KEY_4] = false;
        return 4;
    }
    else if(key[KEY_5]){
        key[KEY_5] = false;
        return 5;
    }
    else if(key[KEY_6]){
        key[KEY_6] = false;
        return 6;
    }
    else if(key[KEY_7]){
        key[KEY_7] = false;
        return 7;
    }
    else if(key[KEY_8]){
        key[KEY_8] = false;
        return 8;
    }
    else return -1;
}

void creat_code(click * mouse, short & entering_code, long & they_live_r, long & they_are_born_r){
    if(mouse->Check_Click(1104, 5 + 125, 167, 45))
        entering_code = 1;
    else if(mouse->Check_Click(1104, 60 + 125, 167, 45))
        entering_code = 2;
    else if(mouse->Check_Click(0, 0, 1280, 720))
        entering_code = 3;

    if(entering_code == 0)
        return;

    int help = key_to_number();
    if(help != -1){
        if(they_live_r == -1)
            they_live_r = 0;
        if(entering_code == 1){
            they_live_r *= 10;
            they_live_r += help;
        }
        if(they_are_born_r == -1)
                they_are_born_r = 0;
        if(entering_code == 2){
            they_are_born_r *= 10;
            they_are_born_r += help;
        }
    }
    if(key[KEY_BACKSPACE]){
        key[KEY_BACKSPACE] = false;
        if(entering_code == 1)
            they_live_r /= 10;
        if(entering_code == 2)
            they_are_born_r /= 10;
        if(they_live_r == 0){
            they_live_r = -1;
        }
        if(they_are_born_r == 0){
            they_are_born_r = -1;
        }
    }
    while(they_live_r > 999999999){
        they_live_r /= 10;
    }
    while(they_are_born_r > 999999999){
        they_are_born_r /= 10;
    }

    if(key[KEY_ENTER] || entering_code == 3){
        key[KEY_ENTER] = false;
        entering_code = 0;
        int i;
        long they_live_r_ = they_live_r;
        long they_are_born_r_ = they_are_born_r;
        for(i = 0; i < 9; i ++){
            they_live[i] = -1;
            they_are_born[i] = -1;
        }
        if(they_live_r_ > -1){
            i = 0;
            do{
                they_live[i] = they_live_r_ % 10;
                they_live_r_ /= 10;
                i ++;
            }while(they_live_r_ > 0);
        }
        if(they_are_born_r_ > -1){
            i = 0;
            do{
                they_are_born[i] = they_are_born_r_ % 10;
                they_are_born_r_ /= 10;
                i ++;
            }while(they_are_born_r_ > 0);
        }
    }
}


int main()
{
    srand(time(NULL));
    allegro_init();
    install_keyboard();
    install_mouse();

    install_timer();
    install_int_ex(ihspeed, BPS_TO_TIMER(100));
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1280, 720, 0, 0);
    BITMAP * backg = NULL;
    BITMAP * cells_backg = NULL;
    BITMAP * buttons_textures = NULL;
    BITMAP * cursor = NULL;
    backg = create_bitmap(1280, 720);
    cells_backg = create_bitmap(1280, 720);
    buttons_textures = load_bmp("buttons.bmp", default_palette);
    cursor = load_bmp("cursor_1.bmp", default_palette);

    FONT * font_1 = NULL;
    font_1 = load_font("font_1.pcx", default_palette, NULL);

    long they_live_r = 23, they_are_born_r = 3;
    assign_variables(they_live_r, they_are_born_r);
    short entering_code = 0;

    int actions_in_second = 0;

    bool cells[size_x][size_y];
    bool cells_updated[size_x][size_y];
    int i, j;
    for(i = 0; i < size_x; i ++){
        for(j = 0; j < size_y; j ++){
            cells[i][j] = 0;
        }
    }

    click mouse;

    while(!key[KEY_ESC]){
        while(hspeed > 0){
            if(mouse_b == 1 && key[KEY_Q] && coll_cursor(0, 0, 1100, 720))
                cells[mouse_x/4][mouse_y/4] = 1;
            while(actions_in_second >= 50){
                update_cells(cells, cells_updated);
                actions_in_second -= 50;
            }
            if(actions_in_second < 0){
                actions_in_second = 0;
            }
            mouse.Do_Click();
            control(&mouse, actions_in_second, cells, they_live_r, they_are_born_r, entering_code);
            creat_code(&mouse, entering_code, they_live_r, they_are_born_r);
            mouse.End_Click();

            hspeed --;
            actions_in_second += _speed;
        }
        clear_to_color(backg, makecol(0, 0, 0));
        clear_to_color(cells_backg, makecol(0, 0, 0));
        draw_cells(cells_backg, cells);
        draw_sprite(backg, cells_backg, 0, 0);

        draw_buttons(backg, buttons_textures, font_1);
        draw_text(backg, font_1, they_live_r, they_are_born_r, entering_code);
        draw_sprite(backg, cursor, mouse_x, mouse_y);
        if(key[KEY_2] && key[KEY_1] && key[KEY_3] && key[KEY_7])
            textout_ex(backg, font, "By Sinomu", 0, 0, makecol(255, 255, 255), 0);
        draw_sprite(screen, backg, 0, 0);
    }

    remove_int(ihspeed);
    destroy_bitmap(backg);
    destroy_bitmap(cells_backg);
    destroy_bitmap(buttons_textures);
    destroy_bitmap(cursor);
    destroy_font(font_1);
    allegro_exit();
    return 0;
}
END_OF_MAIN();
