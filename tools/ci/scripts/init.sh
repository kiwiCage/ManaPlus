#!/bin/bash

pwd
mkdir logs

export dir=$(pwd)
export ERRFILE=${dir}/logs/${LOGFILE}

cat /etc/os-release
$CC --version
$CXX --version

rm ${ERRFILE}

function aptget_update {
    apt-get update
    if [ "$?" != 0 ]; then
        sleep 1s
        apt-get update
        if [ "$?" != 0 ]; then
            sleep 1s
            apt-get update
        fi
    fi
}

function aptget_install {
    apt-get -y -qq install $*
    if [ "$?" != 0 ]; then
        sleep 1s
        apt-get -y -qq install $*
        if [ "$?" != 0 ]; then
            sleep 2s
            apt-get -y -qq install $*
        fi
    fi
}

function run_configure_simple {
    rm $ERRFILE
    autoreconf -i 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi

    rm $ERRFILE
    ./configure $* 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi
}

function run_configure {
    run_configure_simple $*

    rm $ERRFILE
    cd po
    make update-gmo 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi
    cd ..

    rm $ERRFILE
    cd po
    make update-po 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi
    cd ..
}

function run_cmake {
    rm $ERRFILE
    cmake . 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi
}

function run_make {
    rm $ERRFILE
    make -j2 V=0 $* 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi
}

function run_check_warnings {
    DATA=$(cat $ERRFILE)
    if [ "$DATA" != "" ];
    then
        cat $ERRFILE
        exit 1
    fi
}

function run_h {
    rm $ERRFILE
    $CC -c -x c++ $* $includes */*/*/*.h */*/*.h */*.h *.h 2>$ERRFILE
    DATA=$(cat $ERRFILE)
    if [ "$DATA" != "" ];
    then
        cat $ERRFILE
        exit 1
    fi
}

function run_tarball {
    rm $ERRFILE
    make dist-xz 2>$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi

    mkdir $1
    cd $1
    tar xf ../*.tar.xz
    cd manaplus*
}

function run_mplint {
    rm $ERRFILE
    mplint/src/mplint $* >$ERRFILE
    result=$?
    if [ "$result" != 0 ]; then
        cat $ERRFILE
        exit $result
    fi
}

aptget_update