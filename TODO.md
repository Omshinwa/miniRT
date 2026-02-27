# math / raytracing
* implement the lights, do the light calc

# UI / UX
* click to target object, make it moveable
* use mouse instead of keys to turn the cam around
* display infos in the window
* dont make the user mash keys??
* have the render only do 1/100 pixels while moving, then on every frame calculate more pixels

# parse
errors should print to stderr
check for uniqueness of 'C', 'A' etc
brightness should be between 0 and 1 (it doesnt verify it for now)

# errors / cleanup
check for illegal norm (are globals names correctly, defines in the correct place etc)
remove assert()

on exit_and_clean, destroy_image for each obj