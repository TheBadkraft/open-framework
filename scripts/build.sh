 #!/bin/bash
#	requires bash >= 4.x
#	execute from the VSCode working directory         

#	variables
ARGS=( $@ )
ARG_LEN=$#
DEBUG=0
LIBC=0

SRC=''
OBJ=''
BIN=''

#	functions

list_args_usage () {
	echo "${BUILD_TITLE}, version ${BUILD_VERSION} [Bash version ${BASH_VERSION}]"
	echo "Usage:"
	echo "Options:"
	
	exit 0
}

check_bash_version () {
    local major=${1:-4}
    local minor=$2
    local rc=0
    local num_re='^[0-9]+$'

    if [[ ! $major =~ $num_re ]] || [[ $minor && ! $minor =~ $num_re ]]; then
        printf '%s\n' "ERROR: version numbers should be numeric"
        return 1
    fi
    if [[ $minor ]]; then
        local bv=${BASH_VERSINFO[0]}${BASH_VERSINFO[1]}
        local vstring=$major.$minor
        local vnum=$major$minor
    else
        local bv=${BASH_VERSINFO[0]}
        local vstring=$major
        local vnum=$major
    fi
    ((bv < vnum)) && {
        printf '%s\n' "ERROR: Need Bash version $vstring or above, your version is ${BASH_VERSINFO[0]}.${BASH_VERSINFO[1]}"
        rc=1
    }
    
    return $rc
}

set_source () {
	#	get parameter
	local argi=$1
			
	#	is argi > args count
	if [ "${ARGS[argi]}" ]; then	
		SRC="${ARGS[argi]}"
	else
		echo -n "     [ERROR] missing source argument"
		echo
		exit 1
	fi
	
	if [ $DEBUG ]; then
		echo -n "     $SRC"
	fi
}

set_objectoutput () {
	#	get parameter
	local argi=$1
			
	#	is argi > args count
	if [ "${ARGS[argi]}" ]; then	
		OBJ="${ARGS[argi]}"
	else
		echo -n "     [ERROR] missing object output argument"
		echo
		exit 1
	fi
	
	if [ $DEBUG ]; then
		echo -n "     $OBJ"
	fi
}

set_execoutput () {
	#	get parameter
	local argi=$1
			
	#	is argi > args count
	if [ "${ARGS[argi]}" ]; then	
		BIN="${ARGS[argi]}"
	else
		echo -n "     [ERROR] missing binary output argument"
		echo
		exit 1
	fi
	
	if [ $DEBUG ]; then
		echo -n "     $BIN"
	fi
}

#	because I don't like cluttered console screens
clear
MIN_BASH=4
BUILD_TITLE="OpenFramework Build Tool"
BUILD_VERSION="1.0.0-1 (alpha.0)"

check_bash_version $MIN_BASH
if [ $? != 0 ]; then
	exit $?
fi

#	keys
src='-s'
srcx='--src'
obj='-o'
objx='--obj'
bin='-b'
binx='--bin'
libc='-l'
libcx='--libc'
help='-h'
helpx='--help'
debugx='--debug'

#	build args list
arglist=( src, srcx, obj, objx, bin, binx, libc, libcx, help, helpx, debugx )

#	build args dictionary
#declare `A arglist

#	no args given
if [ $ARG_LEN = 0 ]; then
	echo "use -h or --help to list args and usage parameters"
	exit 0
fi
#	if debug
if [[ $@ =~ $debugx ]]; then
	DEBUG=1
	echo "[DEBUG] $(printf %q "$BASH_SOURCE")$((($#)) && printf ' %q' "$@")"
	echo "===================="
	echo
else
	unset DEBUG
fi

unset LIBC

for (( i = 0; i < $ARG_LEN; i++ ))
do

	if [ $DEBUG ]; then
		echo -n $i" "
		echo -n ${ARGS[i]}
	fi
	
	case ${ARGS[i]} in
		$debugx)
			echo
			continue
			;;
		$help | $helpx)
			list_args_usage
			;;
		$libc | $libcx)
			LIBC=1
			echo
			continue
			;;
		$src | $srcx)
			((i++))
			set_source $i
			;;
		$obj | $objx)
			((i++))
			set_objectoutput $i
			;;
		$bin | $binx)
			((i++))
			set_execoutput $i
			;;
		*)
			echo "     unknown arg ${ARGS[i]}"
			echo "===================="
			echo
			list_args_usage
			;;
	esac
	
	if [ $DEBUG ]; then
		echo
	fi
done
 
#	conditionally delete [rm] output files (.o && executable)
if [ -f $OBJ ]; then
	rm $OBJ
fi
if [ -f $BIN ]; then
	rm $BIN
fi

echo "[sucess]     clean ${OBJ}"
echo "[sucess]     clean ${BIN}"

#	stuff ...
echo
echo "execute 'fasm -m 524288 $SRC $OBJ'"
fasm -m 524288 $SRC $OBJ
ret=$?
if ! [ $ret = 0 ]; then
	echo "[error(${ret})]   exit compile ..."
	exit $ret
fi
if ! [ -f $OBJ ]; then
	echo "[fail]       exit compile ..."
	exit 1
else
	echo "[success]     object output:   ${OBJ}"
fi
echo

if [ LIBC ]; then
	echo "execute 'ld -L /usr/lib/x86_64-linux-gnu -l:libc.a -o $BIN $OBJ'"
	ld -L /usr/lib/x86_64-linux-gnu -l:libc.a -o $BIN $OBJ
else
	echo "ececute 'ld -o $BIN $OBJ'"
	ld -o $BIN $OBJ
fi

if ! [ -f $BIN ]; then
	echo "[fail]        exit linker ..."
	exit 1
else
	echo "[success]     linker:    ${BIN}"
fi
