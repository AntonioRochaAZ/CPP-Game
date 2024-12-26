# C++ Game

This small project follows the footsteps of [Let's make games' "How to make a
game"
tutorial](https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx),
as a means of learning C++. The idea is **not**
to stick to the tutorial, but to give my own touches along the
way. For a repository exactly as the tutorial's, see [the tutorial's
repository](https://github.com/carlbirch/BirchEngine).

The game is 2D and has an "8-bit" style. The assets in this
repository are original and are made in [Aseprite](https://www.aseprite.org/).

Finally, the code documentation can be found [in this project's GitHub.io](https://antoniorochaaz.github.io/CPP-Game/).

In this README, you will find:
- How to play!
- Compilation instructions
- Libraries/Requirements
- Documentation (information on how it is generated)
- Notes and to-do's

## How to play!
- You can move the first player with the W, A, S, D keys, and shoot with R.
- For the second player, it can be moved with the I, J, K, L keys, and it shoots with P.
- Finally, you can toggle the camera to follow Player 1 (or Player 2, if the first one is gone) by pressing M.
- You can press 1 at any point to reset the game.
- You can now also drag players around with the mouse (holding left click).
- Finally, you can close the game by pressing ESC.

![Current state of development](assets/current.gif)

## Compilation instructions

A Makefile is provided, but CMake can also be used to compile this project.

The debug configurations defines the `DEBUG_MODE` macro, which
prints more information to the terminal during runtime.

### Makefile

The Makefile uses the g++ compiler and assumes the file structure defined in
[this tutorial](https://youtu.be/Dyz9O7s7B8w?si=E45KSeGEoCbQo0OK) I've used to install the required libraries (see
Libraries/Requirements below). Of course, if can be modified to suit your preferences.

The make file will create a `build` directory, with `debug` and `release` folders
and even make a `build.log`! You can build the release version with `make all` and
`make release`, while you can build the debug version with `make debug`. Finally, it
can also be used to generate the documentation by running `make documentation`.

### CMake
In the main directory:

```
cmake -S . -B cmake     # Making the build files
cmake --build cmake     # Building the project
```

If using g++, my `g++` preset is available, which also adds warnings. In that case,
you may simply run:

```
cmake --preset=g++
cmake --build cmake
```

The additional option `-DCMAKE_BUILD_TYPE=Debug` can be specified in the first 
command (in both cases) for compiling the debug version.


## Libraries/Requirements

This project is developed on MacOS, but should be easily used on Linux. A 
cmake configuration for Windows is currently being worked on.

The [Eigen3 library](https://eigen.tuxfamily.org/index.php?title=Main_Page) is
used for handling vectors.

As in the tutorial, the [SDL2 library](https://www.libsdl.org/) and the SDL2_ttf
libraries are used. The tutorial also uses SDL_image, but I have circumvented this
need by only using bitmap (.bmp) files. This may change in the future.

On MacOS, these libraries can easily be installed with [homebrew package manager](https://brew.sh/),
which is also available for linux. 

I've used [this tutorial](https://youtu.be/Dyz9O7s7B8w?si=E45KSeGEoCbQo0OK)
for setting up the lib, include folders and the original makefile (though it has
been significantly changed since). Something similar can be done without the use of
brew, if you must manually download and extract the files for those libraries.


## Documentation

I try to extensively document (and comment) what I learn along the
way in this repository. [Documentation can be found here](https://antoniorochaaz.github.io/CPP-Game/).

I use [Doxygen](https://www.doxygen.nl/) to document the code (generating xml files), and then [Sphinx](https://www.sphinx-doc.org/en/master/)
to generate the HTML files from the xml files. To connect the two, Sphinx's extension [Breathe](https://breathe.readthedocs.io/en/latest/) is used. 
I was inspired by [this article](https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/) for
this integration. However, the use of CMake seemed too complex and generated too many redundant files. To generate my documentation I use the
command ``make documentation``, defined in the Makefile.

## Notes and to-do's

> Note:
    This is a *work in progress*. I've finished watching the referenced tutorial, but will likely study SDL2 more in-depth to better understand how it works.

> Note:
    This project is developed on a Mac, though the source code should work on
    other platforms (changes to the Makefile might be necessary).

> **KNOWN BUGS:**
    - Game crashes (intended) when objects collide at the same speed.
 
### To-do

In no specific order.

- Generalize the use of the Animation object for all textures, instead of having multiple metadata scattered everywhere.

- Study the use of "const" and passing arguments by reference (check when it is possible/should be done).
- Switch to const char instead of std::string whenever possible.
- Add options to windown size etc.
- Work into compiling for Windows and Linux.
- Long comment under "case (IMMOVABLE_BIT + PUSH_BIT)" inside of Collision::handle_collisions.
- Make updating independent of FPS. (actually not really sure if really necessary, but sure.)
- Learn how to properly center the text on the screen
- Create gifs and a diagram of how the collision handling works (and gifs for showing the states working).
- Adding music and sound effects.

- Optimize rendering: Have different renderers per managers. Only clear background's renderer if needed (if the background moves perhaps, when the player moves, although it may not even be necessary, must check). (Also stop rendering objects off-screen) (Must think if this really is necessary, and if so, when).
- Replace TempEntity entity by a State. Will be much more logical.
- Apply color filters to projectiles to change their colour.
- Improve the mouse controller handling in collision handling, try to make it as generic as possible... so that we don't have to list all MouseController types of components.
- Separate States from Components in ECS.hpp (?).
- Make the texture pointer in Game::AssetManager shared pointers that can be owned by Sprite and UILabel components.
- Create function for changing order of component updates (?) (currently a mess).
