nasm output.asm -felf -o output.o
gcc -m32 output.o -o output
./output
rm output output.o