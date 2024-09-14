KeyboardController (component)
==============================

This component is the base class for custom classes that handle 
keyboard inputs. It currently allows for the assignment of six
keys (up, down, left, right, attack A and attack B), which can
have actions for when the button is pressed and for when the 
button is released. KeyboardController.hpp also defines the
:cpp:enum:`KeyBind` for the different possible keys (as well as
keys for camera toggle, resetting and quitting), and the 
``global_key_bind_map`` which is set up in Game.cpp.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   KeyboardPlayer     <CustomControllers/KeyboardPlayer.rst>

.. doxygenfile :: KeyboardController.hpp
