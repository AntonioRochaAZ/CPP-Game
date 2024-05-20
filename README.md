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
> You can now toggle whether the camera should follow the player or not with the
> P and L keys. You can also exit the game by clicking ESC. You can shoot projectiles
> with K.

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
    This is a *work in progress*. I've finished watching the referenced tutorial, but will likely
    study SDL2 more in-depth to better understand how it works.

> Note:
    This project is developed on a Mac, though the source code should work on
    other platforms (changes to the Makefile might be necessary).

> **KNOWN BUGS:**
    - Segmentation fault when launching the executable by double clicking (instead of using the command line).
      It comes from Game::init, but must still be debugged.
    - Collision recognized when camera follows player, but game crashes (it can't calculate how to avoid it because the player's velocity is zero.).
    - When the camera is following the player, projectiles go up and down with it. This has to do with
      the camera implementation (Projectiles don't have a KeyboardController).

> Note:
    Folder *assets/fonts* is not provided, because I am not sure if the font I am using can be shared (Andale Mono,
    taken directly from MacOS's font catalog).
 
### TODOs

- Review all code, searching for bugs, memory leaks, optimizations,
  restructuring possibilities (some ideas in mind).
- Check if it really is useful to keep pointers to components inside
  of other components, when we can simply reference other components
  using ``component->entity->getComponent<OtherComponent>()``. Sure,
  it is more verbose.
- Move Collision handling to Collision.hpp.
- Must think about the best
  camera implementation.
- Optimize rendering (stop rendering objects off-screen)
- Make updating independent of FPS.
- Create function for changing order of component updates (currently a mess).