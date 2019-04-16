# ProcessExplorer-Windows
A windows application I made that scans all running processes on your system and displays information about them.

I made it with the windows API, C++/CLI and the .NET framework.

This is my first project in C++/CLI, the .NET framework, and the windows API. I motivate myself to learn new languages through projects. This program works like task manager, it will scan all the running processes on your computer and display them neatly with some information, such as #threads and modules, etc. There's nothing that my program does that windows tasks manager & process explorer don't already do, but making this was a fun learning experience. I did most of my research on MSDN's documentation of the windows API and .NET programming.

The release binary I included is compiled 32-bit with debug settings from visual studio's compiler. Any optimization from compiling debug vs release settings should be negligible with the scale of my program. I suggest you run this as an admin, since some protected processes may require certain privileges to open a handle to.

Most of my takeaways from this project will just be the usage of C++/CLI and .NET in general, but I also did learn a lot about the internals of operating systems while reading MSDN's docs, such as how tasks are scheduled, the stack, how a process's memory is split up/structured by the OS, and lots of new terminology!
