# Lightweight Embedded Scheduler
This is a highly portable, lightweight, non-preemptive, priority scheduler for embedded systems written in C.

## How to get started
To make use of the scheduler in your awesome embedded-system projects, follow these steps:
1. Implement and adjust the functions located in *system.c* according to the underlying hardware.
2. Edit *task.h* and *task.c* by adding your new tasks.
3. In your initialization code, call `schedulerInit` to automatically add your tasks to the scheduler queue.
4. In your super-loop code, call `schedulerUpdate` to let the scheduler manage your tasks.

## Credits
This project has been inspired by the [Betaflight](https://github.com/betaflight/betaflight) open-source flight controller.
