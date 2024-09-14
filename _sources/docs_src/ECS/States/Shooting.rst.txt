Shooting (state)
================

Cooldown state for when an entity has recently shot a projectile. It serves
to avoid the entity from spamming projectiles. It is created in 
:cpp:class:`KeyboardPlayer` (currently for a fixed cooldown of 250ms), and it
is also checked there to avoid the creation of new projectiles.

.. note::
    Maybe add a gif of it in effect, though there is no visual indication.

.. doxygenfile :: Shooting.hpp
