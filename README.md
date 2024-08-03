# C++ Game

This small project follows the footsteps of [Let's make games' "How to make a
game"
tutorial](https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx),
as a means of learning a little bit of C++. The idea is **not**
to stick to the tutorial, but to give my own touches along the
way. For a repository exactly as the tutorial's, see [the tutorial's
repository](https://github.com/carlbirch/BirchEngine).

The game is 2D and has an "8-bit" style. The assets in this
repository are original and are made in [Aseprite](https://www.aseprite.org/).

## Current state of development
> You can move the first player with the W, A, S, D keys, and shoot with R.
> For the second player, it can be moved with the I, J, K, L keys, and it shoots with P.
> Finally, you can still toggle the camera to follow Player 1 by pressing M.

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
    - Segmentation fault when launching the executable by double clicking (instead of using the command line).
      It comes from Game::init, but must still be debugged.
 
### TODOs

In no specific order.

- Perhaps create a way of setting animations for a fixed ammount of frames, before going back to
  another one? (complex, if we stop walking in the middle of the animation it will go back to the walking animation).
- Apply color filters to players when they get hit, and to projectiles to change their colour.
- Make the texture pointer in Game::AssetManager shared pointers that can be owned by Sprite and UILabel components.
- Think of how to generalize KeyboardController's calls, as well as Health's (how to have a temporary animation when the entity is hurt). Perhaps use virtual functions and/or state machines, and/or function pointers.
- Change enums to enum classes, and change member definitions that use them to the enum classes' types.

- Perhaps generalize the KeyboardController component later, by allowing different
  actions to be triggered by the attack buttons according to the Entity. Perhaps create
  a sort of attack component.
- Optimize rendering (stop rendering objects off-screen) (Must think if this really is necessary,
  and if so, when).
- Make updating independent of FPS.
- Add options to windown size etc.
- Create function for changing order of component updates (?) (currently a mess).
- Create function for deleting components?
- Two projectiles (from the same player) will destroy each other if you shoot them while moving.
  Either we cap the time between shots, or do something else about it (e.g. make it so that
  projectile don't interact, but it would be interesting to be able to shoot down enemy projectiles, for example).
- Study the use of "const" and passing arguments by reference (check when it is possible/should be done).
- Long comment under "case (IMMOVABLE_BIT + PUSH_BIT)" inside of Collision::handle_collisions.
- Create gifs and a diagram of how the collision handling works.
- Find a way of changing the colour of projectiles without having to create multiple different sprites
  (like adding a filter on top of the texture).