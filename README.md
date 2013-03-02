oodb
====

This library is something between redis and sqlite.
I needed portable database for my desktop app and sqlite is a great solution
of this problem, but I didnt need all that power and might of SQL. All i needed
was hashes, strings and numbers. So actually it shoul be something like redis,
but portable.

Building.

Linux:

Move to oodb and execute "CXX=clang++ make .". The result would be a static
library.

Windows:

Just use cmake to generate solution and build it.
