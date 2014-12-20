CS 3GC3 Final Project - LowPoly Volcano
December 20th, 2014

Anagh Goswami - 1217426
Jake Harwood - 1226732
Ryan Lambert - 1218407
Simon Berhane - 1140677
Stuart Douglas - 1214422

*********************************************

SUMMARY

We’ve created a volcanic and mountainous island landscape, with rocks, trees, and a monkey statue. Lava shoots out of the volcano and slides down it, charring the ground beneath it. If it hits water, steam shoots upwards. Snow also falls upon the land, turning it white upon landing. Both the snow and lava can be toggled from the pause menu. Upon going underwater or hitting lava, your character dies and a screen with options to restart or quit appears. Restarting will bring you back to the island but it has a hellish red tone.

RUNNING / COMPILING 

Included is a .exe file that should work on Windows. If not, just run “make” to use the makefile provided. Tested (successfully) on OS X, Windows 7, Windows 8.1 and Linux. Note that the “images” folder needs to be in the same directory you’re running the application from.

ADDITIONAL GRAPHICS FEATURES

Lighting : lighting is implemented
Textures : our skybox is textured with images we Photoshopped using screenshots from the project to look like similar islands far off
Particle Systems : we used our particle systems for the lava, snow and steam
Non-Geometric primitives : we made a ppm image for the pause menu and the death menu
Advanced Camera Control : our camera system is FPS style
Object Loader : we modelled the rocks, trees, and monkey statue in Blender and created an object loader to load them into the scene

CONTROLS

WASD : walk around (hint: hold down SHIFT to sprint)
mouse : look around (FPS style)
p : pause or unpause (escape also works). A pause menu will be presented on pausing
g : toggle fog (default off)
m : toggle projection matrix as orthogonal or perspective (default perspective)
f : toggle fullscreen (default on)
q : quit