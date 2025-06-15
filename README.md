# [EscapeSnake[]:>
The classic Snake game, but with its own personality — mystery power-ups, dynamic difficulty, engaging audio and a sleek terminal UI. Built from scratch in C.

This game runs in terminal, 

## About the game
EscapeSnake isn't your ordinary snake game. While keeping the nostalgic feel of the classic, it introduces mystery barrels 🛢️ that can completely change your game strategy. Will you risk it for massive bonus points, or play it safe?

### Key Features
+ Classic Snake Mechanics - Grow your snake, avoid walls and yourself
+ Mystery Barrels - Random power-ups and surprises 
+ Dynamic Difficulty - Snake speeds up as it grows longer, keeping the challenge fresh
+ Engaging Audio - Characteristic retro sound effects
+ Colorful Terminal UI - Vibrant colors and a neat dashboard, with Real-time Updates & Visual Feedbacks
+ DashboardReal-time Updates: Live score and length tracking
+ Score Tracking - Take risks & beat your highest score!

### Controls
```
  Arrow keys ←→↑↓  Control Snake movement
  Space            Pause/Resume
  Q                Quit anytime
  S                Disable background music
```

## Installation & Running

#### Prerequisites
* A C compiler (like `gcc` or `clang`).
* The `ncurses` library. On Linux/macOS, it's often called `ncursesw` for wide-character (Unicode) support.
* **(Optional)** For audio on Linux, you may need the `aplay` utility (part of `alsa-utils`).


#### Running On Linux & macOS

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
    cd your-repo-name
    ```
2.  **Compile the code:**
    ```bash
    gcc main.c -o escapesnake -lncursesw
    ```
3.  **Run the game!**
    ```bash
    ./escapesnake
    ```

#### Running On Windows

Running `ncurses` projects on Windows requires a compatibility library called **PDCurses**.

1.  **Set up a compiler:** Install a C compiler like MinGW-w64.
2.  **Get PDCurses:** Download a PDCurses library compatible with your compiler.
3.  **Compile:** You will need to link against the `pdcurses.lib` (or `.a`) file.
    ```bash
    gcc main.c -o escapesnake.exe -lpdcurses 
    # The exact command might vary based on your PDCurses installation.
    ```
4.  **Run:**
    ```bash
    ./escapesnake.exe
    ```

    

(Optional) For Audio:

## Author
Made with chaos, color, and caffeine by **Naman Bhardwaj**

## License
This project is licensed under the MIT License
