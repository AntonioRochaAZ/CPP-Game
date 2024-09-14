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

## Current state of development
> You can move the first player with the W, A, S, D keys, and shoot with R.
> For the second player, it can be moved with the I, J, K, L keys, and it shoots with P.
> Finally, you can still toggle the camera to follow Player 1 (or Player 2, if the first one is gone) by pressing M.
> You can always press 1 to reset the game.
> You can now also drag players around with the mouse (holding left click).
> Finally, you can close the game by pressing ESC.

![Current state of development](assets/current.gif)

## Libraries/Requirements

The [SDL2 library](https://www.libsdl.org/) is used, as stated in
the tutorial. The tutorial also uses SDL2_image. At the start of development, I could
not download it, so I have circumvented its use by using bitmap (.bmp) images, which
are supported by the SDL2 library. After updating my OS, I have managed to install
SDL2_image, so it *might* be used in the future. Finally, SDL2_ttf is used for text.

To set up SDL2 and SDL2_image on MacOS, I've used [the following tutorial](https://www.youtube.com/watch?v=Dyz9O7s7B8w&t=1002s&ab_channel=IndependentPixels). 
This helped me define the Makefile, as well as the *include* and *lib* folders 
(not included in this repository, but referenced in the Makefile).

For setting up SDL2_ttf, that process didn't quite work. The shortcut for SDL2_ttf's *lib* 
folder worked fine, but, to get it to work, I had to copy the SDL2_ttf.h file into the 
include/SDL2 folder (since it tried including SDL2.h, which is only included in that folder).

The Makefile used for compilation is provided.

## Documentation

I try to extensively document (and comment) what I learn along the
way in this repository. [Documentation can be found here](https://antoniorochaaz.github.io/CPP-Game/).

I use [Doxygen](https://www.doxygen.nl/) to document the code (generating xml files), and then [Sphinx](https://www.sphinx-doc.org/en/master/)
to generate the HTML files from the xml files. To connect the two, Sphinx's extension [Breathe](https://breathe.readthedocs.io/en/latest/) is used. 
I was inspired by [this article](https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/) for
this integration. However, the use of CMake seemed too complex and generated too many redundant files. To generate my documentation I use the
command ``make documentation``, defined in the Makefile.

## Notes and TODOs

> Note:
    This is a *work in progress*. I've finished watching the referenced tutorial, but will likely study SDL2 more in-depth to better understand how it works.

> Note:
    This project is developed on a Mac, though the source code should work on
    other platforms (changes to the Makefile might be necessary).

> **KNOWN BUGS:**
    - Graphical bug when players die while facing left (easy to solve: add a shift to the generated temporary entity when there's a sprite flip).
    - Game crashes (intended) when objects collide at the same speed.
 
### TODOs

In no specific order.

- Improve the mouse controller handling in collision handling, try to make it as generic as possible... so that we don't have to list all MouseController types of components.

- Replace TempEntity entity by a State. Will be much more logical.
- Apply color filters to projectiles to change their colour.
- Make the texture pointer in Game::AssetManager shared pointers that can be owned by Sprite and UILabel components.
- Learn how to properly center the text on the screen
- Separate States from Components in ECS.hpp (?).
- Work into compiling for Windows and Linux.

- Optimize rendering: Have different renderers per managers. Only clear background's renderer if needed (if the background moves perhaps, when the player moves, although it may not even be necessary, must check). (Also stop rendering objects off-screen) (Must think if this really is necessary, and if so, when).
- Make updating independent of FPS. (actually not really sure if really necessary, but sure.)
- Add options to windown size etc.
- Create function for changing order of component updates (?) (currently a mess).
- Create function for deleting components?
- Study the use of "const" and passing arguments by reference (check when it is possible/should be done).
- Long comment under "case (IMMOVABLE_BIT + PUSH_BIT)" inside of Collision::handle_collisions.
- Create gifs and a diagram of how the collision handling works.