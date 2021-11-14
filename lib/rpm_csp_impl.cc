/* -*- c++ -*- */
/* 
 * Copyright 2021 steve.
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
#include "rpm_csp_impl.h"

namespace gr {
  namespace vbgen {

    rpm_csp::sptr
    rpm_csp::make(double fWidth, double pWidth, double samp_rate, int nBins, double cov, double scWidth)
    {
      return gnuradio::get_initial_sptr
        (new rpm_csp_impl(fWidth, pWidth, samp_rate, nBins, cov, scWidth));
    }

    /*
     * The private constructor
     */
    rpm_csp_impl::rpm_csp_impl(double fWidth, double pWidth, double samp_rate, int nBins, double cov, double scWidth)
      : gr::sync_block("rpm_csp",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
			  d_rng(0, 0, nBins),
			  d_phase(0),
			  d_fw(fWidth),
			  d_pw(pWidth),
			  d_fs(samp_rate),
			  d_n(nBins),
			  d_bw(cov),
			  d_sc(scWidth),
			  i_sc(0)
    {}

    /*
     * Our virtual destructor.
     */
    rpm_csp_impl::~rpm_csp_impl()
    {
    }

    int
    rpm_csp_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // initial array pointers and working variables
	  gr_complex *out = (gr_complex *) output_items[0];
	  // loop counts
	  const int spp = int(d_pw * d_fs);
	  const int nPulses = int(noutput_items / spp);
	  int32_t angle;
	  // subcarrier constants
	  const long int sps = int(d_fs / d_sc);
	  const long int hsps = int(sps / 2);
	  const float slp = 2.0f * d_bw / sps / d_fs;
	  // working/intermediate variables
	  float pts, oi, oq;
	  float spts = 0;
	  // for each pulse (centered on rpm pulses, not subcarrier sweeps):
	  float sens = 2.0f * float(d_fw / d_fs) / d_n;
	  float offc = d_n / 2.0f;
	  for (int pti = 0; pti < nPulses; pti++) {
		  // select random point and then scale RNG output into sample frequency
		  pts = (d_rng.ran_int() - offc) * sens;
		  // repeat sample spp times into a pulse, and perform frequency modulation simultaneously
		  for (int j = 0; j < spp; j++) {
			  // cumulative sum on slope index (sc) and mod w.r.t. sps
			  i_sc += 1;
			  i_sc = i_sc % sps; // replace mod with if/else chain with reversed slope sign (triangular subs)
			  // cumulative sum on phase (sc)
			  spts += slp * (i_sc - hsps);
			  // cumulative sum on phase (rpm)
			  d_phase += pts;
			  // compute sin and cos of angle (fixed value)
			  angle = gr::fxpt::float_to_fixed((d_phase + spts) * 3.14159f);
			  gr::fxpt::sincos(angle, &oq, & oi);
			  // assign complex modulated value to "out" array
			  *out = gr_complex(oi, oq);
			  // increment pointer
			  out += 1;
		  }
		  // then mod phase sum to 2*PI [-PI, PI] to avoid (eventual) overruns
		  d_phase = std::fmod(d_phase + 1.0f, 2.0f) - 1.0f;
		  spts = std::fmod(spts + 1.0f, 2.0f) - 1.0f;
	  }
      // Tell runtime system how many output items we produced.
      noutput_items = spp * nPulses;
      return noutput_items;
    }

  } /* namespace vbgen */
} /* namespace gr */

