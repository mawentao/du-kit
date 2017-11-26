TARGETS = dunew dumk

.PHONY: all clean

all : $(TARGETS)
	rm -rf output
	mkdir -p output/du
	cp du-new/output/* output/du
	cp du-mk/output/* output/du
	cp -r conf output/du/
	cp -r template output/du/
	cd output; find . -type d -name ".svn" | xargs rm -rf; tar zcvf du.tar.gz du/*
	cd ../

dunew : du-new
	make -C $^

dumk : du-mk
	make -C $^

clean :
	rm -rf output
	make clean -C du-new
	make clean -C du-mk
