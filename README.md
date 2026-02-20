🚀 AsteroidsGame

A C++ recreation of the classic Asteroids arcade game built using raylib.

This project demonstrates clean game architecture, fixed timestep simulation, collision handling with screen wrap, and wave-based gameplay progression.

🎮 Controls
Key	Action
← / →	Rotate ship
↑	Thrust
Space	Shoot
R	Restart (Game Over)
🧠 Features

Smooth ship rotation and thrust physics

Screen wrap-around movement

Edge-triggered shooting (prevents bullet spam)

Bullet lifetime system (1 second)

Asteroids split into 2 smaller pieces when destroyed

Wave progression system (increasing asteroid count)

Score system based on asteroid size

3 lives with temporary invulnerability (blinking effect)

Game Over screen with restart option

Fixed timestep physics using the accumulator pattern (1/60 sec)

⚙️ Technical Overview
Fixed Timestep (Accumulator Pattern)

The game updates physics using a fixed delta time:

1.0 / 60.0 seconds

This ensures:

Stable physics regardless of frame rate

Consistent movement and collision detection

No frame-rate dependent gameplay issues

🧩 Project Structure
AsteroidsGame/
├── main.cpp        # Window creation and main loop
├── game.h/.cpp     # Game state and core systems
├── ship.h/.cpp     # Player movement and rendering
├── AsteroidsGame.slnx
└── .gitignore
Game Responsibilities

The Game class handles:

Entity updates (ship, bullets, asteroids)

Collision detection

Wave spawning

Score and life tracking

Rendering

Game state management (Playing / GameOver)

🛠 Build Instructions (Visual Studio)
Requirements

Visual Studio (Windows)

raylib installed and properly linked

Steps

Clone the repository:

git clone https://github.com/FreeTris/AsteroidsGame.git
cd AsteroidsGame

Open:

AsteroidsGame.slnx

Build and run (Ctrl + F5)

📈 Wave System

First wave: 5 asteroids

Each new wave: 4 + wave number

Asteroids spawn away from the center to prevent immediate player death

🎯 Collision System

Circle-based collision detection

Wrap-aware distance calculation

Bullet vs asteroid collisions

Ship vs asteroid collisions (with invulnerability timer)

💡 Learning Concepts Demonstrated

Fixed timestep simulation

Accumulator-based update loop

Clean separation of update and render

Object-oriented design

Vector management using std::vector

Safe erase patterns using std::remove_if

Simple Euler physics integration

Basic state machine design

📌 Possible Improvements

Sound effects and music

Particle effects

Start menu and pause system

High score saving

More detailed asteroid shapes (polygons instead of circles)

📄 License

This project is intended for educational and portfolio use.
