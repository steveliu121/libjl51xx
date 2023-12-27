# Description

Tool *load_config* is used to load register configurations to Switch Chip over SMI interface.

# Application Guide

- Write your configurations in config_source.txt
- Export *config_source.txt* into *config.h* via script *gen_config.py*
- Build and get tool *load_config*

# Configuration Source File Format

*config_source.txt* is a human readable file. It's composed of a set of syntax lines.
There are 2 types of syntax line:
- Comment Line
- Command Line

## Comment Line
*Commnet Line* starts with "#" or ";". Write your comments here.

## Command Line
There's 5 types of Command:
- Write
- Burst Write(write a set of consecutive registers, at most 8 words could be written in a single Burst Write Command Line)
- Delay(milliseconds)
- Only for SDK(tell script that those lines are only SDK specified)
- Only for EE(tell script that those lines are only EE specified)

### Write Command Line
Command line format:
```
# Write Command Line format
<cmd> <flag> <register address> <value>

# cmd
cmd = WRITE

# flag
flag == 0, Register Direct Access over SMI interface
flag == 1, Register Indirect Access over SMI interface

# register address
if (flag == 0)
register address = ((phy_id << 21) & 0x1F)  |  ((page_id << 5) & 0xFF)  | (reg_id & 0x1F)

if (flag == 1)
register address = register indirect access address

# value
if (flag == 0)
value bitwidth = 16 bits

if (flag == 1)
value bitwidth = 32 bits
```

.e.g:
```
WRITE 0 0x200004 0xde1
WRITE 1 0x100002 0x2800a
```

### Burst Write Command Line
Command line format:
```
# Burst Write Command Line format
<cmd> <flag> <register address> <value0 ...>

# cmd
cmd = BWRITE

# flag
flag = value length in word(should be less than 8)

# register address
register address = register indirect access address

# value
value bitwidth = 32 bits
value length = flag
```

.e.g:
```
BWRITE 3 0x2101077 0x20b8 0x8019 0x7
```

### Delay Command Line
Command Line format:
```
# Delay Command Line format
<cmd> <value>

# cmd
cmd = DELAY

# value
value = delay time in us
value bitwidth = 32 bits
```

.e.g:
```
DELAY 0x100
```

### Only for SDK Command Line
Command Line format:
```
# Only for SDK Command Line format
<cmd> <flag>

# cmd
cmd = ONLY_FOR_SDK

# flag
BEGIN
END
```

.e.g:
```
ONLY_FOR_SDK BEGIN
;lines SDK specified
WRITE 0 0x200004 0xde1
WRITE 1 0x100002 0x2800a
BWRITE 3 0x2101077 0x20b8 0x8019 0x7
DELAY 1000
ONLY_FOR_SDK END
```

### Only for EE Command Line
Command Line format:
```
# Only for EE Command Line format
<cmd> <flag>

# cmd
cmd = ONLY_FOR_EE

# flag
BEGIN
END
```

.e.g:
```
ONLY_FOR_EE BEGIN
;lines EE specified
WRITE 0 0x200004 0xde1
WRITE 1 0x100002 0x2800a
BWRITE 3 0x2101077 0x20b8 0x8019 0x7
DELAY 1000
ONLY_FOR_EE END
```
