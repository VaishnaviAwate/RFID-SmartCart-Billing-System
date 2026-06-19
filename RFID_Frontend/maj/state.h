#ifndef STATE_H
#define STATE_H

typedef enum {
    STATE_IDLE,
    STATE_ADD,
    STATE_DELETE,
    STATE_PAYMENT,
    STATE_ADMIN
} State;

extern State state;

#endif
