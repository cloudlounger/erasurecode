#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/2007297078/EC10Data5CheckTestCase.o \
	${OBJECTDIR}/_ext/2007297078/EC10Data5CheckTestSuite.o \
	${OBJECTDIR}/_ext/2007297078/EC4Data2CheckTestCase.o \
	${OBJECTDIR}/_ext/2007297078/EC4Data2CheckTestSuite.o \
	${OBJECTDIR}/_ext/242478311/GaloisFieldTableTestCase0.o \
	${OBJECTDIR}/_ext/242478311/GaloisFieldTableTestCase1.o \
	${OBJECTDIR}/_ext/1077655445/GaloisFieldTestCase.o \
	${OBJECTDIR}/_ext/1980371744/ReedSolomonDeCodeTestCase_.o \
	${OBJECTDIR}/_ext/1980371744/ReedSolomonEnCodeTestCase.o \
	${OBJECTDIR}/_ext/1386528437/GaloisField.o \
	${OBJECTDIR}/_ext/1386528437/GaloisFieldElement.o \
	${OBJECTDIR}/_ext/1386528437/GaloisFieldPolynomial.o \
	${OBJECTDIR}/_ext/1386528437/ec.o \
	${OBJECTDIR}/_ext/1867565493/GaloisFieldElementTestCase.o \
	${OBJECTDIR}/_ext/148558358/GaloisFieldPolynomialTestCase.o \
	${OBJECTDIR}/_ext/479120213/ReedSolomonDecode.o \
	${OBJECTDIR}/_ext/479120213/ReedSolomonEncode.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fprofile-arcs -ftest-coverage
CXXFLAGS=-fprofile-arcs -ftest-coverage

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lgtest -lgtest_main -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon ${OBJECTFILES} ${LDLIBSOPTIONS} -fprofile-arcs

${OBJECTDIR}/_ext/2007297078/EC10Data5CheckTestCase.o: ../../../UnitTest/ECTestCase/EC10Data5CheckTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2007297078
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2007297078/EC10Data5CheckTestCase.o ../../../UnitTest/ECTestCase/EC10Data5CheckTestCase.cpp

${OBJECTDIR}/_ext/2007297078/EC10Data5CheckTestSuite.o: ../../../UnitTest/ECTestCase/EC10Data5CheckTestSuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2007297078
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2007297078/EC10Data5CheckTestSuite.o ../../../UnitTest/ECTestCase/EC10Data5CheckTestSuite.cpp

${OBJECTDIR}/_ext/2007297078/EC4Data2CheckTestCase.o: ../../../UnitTest/ECTestCase/EC4Data2CheckTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2007297078
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2007297078/EC4Data2CheckTestCase.o ../../../UnitTest/ECTestCase/EC4Data2CheckTestCase.cpp

${OBJECTDIR}/_ext/2007297078/EC4Data2CheckTestSuite.o: ../../../UnitTest/ECTestCase/EC4Data2CheckTestSuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2007297078
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2007297078/EC4Data2CheckTestSuite.o ../../../UnitTest/ECTestCase/EC4Data2CheckTestSuite.cpp

${OBJECTDIR}/_ext/242478311/GaloisFieldTableTestCase0.o: ../../../UnitTest/GaloisFieldTableTestCase/GaloisFieldTableTestCase0.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/242478311
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/242478311/GaloisFieldTableTestCase0.o ../../../UnitTest/GaloisFieldTableTestCase/GaloisFieldTableTestCase0.cpp

${OBJECTDIR}/_ext/242478311/GaloisFieldTableTestCase1.o: ../../../UnitTest/GaloisFieldTableTestCase/GaloisFieldTableTestCase1.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/242478311
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/242478311/GaloisFieldTableTestCase1.o ../../../UnitTest/GaloisFieldTableTestCase/GaloisFieldTableTestCase1.cpp

${OBJECTDIR}/_ext/1077655445/GaloisFieldTestCase.o: ../../../UnitTest/GaloisFieldTestCase/GaloisFieldTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1077655445
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1077655445/GaloisFieldTestCase.o ../../../UnitTest/GaloisFieldTestCase/GaloisFieldTestCase.cpp

${OBJECTDIR}/_ext/1980371744/ReedSolomonDeCodeTestCase_.o: ../../../UnitTest/ReedSolomonCodeTestCase/ReedSolomonDeCodeTestCase\ .cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1980371744
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1980371744/ReedSolomonDeCodeTestCase_.o ../../../UnitTest/ReedSolomonCodeTestCase/ReedSolomonDeCodeTestCase\ .cpp

${OBJECTDIR}/_ext/1980371744/ReedSolomonEnCodeTestCase.o: ../../../UnitTest/ReedSolomonCodeTestCase/ReedSolomonEnCodeTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1980371744
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1980371744/ReedSolomonEnCodeTestCase.o ../../../UnitTest/ReedSolomonCodeTestCase/ReedSolomonEnCodeTestCase.cpp

${OBJECTDIR}/_ext/1386528437/GaloisField.o: ../../../src/GaloisField.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1386528437
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1386528437/GaloisField.o ../../../src/GaloisField.cpp

${OBJECTDIR}/_ext/1386528437/GaloisFieldElement.o: ../../../src/GaloisFieldElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1386528437
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1386528437/GaloisFieldElement.o ../../../src/GaloisFieldElement.cpp

${OBJECTDIR}/_ext/1386528437/GaloisFieldPolynomial.o: ../../../src/GaloisFieldPolynomial.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1386528437
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1386528437/GaloisFieldPolynomial.o ../../../src/GaloisFieldPolynomial.cpp

${OBJECTDIR}/_ext/1386528437/ec.o: ../../../src/ec.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1386528437
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1386528437/ec.o ../../../src/ec.cpp

${OBJECTDIR}/_ext/1867565493/GaloisFieldElementTestCase.o: /home/wxt/Dev/Qiniu/ec/C++/UnitTest/GaloisFieldElementTestCase/GaloisFieldElementTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1867565493
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1867565493/GaloisFieldElementTestCase.o /home/wxt/Dev/Qiniu/ec/C++/UnitTest/GaloisFieldElementTestCase/GaloisFieldElementTestCase.cpp

${OBJECTDIR}/_ext/148558358/GaloisFieldPolynomialTestCase.o: /home/wxt/Dev/Qiniu/ec/C++/UnitTest/PolynomialTestCase/GaloisFieldPolynomialTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/148558358
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/148558358/GaloisFieldPolynomialTestCase.o /home/wxt/Dev/Qiniu/ec/C++/UnitTest/PolynomialTestCase/GaloisFieldPolynomialTestCase.cpp

${OBJECTDIR}/_ext/479120213/ReedSolomonDecode.o: /home/wxt/Dev/Qiniu/ec/C++/src/ReedSolomonDecode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/479120213
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/479120213/ReedSolomonDecode.o /home/wxt/Dev/Qiniu/ec/C++/src/ReedSolomonDecode.cpp

${OBJECTDIR}/_ext/479120213/ReedSolomonEncode.o: /home/wxt/Dev/Qiniu/ec/C++/src/ReedSolomonEncode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/479120213
	${RM} $@.d
	$(COMPILE.cc) -g -fprofile-arcs -ftest-coverage -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/479120213/ReedSolomonEncode.o /home/wxt/Dev/Qiniu/ec/C++/src/ReedSolomonEncode.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
