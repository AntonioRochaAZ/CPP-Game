Sprite
======

Component used for handling Entity's sprites (textures an animations). Although
texture loading and their memory management are now completely handled by the 
:cpp:member:`Game::assets` (class of type 
:cpp:class:`Game::AssetManager`), this
component will keep pointers to the textures involved, and will structure
and handle adding and setting animations.

.. doxygenfile :: Sprite.hpp
