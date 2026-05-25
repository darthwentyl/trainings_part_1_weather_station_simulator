.PHONY: ut debug release coverage clean help

ut:
	mkdir build_ut && cd build_ut && cmake .. -DUT=1

debug:
	mkdir build_debug && cd build_debug && cmake ..

release:
	mkdir build_release && cd build_release && cmake .. -DRELEASE=1

coverage:
	./coverage.sh

clean:
	rm -r build_*

help:
	@echo "Targets:"
	@echo " ut       - prepare build for unit tests"
	@echo " debug    - prepare build for binary with debug print"
	@echo " release  - prepare build for binary without debug print"
	@echo " coverage - start generate coverage report"
	@echo " clean    - remove all prepared builds"
	@echo " help     - show this help"