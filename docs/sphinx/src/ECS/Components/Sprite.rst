Sprite
======

Component used for handling Entity's sprites (textures an animations). Although
texture loading and their memory management are now completely handled by the 
:ref:`Game::assets </src/game.rst#_CPPv4N4Game6assetsE>` (class of type 
:ref:`Game::AssetManager </src/game.rst#_cppv4n4game12assetmanagere>`), this
component will keep pointers to the textures involved, and will structure
and handle adding and setting animations.

.. doxygenfile :: Sprite.hpp
