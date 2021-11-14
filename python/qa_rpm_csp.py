#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2021 steve.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import vbgen_swig as vbgen
import time

class qa_rpm_csp (gr_unittest.TestCase):
	def setUp(self):
		self.tb = gr.top_block()
	def tearDown(self):
		self.tb = None
	def test_001_t(self):
		fWidth = 200.0e3
		pWidth = 100.0e-6
		samp_rate = 4.0e6
		nBins = 64
		cov = 1.0e6
		scRate = 100.0e3
		src = vbgen.rpm_csp(fWidth, pWidth, samp_rate, nBins, cov, 1/scRate)
		thrt = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate, True)
		dst = blocks.null_sink(gr.sizeof_gr_complex*1)
		self.tb.connect(src, thrt)
		self.tb.connect(thrt, dst)
		self.tb.start()
		time.sleep(1)
		self.tb.stop()


if __name__ == '__main__':
    gr_unittest.run(qa_rpm_csp, "qa_rpm_csp.xml")
