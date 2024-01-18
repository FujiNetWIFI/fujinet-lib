# fn_io

Contained here is a CMOC library called libdw which allows for DriveWire communication.

It provides two functions:

## byte dwread(byte *s, int l);

Read bytes to *s from DriveWire with expected length l.

* s must be a pointer to a string of bytes to send.
* l is the length of bytes to send from that pointer. (1-65535)
* the function will return FALSE if fewer than l bytes are received, otherwise TRUE.

***note*** Ensure that your pointer has enough space.

### dwread example

```c
byte buf[256];

// ...

if (!dwread(&buf[0],sizeof(buf)))
{
	printf("ERROR: SHORT READ.");
	return FALSE;
}

// ...

```

## byte *dwwrite(byte *s, int l);

Write l number of bytes from *s to DriveWire.

* s must be a pointer to a string of bytes to write.
* l is the number of bytes to write starting at this pointer. (1-65535)
* the function will return a pointer to the last byte written.

### dwwrite example

```c
void reset_fujinet(void)
{
	byte reset_cmd[2]={0xE2,0xFF};
	
	dwwrite(reset_cmd,sizeof(reset_cmd));
}
```

## author

Thomas Cherryhomes &lt;thom dot cherryhomes at gmail dot com&gt;

## license

GNU Public License v. 3, see LICENSE for details.
