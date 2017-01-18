.PHONY: all run dump-clean clean

export CC=clang
export CXX=clang++

all:
	mkdir -p obj
	cd obj && cmake ../
	cd obj && make 
run:all dump-clean
	mv ./obj/pmnn.out ./
	./pmnn.out
clean:
	rm -rf obj
	rm -rf *.out
dump-clean:
	rm -rf dump/*.png

format:
	
