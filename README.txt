Team name: Placeholdertext
Names: Richard Wang
       Spencer Pao

Instructions: Before either program can be compiled, the information about the instance to connect to AWS with must be hardcoded into the two files. For client.c: the public IP address of the instance, user, password, and database name, as well as port number. The key, generated from the keygen function in the paillier library, must be hardcoded into both files.

For SUM_HE.c: compile into .o file with gcc -I/usr/include/mysql/ -c -Wall -fPIC SUM_HE.c  -L/gmp_install/lib -lgmp. Paillier library must be in same directory.

then compile into .so file with gcc -shared -o SUM_HE.so SUM_HE.o -lgmp
then move to correct directory with sudo cp SUM_HE.so /usr/lib/mysql/plugin/

For client program, client.c: gcc -o client $(mysql_config -cflags) client.c $(mysql_config -libs) paillier.o -lgmp
on a machine with mysql libraries (not csil). The client.c file must be in the same directory as the paillier library.

Then call ./client.
