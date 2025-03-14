clear
gcc -Wall -Wextra -Werror -ansi -pedantic -o proj2 *.c
cd iaed-p2/
echo "----community_tests-----"
./test.sh ../proj2 community_tests/
echo "----p1_mooshak_tests-----"
./test.sh ../proj2 p1-mooshak-tests/
echo "----public_tests-----"
./test.sh ../proj2 public_tests/
cd -
