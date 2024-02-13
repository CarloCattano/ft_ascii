#include "ftascii.h"

void initializeTerm(term_t *term) {
    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    srand(time(NULL));
    setlocale(LC_ALL, "en_US.UTF-8");
    system("stty -echo -icanon -icrnl time 0 min 0");

    // Hide cursor
    write(1, NOMOUSE, 6);
    // Clear screen
    write(1, "\033[2J", 4);
    // Initialize term structure
    *term = (term_t){w.ws_col, w.ws_row, w.ws_col * w.ws_row, NULL, NULL, NULL, 1, 1, 0, {0}, 1.0f};
    init_term(term); 
}

static void clip_limits(term_t *term)
{
    term->sens = term->sens > 4.0f ? 4.0f : term->sens;
    term->sens = term->sens < 0.1f ? 0.1f : term->sens;
    term->frame > 2048 ? term->frame = 1 : term->frame++;
}

int ft_ascii(float *fft_values)
{
	term_t term;
	initializeTerm(&term);

	while(1) 
	{
		ft_keyhook(&term);
        clip_limits(&term);
		draw(&term, fft_values, term.sens);
		usleep(term.delay);
        write(1, "\033[H", 3);
    }

	systemExit();
	return 0;
}
