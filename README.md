# Before the Beta release, this will need to be updated to work as andrew said when we recieved the alpha back.

List of changes before Beta release:

-Create images for the text? Definitley for the title, maybe for other parts? Could actually shorten the amount of lines!!

-Fix up collision detection

-Add music.

-Make random generation with fireballs and walls actually random (currently they are the artificial random). (Not sure if this is nessasary at this point)

-Change walls a bit to make it a more interesting stage. Thinking rather than just moving in the walls randomly, we should have a set of pre-determined stages which it randomly selects on of.

-Fix instructions on how to set up librarys.

-Add a video showing the finished beta.

-Update Documentation

# DUNGEON FALL

COSC345 Group Project Otago University 2019

Ben Highsted (5536340), Matthew Neil (7388248), Jasmine Hindson (8148181)

We are creating a game called 'Dungeon Game' (name to be changed) programmed in C, using the SDL2 library (https://www.libsdl.org/download-2.0.php).

We created our file in Xcode, so to build and run our program you will need to use the XCode project file and then download a couple SDL Library files and place them in specific locations on your PC. These instructions are currently only valid for MACOS operating systems.

First, you will need to download SDL 2.0 from here https://www.libsdl.org/download-2.0.php. Then you will need to place the frame work into your /Library/Frameworks folder (use command + shift + g on finder and use it to search for and find the folder).

Next, you will need to download SDL_TTF from https://www.libsdl.org/projects/SDL_ttf/. Then you will need to place that frame work in your frameworks folder also.

Lastly, you will need to use homebrew to install sdl2_image onto your machine. To install homebrew, simply type /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" into your MACOS terminal, and it will install. Once its installed or if you already have it, type 'brew install sdl2_image'. This will install sdl2 image into the correct directory and after doing this your librarys will all be setup to build and run our game.

In the ZIP folder you download will be a Xcode project which, if you set up the librarys correctly, will have no issues to build and run. If you wish to run it through Xcode (rather than the executable provided), you will need to open the Xcode project, on the left side of the screen will be some folders (i.e. Headers, Assignment, Products) under the products folder will be an executable. If you right click it, and click open in finder (you should be taken to a folder called Debug), and then copy all the images given directly next to the executable, images will now work when running in Xcode. (see https://imgur.com/a/e5xLjzg)

An exectuable will also be contained in another folder (called ExectuableRelease), which will have an executable and some images, which if you double click to run (and allow it in system preferences -> security and privacy) it will run our program outside of Xcode.

Here is a video showing our Alpha Release: https://www.youtube.com/watch?v=GJCgZItPklY.

