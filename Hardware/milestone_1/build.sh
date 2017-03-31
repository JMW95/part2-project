#!/bin/bash

qsys-generate --synthesis=VERILOG gp_socfpga_system.qsys

quartus_sh --flow compile gp_socfpga

quartus_cpf -c convert_to_rbf.cof
