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
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=CLang-Linux-x86
CND_DLIB_EXT=so
CND_CONF=ReleaseClang
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1398168119/CRef.o \
	${OBJECTDIR}/_ext/1475903636/CTeseCase_ErrorLocationInfo.o \
	${OBJECTDIR}/_ext/1475903636/CTestCase_decodeRS.o \
	${OBJECTDIR}/_ext/1475903636/CTestCase_encodeRS.o \
	${OBJECTDIR}/_ext/912457491/OperatorTableTestCase0.o \
	${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeGramma.o \
	${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeOmega.o \
	${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeSyndrome.o \
	${OBJECTDIR}/_ext/1392174414/RSEncodeTestCase.o \
	${OBJECTDIR}/_ext/1392174414/RSEncodesTestCase.o \
	${OBJECTDIR}/_ext/1263938044/EC10Data5CheckTestCase.o \
	${OBJECTDIR}/_ext/1263938044/EC10Data5CheckTestCaseTestSuite.o \
	${OBJECTDIR}/_ext/1263938044/EC4Data2CheckTestCase.o \
	${OBJECTDIR}/_ext/1263938044/EC4Data2CheckTestSuite.o \
	${OBJECTDIR}/_ext/685586909/GaloisField.o \
	${OBJECTDIR}/_ext/685586909/GaloisFieldElement.o \
	${OBJECTDIR}/_ext/685586909/GaloisFieldPolynomial.o \
	${OBJECTDIR}/_ext/685586909/ReedSolomonDecode.o \
	${OBJECTDIR}/_ext/685586909/ReedSolomonEncode.o \
	${OBJECTDIR}/_ext/1445274692/GaliosField8GeneralPoly_0.o \
	${OBJECTDIR}/_ext/1445274692/GaliosField8Table_0.o \
	${OBJECTDIR}/_ext/1445274692/RSGlobal.o \
	${OBJECTDIR}/_ext/1445274692/rs.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread /usr/lib/libgtest.a /usr/lib/libgtest_main.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon: /usr/lib/libgtest.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon: /usr/lib/libgtest_main.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reedsolomon ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1398168119/CRef.o: ../../UnitTest/CRef/CRef.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1398168119
	${RM} $@.d
	$(COMPILE.c) -O2 -std=c99 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1398168119/CRef.o ../../UnitTest/CRef/CRef.c

${OBJECTDIR}/_ext/1475903636/CTeseCase_ErrorLocationInfo.o: ../../UnitTest/CTestCase/CTeseCase_ErrorLocationInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1475903636
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1475903636/CTeseCase_ErrorLocationInfo.o ../../UnitTest/CTestCase/CTeseCase_ErrorLocationInfo.cpp

${OBJECTDIR}/_ext/1475903636/CTestCase_decodeRS.o: ../../UnitTest/CTestCase/CTestCase_decodeRS.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1475903636
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1475903636/CTestCase_decodeRS.o ../../UnitTest/CTestCase/CTestCase_decodeRS.cpp

${OBJECTDIR}/_ext/1475903636/CTestCase_encodeRS.o: ../../UnitTest/CTestCase/CTestCase_encodeRS.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1475903636
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1475903636/CTestCase_encodeRS.o ../../UnitTest/CTestCase/CTestCase_encodeRS.cpp

${OBJECTDIR}/_ext/912457491/OperatorTableTestCase0.o: ../../UnitTest/OperatorTableTestCase/OperatorTableTestCase0.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/912457491
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/912457491/OperatorTableTestCase0.o ../../UnitTest/OperatorTableTestCase/OperatorTableTestCase0.cpp

${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeGramma.o: ../../UnitTest/RSDecodeTestCase/RSDecodeTestCase_computeGramma.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1754011274
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeGramma.o ../../UnitTest/RSDecodeTestCase/RSDecodeTestCase_computeGramma.cpp

${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeOmega.o: ../../UnitTest/RSDecodeTestCase/RSDecodeTestCase_computeOmega.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1754011274
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeOmega.o ../../UnitTest/RSDecodeTestCase/RSDecodeTestCase_computeOmega.cpp

${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeSyndrome.o: ../../UnitTest/RSDecodeTestCase/RSDecodeTestCase_computeSyndrome.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1754011274
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1754011274/RSDecodeTestCase_computeSyndrome.o ../../UnitTest/RSDecodeTestCase/RSDecodeTestCase_computeSyndrome.cpp

${OBJECTDIR}/_ext/1392174414/RSEncodeTestCase.o: ../../UnitTest/RSEncodeTestCase/RSEncodeTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1392174414
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1392174414/RSEncodeTestCase.o ../../UnitTest/RSEncodeTestCase/RSEncodeTestCase.cpp

${OBJECTDIR}/_ext/1392174414/RSEncodesTestCase.o: ../../UnitTest/RSEncodeTestCase/RSEncodesTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1392174414
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1392174414/RSEncodesTestCase.o ../../UnitTest/RSEncodeTestCase/RSEncodesTestCase.cpp

${OBJECTDIR}/_ext/1263938044/EC10Data5CheckTestCase.o: ../../UnitTest/RSTestCase/EC10Data5CheckTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263938044
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1263938044/EC10Data5CheckTestCase.o ../../UnitTest/RSTestCase/EC10Data5CheckTestCase.cpp

${OBJECTDIR}/_ext/1263938044/EC10Data5CheckTestCaseTestSuite.o: ../../UnitTest/RSTestCase/EC10Data5CheckTestCaseTestSuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263938044
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1263938044/EC10Data5CheckTestCaseTestSuite.o ../../UnitTest/RSTestCase/EC10Data5CheckTestCaseTestSuite.cpp

${OBJECTDIR}/_ext/1263938044/EC4Data2CheckTestCase.o: ../../UnitTest/RSTestCase/EC4Data2CheckTestCase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263938044
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1263938044/EC4Data2CheckTestCase.o ../../UnitTest/RSTestCase/EC4Data2CheckTestCase.cpp

${OBJECTDIR}/_ext/1263938044/EC4Data2CheckTestSuite.o: ../../UnitTest/RSTestCase/EC4Data2CheckTestSuite.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1263938044
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1263938044/EC4Data2CheckTestSuite.o ../../UnitTest/RSTestCase/EC4Data2CheckTestSuite.cpp

${OBJECTDIR}/_ext/685586909/GaloisField.o: ../../UnitTest/StandardRef/GaloisField.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/685586909
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/685586909/GaloisField.o ../../UnitTest/StandardRef/GaloisField.cpp

${OBJECTDIR}/_ext/685586909/GaloisFieldElement.o: ../../UnitTest/StandardRef/GaloisFieldElement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/685586909
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/685586909/GaloisFieldElement.o ../../UnitTest/StandardRef/GaloisFieldElement.cpp

${OBJECTDIR}/_ext/685586909/GaloisFieldPolynomial.o: ../../UnitTest/StandardRef/GaloisFieldPolynomial.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/685586909
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/685586909/GaloisFieldPolynomial.o ../../UnitTest/StandardRef/GaloisFieldPolynomial.cpp

${OBJECTDIR}/_ext/685586909/ReedSolomonDecode.o: ../../UnitTest/StandardRef/ReedSolomonDecode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/685586909
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/685586909/ReedSolomonDecode.o ../../UnitTest/StandardRef/ReedSolomonDecode.cpp

${OBJECTDIR}/_ext/685586909/ReedSolomonEncode.o: ../../UnitTest/StandardRef/ReedSolomonEncode.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/685586909
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/685586909/ReedSolomonEncode.o ../../UnitTest/StandardRef/ReedSolomonEncode.cpp

${OBJECTDIR}/_ext/1445274692/GaliosField8GeneralPoly_0.o: ../../src/GaliosField8GeneralPoly_0.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} $@.d
	$(COMPILE.c) -O2 -std=c99 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445274692/GaliosField8GeneralPoly_0.o ../../src/GaliosField8GeneralPoly_0.c

${OBJECTDIR}/_ext/1445274692/GaliosField8Table_0.o: ../../src/GaliosField8Table_0.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} $@.d
	$(COMPILE.c) -O2 -std=c99 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445274692/GaliosField8Table_0.o ../../src/GaliosField8Table_0.c

${OBJECTDIR}/_ext/1445274692/RSGlobal.o: ../../src/RSGlobal.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} $@.d
	$(COMPILE.c) -O2 -std=c99 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445274692/RSGlobal.o ../../src/RSGlobal.c

${OBJECTDIR}/_ext/1445274692/rs.o: ../../src/rs.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/1445274692
	${RM} $@.d
	$(COMPILE.c) -O2 -std=c99 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1445274692/rs.o ../../src/rs.c

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
