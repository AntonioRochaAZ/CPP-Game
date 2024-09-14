Custom Controllers
==================

Files CustomControllers.hpp and CustomControllers.cpp contain functions
and definitions that are dependent on the specific game being made, and
are thus outside from the ECS folder for the user to edit it as necessary.

These files serve to import all custom controllers define with one import,
but also to define the list of custom MouseControllers defined. This is
because :cpp:member:`Collision::handle_collisions` needs to know all of
the MouseController type components when checking if the cursor is hovering
over an entity containing one of these components. The 
``number_of_mice_controllers`` can be changed in the header, and the list of
MouseController type components (``controller_array``) can be changed in the
.cpp file (it is actually a list of their component IDs).

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   KeyboardPlayer   <ECS/Components/CustomControllers/Transform.rst>
   MouseTest        <Components/MouseTest.rst>