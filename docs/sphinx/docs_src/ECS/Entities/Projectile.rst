Projectile
==========

This class defines a Projectile Entity. Currently, it is only instantiated
in the :cpp:class:`KeyboardController` component, in a not very generic call.

As of now, not very generic. Eventually, it would be ideal to be able to have multiple
animations per projectile (beginning, flying and ending, though the beginning one might
be included in the Player/Enemy entity shooting animation instead).

As of now, all projectiles have the :cpp:enumerator:`collision_handle::CollisionHandle::DESTROY`
collider tag.

.. doxygenfile :: Projectile.hpp
