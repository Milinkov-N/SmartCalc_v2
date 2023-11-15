NAME       := SmartCalcV2

CXX        := g++
CXXFLAGS   := -std=c++17 -Wall -Werror -Wextra
LDFLAGS    := -lgtest
CKFLAGS    := -lgcov --coverage

TEST_SRC   := tests/*.cc

BUILD_DIR  := build

all: test build run

install: build
	cp -rf $(BUILD_DIR)/$(NAME).app $(HOME)/Applications/
	make clean

uninstall: 
	rm -rf $(HOME)/Applications/$(NAME).app

clean: clean_test
	rm -rf $(BUILD_DIR)*/ report/ report.info

dvi:
	open dvi.html

dist: build
	mkdir -p dist
	tar -cvf ./dist/$(NAME).tar $(BUILD_DIR)/$(NAME).app dvi.html
	gzip ./dist/$(NAME).tar

.PHONY: test
test: clean_test
	mkdir -p $(BUILD_DIR) \
		&& $(CXX) $(CXXFLAGS) $(LDFLAGS) $(CKFLAGS) -Imodel model/model.cc $(TEST_SRC) -o $(BUILD_DIR)/tests \
		&& ./$(BUILD_DIR)/tests

gcov_report: test
	lcov -t "$<" -o report.info -c -d .
	lcov --remove report.info \
		$(shell pwd)/tests/* \
		/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/* \
		/usr/local/include/c++/10.3.0/* \
		/usr/local/include/c++/10.3.0/bits/* \
		/usr/local/include/c++/10.3.0/ext/* \
		/usr/local/include/gtest/* \
		/usr/local/include/gtest/internal/* \
		-o report.info
	genhtml -o report report.info
	open ./report/index.html

.PHONY: rebuild
rebuild: clean build

.PHONY: build
build:
	mkdir -p $(BUILD_DIR) \
		&& cd $(BUILD_DIR) \
		&& qmake .. \
		&& make

.PHONY: run
run:
	./$(BUILD_DIR)/$(NAME).app/Contents/MacOS/$(NAME)

.PHONY: clean_test
clean_test:
	rm -rf $(BUILD_DIR)/tests *.gcda *.gcno
