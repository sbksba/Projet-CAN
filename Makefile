CC=gcc
CFLAGS=-Wall
LDFLAGS=-L $(LIB) -lcanP2P -lrt
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src
LOG=log
EXEC=CANp2p

all: directories $(EXEC)

directories: ${OBJ} ${BIN} ${LIB} ${LOG}

${OBJ}:
	mkdir -p ${OBJ}

${BIN}:
	mkdir -p ${BIN}

${LIB}:
	mkdir -p ${LIB}

${LOG}:
	mkdir -p ${LOG}

# =====
# OBJET
# =====
${OBJ}/can.o: ${SRC}/can.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/canListe.o: ${SRC}/canListe.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/canEspace.o: ${SRC}/canEspace.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/canInsert.o: ${SRC}/canInsert.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

${OBJ}/main.o: ${SRC}/main.c
	${CC} -c -o $@ $< $(CFLAGS) -I${INC}

# ============
# BIBLIOTHEQUE
# ============
${LIB}/libcanP2P.a : ${OBJ}/can.o ${OBJ}/canListe.o ${OBJ}/canEspace.o ${OBJ}/canInsert.o
	ar -rs $@ $^

# ==========
# EXECUTABLE
# ==========
$(EXEC): ${LIB}/libcanP2P.a ${OBJ}/main.o
	${CC} -o ${BIN}/$@ $^ ${LDFLAGS}

.PHONY: all proper clean cleanall

proper:
	rm -f *~ ${INC}/*~ ${SRC}/*~ ${LOG}/*.log

clean: proper
	rm -f ${OBJ}/* ${BIN}/* ${LIB}/*

cleanall: clean
	rm -r ${OBJ} ${BIN} ${LIB}
