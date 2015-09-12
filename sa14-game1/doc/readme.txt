
GUIDELINES.

  1. No lines may contain more than 80 characters.

  2. Indentation is done with four spaces. No tab characters.

  4. Naming:
       macroName
       ConstantName
       typeNameT
       variable_name
       functionName()
       file_name.c

ARCHITECTURE.

  1. The engine is built with a component-based design in mind. Essentially, the
     game consists of entities, subsystems and entities. Each subsystem handles
     a specific type of component, and each entity consists of several different
     components. For example, an entity might be composed of one physical and
     one graphical component. The two components will exist in two different
     subsystems and will thus be updated in different stages of the game frame
     mechanism.
       More info: https://en.wikipedia.org/wiki/Entity_component_system

  2. As the game is an educational project, optimizations are not prioritized
     over clear and easy-to-understand code. However, it is required that the
     game runs at playable framerates, preferable a stable 60.0 fps for really
     smooth animation. There are many more optimizations that can be done, but
     I had to draw the line.

  3. Almost everything is implemented from scratch. This is a statement. Of
     course I could use a library for BMP file loading, physics, the game engine
     itself and many other parts of the program. Since the entire point of the
     project is to learn (and hopefully teach), I decided to implement almost
     everything on my own. This obviously adds a lot of work, but also a lot of
     learning opportunities and experiences.

  BUGS.

    Please report any bugs to contact@philiparvidsson.com.

------------------------------------------------------------

    - Philip Arvidsson, University of Borås
