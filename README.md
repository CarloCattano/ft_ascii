### WIP 
Branches as of today are:

- unicode_portaudio : Audio reactive unicode ( bug with wide unicodes ) 

- audio_minimal : fft modulated visuals via audio input

- refactor : try to go full unicode and per char color support

- main : simple drawing that draws different ascii brushes

### Preface

Interested in rendering graphics to terminal using just *ASCII*
The original idea was to replace mlx library with this, in order to run cub3d on the terminal ( 42 )
Its a WIP experimenting around this in order to see whats needed.

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
