/*
  INF01142 - Sistemas Operacionais I
  T2FS - 2017/2

  Douglas Lazaro

  Header para simplificar o include dentro dos arquivos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apidisk.h"
#include "t2fs.h"

#include "constants.h"
#include "types.h"

#include "helpers/print.h"
#include "helpers/util.h"
#include "helpers/files.h"
#include "helpers/ldaa.h"
#include "helpers/fat.h"

#include "disk.h"
#include "parse.h"
#include "files.h"

extern struct Configuration config;
extern struct Constants constants;
