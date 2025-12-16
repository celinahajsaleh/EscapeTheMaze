"Hello! my name is Celina Hajsaleh, Today I'm going to walk you through a complete OpenGL game project I built. 
It's a 3D maze game where you control a ball, navigate through procedurally generated mazes, and find your way out. 

 this is an infinite game. There's no final level. Each time you complete a maze, the next one gets bigger - 
 literally! The maze dimensions increase by 2 in both rows and columns every level. starting from Level 1 is 11×11, level 2 is 13×13, ,
  and it keeps growing forever. 
The game continues until you decide to quit or fall off the map. It's an endless challenge that gets progressively harder.

"On the screen, you’ll always see which level you’re on displayed at the top,
 along with the ball’s current row and column in the maze. If you press the 'i' key, a small box pops up  with instructions 


[0:30 - 1:30] PROJECT OVERVIEW
-------------------------------
"The project is built with C++, OpenGL for rendering, Box2D for physics, and SDL2 for audio.The code is organized 
using object-oriented principles
 so Let's jump into the code!"




[1:30 - 2:30] MAIN.CPP - THE ENTRY POINT
-----------------------------------------
Let’s start with main.cpp, which is the entry point of the application.
Here we initialize GLUT, create the window, and register all callback functions.

Each function pointer tells GLUT what to call when something happens.
 The idle callback runs continuously, giving us our game loop.




[2:30 - 4:00] GAME.CPP - THE HEART OF EVERYTHING
-------------------------------------------------
The Game class is responsible for managing the overall flow of the game

It controls:
Game states
Input handling
Physics updates
Camera movement
Rendering
Level progression

Every frame, input is converted into a force applied to the ball.
Box2D then updates the physics simulation, handling movement and collisions automatically.
After that, the camera smoothly follows the ball, and the game checks whether the player has reached the goal.

For rendering, actual code from game.cpp:
   
The state machine handles all the transitions between game states.
 When a level starts, we begin in FADE_IN - lines 172-178 in game.cpp - which gradually brightens the lighting. Once the fade completes,
  we transition to PLAY where all the action happens. When you reach the goal at the exit, 
    we play the victory sound and technically the game does a fade-out (see game.cpp, line 161: state = FADE_OUT;),
     but it happens so fast you barely notice it—the next level starts almost instantly. 
  we call nextLevel() - lines 165-169 - which increases the maze dimensions and starts the cycle again with FADE_IN lines 172-178.
"


[4:00 - 5:00] MAZE.CPP - PROCEDURAL GENERATION
-----------------------------------------------
"Let's talk about maze.cpp 

We use the recursive backtracker algorithm to generate random mazes. Here's how it works - 
The algorithm starts from a single cell and recursively carves paths by visiting neighboring cells in a randomized order.


Shuffling the directions ensures that the maze layout is different each time, even though the same algorithm is used.
The result is a perfect maze with a single valid path and no isolated areas


When the goal is reached, the game transitions to the next level
The dimension increases infinitely with each level. The code in game.cpp is simple:
    currentLevel++;
    mazeDimension += 2;

 


[5:00 - 6:00] PHYSICS.CPP - BOX2D INTEGRATION
----------------------------------------------
"Physics.cpp Physics is handled using Box2D.

Box2D is a 2D physics engine
Physics is simulated in 2D, while the game is rendered in 3D


Now here's the critical question: WHY do we need a physics engine for this game?

Gravity is set to ZERO. This isn't a physics simulation where things fall - it's top-down control.
 You apply forces directly with arrow keys. This makes the game about precision and control, not fighting gravity.


Box2D automatically handles collision detection, 
bounce response, and friction, allowing the game to feel smooth and responsive

[6:00 - 6:45] CAMERA.CPP - SMOOTH FOLLOWING
--------------------------------------------
"Camera.cpp 

The camera uses linear interpolation (lerp) to smoothly follow the ball. Instead of snapping directly to the ball position, it gradually moves towards it.

Each frame, we close 10% of the gap between current position and target. This creates a smooth, elastic following effect. When the ball is far, camera moves fast.
 When close, it slows down naturally.

We use glm::lookAt to create the view matrix - the camera looks down at the ball from an angle, giving us that isometric-style perspective.




[6:45 - 7:30] INPUT.CPP - KEYBOARD HANDLING
--------------------------------------------
"Input.cpp handles all keyboard input 

We support three control schemes - arrow keys for traditional control, WASD , and Vim keys (H/J/K/L).

The Input class tracks which keys are currently pressed using a map. When you press a key, we mark it as true. When you release it, we mark it as false.




Multiple keys can be pressed simultaneously - diagonal movement works. The normalize() call  prevents diagonal movement from being faster than straight movement


ESC key, quits the game (
When pressed, exit(0) is called and the program closes immediately.


[7:30 - 9:00] AUDIO.CPP - PROCEDURAL SOUND GENERATION
-----------------------------------------------------
"Audio.cpp is the most interesting file

Audio wasn’t required for the assignment, but I added it because Without sounds, the game feels flat—audio makes every action more satisfying and engaging.

sounds are mix of procedural and file-based audio.

al
    }

If the file doesn't exist, it falls back to procedural generation. 


Procedural sounds are generated using sine waves with decay envelopes to create rolling, 
collision, and victory feedback.


[9:00 - 10:30] RENDERING DETAILS - WALLS, BALL & LIGHTING
----------------------------------------------------------

"Now let me explain some visual details that make the game work.

First, THE WALLS. Each wall is just a brown cube with a brick pattern drawn on top

Then we draw dark brown lines on it to create a brick texture - horizontal and vertical lines.
 Simple but effective!


    shuffle(begin(directions), end(directions), g); 
Every time we generate a maze, we shuffle the directions
This ensures the recursive backtracker makes different choices every playthrough—same algorithm,
 different maze.

Each wall is a static Box2D body, so the ball collides correctly
   
    

Then we create a box shape for it:

    

Box2D automatically handles collisions—if the ball hits a wall, 
it bounces back with friction applied.



    
This makes it look like a metal ball. 

We also add dark grooves that rotate with the ball to show rolling motion

The SHADOW under the ball  - is Not a real shadow, just a transparent gradient circle on the ground

    

   FORCE_MULTIPLIER controls how strong your pushes are
    FRICTION_MULTIPLIER gradually slows the ball, so it doesn’t slide forever

Tweak these numbers and you completely change how the game feels!


The LIGHTING follows the ball everywhere:


    // See game.cpp, line 373: float lightDir[] = {0.0f, 0.0f, -2.0f};  // Point straight down
    // See game.cpp, line 374: glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0f);
    The light is always 2 units above the ball, pointing straight down.
     Soft edges and gradual fade create a focused, realistic effect.


Thanks for watching! If you want to explore the code yourself, it's all there in the src/ folder.


I really enjoyed working on this project and learning from it.
Thank you so much, Doctor, for making the subject enjoyable and easy to understand!
 If you have any feedback or comments, please let me know—I’d love to hear your thoughts.





"Hello! my name is Celina Hajsaleh, Today I'm going to walk you through a complete OpenGL game project I built. 
It's a 3D maze game where you control a ball, navigate through procedurally generated mazes, and find your way out. 

 this is an infinite game. There's no final level. Each time you complete a maze, the next one gets bigger - 
 literally! The maze dimensions increase by 2 in both rows and columns every level. starting from Level 1 is 11×11, level 2 is 13×13, ,
  and it keeps growing forever. 
The game continues until you decide to quit or fall off the map. It's an endless challenge that gets progressively harder.

"On the screen, you’ll always see which level you’re on displayed at the top,
 along with the ball’s current row and column in the maze. If you press the 'i' key, a small box pops up  with instructions 


[0:30 - 1:30] PROJECT OVERVIEW
-------------------------------
"The project is built with C++, OpenGL for rendering, Box2D for physics, and SDL2 for audio.The code is organized 
using object-oriented principles
 so Let's jump into the code!"




[1:30 - 2:30] MAIN.CPP - THE ENTRY POINT
-----------------------------------------
Let’s start with main.cpp, which is the entry point of the application.
Here we initialize GLUT, create the window, and register all callback functions.

Each function pointer tells GLUT what to call when something happens.
 The idle callback runs continuously, giving us our game loop.




[2:30 - 4:00] GAME.CPP - THE HEART OF EVERYTHING
-------------------------------------------------
The Game class is responsible for managing the overall flow of the game

It controls:
Game states
Input handling
Physics updates
Camera movement
Rendering
Level progression


Every frame, input is converted into a force applied to the ball.
Box2D then updates the physics simulation, handling movement and collisions automatically.
After that, the camera smoothly follows the ball, and the game checks whether the player has reached the goal.

For rendering, actual code from game.cpp:
   

The state machine handles all the transitions between game states.
 When a level starts, we begin in FADE_IN - lines 172-178 in game.cpp - which gradually brightens the lighting. Once the fade completes,
  we transition to PLAY where all the action happens. When you reach the goal at the exit, 
    we play the victory sound and technically the game does a fade-out (see game.cpp, line 161: state = FADE_OUT;),
     but it happens so fast you barely notice it—the next level starts almost instantly. 
  we call nextLevel() - lines 165-169 - which increases the maze dimensions and starts the cycle again with FADE_IN lines 172-178.
"


[4:00 - 5:00] MAZE.CPP - PROCEDURAL GENERATION
-----------------------------------------------
"Let's talk about maze.cpp 

We use the recursive backtracker algorithm to generate random mazes. Here's how it works - 
The algorithm starts from a single cell and recursively carves paths by visiting neighboring cells in a randomized order.

The shuffle on line 38  -
Shuffling the directions ensures that the maze layout is different each time, even though the same algorithm is used.
The result is a perfect maze with a single valid path and no isolated areas


When the goal is reached, the game transitions to the next level
The dimension increases infinitely with each level. The code in game.cpp is simple:
    currentLevel++;
    mazeDimension += 2;

 


[5:00 - 6:00] PHYSICS.CPP - BOX2D INTEGRATION
----------------------------------------------
"Physics.cpp Physics is handled using Box2D.

Box2D is a 2D physics engine
Physics is simulated in 2D, while the game is rendered in 3D


Now here's the critical question: WHY do we need a physics engine for this game?

Gravity is set to ZERO. This isn't a physics simulation where things fall - it's top-down control.
 You apply forces directly with arrow keys. This makes the game about precision and control, not fighting gravity.


Box2D automatically handles collision detection, 
bounce response, and friction, allowing the game to feel smooth and responsive

[6:00 - 6:45] CAMERA.CPP - SMOOTH FOLLOWING
--------------------------------------------
"Camera.cpp 

The camera uses linear interpolation (lerp) to smoothly follow the ball. Instead of snapping directly to the ball position, it gradually moves towards it.

Each frame, we close 10% of the gap between current position and target. This creates a smooth, elastic following effect. When the ball is far, camera moves fast.
 When close, it slows down naturally.

We use glm::lookAt to create the view matrix - the camera looks down at the ball from an angle, giving us that isometric-style perspective.




[6:45 - 7:30] INPUT.CPP - KEYBOARD HANDLING
--------------------------------------------
"Input.cpp handles all keyboard input 

We support three control schemes - arrow keys for traditional control, WASD , and Vim keys (H/J/K/L).

The Input class tracks which keys are currently pressed using a map. When you press a key, we mark it as true. When you release it, we mark it as false.

Multiple keys can be pressed simultaneously - diagonal movement works. The normalize() call  prevents diagonal movement from being faster than straight movement


ESC key, quits the game (see input.cpp, line 35: case 27: // ESC key). 
When pressed, exit(0) is called and the program closes immediately.


[7:30 - 9:00] AUDIO.CPP - PROCEDURAL SOUND GENERATION
-----------------------------------------------------
"Audio.cpp is the most interesting file

Audio wasn’t required for the assignment, but I added it because Without sounds, the game feels flat—audio makes every action more satisfying and engaging.

sounds are mix of procedural and file-based audio.

If the file doesn't exist, it falls back to procedural generation. 


Procedural sounds are generated using sine waves with decay envelopes to create rolling, 
collision, and victory feedback.

Look at lines 212-240 in audio.cpp - 


[9:00 - 10:30] RENDERING DETAILS - WALLS, BALL & LIGHTING
----------------------------------------------------------

"Now let me explain some visual details that make the game work.

First, THE WALLS. Each wall is just a brown cube with a brick pattern drawn on top

    

Then we draw dark brown lines on it to create a brick texture - horizontal and vertical lines.
 Simple but effective!


    shuffle(begin(directions), end(directions), g); 
Every time we generate a maze, we shuffle the directions
This ensures the recursive backtracker makes different choices every playthrough—same algorithm,
 different maze.

Each wall is a static Box2D body, so the ball collides correctly

Then we create a box shape for it:

    

Box2D automatically handles collisions—if the ball hits a wall, 
it bounces back with friction applied.



    
This makes it look like a metal ball. 
// See game.cpp, line 347: dark grooves rotation effect
We also add dark grooves that rotate with the ball to show rolling motion

The SHADOW under the ball  - is Not a real shadow, just a transparent gradient circle on the ground
   

   FORCE_MULTIPLIER controls how strong your pushes are
    FRICTION_MULTIPLIER gradually slows the ball, so it doesn’t slide forever

Tweak these numbers and you completely change how the game feels!


The LIGHTING follows the ball everywhere:


   
    The light is always 2 units above the ball, pointing straight down.
     Soft edges and gradual fade create a focused, realistic effect.


Thanks for watching! If you want to explore the code yourself, it's all there in the src/ folder.


I really enjoyed working on this project and learning from it.
Thank you so much, Doctor, for making the subject enjoyable and easy to understand!
 If you have any feedback or comments, please let me know—I’d love to hear your thoughts.




