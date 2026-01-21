#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>

/* Initialize button (returns 0 on success, -ENOTSUP if button not present) */
int button_init(void);

/* Returns true if button currently pressed. If button not present, returns false. */
bool button_pressed(void);

#endif /* BUTTON_H */