States
======

States are, as of now, temporary components (:cpp:class:`Component` class),
although this may be changed in the future. They are used to communicate
to other components and parts of the code if the entity is in a given state
which has special properties. 

For example, the :cpp:struct:`Hurting`
state indicates to the :cpp:class:`Sprite` component that a red filter
should be applied to the entity's render to indicate it is hurting. It
also indicates to the :cpp:class:`Health` component that the entity it
temporarily invulnerable.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Cooldown     <States/Cooldown.rst>

.. doxygenfile :: States.hpp
