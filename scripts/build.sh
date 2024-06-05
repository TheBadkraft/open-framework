 #!/bin/bash         

#	execute from the VSCode working directory
echo "compile and link " $1
echo "source: src/"$1".asm"
echo "object: obj/"$1".o"
echo "binary: bin/"$2

fasm -m 524288 src/$1.asm obj/$1.o && ld -o bin/$2 obj/$1.o
