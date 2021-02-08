/****

	This is the glossary for Step 1, Linux 
	It aims to give precise understanding of the concept used in the course.

****/



`ISA` 	(Instruction Set Architecture): Part of the processor that is visible to the
		programmer or compiler writer. Its serves as the boundary between software and hardware.

`HAL` 	(Hardware Abstract Interface): Layer of programming that allows a computer OS 
		to interact with the hardware device at a abstract level rather than at a 
		detailed hardware level


`ABI` 	(Application Binary Interface): Interface between two binary program modules. 
		Compared with `API`, ABI defines the mechanics of how arguments are passed (registers, stack, etc..), where as API defines which functions are part of the library.


`Syscalls` 	Programmatic way in which a computer prgram requests a service from `Kernel`.

`Kernel`	A computer program at the core of OS,that has complete control over 
			everything in the system. Its is the interface between compurer hardware and processus. Kernal has 4 functions: Memory manager, Process manager, Hardware driver, and `Syscalls` and security manager.

`MBR` 	(Master Boot Record) : A special type of boot sector at the very beginning of
		partitioned computer mass storage devices intended for use with IBM
		PC-compatible systems and beyond. It holds the information about logical partitions, containing filesystems, and also contains the code to function as a loader for the installed OS, by passing control to the loader's second stage.

`Stage 1` (Boot Process) Hardware initialization. Wakes s the processor up, then the 
			other peripherals on the computer's motherboard( including graphics card, which is connected to the monitor).

