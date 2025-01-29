# Snake Game

This is a simple Snake Game implemented in C++ for the console. The game uses basic input handling, rendering, and logic to simulate a classic snake game experience. The game also includes a high score feature that saves and loads the highest score from a file.

## Features

- Move the snake using the WASD keys.
- Eat fruits to grow the snake and increase your score.
- Choose to play with or without walls.
- Pause and resume the game by pressing 'p'.
- Save and load the high score.

## Prerequisites

- Windows operating system
- C++ compiler (e.g., MinGW, Visual Studio)

## File Structure

- `snakegame.cpp`: The main game implementation.
- `CRUD.hpp`: A helper class for file operations (CRUD operations).

## How to Compile and Run

### Using a C++ Compiler

1. **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/snakegame.git
    cd snakegame
    ```

2. **Compile the game:**
    ```bash
    g++ -o snakegame snakegame.cpp -std=c++11
    ```

3. **Run the game:**
    ```bash
    ./snakegame
    ```
    
## How to Play

- **Movement:** Use the `W`, `A`, `S`, `D` keys to move the snake up, left, down, and right respectively.
- **Pause/Resume:** Press `P` to pause and resume the game.
- **Quit:** Press `X` to quit the game.

### Game Modes

- **No Walls:** The snake wraps around the screen edges.
- **With Walls:** The snake dies if it hits the screen edges.

### High Score

The high score is saved in a file named `highScore.txt` located in the `Documents` folder of the user's profile. The high score is loaded at the start of the game and updated after each game session.

## Code Overview

### `snakegame.cpp`

This file contains the main game logic, including:

- **Setup:** Initializes the game state, including the snake's position, fruit position, and score.
- **Input Handling:** Captures user input to control the snake's movement.
- **Game Loop:** The main loop that updates the game state, checks for collisions, and renders the game.
- **High Score:** Loads and saves the high score using the `CRUD` class.

### `CRUD.hpp`

This header file defines the `CRUD` class, which provides basic file operations:

- **fileExists:** Checks if a file exists.
- **getDocumentsPath:** Returns the path to the `Documents` folder.
- **createFile:** Creates a file and writes the high score.
- **readFileAndExtractIntegers:** Reads the high score from a file.
- **deleteFile:** Deletes a file.

# Thank You
