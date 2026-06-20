.PHONY: build test clean sdist wheel rpm deb

build:
	python3 setup.py build

test: build
	@BUILD_DIR=$$(ls -d build/lib.*-*-* 2>/dev/null | head -1); \
	[ -n "$$BUILD_DIR" ] || (echo "Error: build directory not found." && exit 1); \
	PYTHONPATH=$$BUILD_DIR python3 -m unittest tests.test -v

sdist:
	python3 setup.py sdist

wheel: build
	python3 setup.py bdist_wheel

SRCNAME  := python3-libconfig
VERSION  := $(shell python3 setup.py --version 2>/dev/null)

rpm:
	mkdir -p ~/.rpmbuild/SOURCES
	git archive HEAD --format=tar.gz --prefix=$(SRCNAME)-$(VERSION)/ \
		-o ~/.rpmbuild/SOURCES/$(SRCNAME)-$(VERSION).tar.gz
	rpmbuild -ba python3-libconfig.spec

clean:
	rm -rf build dist pylibconfig.egg-info *.deb
	find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true
	find . -name '*.pyc' -delete 2>/dev/null || true