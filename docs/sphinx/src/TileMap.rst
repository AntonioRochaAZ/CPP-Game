Tile Map
========

The TileMap class serves to implement a specific type of background which
consists of a grid of individual block textures. It is a child of the 
:ref:`Manager </src/ecs/ecs.rst#_cppv47manager>` class, since it also handles
a list of entites (each individual block).

.. note::
    This class can still be optimized so as to not draw tiles that are
    off-screen. Actually, this should probably be done in :ref:`Sprite::render </src/ecs/components/sprite.rst#_CPPv4N6Sprite6renderEv>`
    for it to be more general (or not, calculations may be quicker in the TileMap)
    because of symmetry etc.

.. doxygenfile :: Game.hpp
