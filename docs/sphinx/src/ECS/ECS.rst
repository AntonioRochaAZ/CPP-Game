Entity Component System (ECS)
=============================

Files ECS.hpp and ECS.cpp contain the implementation of the basis of the 
Entity Component System (ECS). This includes the definition of the base Entity,
Component and Manager classes.

The Manager class handles the updating, rendering and deletion of Entities. Entities
are grouped according to the groups defined in the 
:ref:`group_labels </src/ecs/ecs.rst#_CPPv412group_labels>` enum, and groups
are rendered in order. The addition of the entity to a group must be done manually.

.. note::
    As of now, an Entity can be part of multiple groups, which may lead to repetitive
    rendering of the same Entity. 
    :ref:`Manager::render </src/ecs/ecs.rst#_CPPv4N7Manager6renderEv>` could eventually 
    be changed so this won't happen.

Entities contain Components, which define things like their positioning, their sprites
and animations, how they handle keyboard inputs etc.

.. note::
    The order in which components are updated may be sometimes important. They are now
    updated in the order that the components were added to the entity. For example:

    .. code-block:: c++
        
        /* In this code, component Transform will be updated before its
        Sprite, which will be updated before its KeyboardController.*/ 
        player.addComponent<Transform>();
        player.addComponent<Sprite>(Game::assets.get_tuple("player"), true);
        player.addComponent<KeyboardController>();
        player.add_group(PlayerGroup);

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Components   <Components.rst>
   Entities     <Entities.rst>

.. doxygenfile :: ECS.hpp
