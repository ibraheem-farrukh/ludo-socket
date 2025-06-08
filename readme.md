# Ludo (but multiprocess)

This project implements a simple multiplayer Ludo game using C++ for networking and [SFML](https://www.sfml-dev.org/) for graphics. The game consists of a server that manages the game state and multiple clients (players) that connect to the server to play the game.

## Table of Contents

- Overview
- Theoretical Context
- Project Structure
- How It Works
- Building and Running
- Dependencies
- Gameplay
- Notes

---

## Overview

This project demonstrates a networked board game (Ludo) where:
- The **server** (server.cpp) manages the game state, player turns, and board rendering.
- Each **client** (player.cpp) represents a player, allowing them to roll dice and move pieces.
- Communication is handled via TCP sockets.
- The board and pieces are rendered using SFML.

---

## Some Context

### Ludo Game

Ludo is a classic board game for 2-4 players, derived from the ancient Indian game Pachisi. Each player has four pieces that must travel from their starting area to the center of the board, following a set path. Movement is determined by dice rolls, and players can "capture" opponents' pieces by landing on them, sending them back to their starting area.

### Networking Model

This project uses a **client-server** architecture:
- The **server** listens for connections from up to four clients.
- Each client sends its moves to the server, which updates the game state and broadcasts updates to all clients.
- Shared memory and sockets are used for inter-process communication.

### Graphics

The game uses [SFML](https://www.sfml-dev.org/) for 2D graphics rendering, including:
- Board background (ludo.jpg)
- Piece rendering and movement
- Custom font for piece labels (balls.ttf)

---

## Project Structure

```
balls.ttf           # Font used for piece labels
game.sh             # Shell script to build and run the server
ludo.jpg            # Board background image
player.cpp          # Client/player code
server.cpp          # Server/game logic and rendering
.vscode/            # VSCode configuration
    settings.json
    tasks.json
```

---

## How It Works

### Server (server.cpp)

- Initializes a TCP socket and waits for up to 4 clients.
- Loads the board image and font, sets up the SFML window.
- Manages the positions of all pieces and the turn order.
- Receives moves from clients, updates the game state, and sends updates to all clients.
- Handles piece movement, dice rolls, and captures.

### Client (player.cpp)

- Connects to the server via TCP.
- Receives initial piece positions and game state.
- Waits for its turn, rolls the dice, and selects which piece to move.
- Sends the move to the server and receives updated game state.

---

## Building and Running

### Prerequisites

- C++ compiler (e.g., `g++`)
- [SFML library](https://www.sfml-dev.org/) (graphics, window, system)
- Linux environment (for sockets and shared memory)

### Build and Run Server

You can use the provided shell script:

```sh
./game.sh
```

Or manually:

```sh
g++ -c -w server.cpp
g++ server.o -o server -lsfml-graphics -lsfml-window -lsfml-system
./server
```

### Build and Run Client

In a separate terminal, for each player:

```sh
g++ player.cpp -o player
./player
```

Repeat up to 4 times for 4 players.

---

## Dependencies

- [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
- Standard C++ libraries
- Linux system libraries for networking (`netinet/in.h`, `unistd.h`, etc.)

---

## Gameplay

- Each player waits for their turn.
- On their turn, the client rolls a dice and selects which piece to move.
- The server validates the move, updates the board, and notifies all clients.
- If a piece lands on an opponent's piece, the opponent's piece is sent back to its starting position.
- The first player to move all four pieces to the center wins.

---

## Notes

- The game is a simplified version of Ludo and may not implement all rules or edge cases.
- The server must be started before any clients connect.
- All assets (ludo.jpg, balls.ttf) must be present in the working directory.
- The project is intended for educational purposes to demonstrate basic networking and graphics in C++.

---

## License

This project is for educational use. See individual file headers for more information.

---

## Authors

- [Your Name Here]

---

Feel free to modify and extend the game!