Tile Map
========

The TileMap class serves to implement a specific type of background which
consists of a grid of individual block textures. It is a child of the 
:cpp:class:`Manager` class, since it also handles
a list of entites (each individual block).

.. note::
    This class can still be optimized so as to not draw tiles that are
    off-screen. Actually, this should probably be done in :cpp:member:`Sprite::render`
    for it to be more general (or not, calculations may be quicker in the TileMap)
    because of symmetry etc.

.. doxygenfile:: TileMap.hpp
