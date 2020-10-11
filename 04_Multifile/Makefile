GENERATES = prog prog-a prog-so README *.so
TRASH = *.o *~ o.* *.a

all:	README prog

prog:	const.o fun.o prog.o

prog-a:	prog.o liboutput_static.a
	cc -L. prog.o -lout -o prog-a

prog-so: prog.o liboutput.so
	cc -L. prog.o -lout -o prog-so

liboutput_static.a: const.o fun.o
	ar -rcs libout.a const.o fun.o

liboutput.so: const.o fun.o
	cc -shared fun.o const.o -o liboutput.so

test:	prog prog-a prog-so
	for program in $^ ; do \
		./$$program 1>$${program}_out_no_param~ 2>$${program}_err~; \
		./$$program param 1>$${program}_out_1_param~; \
		./$$program param pam pam 1>$${program}_out_3_param~; \
	done
	for param_count in no_param 1_param 3_param; do \
		for program in $^ ; do \
			for other_prog in $^ ; do \
				cmp $${program}_out_$${param_count}~ $${other_prog}_out_$${param_count}~ ; \
			done \
		done \
	done
	make clean


README:	prog
	./$< 2> $@

fun.o:	outlib.h

clean:
	rm -f $(TRASH)

distclean:      clean
	rm -rf $(GENERATES)
