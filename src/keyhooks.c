#include "ftascii.h"
#include <string.h>
#include <unistd.h>

static t_key ft_decode_escape(const char key[4]) {
    if (key[0] != '\033')
        return FT_KEY_NONE;
    if (key[1] != '[')
        return FT_KEY_ESC;
    if (key[2] == ARROW_UP)
        return FT_KEY_UP;
    if (key[2] == ARROW_DOWN)
        return FT_KEY_DOWN;
    if (key[2] == ARROW_LEFT)
        return FT_KEY_LEFT;
    if (key[2] == ARROW_RIGHT)
        return FT_KEY_RIGHT;
    return FT_KEY_ESC;
}

static int ft_event_from_bytes(const char key[4], ssize_t n, t_event *event) {
    if (n <= 0)
        return 0;
    memset(event, 0, sizeof(*event));
    if (key[0] == '\033') {
        event->key = ft_decode_escape(key);
        return 1;
    }
    if (key[0] == 'q' || key[0] == 'p') {
        event->key = FT_KEY_QUIT;
        event->ch  = key[0];
        return 1;
    }
    event->key = FT_KEY_CHAR;
    event->ch  = key[0];
    return 1;
}

int ft_poll_event(t_ascii *ctx, t_event *event) {
    char    key[4];
    ssize_t n;

    if (ctx == NULL || event == NULL)
        return 0;
    memset(event, 0, sizeof(*event));
    if (ft_ascii_check_signal(ctx) == FT_ERR)
        return 0;
    n = read(STDIN_FILENO, key, sizeof(key));
    if (!ft_event_from_bytes(key, n, event))
        return 0;
    if (event->key == FT_KEY_QUIT || event->key == FT_KEY_ESC)
        ft_ascii_stop(ctx);
    return 1;
}

void ft_poll_events(t_ascii *ctx) {
    t_event event;

    while (ft_poll_event(ctx, &event)) {
        /* Agentic note: default policy consumes quit/escape only; callers that
         * need gameplay input should call ft_poll_event() themselves. */
        if (event.key == FT_KEY_QUIT || event.key == FT_KEY_ESC)
            return;
    }
}
