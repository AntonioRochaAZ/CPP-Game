Transform
=========

Component defining the position of an Entity on screen. Other components
such as the :cpp:class:`Sprite` and the 
:cpp:class:`KeyboardController` may 
depend on this Component.

.. warning::
    Velocity/Speed members should later be changed to depend on real time
    instead of tick (for example pixels/ms instead of pixels/tick). That way, 
    the FPS can be fine-tuned without messing these up.

.. note::
    Since it is extremely repetitive to define get/set functions for each member
    and overload possibility, I might eventually just render all members public,
    or limit the number of overloading options.

.. doxygenfile :: Transform.hpp
