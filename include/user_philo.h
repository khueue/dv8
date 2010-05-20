#ifndef USER_PHILO_H
#define USER_PHILO_H

#define NUM_PHILO 5

#define CMD_LEFT_CHOPSTICK 0
#define CMD_RIGHT_CHOPSTICK 1
#define CMD_PUT_BACK_CHOPSTICKS 2
#define CMD_LEAVE_TABLE 3

void
philosopher(void);

void
dtable(void);

void
dphilo_init(void);

#endif
