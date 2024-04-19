# C++ Game)

This small project follows the footsteps of [Let's make games' "How to make a
game"
tutorial](https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx),
as a means of learning a little bit of C++. The idea is **not**
to stick to the tutorial, but to give my own touches along the
way. For a repository exactly as the tutorial's, see [the tutorial's
repository](https://github.com/carlbirch/BirchEngine).

> Note:
    This is a *work in progress*. I've finished watching the referenced tutorial, but will likely
    study SDL2 more in-depth to better understand how it works.

> Note:
    This project is developed on a Mac, though the source code should work on
    other platforms (changes to the Makefile might be necessary).

> **KNOWN BUGS:**
    - Segmentation fault when launching the executable by double clicking (instead of using the command line).
    - Segmentation faults when creating too many projectiles at once. Probably due to a memory leak in the *Manager* object.
    - Multiple other small bugs that will be fleshed out in the future.

To set up SDL2 and SDL2_image on MacOS, I've used [the following tutorial](https://www.youtube.com/watch?v=Dyz9O7s7B8w&t=1002s&ab_channel=IndependentPixels). This helped me define the make file, as well as the *include* and *lib* folders (not included in this repository, but referenced in the make file).

For setting up SDL2_ttf, that process didn't quite work. The shortcut for SDL2_ttf's *lib* folder worked fine, but,
to get it to work, I had to copy the SDL2_ttf.h file into the include/SDL2 folder (since it tried including SDL2.h, which
is only included in that folder).

The game is 2D and has an "8-bit" style. The assets in this
repository are original and were made in
[Aseprite](https://www.aseprite.org/).

I try to extensively document (and comment) what I learn along the
way in this repository.

### Current state of development
> You can now toggle whether the camera should follow the player or not with the
> P and L keys. You can also exit the game by clicking ESC. You can shoot with K
> (but shooting multiple shots at once will result in segmentation fault as of now).

![Current state of development](assets/current.gif)

### TODOs

- Review all code, searching for bugs, memory leaks, optimizations,
  restructuring possibilities (some ideas in mind).
- In this process, also add extensive documentation, using something
  like doxygen and sphinx for generating HTML pages.

### Libraries/Requirements

The [SDL2 library](https://www.libsdl.org/) is used, as stated in
the tutorial. The tutorial also uses SDL2_image. At the start of development, I could
not download it, so I have circumvented its use by using bitmap (.bmp) images, which
are supported by the SDL2 library. After updating my OS, I have managed to install
SDL2_image, so it might be used in the future. Finally, SDL2_ttf is used for text.

The Makefile used for compilation in VSCode is provided.