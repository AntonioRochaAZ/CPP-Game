How does the game run?
======================

Main game loop
--------------

Like most games, the game works by running a loop, simplified below 
(the real code can be found in main.cpp).

.. code-block:: cpp

   while (game.running()){
      game.handle_events();   // Get keyboard and mouse inputs
      game.update();          // Update objects and handle collisions
      game.render();          // Draw the objects to the screen
   }

Currently, the frame rate is fixed at 30 FPS, as is the window size,
but this may be changed in the future.

Entity-Component-System
-----------------------

The game uses an `Entity-Component-System <https://en.wikipedia.org/wiki/Entity_component_system>`_
(ECS), for handling objects. In it, :cpp:class:`Entities <Entity>` (objects, players, enemies, anything really) 
contain :cpp:class:`Components <Component>`, which define how these entities respond to their
environment and are updated ("system"). For example, the :cpp:class:`Transform` component defines their 
position and speed, while the component :cpp:class:`Collider` defines how to handle their 
collisions.

Entites are stored in :cpp:class:`Manager` classes, which handle their updating and
rendering (rendering layers can be fine-tuned by giving the Entities groups, by calling
:cpp:member:`Entity::add_group`). 

Game.hpp and Game.cpp
---------------------

The :cpp:class:`Game` defines the main class that groups the Game's main variables and
functionalities, such as the window, camera position, and methods for handling events,
updating and rendering. It also includes the class :cpp:member:`Game::AssetManager`
used for managing the games assets (sprites and fonts). 

While the Game.hpp header is mostly generic and could be used in other games,
the Game.cpp implementation is not, and should be changed according to your game.

Making YOUR game!
=================

If you want to use this engine for your game, you can always add new components 
and entites to the existing ones! You'll need to update the Game.cpp implementation 
to match your game. 

If you want to create new mouse controllers (Components used for
controlling entities using the mouse) special attention must be given:
**See :ref:`custom_controllers_doc`**.