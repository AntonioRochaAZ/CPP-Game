Transform
=========

Component defining the position of an Entity on screen. Other components
may depend on this one for getting the position of the entity on the screen.

.. warning::
    Velocity/Speed members should later be changed to depend on real time
    instead of tick (for example pixels/ms instead of pixels/tick). That way, 
    the FPS can be fine-tuned without messing these up.

.. doxygenfile :: Transform.hpp
