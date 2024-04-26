#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <termios.h>
#include <ncurses.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdint.h>
#include "lcd.h"
unsigned char sqrt_ascii = 0xE8;
int fd;
double res;
typedef enum
{
    DOUBLE_TYPE,
    INT_TYPE
} type_t;

double calculate(uint8_t *math, uint8_t *index, char next_request);
int check_math_avaiable(uint8_t *str);
type_t check_var(double var);
static void lcd_sync(uint8_t *buff, uint8_t row);

void sig_int(int signo);

int main()
{
    uint8_t *token;
    char math[17];
    char Memory[17];
    char input;
    unsigned char i = 0;
    signal(SIGINT, sig_int);
    if (-1 == lcd_start(&fd))
        return -1;
    clear_lcd(&fd);
    initscr(); // init screen
    cbreak();  // Line buffering disabled
    noecho();  // Don't echo &input characters
    memset(math, 0, sizeof(math));
    while (1)
    {
        refresh();
        input = (char)getchar();
        switch (input)
        {
        case '+':
            if (!check_math_avaiable(math))
            {
                math[i++] = input;
                lcd_sync(math, 0);
            }
            else
                res = calculate(math, &i, '+');
            break;
        case '-':
            if (!check_math_avaiable(math))
            {
                math[i++] = input;
                lcd_sync(math, 0);
            }
            else
                res = calculate(math, &i, '-');
            break;
        case '*':
            if (!check_math_avaiable(math))
            {
                math[i++] = input;
                lcd_sync(math, 0);
            }
            else
                res = calculate(math, &i, '*');
            break;
        case '/':
            if (!check_math_avaiable(math))
            {
                math[i++] = input;
                lcd_sync(math, 0);
            }
            else
                res = calculate(math, &i, '/');
            break;
        case '%':
            if (!check_math_avaiable(math))
            {
                math[i++] = input;
                lcd_sync(math, 0);
            }
            else
                res = calculate(math, &i, '%');
            break;
        case 'z': // Memory addition
            memset(Memory, 0, sizeof(Memory));
            sprintf(Memory, "+%.2f", res);
            memcpy(&math[i], Memory, strlen(Memory));
            res = calculate(math, &i, 0);
            break;
        case 'x': // Memory subtraction
            memset(Memory, 0, sizeof(Memory));
            sprintf(Memory, "-%.2f", res);
            memcpy(&math[i], Memory, strlen(Memory));
            res = calculate(math, &i, 0);
            break;
        case 'v': // Memory recall
            memset(math, 0, 17);
            if (check_var(res) == DOUBLE_TYPE)
                sprintf(math, "%.2f", res);
            else
                sprintf(math, "%d", (int)res);
            i = strlen(math);
            lcd_sync(math, 0);
            break;
        case 'b': // Memory Clear
            res = 0;
            break;
        case 'n': // Change Sign
            res = -res;
            break;
        case 's': // sqrt
            res = calculate(math, &i, 's');
            break;
        case 'd': // delete last index
            math[--i] = 0;
            lcd_sync(math, 0);
            break;
        case 'c': // clear all
            memset(math, 0, sizeof(math));
            i = 0;
            clear_lcd(&fd);
            break;
        case '=': // get result
            res = calculate(math, &i, 0);
            break;
        default:
            math[i++] = input;
            lcd_sync(math, 0);
            break;
        }
    //    system("clear");
    //    printf("%s", math);
        i = (i >= 16) ? 0 : i;
    }
    return res;
}

type_t check_var(double var)
{
    if (fabs(var - (int)var) > 0)
        return DOUBLE_TYPE;
    else
        return INT_TYPE;
}

int check_math_avaiable(uint8_t *str)
{
    if (strstr(str, "+") || strstr(str, "-") || strstr(str, "*") || strstr(str, "/"))
        return -1;
    return 0;
}
void sig_int(int signo)
{
    endwin();
    clear_lcd(&fd);
    lcd_stop(&fd);
    printf("User request exit!\n");
    exit(EXIT_SUCCESS);
}
double calculate(uint8_t *math, uint8_t *index, char next_request)
{
    double first_parameter, secon_parameter, result;
    uint8_t *token = NULL;
    char cache[17];
    uint8_t idx;
    memset(cache, 0, 17);
    lcd_sync(math, 0);
    if (strstr(math, "+"))
    {
        token = strstr(math, "+");
        idx = token - math;
        token = strtok(math, "+");
        if (token)
            first_parameter = strtod(token, NULL);
        else
        {
            lcd_sync("Syntax Error", 0);
            return res;
        }
        token = strtok(NULL, "+");
        if (token)
        {
            secon_parameter = strtod(token, NULL);
            result = first_parameter + secon_parameter;
        }
        else
        {
            math[idx] = next_request;
            lcd_sync(math, 0);
            result = first_parameter;
        }
    }
    else if (strstr(math, "-"))
    {
        token = strstr(math, "-");
        idx = token - math;
        token = strtok(math, "-");
        if (token)
            first_parameter = strtod(token, NULL);
        else
        {
            lcd_sync("Syntax Error", 0);
            return res;
        }
        token = strtok(NULL, "-");
        if (token)
        {
            secon_parameter = strtod(token, NULL);
            result = first_parameter - secon_parameter;
        }
        else
        {
            math[idx] = next_request;
            lcd_sync(math, 0);
            result = first_parameter;
        }
    }
    else if (strstr(math, "*"))
    {
        token = strstr(math, "*");
        idx = token - math;
        token = strtok(math, "*");
        if (token)
            first_parameter = strtod(token, NULL);
        else
        {
            lcd_sync("Syntax Error", 0);
            return res;
        }
        token = strtok(NULL, "*");
        if (token)
        {
            secon_parameter = strtod(token, NULL);
            result = first_parameter * secon_parameter;
        }
        else
        {
            math[idx] = next_request;
            lcd_sync(math, 0);
            result = first_parameter;
        }
    }
    else if (strstr(math, "/"))
    {
        token = strstr(math, "/");
        idx = token - math;
        token = strtok(math, "/");
        if (token)
            first_parameter = strtod(token, NULL);
        else
        {
            lcd_sync("Syntax Error", 0);
            return res;
        }
        token = strtok(NULL, "/");
        if (token)
        {
            secon_parameter = strtod(token, NULL);
            result = first_parameter / secon_parameter;
        }
        else
        {
            math[idx] = next_request;
            lcd_sync(math, 0);
            result = first_parameter;
        }
    }
    else if (strstr(math, "%"))
    {
        token = strstr(math, "%");
        idx = token - math;
        token = strtok(math, "%");
        if (token)
            first_parameter = strtod(token, NULL);
        else
        {
            lcd_sync("Syntax Error", 0);
            return res;
        }
        token = strtok(NULL, "%");
        if (token)
        {
            secon_parameter = strtod(token, NULL);
            result = (first_parameter/secon_parameter)*100;
        }
        else
        {
            math[idx] = next_request;
            lcd_sync(math, 0);
            result = first_parameter;
        }
    }
    if (next_request == 's')
    {
        lcd_sync(math, 0);
        lcd_send(&fd, &sqrt_ascii);
        if (strtod(math, NULL) > 0)
        {
            result = sqrt(strtod(math, NULL));
            memset(math, 0, 17);
            if (check_var(result) == DOUBLE_TYPE)
                sprintf(math, "%.2f", result);
            else
                sprintf(math, "%d", (int)result);
            *index = strlen(math);
            lcd_sync(math, 1);
            return result;
        }
    }
    else if (next_request != 's' && next_request)
    {
        memset(math, 0, 17);
        if (check_var(result) == DOUBLE_TYPE)
            sprintf(math, "%.2f%c", result, next_request);
        else
            sprintf(math, "%d%c", (int)result, next_request);
        memcpy(cache, math, strlen(math) - 1);
    }
    else
    {
        memset(math, 0, 17);
        if (check_var(result) == DOUBLE_TYPE)
            sprintf(math, "%.2f", result);
        else
            sprintf(math, "%d", (int)result);
        memcpy(cache, math, strlen(math));
    }
    *index = strlen(math);
    lcd_sync(cache, 1);
    return result;
}
static void lcd_sync(uint8_t *buff, uint8_t row)
{
    lcd_clear_row(&fd, row);
    lcd_goto_xy(&fd, 0, row);
    usleep(1000);
    lcd_send(&fd, buff);
}
