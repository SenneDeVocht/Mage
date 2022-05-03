MAGE is a 2D game engine I made using C++. It uses SDL and OpenGL for rendering, Box2D for physics, and ImGui for debug menus. The engine has a simple entity component system similar to the one Unity uses. Objects are only used to hold components, and do nothing by themselves. There are a lot of built-in components, such as a text component, a rigidbody and collider component, a sprite component and its animated variant, a camera component, and of course a transform component.

I tried to make it so this engine can be used for making "pixel-perfect" pixel art games. For instance, text can easily be aligned to the pixel grid. That being said, it can of course still be used to make games with other art styles.

While debugging, you can easily change the values of components through ImGui. I eventually want to make this a fully working editor, as now all components and objects still have to be added through code.
