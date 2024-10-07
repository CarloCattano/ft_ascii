## UNICODE TERMINAL GRAPHICS IN C

### Preface

Interested in rendering graphics to terminal using just *ASCII* + *ANSI* + some *Unicode* characters.

The original idea was to replace the use of minilibx library, in order to run [cub3d](https://github.com/CarloCattano/cub3d)  on the terminal ( 42 )

There are numerous implementations and literature on this matter, however, they always usually rely libs like
termios or ncurses. In here however, we are trying to do it the 42 way, without the strict norm rules.

### Implemented features:
- Keyhooks
- Unicode characters
- ANSI colors
- Terminal setup
- Drawing functions

### To be implemented:
- Resize handling
- Mouse handling
- XOR drawing only whats changed
- std_img convert images to ascii


### other branches:

- main : minimal example drawing a bouncing ball
- origins : Original test implementation with 4 draw heads and a build script.
- pong :
    use case study of ft_ascii as a library for an online pong game.
    With unicode graphics to be used as a CLI game via websockets for multiplayer in the 
    42 school project [ft_transcendence](https://github.com/mc-putchar/ft_transcendence)
- unicode_portaudio:
    Some examples on creating audio reactive visuals like an equalizer etc using portaudio and fft.

