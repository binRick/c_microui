default: all
##############################################################
PWD=$(shell command -v pwd)
DIR=$(shell $(PWD))
VENV_DIR=$(DIR)/.venv
GIT=$(shell command -v git)
SED=$(shell command -v gsed||command -v sed)
NODEMON=$(shell command -v nodemon)
UNCRUSTIFY=$(shell command -v uncrustify)
LOC=$(shell command -v loc)
##############################################################
MESON=$(VENV_DIR)/bin/meson
NINJA=$(VENV_DIR)/bin/ninja
##############################################################
SOURCE_VENV_CMD=source $(VENV_DIR)/bin/activate
NM_TARGET=nodemon	
##############################################################
TIDIED_FILES = \
			   *mui*/*.c *mui*/*.h
##############################################################
run:
	@$(DIR)/build/mui-test/mui-test
python-venv: do-python-venv python-venv-meson
do-python-venv:
	@[[ -f $(VENV_DIR)/bin/activate ]] ||  { python3 -m venv $(VENV_DIR) && $(SOURCE_VENV_CMD) && pip3 install pip -U; }
	@true
python-venv-meson:
	@[[ -e $(MESON) ]] || { $(SOURCE_VENV_CMD) && pip3 install meson -U; }
	@[[ -e $(NINJA) ]] || { $(SOURCE_VENV_CMD) && pip3 install ninja -U; }
	@true
do-loc: 
	@$(LOC) \
		--files \
		--exclude 'submodules' \
		--exclude 'subprojects' \
		--exclude 'build' \
		--exclude '.cache'
loc: do-loc
uncrustify:
	@$(UNCRUSTIFY) -c etc/uncrustify.cfg --replace $(TIDIED_FILES)||true
uncrustify-clean:
	@find  . -type f -maxdepth 2 -name "*unc-back*"|xargs -I % unlink %
clean:
	@rm -rf build .cache
clean-venv: 
	@[[ -d $(VENV_DIR) && rm -rf $(VENV_DIR)
	@true
fix-dbg:
	@$(SED) 's|, % s);|, %s);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % lu);|, %lu);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % d);|, %d);|g' -i $(TIDIED_FILES)
	@$(SED) 's|, % zu);|, %zu);|g' -i $(TIDIED_FILES)
install: all do-meson-install
do-meson-install:
	@cd build && meson install
do-meson:
	@$(MESON) build || { meson build --reconfigure || { meson build --wipe; } && meson build; }
do-ninja:
	@$(NINJA) -C build
do-ninja-test:
	@$(NINJA) test -C build -v
do-nodemon:
	@$(NODEMON) \
		--delay .1 \
		-w "meson_options.txt" \
		-w "mui/*.c" -w "mui/*.h" \
		-w "mui-*/*.c" -w "mui-*/*.h" \
		-w "*-test/*.c" -w "*-test/*.h" \
		-w 'meson/meson.build' \
		-w 'meson/deps/*/meson.build' \
		-w 'meson.build' \
		-w Makefile \
		-i 'build/*' \
			-e Makefile,build,sh,c,h,txt \
			-x env -- bash -c 'make nodemon||true'
git-submodules-pull:
	@$(GIT) submodule foreach git pull origin master --jobs=10
git-submodules-update:
	@$(GIT) submodule update --init
git-pull:
	@$(GIT) pull --recurse-submodules
do-uncrustify: uncrustify uncrustify-clean fix-dbg
do-build: do-meson do-ninja do-test
do-test: do-ninja-test
test: do-test
build: do-meson do-build
ansi: all do-sync do-ansi-make
tidy: \
	do-uncrustify \
	do-build \
	git-add
dev: clean do-nodemon
do-setup: do-clear python-venv
do-clear:
	@clear
all: do-setup do-loc do-build 
dev-nodemon: clean all
nodemon: all
meson-introspect-targets:
	@meson introspect --targets -i meson.build
meson-binaries:
	@meson introspect --targets  meson.build -i | jq 'map(select(.type == "executable").filename)|flatten|join("\n")' -Mrc
