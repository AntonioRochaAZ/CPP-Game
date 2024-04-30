UILabel
=======

This component renders text on screen. It depends on the :cpp:class:`Transform` component
of the :cpp:class:`Entity`. The :cpp:class:`Transform` component must update before 
this, so it must be added to the :cpp:class:`Entity` first.

.. note::
    TODO: learn more about how text rendering in SDL works so that we can change the
    size of the font and add more options.

.. doxygenfile :: UILabel.hpp
