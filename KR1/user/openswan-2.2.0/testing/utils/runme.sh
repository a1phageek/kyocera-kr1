#!/bin/sh

#
# $Id: runme.sh,v 1.1 2004/12/24 07:17:41 rupert Exp $
#
# use this script to run a single test from within that test directory.
# note that this assumes a "klipstest" type test.
#

. ../../../umlsetup.sh
. ../setup.sh
. $UTILS/functions.sh
. testparams.sh

if [ -z "${TEST_TYPE}" ]
then
    echo runme.sh now requires that testparams.sh defines TEST_TYPE=
fi

( cd .. && $TEST_TYPE $TESTNAME good )

perl ${OPENSWANSRCDIR}/testing/utils/regress-summarize-results.pl ${REGRESSRESULTS} notest


# $Log: runme.sh,v $
# Revision 1.1  2004/12/24 07:17:41  rupert
# +: Add OPENSWANS Package
#
# Revision 1.3  2004/05/28 02:07:02  mcr
# 	update test case summary at end of manual run.
#
# Revision 1.2  2002/05/23 14:26:39  mcr
# 	verify that $TEST_TYPE is actually set.
#
# Revision 1.1  2002/05/05 23:12:05  mcr
# 	runme.sh script now common for all test types.
#
# 

