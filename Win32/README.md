# Background

This project has its roots in my earliest attempts dating back to 1991 at learning C++
under MacOS (the original one, aka "Classic"), which evolved in a cross-platform
application framework (half-jokingly named Casaubon) targeting Win32 and, to a lesser extent, Unix (Solaris and Linux).
Many mistakes were made in the process, which fed back into equally many iterations
of the implementation; but in so doing I learned many invaluable design and coding principles
that serve me well to this day.

As such, some of the code as written may have been made obsolescent by other technologies
that emerged since (modernized); in some cases, I may simply not have had the time to catch
up with the technologies that overtook me; however, it's been my observation that not all
of the lessons I learned have been adopted shared by industry and common practice.
Even though I stopped working on this code intensively in the late 2000s, I've kept it
going at a low pitch as a testbed on which I can still experiment and validate
new design and implementation methods.

You may find my indentation style uncommon, or even jarring.  I can commiserate because
it's exactly how I feel when reading _your_ code.  This style is not accidental but
something I developed intentionally long ago, and over many years of independent programming
in isolation, before homogenization by the Internet.  I've gotten used to the style
and have concrete reasons to believe it is objectively superior to common practice.
If it helps make it more palatable, it appears to be known as
[Ratliff style](https://en.wikipedia.org/wiki/Indent_style#Ratliff_style) and
also resembles practically what Python ended up adopting.

## This Project

The 'Native' projects are object-oriented abstractions of features of specific operating systems;
they are specifically _not_ cross-platform abstractions.  I found this idea of being specific
about what purpose a library serves, or specifically, _what_ it is trying to abstract,
a very clarifying and helpful design principle.