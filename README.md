# EscapeSnake
The classic Snake game, but with its own personality — mystery power-ups, dynamic difficulty, engaging audio and a sleek terminal UI. 
Built from scratch in C with ncurses.

**[EscapeSnake[]:>**  preserves the classic nostalgia, while adding unpredictable power‑ups that can either make or break your game.

### Key Features
+ **Classic Snake Mechanics** - Grow your snake, avoid walls and yourself
+ **Mystery Barrels** - Random power-ups, surprises or penalties
+ **Dynamic Difficulty** - Speed increases as you grow
+ **Retro Audio - Engaging** sound effects to enhance the experience
+ **Rich Terminal UI** - Vibrant colors, with real-time score updates & visual feedback


### Controls
```
← ↑ → ↓   Move the snake  
Space     Pause/Resume  
S         Disable Music  
Q         Quit  
```



**A glimpse of the game**
![Snake_snippet](https://github.com/user-attachments/assets/7ce25f56-516f-4dfc-bef9-ff3f0c867f0a)



## Installation & Running

#### Prerequisites
* A C compiler (like `gcc` or `clang`).
* The `ncurses` library. (ncursesw for Unicode support).
* This code is **macOS** & **linux** compatible
  
#### Running On Linux & macOS
```
git clone https://github.com/Nnamann/EscapeSnake.git
cd EscapeSnake

gcc main.c -o escapesnake -lncursesw
./escapesnake
```

## Contributing
Feel free to:

+ Report bugs
+ Suggest features
+ Submit pull requests
+ Star if you enjoyed the game


## Dev Note

EscapeSnake began as a tangle of raw terminal escape sequences and grew into a somewhat polished ncurses game. From implementing the core mechanics to refining the UI, each challenge was a new learning experience.

Thank you for checking it out!

Made with chaos, color, and lots of caffeine.




![Snake_snippet](https://github.com/user-attachments/assets/bbc77cf4-ab7a-4242-abc9-bd097a1e3da2)
