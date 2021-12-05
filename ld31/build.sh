#! /usr/bin/env -S bash -euo pipefail

g++ -O2 *.cpp -o game -I. -I.. -I../bzsf \
	../bzsf/lib/libbzsf.a ../TBC-1.0/lib/libtbc.a \
	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
