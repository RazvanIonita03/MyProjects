#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ncurses.h>
#include <signal.h>
#include <ctype.h>

/* structuri pentru grafica */
WINDOW *input_win[4], *output_win[4];
int height, width;

int Windows_number = 1, Current_window = 0;

void init_ncurses()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    getmaxyx(stdscr, height, width);

    /* Creare ferestre */
    output_win[0] = newwin(height - 5, width, 0, 0);
    input_win[0] = newwin(5, width, height - 5, 0);

    scrollok(output_win[0], TRUE);
    box(output_win[0], 0, 0);
    box(input_win[0], 0, 0);
    wrefresh(output_win[0]);
    wrefresh(input_win[0]);
}

void close_ncurses()
{
    for (int i = 0; i < Windows_number; i++)
    {
        delwin(input_win[i]);
        delwin(output_win[i]);
    }
    endwin();
}

void print_to_output(int window, const char *message)
{
    int max_y, max_x;
    getmaxyx(output_win[window], max_y, max_x);
    int len = strlen(message);
    int start = 0;

    while (start < len)
    {
        int end = start;
        while (end < len && message[end] != '\n')
        {
            end++;
        }

        char temp[end - start + 1];
        strncpy(temp, message + start, end - start);
        temp[end - start] = '\0';

        int cur_y = getcury(output_win[window]);
        wmove(output_win[window], cur_y + 1, 1);
        wprintw(output_win[window], "%s", temp);

        if (end < len && message[end] == '\n')
        {
            end++;
        }
        start = end;
        box(output_win[window], 0, 0);
    }
    wrefresh(output_win[window]);
}

void clear_input(int window)
{
    werase(input_win[window]);
    box(input_win[window], 0, 0);
    wrefresh(input_win[window]);
}

void handle_character_input(WINDOW *win, char *buf, size_t max_len)
{
    int ch, pos = 0;
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);
    memset(buf, 0, max_len);
    wmove(win, 1, 1);
    while ((ch = wgetch(win)) != '\n' && pos < max_len - 1)
    {
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (pos > 0)
            {
                pos--;
                buf[pos] = '\0';
                mvwaddch(win, 1 + (pos)/ (max_x - 2), ((pos) % (max_x - 2)) + 2, ' ');
            }
        }
        else if (isprint(ch))
        {
            buf[pos++] = ch;
            mvwaddch(win, 1 + pos / (max_x - 2), ((pos)% (max_x - 2)) + 1, ch);
        }
        box(win, 0, 0);
        wrefresh(win);
    }
    buf[pos] = '\0';
}

void create_new_window()
{
    if (Windows_number < 4)
    {
        int new_win = Windows_number++;
        int new_width = width / Windows_number;

        for (int i = 0; i < Windows_number; i++)
        {
            if (output_win[i] != NULL)
            {
                delwin(output_win[i]);
                delwin(input_win[i]);
            }

            output_win[i] = newwin(height - 5, new_width, 0, i * new_width);
            input_win[i] = newwin(5, new_width, height - 5, i * new_width);

            scrollok(output_win[i], TRUE);
            box(output_win[i], 0, 0);
            box(input_win[i], 0, 0);
            wrefresh(output_win[i]);
            wrefresh(input_win[i]);
        }

        print_to_output(new_win, "New window created!");
    }
    else
    {
        print_to_output(Current_window, "Maximum number of windows reached!");
    }
}

void move_to_window(int window)
{
    if (window >= 0 && window < Windows_number)
    {
        Current_window = window;
        print_to_output(Current_window, "Switched to this window!");
    }
    else
    {
        print_to_output(Current_window, "Invalid window number!");
    }
}

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char *argv[])
{
    int sd;                    // descriptorul de socket
    struct sockaddr_in server; // structura folosita pentru conectare
    char buf[2048];

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Please use: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi(argv[2]);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(port);

    /* ne conectam la server */
    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[client]Error connect().\n");
        return errno;
    }

    /* initializare ncurses */
    init_ncurses();
    print_to_output(Current_window, "Conectat la server. Logati-va folosind //login : Admin Admin// Introduceti comenzi sau 'quit' pentru a iesi.");

    while (1)
    {
        wrefresh(input_win[Current_window]);

        handle_character_input(input_win[Current_window], buf, sizeof(buf));
        clear_input(Current_window);

        if (strcmp(buf, "quit") == 0)
            break;
        if (strcmp(buf, "new") == 0)
        {
            create_new_window();
        }
        if(strcmp(buf,"move") == 0)
        {
            move_to_window((Current_window+1)%Windows_number);
        }
        else
        {
            /* trimiterea mesajului la server */
            if (write(sd, buf, strlen(buf)) <= 0)
            {
                perror("[client]Eroare la write() spre server.\n");
                break;
            }

            memset(buf, 0, sizeof(buf));

            /* citirea raspunsului de la server */
            int responseBytes = read(sd, buf, sizeof(buf) - 1);
            if (responseBytes <= 0)
            {
                perror("[client]Eroare la read() de la server.\n");
                break;
            }
            buf[responseBytes] = '\0';

            print_to_output(Current_window,buf); // afisare raspuns in fereastra output
        }
    }

    /* inchidere ncurses si conexiune */
    close_ncurses();
    close(sd);

    return 0;
}
