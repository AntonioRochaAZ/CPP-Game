Collision class
===============

This class groups functions for handling collision. The use of a class seems unnecessary, and might
be reviewed later. This could potentially be implemented directly inside of Game.hpp and Game.cpp, 
but this modularity seems nevertheless positive.

.. warning::
    No checks have been put in place to avoid phasing. This means that if two objects are moving very
    fast, they will pass through each other (this happens because between one update cycle and the other,
    they'll have moved so far that their collider do not collide).
    A quick solution to this could be to define a quicker game cycle, but where only Transform components
    (and Collider and maybe other objects) are updated, and rendering is done at the regular rate.

.. note::
    Include diagrams and gifs explaining the collision handling process.

.. doxygenfile :: Collision.hpp
