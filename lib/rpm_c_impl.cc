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
#include "rpm_c_impl.h"

namespace gr {
  namespace vbgen {

    rpm_c::sptr
    rpm_c::make(double fWidth, double pWidth, double samp_rate, int nBins)
    {
      return gnuradio::get_initial_sptr(new rpm_c_impl(fWidth, pWidth, samp_rate, nBins));
    }


    /*
     * The private constructor
     */
    rpm_c_impl::rpm_c_impl(double fWidth, double pWidth, double samp_rate, int nBins)
      : gr::sync_block("rpm_c",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
			  o_rng(0, 0, nBins),
			  f_phase(0),
			  d_fw(fWidth),
			  d_pw(pWidth),
			  d_fs(samp_rate),
			  i_n(nBins)
    {}

    /*
     * Our virtual destructor.
     */
    rpm_c_impl::~rpm_c_impl()
    {
    }

    int
    rpm_c_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
	  // initial array pointers and working variables
	  gr_complex *out = (gr_complex *) output_items[0];
	  // loop counts
	  const int spp = int(d_pw * d_fs);
	  const int nPulses = int(noutput_items / spp);
	  int32_t angle;
	  // working/intermediate variables
	  float pts, oi, oq;
	  // for each pulse:
	  float sens = 2.0f * float(d_fw / d_fs) / i_n;
	  float offc = i_n / 2.0f;
	  for (int pti = 0; pti < nPulses; pti++) {
		  // select random point and then scale RNG output into sample frequency
		  pts = (o_rng.ran_int() - offc) * sens;
		  // repeat sample spp times into a pulse, and perform frequency modulation simultaneously
		  for (int j = 0; j < spp; j++) {
			  // cumulative sum on phase,
			  f_phase = f_phase + pts;
			  // compute sin and cos of angle (fixed value)
			  angle = gr::fxpt::float_to_fixed(f_phase * 3.14159f);
			  gr::fxpt::sincos(angle, &oq, & oi);
			  // assign complex modulated value to "out" array
			  *out = gr_complex(oi, oq);
			  // increment pointer
			  out += 1;
		  }
		  // then mod phase sum to 2*PI [-PI, PI] to prevent overruns
		  f_phase = std::fmod(f_phase + 1.0f, 2.0f) - 1.0f;
	  }
      // Tell runtime system how many output items we produced.
      noutput_items = spp * nPulses;
      return noutput_items;
    }

  } /* namespace vbgen */
} /* namespace gr */

