[简体中文](https://github.com/Thintime123/Minesweeper/blob/master/Minesweeper/README_CN.me)
# Minesweeper Game

This project is a simple implementation of the classic Minesweeper game using C++ and the Win32 API. The game includes basic functionalities such as initializing the game board, handling mouse operations, and determining win/lose conditions.

## Features

1. **Initialization**: Initializes the game board with mines and numbers.
2. **Mouse Operations**: Handles left and right mouse clicks for revealing cells and marking mines.
3. **Win/Lose Conditions**: Determines if the player has won or lost the game.
4. **Game Restart**: Allows the player to restart the game after winning or losing.
5. **Graphics**: Uses the Win32 API for graphical representation of the game board.

## Requirements

- Visual Studio 2022
- Windows operating system

## How to Build and Run

1. Open the project in Visual Studio 2022.
2. Build the project by selecting `Build > Build Solution`.
3. Run the project by selecting `Debug > Start Without Debugging`.

## Code Structure

- `main.cpp`: Contains the main game logic and WinMain function.
- `init()`: Initializes the game board.
- `draw()`: Updates the graphical representation of the game board.
- `Mouseopration()`: Handles mouse click events.
- `ZeroBoom()`: Handles the chain reaction of revealing cells when a zero cell is clicked.
- `judge()`: Determines if the game is won or lost.

## How to Play

- Left-click to reveal a cell.
- Right-click to mark a cell as a mine.
- The game ends when all non-mine cells are revealed or a mine is clicked.
