/* -*- c++ -*- */
/* 
 * Copyright 2021 gr-vbgen author.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/fxpt.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <cmath>
#include "rpm_f_impl.h"

namespace gr {
  namespace vbgen {

    rpm_f::sptr
    rpm_f::make(double fWidth, double pWidth, double samp_rate, int nBins)
    {
      return gnuradio::get_initial_sptr
        (new rpm_f_impl(fWidth, pWidth, samp_rate, nBins));
    }

    /*
     * The private constructor
     */
    rpm_f_impl::rpm_f_impl(double fWidth, double pWidth, double samp_rate, int nBins)
      : gr::sync_block("rpm_f",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(float))),
			  d_rng(0, 0, nBins),
			  f_phase(0),
			  d_fw(fWidth),
			  d_pw(pWidth),
			  d_fs(samp_rate),
			  i_n(nBins)
    {}

    /*
     * Our virtual destructor.
     */
    rpm_f_impl::~rpm_f_impl()
    {
    }

    int
    rpm_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
#define F_PI ((float)(GR_M_PI))
	  // initial array pointers and working variables
	  float *out = (float *) output_items[0];
	  // loop counts
	  set_spp();
	  int spv = get_spp();
	  set_nPulses(noutput_items);
	  int npp = get_nPulses();
	  // working/intermediate variables
	  float pts = 0;
	  // for each pulse:
	  for (int pti = 0; pti < npp; ++pti) {
		  // select random points and then scale RNG output into sample of modulating waveform (in rads)
		  pts = (float(d_rng.ran_int()) - float(i_n) / 2.0f) * float(d_fw) / float(i_n);
		  // repeat sample spv times into a pulse
		  for (int j = 0; j < spv; ++j) {
			  memcpy(out, &pts, sizeof(float));
			  // increment pointer
			  out += 1;
		  }
	  }
      // Tell runtime system how many output items we produced.
      noutput_items = spv * npp;
      return noutput_items;
    }

  } /* namespace vbgen */
} /* namespace gr */

