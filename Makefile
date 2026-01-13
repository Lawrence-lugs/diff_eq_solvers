
images/forwardEuler.gif: results/forwardEuler.bin	
	python3 plot_results.py results/forwardEuler.bin

results/forwardEuler.bin: build/forwardEuler
	./build/forwardEuler > results/forwardEuler.log
	mv forwardEuler.bin results/

build/forwardEuler: forwardEuler.c build/tensors.o
	cd build && \
	clang tensors.o ../forwardEuler.c -o forwardEuler -O3

build/tensors.o: tensors.c tensors.h
	cd build && \
	clang -c ../tensors.c -o tensors.o

.PHONY: run
run: build/forwardEuler
	./build/forwardEuler 2>&1 | tee results/forwardEuler.log
	mv forwardEuler.bin results/

.PHONY: gif
gif: images/forwardEuler.gif
