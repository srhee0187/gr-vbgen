/* -*- c++ -*- */

#define VBGEN_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "vbgen_swig_doc.i"

%{
#include "vbgen/rpm_f.h"
#include "vbgen/rpm_c.h"
#include "vbgen/rpm_cp.h"
#include "vbgen/rpm_cs.h"
%}


%include "vbgen/rpm_f.h"
GR_SWIG_BLOCK_MAGIC2(vbgen, rpm_f);
%include "vbgen/rpm_c.h"
GR_SWIG_BLOCK_MAGIC2(vbgen, rpm_c);
%include "vbgen/rpm_cp.h"
GR_SWIG_BLOCK_MAGIC2(vbgen, rpm_cp);
%include "vbgen/rpm_cs.h"
GR_SWIG_BLOCK_MAGIC2(vbgen, rpm_cs);
