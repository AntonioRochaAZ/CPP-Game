Spawning (state)
================

Cooldown state for when an entity has recently spawned. It grants it 
invincibility, while also applying a grey flickering grey filter to its render.

It is added specifically to :cpp:class:`Player` entities when spawning (in their 
constructor, currently for a fixed duration of 2000ms).

.. note::
    Maybe add a gif of it in effect.

.. doxygenfile :: Spawning.hpp
