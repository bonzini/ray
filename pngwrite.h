// Example ray tracing program
// PNG writing routines
// Written by Paolo Bonzini, 2003

#ifndef PTGEN_PNGWRITE_H
#define PTGEN_PNGWRITE_H

#include <iostream>

// Scrive un file PNG utilizzando la libreria libpng.  Se questa non
// e' disponibile, o se c'e' un errore, ritorna false.

bool write_png (std::ostream & os, const char *img, int width, int height);

#endif
