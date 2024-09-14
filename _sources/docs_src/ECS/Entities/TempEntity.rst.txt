TempEntity (entity)
===================

This class defines a temporary entity, that only last until a given
animation is finished playing out. This has been created so that the
:cpp:class:`Player` entity could have a death animation. This was
necessary because the :cpp:class:`Health` component calls the 
:cpp:member:`Entity::destroy` method directly (overridden in the
player's definition). 

In hindsight, the strategy could've been different, especially since
the addition of `States <../States.html>`, as the
destroy method had to be overridden anyway in the end. This might thus
be changed in the future, but this approach is relatively generic either 
way.

.. doxygenfile :: TempEntity.hpp
