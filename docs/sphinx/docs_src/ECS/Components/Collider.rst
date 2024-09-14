Collider (component)
====================

Component for defining Entity's collision information, such as its bounding boxes and collision
attributes defined by the collision_handle enumerator.

The shape of the collider can be dynamically updated according to the shape of the :cpp:class:`Sprite` component,
which must be enabled by calling :cpp:member:`Collider::enable_dynamic_shape`.

For checking how the collision is actually handled, see :cpp:struct:`Collision`.

.. doxygenfile :: Collider.hpp
