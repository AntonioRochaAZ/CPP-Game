Hurting (state)
===============

State for when an entity has been damaged, and must have a period
of invulnerability and/or must have a flickering filter applied to it.

It is added to the entity in :cpp:struct:`Health` component when the entity takes
damage (currently with a fixed cooldown of 450ms), and it is identified
by the :cpp:struct:`Sprite` component so that a red, flickering filter is
applied to the entity.

.. note::
    Add a gif of it in effect.

.. doxygenfile :: Hurting.hpp
