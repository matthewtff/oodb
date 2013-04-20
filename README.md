oodb
====

This library is something between redis and sqlite.
I needed portable database for my desktop app and sqlite is a great solution
of this problem, but I didnt need all that power and might of SQL. All i needed
was hashes, strings and numbers. So actually it should be something like redis,
but portable.

Building.

Linux:

Move to oodb and execute "CXX=clang++ make .". The result would be a static
library. Make sure you've got libc++ (by LLVM) installed. Problem using gcc
and libstdc++ is absence of std::regex support.

Windows:

Just use cmake to generate solution and build it.

MacOS:

It also compiles on MacOS using clang.
