# Shell to lanuagh the unit test.
TEST_DIR=./Test

# ./Test/SocketManagerTest s&

for file in $(ls $TEST_DIR)
do
	$TEST_DIR/$file
done
