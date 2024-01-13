#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Determines number of rows and columns.
 */
const int MAP_SIZE = 10;

/**
 * Minimum number of enemies spawned.
 */
const int MIN_ENEMIES = MAP_SIZE;

/**
 * Maximum number of enemies spawned.
 */
const int MAX_ENEMIES = MAP_SIZE + 5;

/**
 * Represents a 2-dimensional vector.
 */
struct Point {
  int x;
  int y;
};

/**
 * Stores game state.
 */
struct Game {
  int num_enemies;
  int health;
  Point spaceship_init;
  Point spaceship;
  bool (*map)[10];
};

/**
 * Clears the terminal screen.
 */
void clear();

/**
 * Sets current print color.
 */
void set_color(int color);

/**
 * Returns a random number in range [a, b).
 */
int rand_range(int a, int b);

/**
 * Generates n enemies on the map randomly.
 */
void generate_enemies(bool map[MAP_SIZE][MAP_SIZE], int n);

/**
 * Checks whether map contains any full row.
 */
bool contains_full_row(bool map[MAP_SIZE][MAP_SIZE]);

/**
 * Checks whether map contains any full column.
 */
bool contains_full_col(bool map[MAP_SIZE][MAP_SIZE]);

/**
 * Initializes the game object with either zero values or randomly generated
 * values.
 */
Game init_game(bool map[MAP_SIZE][MAP_SIZE]);

/**
 * Prints the state of game on the screen.
 */
void render(const Game &game);

/**
 * Handles user inputs infinitely until user quits.
 */
void handle_input(Game &game);

int main() {
  srand(time(0));

  bool map[MAP_SIZE][MAP_SIZE] = {};
  Game game = init_game(map);

  while (true) {
    render(game);

    if (game.health == 0) {
      clear();

      set_color(4);
      cout << " --------------------\n";
      cout << "|     YOU LOSE!      |\n";
      cout << "|  Better luck next  |\n";
      cout << "|       time!        |\n";
      cout << " --------------------\n";

      char c;
      cin >> c;

      exit(0);
    }
    if (game.num_enemies == 0) {
      clear();

      set_color(9);
      cout << " --------------------\n";
      cout << "|      YOU WON!      |\n";
      cout << "|  Congratulations!  |\n";
      cout << " --------------------\n";

      char c;
      cin >> c;

      exit(0);
    }

    handle_input(game);
  }
}

void clear() {
#ifdef __linux__
  system("clear");
#else
  system("cls");
#endif
}

void set_color(int color) {
#ifdef _WIN32
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
  cout << "\033[1;" << color << "m";
#endif
}

int rand_range(int a, int b) { return rand() % (b - a) + a; }

void generate_enemies(bool map[MAP_SIZE][MAP_SIZE], int n) {
  int num_inserted_enemies = 0;

  while (num_inserted_enemies < n) {
    Point p = {
        .x = rand() % MAP_SIZE,
        .y = rand() % MAP_SIZE,
    };

    if (!map[p.y][p.x]) {
      map[p.y][p.x] = true;
      ++num_inserted_enemies;
    }
  }
}

bool contains_full_row(bool map[MAP_SIZE][MAP_SIZE]) {
  for (int i = 0; i < MAP_SIZE; ++i) {
    bool full = true;
    for (int j = 0; j < MAP_SIZE; ++j) {
      if (!map[i][j]) {
        full = false;
      }
    }

    if (full) {
      return true;
    }
  }

  return false;
}

bool contains_full_col(bool map[MAP_SIZE][MAP_SIZE]) {
  for (int i = 0; i < MAP_SIZE; ++i) {
    bool full = true;
    for (int j = 0; j < MAP_SIZE; ++j) {
      if (!map[j][i]) {
        full = false;
      }
    }

    if (full) {
      return true;
    }
  }

  return false;
}

Game init_game(bool map[MAP_SIZE][MAP_SIZE]) {
  int num_enemies = rand_range(MIN_ENEMIES, MAX_ENEMIES + 1);

  do {
    generate_enemies(map, num_enemies);
  } while (contains_full_row(map) || contains_full_col(map));

  Point spaceship;
  do {
    spaceship.y = rand() % MAP_SIZE;
    spaceship.x = rand() % MAP_SIZE;
  } while (map[spaceship.y][spaceship.x]);

  Game game = {
      .num_enemies = num_enemies,
      .health = 3,
      .spaceship_init = spaceship,
      .spaceship = spaceship,
      .map = map,
  };

  return game;
}

void render(const Game &game) {
  clear();

  cout << "health: " << game.health << "\n\n";

  set_color(9);
  for (int ifor = 0; ifor < 2 * MAP_SIZE; ++ifor) {
    if (ifor % 2 == 0) {
      for (int j = 0; j < MAP_SIZE; ++j) {
        cout << " ---";
      }

      cout << endl;
      continue;
    }

    int i = ifor / 2;
    for (int j = 0; j < MAP_SIZE; ++j) {
      cout << "|";
      char ch = ' ';
      if (game.spaceship.y == i && game.spaceship.x == j) {
        set_color(6);
        ch = 'O';
      } else if (game.map[i][j]) {
        set_color(4);
        ch = '*';
      }

      cout << " " << ch << " ";
      set_color(9);
    }

    cout << "|" << endl;
  }

  for (int j = 0; j < MAP_SIZE; ++j) {
    cout << " ---";
  }
  cout << endl;

  set_color(15);
}

void handle_input(Game &game) {
  while (true) {
    unsigned char key;
    cout << endl << "A : fire to right side, D : fire to left side ";
    cout << endl << "Use arrow keys to move ";
    key = _getch();

    switch (key) {
    case 224: {
      int dir;
      dir = _getch();

      switch (dir) {
      case 72:
        if (game.spaceship.y > 0)
          --game.spaceship.y;
        break;
      case 75:
        if (game.spaceship.x > 0)
          --game.spaceship.x;
        break;
      case 80:
        if (game.spaceship.y < MAP_SIZE - 1)
          ++game.spaceship.y;
        break;
      case 77:
        if (game.spaceship.x < MAP_SIZE - 1)
          ++game.spaceship.x;
        break;
      }

      if (game.map[game.spaceship.y][game.spaceship.x]) {
        --game.health;
        game.spaceship = game.spaceship_init;
      }

      return;
    }
    case 'a':
    case 'A': {
      for (int j = game.spaceship.x - 1; j >= 0; --j) {
        if (game.map[game.spaceship.y][j]) {
          game.map[game.spaceship.y][j] = false;
          --game.num_enemies;
          break;
        }
      }
      return;
    }
    case 'd':
    case 'D': {
      for (int j = game.spaceship.x + 1; j < MAP_SIZE; ++j) {
        if (game.map[game.spaceship.y][j]) {
          game.map[game.spaceship.y][j] = false;
          --game.num_enemies;
          break;
        }
      }
      return;
    }
    case 'q':
    case 'Q':
      exit(0);
    default:
      set_color(4);
      cout << endl << "error: invalid input\n\n";
      set_color(15);
    }
  }
}
