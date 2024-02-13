### UNICODE_AUDIO
this branch uses portaudio to get the audio input into -> fftw3 -> we get the amplitude of a set
of frequencies, to use them in our code that renders ascii to terminal using ft_ascii

### Preface

Interested in rendering graphics to terminal using just *ASCII*
The original idea was to replace mlx library with this, in order to run cub3d on the terminal ( 42 )


### The problem / learning opportunity
There are numerous implementations and literature on this matter, however, they always usually rely libs like
termios or ncurses. In here however, we are trying to do it the 42 way, without the strict norm rules.

### Building ( Linux ) 
a convenient build.sh is provided, if you like to suffer with make, cmake, xzmake, go ahead.

Controls:

 - c : enable/disable clearing the screen.
 - p : change char set for drawing _player_ .
 - a/d/w/s : move the player cursor.
 - q : quit gracefully
 - 9 : decrease speed
 - 9 : increase speed 
 - 1 : increase audio sensitivity
 - 2 : decrease audio sensitivity
