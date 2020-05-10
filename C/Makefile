all:
	g++ -c src/GaliosField8GeneralPoly_0.c
	g++ -c src/GaliosField8Table_0.c
	g++ -c src/RSGlobal.c
	g++ -c src/rs.c
	-mkdir build
	ar rs build/libqboxec.a GaliosField8GeneralPoly_0.o GaliosField8Table_0.o RSGlobal.o rs.o
	-rm *.o

clean:
	-rm build/*.a
	-rm *.o
