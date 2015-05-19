
BldPath=bld
SrcPath=src
DepPath=dep
CxPath=$(DepPath)/cx

CMAKE=cmake
GODO=$(CMAKE) -E chdir
MKDIR=$(CMAKE) -E make_directory

.PHONY: default
default:
	if [ ! -d $(BldPath)/ext ] ; then $(MAKE) cmake ; fi
	$(MAKE) proj

.PHONY: all
all:
	$(MAKE) cmake
	$(MAKE) proj

.PHONY: cmake
cmake:
	if [ ! -d $(BldPath) ] ; then $(MKDIR) $(BldPath) ; fi
	if [ ! -d $(BldPath)/ext ] ; then $(MKDIR) $(BldPath)/ext ; fi
	$(GODO) $(BldPath)/ext $(CMAKE) ../..

.PHONY: proj
proj:
	$(GODO) $(BldPath)/ext $(MAKE)

.PHONY: test
test:
	$(GODO) $(BldPath) $(MAKE) test

.PHONY: clean
clean:
	$(GODO) $(BldPath) $(MAKE)/ext clean


