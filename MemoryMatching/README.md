# Memory Matching Game in C++

<img src="MemoryMatching/fruit-match-demo3.png" height="400">

## Description

Memory Matching is a classic card-matching game implemented in C++. Test and challenge your memory skills by flipping cards and matching pairs of identical symbols.

## Screenshots

<img src="MemoryMatching/fruit-match-demo1.png" height="400">
<img src="MemoryMatching/fruit-match-demo2.png" height="400">
<img src="MemoryMatching/fruit-match-demo3.png" height="400">

## Installation

### Prerequisites

Before running the Memory Matching Game, ensure you have the following:

1. [Visual Studio](https://visualstudio.microsoft.com/) with C++ development tools.
2. [OpenCV](https://opencv.org/) installed on your system.

### Setting up OpenCV

1. Download and install OpenCV on your computer.
2. After installation, locate the `opencv_world<version>d.dll` file in the OpenCV installation directory.
3. Copy the `opencv_world<version>d.dll` file and paste it into the `debug` folder of the Memory Matching Game project.

### Configuring Visual Studio

1. Open the Memory Matching Game project in Visual Studio.
2. Go to Project Properties > Configuration Properties > VC++ Directories.
3. Set the "Include Directories" to include the path to the `opencv\include` folder of your OpenCV installation.
4. Set the "Library Directories" to include the path to the `opencv\x64\vc15\lib` folder of your OpenCV installation (adjust the folder name according to your OpenCV version and architecture).
5. Go to Project Properties > Configuration Properties > Linker > Input.
6. Add `opencv_world<version>.lib` to the "Additional Dependencies" field under "Input".

### Build and Run

1. Build the Memory Matching Game project in Visual Studio.
2. Run the executable to start the game.

### How to Play

1. When the game starts, you will see a grid of face-down cards.
2. Click on any card to reveal the symbol on the card.
3. Click on another card to reveal its symbol.
4. If the symbols on the two cards match, they will turn neon-green, and you will be award 5 points.
5. If the symbols do not match, both cards will flip back face-down, and you will be deducted 1 point.
6. Continue flipping cards and trying to match pairs until all cards are face-up. Your final point count will be shown in the command line.

## Contributing

Contributions to this project are welcome! If you have any ideas for improvements or bug fixes, feel free to open an issue or submit a pull request.
