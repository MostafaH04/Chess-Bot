# Chess Bot 🤖♟️

[![Chess Bot Demo](https://img.youtube.com/vi/2D1FdVrN6-M/0.jpg)](https://www.youtube.com/watch?v=2D1FdVrN6-M)

## Introduction
The game of chess is a board game between two players and is an ancient game of strategy that simulates war between two kingdoms. The chess bot is an automated chess player that was built to train humans to play a truly difficult opponent that will make the best possible move 100% of the time. Various chess players often find little challenge against friends and family; therefore, the chess bot provides the most challenging opponent one could play at Stockfish level 15. Instead of merely playing chess online, the chess bot allows users to get a traditional feel for the game while practicing playing composed in real one-on-one matches.

## Scope
The original goal of the project was to make a chess robot that can play chess against a human, make calculated moves, and perform typical chess procedures. This includes castling and removing captured pieces with little to no human assistance. The scope overall changed slightly in a variety of ways throughout our project as new and more efficient methods were developed to solve previous problems. The chess bot also encountered a wide variety of issues that altered our mechanical design as well as the structure for our code.

Sensors are extremely vital for our project. Two touch sensors were installed in the bottom left and right of the framework of the chess bot meant to calibrate the XY gantry system after a couple of moves. After the moving y axis hits the touch sensor it starts moving in the x direction till it hits that touch sensor. After the second sensor is hit, it moves slightly in both the x and y direction to align itself with the bottom right square under the chessboard. A camera is another main input that is used to pass a live video stream to the python script which extracts the positions of the pieces on the board using an Open CV program that checks the state of all 64 squares.

The robot has two motors, one large motor moving the y-axis and one medium motor moving the x-axis. The large motor is built on a stable framework of Lego that keeps it tilted at a specific height and angle so it can move the main belt. This belt is connected to the x-axis section of the robot which is built upon a steel rod. Attached to the steel rod is a medium motor that runs another belt meant to move the servo motor with the latch capabilities in the x-direction. Since the motor moving the servo in the y-direction is also moving the x-direction belt and medium motor, a powerful motor with torque was required. Therefore, the large EV3 motor was used.

Using the Stockfish Chess engine, the python program can determine when the player or robot is in checkmate. Once it recognizes the king piece cannot move in any direction without getting captured, the gantry system moves the servo motor to the bottom right corner and the program shuts down. The program will need to be re-run if the player wants a rematch or someone else wants to play against the chess robot.

Fortunately, little changes were made from the original planning in the earlier stages of the project. Even though several parts were changed such as the type of metal rod used by the bearings, our overall scope and functionality of the chest bot stayed true till the end of the project.

## Constraints & Criteria
The following constraints were followed during the implementation of the project:

- The board size should be a maximum of 30 cm by 30 cm due to hardware limitations. This limitation became apparent as the longest square available was 40 cm long, which restricted the distance the robot could have moved in the x-axis.
- The budget for this project is a maximum of $100 CAD. This constraint was implemented in order to be precise with measurements and tolerances before the components were printed. It restricted the ability to have infinite iterations of 3D printed parts.

The following criteria were followed during the implementation of the project:

- The robot plays a chess game with relative speed. The robot responds to the human’s moves within three minutes. This criterion was used to make an optimal method to move chess pieces. It required the robot to move the pieces in an efficient route with smooth movements.
- The robot must play legal chess moves. This criterion was crucial for the robot to play a game of chess with adequate moves. The robot’s scope is to play a game of chess with the use so being able to play legal chess moves and checking it is a crucial criteria.

Chess Bot is a remarkable fusion of robotics, AI, and chess strategy. Experience the thrill of playing against an unbeatable opponent with our autonomous chess-playing robot!

## Dependencies
- Stockfish Chess engine
- OpenCV
- LEGO EV3 components

For more details and images, please visit our [Automated Chess Bot Image Portfolio](https://www.behance.net/gallery/160377969/Automated-Chess-Bot).
