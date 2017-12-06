# autogit
A Linux kernel module that inits a git folder, writes, commits, and pushes it to remote.

## Compilation
### Build
Clone the repository and run:

    # make
Note that you need to have the linux kernel headers installed for your running kernel version.

To insert the module into the kernel, run:

    # insmod autogit.ko
    OR
    # make load

To unload the module (and clear the logs), run:

    # rmmod autogit

## Developer
Copyright (C) 2017 [WinstonDu](mailto:wenhaodu694@gmail.com)