GENERATES = *.so
TRASH = *.o *~ o.* *.a

all:
	cc -shared -fPIC protector.c -ldl -o protector.so

test:
	make all
	for fname in not_protected~ FIX_protected~; do \
		touch $${fname}; \
		LD_PRELOAD=`pwd`/protector.so rm $${fname} && \
		echo "successfully deleted: $${fname}" || echo "not permitted: $${fname}" ; \
	done
	make distclean

clean:
	rm -f $(TRASH)

distclean:      clean
	rm -rf $(GENERATES)