UILabel
=======

This component renders text on screen. It depends on the :cpp:class:`Transform` component
of the :cpp:class:`Entity`. The :cpp:class:`Transform` component must update before 
this, so it must be added to the :cpp:class:`Entity` first.

Font sizes are dealt with inside of this component, with the :cpp:member:`font_size`, used
in the :cpp:member:`set_text` function. In SDL2, the font size is stored inside of the 
TTF_Font object, meaning that everytime this function is called this pointer's information
is updated. This isn't a problem, since :cpp:member:`set_text` is only called when updating
the text, which is not done all the time.

.. doxygenfile :: UILabel.hpp
