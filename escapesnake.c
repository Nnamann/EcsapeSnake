#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h> // Required for setlocale()

#ifdef _WIN32 // to play audio cross platform
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

int play_intro_audio();

int eat_audio();

int over_audio();

int stop_audio();

int randomness(int n); // Generates random no. (where needed)

int surprise[2] = {1, 1}; // Stores location of last surprise food

int show_start_screen();
char show_game_result_screen(int result, int score, int high_score);
int show_exit_screen();

#define row 20 // Dimensions of the window
#define column 32

int fruit(char win[row][column], char ch);

int main()
{

  // Initialize ncurses
  setlocale(LC_ALL, ""); // LC_ALL sets all locale categories, "" uses environment defaults
  initscr();             // Start ncurses mode
  start_color();         // Starts colors functionality
  cbreak();              // Disable line buffering
  noecho();              // Don't echo input
  curs_set(0);           // Hide cursor
  keypad(stdscr, TRUE);  // Enable arrow keys
  nodelay(stdscr, TRUE); // Non-blocking input

  // Define your color pairs here
  init_pair(1, COLOR_GREEN, COLOR_BLACK);   // Snake body
  init_pair(2, COLOR_RED, COLOR_BLACK);     // Food
  init_pair(3, COLOR_CYAN, COLOR_BLACK);    // surprise
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // Border
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);  // Score
  // FOR WINDOW
  init_pair(6, COLOR_WHITE, COLOR_BLUE);
  init_pair(7, COLOR_GREEN, COLOR_BLUE);
  init_pair(8, COLOR_YELLOW, COLOR_BLUE);
  init_pair(9, COLOR_RED, COLOR_BLUE);
  init_pair(11, COLOR_RED, COLOR_WHITE);

  srand(time(NULL)); // seeds srand fot fruit generation

  int i, j, x, y, len; // x&y :- coordinates,len :- length of snake head

  int edge = 0, x1 = 0, y1 = 1, gate1[2], gate2[2];                             // edge:controls direction, x1 & y1: directs flow of head(difference), gates: two gates on righ and left with their x and y coordinates
  int time = 0, eaten = 1, scr = 50, update = 0, result = 0, time_control = 90; // time: time based control for sanke input(when time= 0 or 10) (time= 10: 0.1 sec);  eaten, a flag to regulate fruit pop up; update: flag for display of powerups; scr=score; time_control= influences frame rate and snake speed,  won = winning flag
  int high_score = 0;                                                           // stores the highest score

  char win[row][column]; // win:the window;
  int ch = 0;            // ch= choice of key

  char score1[column + 1]; // +1 for null terminator
  char score2[column];
  char score3[column];

again: // game starts again

  if (show_start_screen() == 1) // Starting menu
  {
    show_exit_screen();
  }

  { // intialising Dashboard display
    for (int i = 0; i < column; i++)
      score1[i] = '=';
    score1[0] = '+';
    score1[column - 1] = '+';
    score1[column] = '\0';

    for (int i = 0; i < column; i++)
    {
      score2[i] = ' ';
      score3[i] = ' ';
    }

    score2[1] = 'S';
    score2[2] = 'C';
    score2[3] = 'O';
    score2[4] = 'R';
    score2[5] = 'E';
    score2[6] = '=';
    score2[7] = '0';
    score2[8] = '0';
    score2[9] = '5';
    score2[10] = '0';

    score2[column - 7] = '0';
    score2[column - 6] = '5';
    score2[column - 5] = '/';
    score2[column - 4] = '5';
    score2[column - 3] = '0';

    score2[column - 2] = '\0';

    score3[column - 2] = '\0';

    { // Name Initalisation:   <EscapeSnake[][]:>
      score3[0] = ' ';
      score3[1] = '[';
      score3[2] = 'E';
      score3[3] = 's';
      score3[4] = 'c';
      score3[5] = 'a';
      score3[6] = 'p';
      score3[7] = 'e';
      score3[8] = 'S';
      score3[9] = 'n';
      score3[10] = 'a';
      score3[11] = 'k';
      score3[12] = 'e';
      score3[13] = '[';
      score3[14] = ']';
      score3[15] = ':';
      score3[16] = '>';
    }
  }

  for (i = 0; i < row; i++) // initializing the characters for window
  {

    for (j = 0; j < column; j++)
    {
      if (i == 0 || i == row - 1) // horizontal boundary
      {
        win[i][j] = '=';
        win[i][0] = win[i][column - 1] = '+';
      }

      else if (j == 0 || j == column - 1)
        win[i][j] = '|'; // vertical boundary

      else
        win[i][j] = ' '; // the screen fill up
    }
  }

  { // initializing snake
    x = 5;
    y = 10;
    len = 5;
    i = 0;
    for (j = y; j > y - len; j--) // initializing inital position of snake
      win[x][j] = 65 + i++;       // provides each snake block an integer value in increasing order
                                  // 65 here taken just as a base value(65=A)
  }

  { // gate positions
    gate1[1] = row / 2;
    gate2[1] = row / 2;
    gate1[0] = 0;
    gate2[0] = column - 1;

    win[gate1[1]][gate1[0]] = ' ';
    win[gate2[1]][gate2[0]] = ' ';
    win[gate1[1] - 1][gate1[0]] = '=';
    win[gate2[1] - 1][gate2[0]] = '=';
    win[gate1[1] + 1][gate1[0]] = '=';
    win[gate2[1] + 1][gate2[0]] = '=';
  }

  while (y < (column - 1) && y > 0 && x > 0 && x < (row - 1)) // GAME STARTS
  {

    if (eaten == 1) // fruit integration into matrix
    {
      eaten = fruit(win, '$');
    } //!!!!

    clear();   // Clear screen
    refresh(); // Update screen

    // Display
    { // DASHBOARD 1
      attron(COLOR_PAIR(4) | A_BOLD);
      printw("                     %s\n                     |", score1);
      attroff(COLOR_PAIR(4) | A_BOLD);

      attron(COLOR_PAIR(5) | A_BOLD);
      printw("%s", score2); // Score Display
      attroff(COLOR_PAIR(5) | A_BOLD);

      attron(COLOR_PAIR(4) | A_BOLD);
      printw("|\n");
      attroff(COLOR_PAIR(4) | A_BOLD);
    }
    for (i = 0; i < row; i++) // SCREEN DISPLAY
    {
      printw("                     ");
      for (j = 0; j < column; j++)
      {

        if (win[i][j] == 65 + len) //(END REMOVAL) this condition removes the extra last block from end
          win[i][j] = ' ';

        if (win[i][j] >= 65 && win[i][j] <= 122)
        {
          {
            attron(COLOR_PAIR(1) | A_BOLD);
            printw("𝚯"); // Snake
            attroff(COLOR_PAIR(1) | A_BOLD);
          }
          if (time == 2) // controls snake movement within the set movement rate by the time condition
            ++win[i][j];
        }
        else if (win[i][j] == '$')
        {
          attron(COLOR_PAIR(2) | A_BOLD);
          printw("♥︎"); // Food
          attroff(COLOR_PAIR(2) | A_BOLD);
        }
        else if (win[i][j] == '?')
        {
          printw("🛢️"); // Surprise
        }
        else if (win[i][j] == '|' || win[i][j] == '=' || win[i][j] == '+')
        {
          attron(COLOR_PAIR(4) | A_BOLD);
          printw("%c", win[i][j]); // Border
          attroff(COLOR_PAIR(4) | A_BOLD);
        }
        else
          printw("%c", win[i][j]);
      }
      printw("\n");
    }
    { // DASHBOARD 2
      attron(COLOR_PAIR(4) | A_BOLD);
      printw("                     |");
      attroff(COLOR_PAIR(4) | A_BOLD);

      attron(COLOR_PAIR(5) | A_BOLD);
      printw("%s", score3); // Dashboard
      attroff(COLOR_PAIR(5) | A_BOLD);

      attron(COLOR_PAIR(4) | A_BOLD);
      printw("|\n                     %s\n", score1);
      attroff(COLOR_PAIR(4) | A_BOLD);
    }
    refresh();

    // the Keyboard actions
    if (time == 2) //   Snake movement and direction input moderation
    {
      ch = getch();

      if (ch) // Direction input logics
      {

        if (edge % 2 == 0)
          switch (ch)
          {
          case KEY_UP:
            x1 = -1;
            y1 = 0;
            ++edge;
            break;

          case KEY_DOWN:
            x1 = 1;
            y1 = 0;
            ++edge;
            break;

          case 'q':
            goto end;
          }
        else
          switch (ch)
          {
          case KEY_LEFT:
            x1 = 0;
            y1 = -1;
            ++edge;
            break;

          case KEY_RIGHT:
            x1 = 0;
            y1 = 1;
            ++edge;
            break;

          case 'q':
            goto end;
          }
        if (ch == ' ') // PAUSE THE GAME
        {

          attron(COLOR_PAIR(8) | A_BOLD);
          printw("                     GAME PAUSED!   PRESS 'SPACEBAR' \n                                    PRESS 'Q' to exit");
          attroff(COLOR_PAIR(8) | A_BOLD);

          char pause;

        unpause:
          pause = getch();
          if (pause == 'q' || pause == 'Q')
            goto end;
          else if (pause != ' ')
            goto unpause;
        }
      }

      ch = 0;

      x = x + x1; // new head positions
      y = y + y1;

      {                                           // GAME LOGICS
        if ((y == (column - 1) && x == gate1[1])) // Gate logic
          y = 1;
        else if (y == (0) && x == gate2[1])
          y = column - 2;

        if (y >= (column - 1) || y <= 0 || x <= 0 || x >= (row - 1)) // crash
        {
          goto end;
        }

        else if (win[x][y] == ' ') // regular update
        {
          win[x][y] = 65;
        } // signifies forward movement(front block added)

        else if (win[x][y] == '$') // if fruit is eaten snake grows
        {
          len++;
          eaten = 1;

          win[x][y] = 65;

          { // score update
            scr = scr + 10;
            score2[7] = scr / 1000 + 48;
            score2[8] = (scr % 1000) / 100 + 48;
            score2[9] = (scr % 100) / 10 + 48;

            score2[column - 7] = len / 10 + 48;
            score2[column - 6] = len % 10 + 48;
          }

          if (scr % 80 == 0) // implants surprise foods
          {
            if (win[surprise[0]][surprise[1]] == '?') // Clears previous surprise if left
              win[surprise[0]][surprise[1]] = ' ';

            fruit(win, '?');
          }

          { // dashboard update  w.r.t to length increase
            score3[0] = ' ';
            score3[1] = '[';
            score3[2] = 'E';
            score3[3] = 's';
            score3[4] = 'c';
            score3[5] = 'a';
            score3[6] = 'p';
            score3[7] = 'e';
            score3[8] = 'S';
            score3[9] = 'n';
            score3[10] = 'a';
            score3[11] = 'k';
            score3[12] = 'e';
            score3[13] = '[';
            score3[14] = ']';
            score3[15] = ':';
            score3[16] = '>';

            score3[column - 8] = 'Y';
            score3[column - 7] = 'U';
            score3[column - 6] = 'M';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = ')';
            update = 0;
          }

          if (len % 10 == 0) // SPEED UP
          {
            time_control -= 10;
            score3[column - 8] = ' ';
            score3[column - 7] = ' ';
            score3[column - 6] = '>';
            score3[column - 5] = '>';
            score3[column - 4] = '>';
            score3[column - 3] = '>';
          }

          eat_audio(); // sound: beep
          refresh();
        }

        else if (win[x][y] >= 65 && win[x][y] < 122) // self eaten
        {
          printw("DEAD!!");
          goto end;
        }

        else if (win[x][y] == '?') // surprise food eaten
        {
          win[x][y] = 65;
          int r = randomness(25); // generates random no. to create random foods
         
          if (r <= 6) // FRUIT
          {
            scr = scr + 80;
            score3[1] = ' ';
            score3[2] = ' ';
            score3[3] = 'F';
            score3[4] = 'R';
            score3[5] = 'U';
            score3[6] = 'I';
            score3[7] = 'T';
            score3[8] = 'S';
            score3[9] = ' ';
            score3[10] = ' ';
            score3[11] = ' ';
            score3[12] = '+';
            score3[13] = '8';
            score3[14] = '0';

            score3[column - 8] = 'Y';
            score3[column - 7] = 'U';
            score3[column - 6] = 'M';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = ')';
            update = 0;
          }
          else if (r <= 9) // SLOW
          {
            time_control += 15;
            score3[1] = '<';
            score3[2] = '<';
            score3[3] = 'S';
            score3[4] = 'L';
            score3[5] = 'O';
            score3[6] = 'W';
            score3[7] = ' ';
            score3[8] = 'D';
            score3[9] = 'O';
            score3[10] = 'W';
            score3[11] = 'N';
            score3[12] = '<';
            score3[13] = '<';
            score3[14] = ' ';

            score3[column - 8] = 'Y';
            score3[column - 7] = 'A';
            score3[column - 6] = 'Y';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = ')';
            update = 0;
          }
          else if (r <= 14) // JUNK
          {
            scr = scr - 50;

            if (scr < 0)
              scr = 0;

            score3[1] = ' ';
            score3[2] = ' ';
            score3[3] = 'J';
            score3[4] = 'U';
            score3[5] = 'N';
            score3[6] = 'K';
            score3[7] = ' ';
            score3[8] = ' ';
            score3[9] = ' ';
            score3[10] = ' ';
            score3[11] = ' ';
            score3[12] = '-';
            score3[13] = '5';
            score3[14] = '0';

            score3[column - 8] = 'H';
            score3[column - 7] = 'U';
            score3[column - 6] = 'H';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = '(';
            update = 0;
          }
          else if (r <= 18) // FAST
          {
            time_control -= 5;
            score3[1] = ' ';
            score3[2] = ' ';
            score3[3] = 'F';
            score3[4] = 'A';
            score3[5] = 'S';
            score3[6] = 'T';
            score3[7] = ' ';
            score3[8] = 'L';
            score3[9] = 'I';
            score3[10] = 'F';
            score3[11] = 'E';
            score3[12] = ' ';
            score3[13] = '>';
            score3[14] = '>';

            score3[column - 8] = 'H';
            score3[column - 7] = 'U';
            score3[column - 6] = 'H';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = '(';
            update = 0;
          }
          else if (r == 19) // Dead
          {
            score3[1] = ' ';
            score3[2] = 'P';
            score3[3] = 'O';
            score3[4] = 'I';
            score3[5] = 'S';
            score3[6] = 'O';
            score3[7] = 'N';
            score3[8] = 'E';
            score3[9] = 'D';
            score3[10] = ' ';
            score3[11] = ' ';
            score3[12] = ' ';
            score3[13] = ' ';
            score3[14] = ' ';
            score3[15] = ' ';
            score3[16] = ' ';

            score3[column - 8] = 'H';
            score3[column - 7] = 'U';
            score3[column - 6] = 'H';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = '(';
            update = 0;
            goto end;
          }
          else if (r <= 22) // Super
          {
            scr = scr + 500;
            score3[1] = 'J';
            score3[2] = 'A';
            score3[3] = 'C';
            score3[4] = 'K';
            score3[5] = 'P';
            score3[6] = 'O';
            score3[7] = 'T';
            score3[8] = ' ';
            score3[9] = ' ';
            score3[10] = ' ';
            score3[11] = '+';
            score3[12] = '5';
            score3[13] = '0';
            score3[14] = '0';

            score3[column - 8] = 'H';
            score3[column - 7] = 'E';
            score3[column - 6] = 'H';
            score3[column - 5] = 'E';
            score3[column - 4] = ':';
            score3[column - 3] = ')';
            update = 0;
          }
          else // EMPTY
          {
            scr = scr + 0;
            score3[1] = 'N';
            score3[2] = 'O';
            score3[3] = ' ';
            score3[4] = 'L';
            score3[5] = 'U';
            score3[6] = 'C';
            score3[7] = 'K';
            score3[8] = ' ';
            score3[9] = ' ';
            score3[10] = ' ';
            score3[11] = ' ';
            score3[12] = '+';
            score3[13] = '0';
            score3[14] = '0';

            score3[column - 8] = 'H';
            score3[column - 7] = 'U';
            score3[column - 6] = 'H';
            score3[column - 5] = ' ';
            score3[column - 4] = ':';
            score3[column - 3] = '|';
            update = 0;
          }

          // Score update
          score2[7] = scr / 1000 + 48;
          score2[8] = (scr % 1000) / 100 + 48;
          score2[9] = (scr % 100) / 10 + 48;
          eat_audio();
        }

        else // undefined condition
        {
          printw("Undefined Condition!!!");
          goto end;
        }

        if (len >= 50) // won
        {
          result = 1;
          goto end;
        }
      }
      time = 0;
    }

    if (update == 12)
    {
      // DASHBOARD UPDATES
      score3[column - 8] = ' ';
      score3[column - 7] = ' ';
      score3[column - 6] = ' ';
      score3[column - 5] = ' ';
      score3[column - 4] = ' ';
      score3[column - 3] = ' ';

      update = 0;
    }

    if (time_control <= 20) // regulates edge condition
    {
      time_control = 20;
    }
    napms(time_control); // <=0.11s fps
    time++;
    update++;
  }
end:
  win[x][y] = 65;
  over_audio();

  if (scr >= high_score)
  {
    high_score = scr;
  }

  // END DISPLAY
  {
    clear();   // Clear screen
    refresh(); // Update screen

    // Display
    { // DASHBOARD 1
      attron(COLOR_PAIR(4) | A_BOLD);
      printw("                     %s\n                     |", score1);
      attroff(COLOR_PAIR(4) | A_BOLD);

      attron(COLOR_PAIR(5) | A_BOLD);
      printw("%s", score2); // Score Display
      attroff(COLOR_PAIR(5) | A_BOLD);

      attron(COLOR_PAIR(4) | A_BOLD);
      printw("|\n");
      attroff(COLOR_PAIR(4) | A_BOLD);
    }
    for (int i = 0; i < row; i++) // SCREEN DISPLAY
    {
      printw("                     ");
      for (int j = 0; j < column; j++)
      {

        if (win[i][j] == 65 + len) //(END REMOVAL) this condition removes the extra last block from end
          win[i][j] = ' ';

        if (win[i][j] > 65 && win[i][j] <= 122)
        {
          {
            attron(COLOR_PAIR(1) | A_BOLD);
            printw("𝚯"); // Snake
            attroff(COLOR_PAIR(1) | A_BOLD);
          }
        }
        else if (win[i][j] == 65)
        {
          if (result) // won
          {
            attron(COLOR_PAIR(5) | A_BOLD);
            printw("♛"); // Snake Head
            attroff(COLOR_PAIR(5) | A_BOLD);
          }
          else // lost
          {
            attron(COLOR_PAIR(5) | A_BOLD);
            printw("✝"); // Snake Head
            attroff(COLOR_PAIR(5) | A_BOLD);
          }
        }
        else if (win[i][j] == '$')
        {
          attron(COLOR_PAIR(2) | A_BOLD);
          printw("♥︎"); // Food
          attroff(COLOR_PAIR(2) | A_BOLD);
        }
        else if (win[i][j] == '?')
        {
          printw("🛢️"); // Surprise
        }
        else if (win[i][j] == '|' || win[i][j] == '=' || win[i][j] == '+')
        {
          attron(COLOR_PAIR(4) | A_BOLD);
          printw("%c", win[i][j]); // Border
          attroff(COLOR_PAIR(4) | A_BOLD);
        }
        else
        {
          printw("%c", win[i][j]);
        }
      }
      printw("\n");
    }
    { // DASHBOARD 2
      attron(COLOR_PAIR(4) | A_BOLD);
      printw("                     |");
      attroff(COLOR_PAIR(4) | A_BOLD);

      attron(COLOR_PAIR(5) | A_BOLD);
      printw("%s", score3); // Dashboard
      attroff(COLOR_PAIR(5) | A_BOLD);

      attron(COLOR_PAIR(4) | A_BOLD);
      printw("|\n                     %s\n", score1);
      attroff(COLOR_PAIR(4) | A_BOLD);
    }
    refresh();
  }

  attron(COLOR_PAIR(11) | A_BOLD);
  if (result == 0)
    printw("                                !!DEAD!!                ");
  else
    printw("                           !!CONGRATULATIONS!!          ");
  attroff(COLOR_PAIR(11) | A_BOLD);
  refresh();

  napms(1000);

  if (show_game_result_screen(result, scr, high_score) == '\n') // if game starts
  {
    edge = 0, x1 = 0, y1 = 1, time = 0, eaten = 1, scr = 50, update = 0, result = 0, time_control = 90;
    stop_audio();
    goto again;
  }

  stop_audio();
  show_exit_screen();
  endwin();
  return 0;
}

int fruit(char win[row][column], char ch) // generates fruits and surprises
{
  int r1, r2, r3, r4, r5, r6;

  r1 = rand() % (row - 2) + 1;
  r2 = rand() % (column - 2) + 1;
  r3 = rand() % (row - 2) + 1;
  r4 = rand() % (column - 2) + 1;
  r5 = rand() % (row - 2) + 1;
  r6 = rand() % (column - 2) + 1;

  if (win[r1][r2] == ' ')
  {
    win[r1][r2] = ch;
    if (ch == '?')
    {
      surprise[0] = r1;
      surprise[1] = r2;
    }
  }
  else if (win[r3][r4] == ' ')
  {
    win[r3][r4] = ch;
    if (ch == '?')
    {
      surprise[0] = r3;
      surprise[1] = r4;
    }
  }
  else if (win[r5][r6] == ' ')
  {
    win[r5][r6] = ch;
    if (ch == '?')
    {
      surprise[0] = r5;
      surprise[1] = r6;
    }
  }
  else
    return 1;
  return 0;
}

int randomness(int n) // generates random values
{
  int r;
  r = rand() % n + 1; // rand()%n+1 returns random values in range from(1 to n);   IN CASE OF EASY MODE INPUT (1 TO 9)
  return r;
}

int play_intro_audio()
{
#ifdef __APPLE__
  system("afplay intro.wav &");
#elif __linux__
  system("aplay intro.wav &");
#elif _WIN32
  PlaySound(TEXT("intro.wav"), NULL, SND_ASYNC);
#else
  // Unknown OS, no sound
#endif
  return 0;
}

int eat_audio()
{
#ifdef __APPLE__
  system("afplay eat.wav &");
#elif __linux__
  system("aplay eat.wav &");
#elif _WIN32
  PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC);
#else
  // Unknown OS, no sound
#endif
  return 0;
}

int over_audio()
{
#ifdef __APPLE__
  system("afplay over.wav &");
#elif __linux__
  system("aplay over.wav &");
#elif _WIN32
  PlaySound(TEXT("over.wav"), NULL, SND_ASYNC);
#else
  // Unknown OS, no sound
#endif
  return 0;
}


int stop_audio()
{
#ifdef __APPLE__
  system("killall afplay");
#elif __linux__
  system("killall aplay"); // or play/mpg123 depending on what you used
#elif _WIN32
  PlaySound(NULL, 0, 0); // stops audio
#endif
  return 0;
}

int show_start_screen()
{

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  int win_height = 28;
  int win_width = 60;
  int start_y = (max_y - win_height) / 2;
  int start_x = (max_x - win_width) / 2;

  clear();

  // Draw main border
  attron(COLOR_PAIR(4)); // Magenta border

  // Top border
  mvprintw(start_y, start_x, "+");
  for (int i = 1; i < win_width - 1; i++)
  {
    mvprintw(start_y, start_x + i, "=");
  }
  mvprintw(start_y, start_x + win_width - 1, "+");

  // Side borders
  for (int i = 1; i < win_height - 1; i++)
  {
    mvprintw(start_y + i, start_x, "|");
    mvprintw(start_y + i, start_x + win_width - 1, "|");
  }

  // Bottom border
  mvprintw(start_y + win_height - 1, start_x, "+");
  for (int i = 1; i < win_width - 1; i++)
  {
    mvprintw(start_y + win_height - 1, start_x + i, "=");
  }
  mvprintw(start_y + win_height - 1, start_x + win_width - 1, "+");

  attroff(COLOR_PAIR(4));

  // Game Title
  attron(COLOR_PAIR(1) | A_BOLD);
  mvprintw(start_y + 2, start_x + 20, "[EscapeSnake[]:>");
  attroff(COLOR_PAIR(1) | A_BOLD);

  // Subtitle
  attron(COLOR_PAIR(3));
  mvprintw(start_y + 4, start_x + 8, "The Classic Snake Game, Unexpected Surprises!");
  attroff(COLOR_PAIR(3));

  // Separator line
  attron(COLOR_PAIR(4));
  mvprintw(start_y + 5, start_x + 5, "===================================================");
  attroff(COLOR_PAIR(4));

  // Controls section
  attron(COLOR_PAIR(5)); // Yellow
  mvprintw(start_y + 7, start_x + 2, "CONTROLS:");
  attroff(COLOR_PAIR(5));
  mvprintw(start_y + 8, start_x + 8, "Use Arrow Keys to move");

  // Game elements section
  attron(COLOR_PAIR(5)); // Yellow
  mvprintw(start_y + 10, start_x + 2, "GAME ELEMENTS:");
  attroff(COLOR_PAIR(5));

  attron(COLOR_PAIR(2)); // Red
  mvprintw(start_y + 11, start_x + 4, "♥");
  attroff(COLOR_PAIR(2));
  mvprintw(start_y + 11, start_x + 6, "Fruit - Grow & Score");

  attron(COLOR_PAIR(3));
  mvprintw(start_y + 12, start_x + 4, "🛢");
  attroff(COLOR_PAIR(3));
  mvprintw(start_y + 12, start_x + 7, "Mystery Barrel - Random Surprise!");

  mvprintw(start_y + 13, start_x + 8, "• Bonus points  • Speed changes");
  mvprintw(start_y + 14, start_x + 8, "• Voids         • Poison ☠");

  // Rules section
  attron(COLOR_PAIR(5)); // Yellow
  mvprintw(start_y + 16, start_x + 2, "RULES:");
  attroff(COLOR_PAIR(5));
  mvprintw(start_y + 17, start_x + 4, "• Avoid walls and your own body");
  mvprintw(start_y + 18, start_x + 4, "• Snake speeds up as it grows  >>>> ");
  mvprintw(start_y + 19, start_x + 4, "• Maximum length: 50");
  mvprintw(start_y + 20, start_x + 4, "• Risk vs Reward with mystery barrels!");

  // Challenge
  attron(COLOR_PAIR(2) | A_BOLD); // Red bold
  mvprintw(start_y + 22, start_x + 8, "Can you reach the Highest Score?");
  attroff(COLOR_PAIR(2) | A_BOLD);

  // Footer
  attron(COLOR_PAIR(4));
  mvprintw(start_y + 24, start_x + 5, "==================================================");
  attroff(COLOR_PAIR(4));

  attron(COLOR_PAIR(5) | A_BOLD); // Yellow bold
  mvprintw(start_y + 25, start_x + 18, "Press ENTER to play!");
  attroff(COLOR_PAIR(5) | A_BOLD);

  mvprintw(start_y + 7, start_x + 39, "To disable audio:'s'");
  mvprintw(start_y + 8, start_x + 39, "To pause: SPACEBAR");

  refresh();

  mvprintw(0, 0, " ");
  play_intro_audio(); // intro music

  int w = 1;
  while (1)
  {
    char ch = getch();

    if (w == 1 && (ch == 's' || ch == 'S'))
    {
      stop_audio();
      w++;
    }

    if (ch == '\n')
      break;
    else if (ch == 'q' || ch == 'Q')
      return 1;
  }
  return 0;
}

char show_game_result_screen(int result, int score, int high_score)
{
  int height = 15;
  int width = 40;
  int starty = (LINES - height) / 2;
  int startx = (COLS - width) / 2;

  attron(COLOR_PAIR(4)); // Magenta border

  // Top border
  mvprintw(starty, startx, "+");
  for (int i = 1; i < width - 1; i++)
  {
    mvprintw(starty, startx + i, "=");
  }
  mvprintw(starty, startx + width - 1, "+");

  // Side borders
  for (int i = 1; i < height - 1; i++)
  {
    mvprintw(starty + i, startx, "|");
    mvprintw(starty + i, startx + width - 1, "|");
  }

  // Bottom border
  mvprintw(starty + height - 1, startx, "+");
  for (int i = 1; i < width - 1; i++)
  {
    mvprintw(starty + height - 1, startx + i, "=");
  }
  mvprintw(starty + height - 1, startx + width - 1, "+");

  attroff(COLOR_PAIR(4));

  // Title
  attron(COLOR_PAIR(1) | A_BOLD);
  mvprintw(starty + 1, startx + 12, "[EscapeSnake[]:>");
  attroff(COLOR_PAIR(1) | A_BOLD);

  // Separator line
  attron(COLOR_PAIR(4));
  mvprintw(starty + 2, startx + 5, "================================");
  attroff(COLOR_PAIR(4));

  // Result message
  if (result == 1)
  {
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(starty + 4, startx + (width - 10) / 2, "YOU WON!!! 🏆");
    attroff(COLOR_PAIR(2) | A_BOLD);
  }
  else
  {
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(starty + 4, startx + (width - 11) / 2, "YOU LOST!!! :(");
    attroff(COLOR_PAIR(2) | A_BOLD);
  }

  // Score section header
  attron(COLOR_PAIR(5));
  mvprintw(starty + 6, startx + 4, "FINAL SCORE:");
  attroff(COLOR_PAIR(5));

  // Score info
  attron(COLOR_PAIR(1));
  mvprintw(starty + 7, startx + 6, "Your Score    : %d", score);
  mvprintw(starty + 8, startx + 6, "Highest Score : %d", high_score);
  attroff(COLOR_PAIR(1));

  // Separator line
  attron(COLOR_PAIR(4));
  mvprintw(starty + 9, startx + 5, "================================");
  attroff(COLOR_PAIR(4));

  // Instructions header
  attron(COLOR_PAIR(5));
  mvprintw(starty + 11, startx + 4, "OPTIONS:");
  attroff(COLOR_PAIR(5));

  // Instructions
  attron(COLOR_PAIR(3));
  mvprintw(starty + 12, startx + 6, "To Play Again: PRESS ENTER");
  mvprintw(starty + 13, startx + 6, "To Quit:       PRESS Q");
  attroff(COLOR_PAIR(3));

  refresh();

  // user input
  char ch;
  while (1)
  {
    ch = getch();
    if (ch == '\n' || ch == 'q' || ch == 'Q')
      break;
  }

  return ch;
}

int show_exit_screen()
{
  init_pair(6, COLOR_WHITE, COLOR_BLUE);  // Background
  init_pair(7, COLOR_GREEN, COLOR_BLUE);  // Heading
  init_pair(8, COLOR_YELLOW, COLOR_BLUE); // Message

  int win_height = 12, win_width = column + 2;
  int win_starty = (LINES - win_height) / 2;
  int win_startx = (COLS - win_width) / 2;

  WINDOW *win = newwin(win_height, win_width, win_starty, win_startx);
  wbkgd(win, COLOR_PAIR(6));
  box(win, 0, 0);

  // Title
  wattron(win, COLOR_PAIR(7) | A_BOLD);
  mvwprintw(win, 1, (win_width - 20) / 2, "[EscapeSnake[]:>");
  wattroff(win, COLOR_PAIR(7) | A_BOLD);

  // Message
  wattron(win, COLOR_PAIR(8));
  mvwprintw(win, 3, 4, "Thank you for messing around");
  mvwprintw(win, 4, 4, "EscapeSnake!");
  mvwprintw(win, 6, 4, "See you back soon :)");
  mvwprintw(win, 8, 4, "            —Naman Bhardwaj");
  wattroff(win, COLOR_PAIR(8));

  wrefresh(win);

  timeout(5000);
  getch();
  timeout(-1);

  endwin();
  return 0;
}

// ☯︎
