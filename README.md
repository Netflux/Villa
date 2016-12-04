# Villa - A Humble Village AI Simulation
Villa is a simulation designed to emulate the daily life of a village. Each simulation begins with a town hall and several villagers, who must gather resources in order to expand the village and survive.

### Table of Contents

* [Build Guide](#build-guide)
* [Credits](#credits)
* [License](#license)

## Build Guide
If you're working on Linux:

1. Download [SDL2](http://libsdl.org/) from your distribution's package repository.
2. Clone the repository using `git clone https://github.com/Netflux/Villa.git`.
3. Using the terminal, `cd` into the folder and use `make`.
4. The binary file `Villa` will be available in the `bin` folder.

If you're working on Microsoft Windows:

1. Download and install the [MinGW-W64](https://mingw-w64.org/doku.php) compiler.
2. Clone the repository using `git clone https://github.com/Netflux/Villa.git`.
3. Using the command prompt, `cd` into the folder and use `mingw32-make`.
4. The binary file `Villa` will be available in the `bin` folder.

## Credits
* Artwork (kenney.nl)
* Perlin Noise Generator (https://github.com/sol-prog/Perlin_Noise)
* A* Search Algorithm (http://www.redblobgames.com/pathfinding/a-star/implementation.html)

## License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.