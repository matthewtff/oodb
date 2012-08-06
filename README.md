oodb
====

This library is something between redis and sqlite.
I needed portable database for my desktop app and sqlite is a great solution
of this problem, but I didnt need all that power and might of SQL. All i needed
was hashes, strings and numbers. So actually it shoul be something like redis,
but portable.

Building.

Linux:

Move to oodb/src and execute "make". The result would be shared and static
library.

Windows:

Just create a project for static library and add files from oodb/src to it.


Tests:

For now there are no tests for this library :(
