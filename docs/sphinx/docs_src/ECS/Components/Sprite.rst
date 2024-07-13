Sprite
======

Component used for handling Entity's sprites (textures and animations). Although
texture loading and their memory management are now completely handled by the 
:cpp:member:`Game::assets` (class of type 
:cpp:class:`Game::AssetManager`), this
component will keep pointers to the textures involved. Some manual handling is
still possible with the use of the :cpp:member`Sprite::add_animation`,
:cpp:member`Sprite::set_animation`, and :cpp:member`Sprite::set_texture` functions.
This can be useful in the future.

Function :cpp:member:`Sprite::maybe_update_collider` will handle updating the 
:cpp:struct:`Collider` component's shape automatically if it is enabled. It is
called in the setter functions defined here. That is why the destination rectangle
(``dst_rect``) and the scale variables (``scale_x, scale_y``) are private members.

.. note::
    In the future, it would be useful to be able to render more than one animation
    at once. This would allow better control of some animations.

.. doxygenfile :: Sprite.hpp
