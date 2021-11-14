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

#ifndef INCLUDED_VBGEN_RPM_CSP_IMPL_H
#define INCLUDED_VBGEN_RPM_CSP_IMPL_H

#include <vbgen/rpm_csp.h>
#include <gnuradio/random.h>

namespace gr {
  namespace vbgen {

    class rpm_csp_impl : public rpm_csp
    {
     private:
      gr::random o_rng;
      float f_phase;
      float f_pts;
      double d_fw;
      double d_pw;
      double d_fs;
      int i_n;
			double d_bw;
			double d_sc;
      float f_spts;
			long int ii_sc;
      long int ii_pw;

     public:
      rpm_csp_impl(double fWidth, double pWidth, double samp_rate, int nBins, double cov, double scWidth);
      ~rpm_csp_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace vbgen
} // namespace gr

#endif /* INCLUDED_VBGEN_RPM_CSP_IMPL_H */

