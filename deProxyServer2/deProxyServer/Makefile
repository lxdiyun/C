###################################################################
# @file Makefile
# 
# ProxyServer Makefile 
# 
# Created at Mon Dec 10 21:02:39 CST 2007
#
# Created by AD_Li
###################################################################

########### DIRECTORIES TO CONFIGURE ##############################

SRC_DIR = ./src
OUT_DIR = ./OUTPUT
BIN_DIR = ./BIN
TEST_IN_DIR = $(SRC_DIR)/Test
TEST_OUT_DIR = ./Test
SHELL_DIR = ./shell
DOC_OUT_DIR = ./DOC_OUTPUT

###################################################################

########### FILE LIST #############################################

DOXYGEN_CFG_FILE = ./ProxyServerDOXY.cfg

###################################################################

########### Other Variable ########################################

# comiple command
CC = colorg++ -Wall -O2 

###################################################################

# ---------
# functions
# ---------

# Do The Test
define DO_UNIT_TEST
@echo "-------- Start Unit Test --------"
sh $(SHELL_DIR)/do_unit_test.sh
@echo "-------- Unit Test Complete --------"
endef

# Setup The Test Dir
define SETUP_TEST_DIR
# if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi 
# if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi
if [ ! -d $(TEST_OUT_DIR) ]; then mkdir $(TEST_OUT_DIR); fi
endef


# -------
# Targets
# ------- 

Main: Build

Setup_Test_Dir: $(TEST_OUT_DIR)


$(TEST_OUT_DIR):
	mkdir $(TEST_OUT_DIR)

clean_all: clean_build clean_test clean_doc
	@echo "-------- Clean All Done --------"

clean_build: 
	rm -f *.o ProxyServer

clean_test:
	rm -rf $(TEST_OUT_DIR)

clean_doc:
	rm -rf $(DOC_OUT_DIR)
clean_output:
	rm -rf *.o

doc:
	doxygen $(DOXYGEN_CFG_FILE)

test: Setup_Test_Dir build_test
	$(DO_UNIT_TEST)

# -----------
# Build Rules
# -----------

Build: ProxyServer
	@echo "-------- Build Complete -------- "

ProxyServer: $(SRC_DIR)/ProxyServer.cpp \
	SocketManager.o ThreadManager.o HTTPPack.o
	${CC} $^ -o $@ -lpthread

ThreadManager.o: %.o : $(SRC_DIR)/%.cpp 
	$(CC) -c $< -o $@

SocketManager.o: %.o: $(SRC_DIR)/connection/%.cpp 
	$(CC) -c $< -o $@

HTTPPack.o: %.o: $(SRC_DIR)/%.cpp 
	$(CC) -c $< -o $@

build_test: 	$(TEST_OUT_DIR)/SocketManagerTest \
		$(TEST_OUT_DIR)/HTTPPackTest
	@echo "-------- Unit Test Compile Complete --------"

$(TEST_OUT_DIR)/SocketManagerTest: \
	$(TEST_IN_DIR)/SocketManagerTest.cpp SocketManager.o
	$(CC) $^ -o $@ 

$(TEST_OUT_DIR)/HTTPPackTest: \
	$(TEST_IN_DIR)/HTTPPackTest.cpp HTTPPack.o
	$(CC) $^ -o $@

