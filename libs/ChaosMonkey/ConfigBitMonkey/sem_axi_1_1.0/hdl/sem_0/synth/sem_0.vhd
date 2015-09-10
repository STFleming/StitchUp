-- (c) Copyright 1995-2015 Xilinx, Inc. All rights reserved.
-- 
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
-- 
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
-- 
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
-- 
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
-- 
-- DO NOT MODIFY THIS FILE.

-- IP VLNV: xilinx.com:ip:sem:4.0
-- IP Revision: 3

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

LIBRARY sem_v4_0;
USE sem_v4_0.sem_v4_0;

ENTITY sem_0 IS
  PORT (
    status_heartbeat : OUT STD_LOGIC;
    status_initialization : OUT STD_LOGIC;
    status_observation : OUT STD_LOGIC;
    status_correction : OUT STD_LOGIC;
    status_classification : OUT STD_LOGIC;
    status_injection : OUT STD_LOGIC;
    status_essential : OUT STD_LOGIC;
    status_uncorrectable : OUT STD_LOGIC;
    monitor_txdata : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
    monitor_txwrite : OUT STD_LOGIC;
    monitor_txfull : IN STD_LOGIC;
    monitor_rxdata : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
    monitor_rxread : OUT STD_LOGIC;
    monitor_rxempty : IN STD_LOGIC;
    inject_strobe : IN STD_LOGIC;
    inject_address : IN STD_LOGIC_VECTOR(39 DOWNTO 0);
    icap_o : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    icap_csib : OUT STD_LOGIC;
    icap_rdwrb : OUT STD_LOGIC;
    icap_i : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    icap_clk : IN STD_LOGIC;
    icap_request : OUT STD_LOGIC;
    icap_grant : IN STD_LOGIC;
    fecc_crcerr : IN STD_LOGIC;
    fecc_eccerr : IN STD_LOGIC;
    fecc_eccerrsingle : IN STD_LOGIC;
    fecc_syndromevalid : IN STD_LOGIC;
    fecc_syndrome : IN STD_LOGIC_VECTOR(12 DOWNTO 0);
    fecc_far : IN STD_LOGIC_VECTOR(25 DOWNTO 0);
    fecc_synbit : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
    fecc_synword : IN STD_LOGIC_VECTOR(6 DOWNTO 0)
  );
END sem_0;

ARCHITECTURE sem_0_arch OF sem_0 IS
  ATTRIBUTE DowngradeIPIdentifiedWarnings : string;
  ATTRIBUTE DowngradeIPIdentifiedWarnings OF sem_0_arch: ARCHITECTURE IS "yes";

  COMPONENT sem_v4_0 IS
    GENERIC (
      c_xdevice : STRING;
      c_xpackage : STRING;
      c_xspeedgrade : STRING;
      c_xdevicefamily : STRING;
      c_family : STRING;
      c_device_array : INTEGER;
      c_icapwidth : INTEGER;
      c_eipwidth : INTEGER;
      c_farwidth : INTEGER;
      c_component_name : STRING;
      c_clock_per : INTEGER;
      c_feature_set : INTEGER;
      c_hardware_cfg : INTEGER;
      c_software_cfg : INTEGER;
      b_debug : INTEGER;
      b_cosim : INTEGER;
      b_dfset : INTEGER;
      b_gen_user_app : BOOLEAN
    );
    PORT (
      status_heartbeat : OUT STD_LOGIC;
      status_initialization : OUT STD_LOGIC;
      status_observation : OUT STD_LOGIC;
      status_correction : OUT STD_LOGIC;
      status_classification : OUT STD_LOGIC;
      status_injection : OUT STD_LOGIC;
      status_essential : OUT STD_LOGIC;
      status_uncorrectable : OUT STD_LOGIC;
      fetch_txdata : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
      fetch_txwrite : OUT STD_LOGIC;
      fetch_txfull : IN STD_LOGIC;
      fetch_rxdata : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
      fetch_rxread : OUT STD_LOGIC;
      fetch_rxempty : IN STD_LOGIC;
      fetch_tbladdr : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      monitor_txdata : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
      monitor_txwrite : OUT STD_LOGIC;
      monitor_txfull : IN STD_LOGIC;
      monitor_rxdata : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
      monitor_rxread : OUT STD_LOGIC;
      monitor_rxempty : IN STD_LOGIC;
      inject_strobe : IN STD_LOGIC;
      inject_address : IN STD_LOGIC_VECTOR(39 DOWNTO 0);
      icap_o : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      icap_csib : OUT STD_LOGIC;
      icap_rdwrb : OUT STD_LOGIC;
      icap_i : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      icap_clk : IN STD_LOGIC;
      icap_request : OUT STD_LOGIC;
      icap_grant : IN STD_LOGIC;
      fecc_crcerr : IN STD_LOGIC;
      fecc_eccerr : IN STD_LOGIC;
      fecc_eccerrsingle : IN STD_LOGIC;
      fecc_syndromevalid : IN STD_LOGIC;
      fecc_syndrome : IN STD_LOGIC_VECTOR(12 DOWNTO 0);
      fecc_far : IN STD_LOGIC_VECTOR(25 DOWNTO 0);
      fecc_synbit : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
      fecc_synword : IN STD_LOGIC_VECTOR(6 DOWNTO 0)
    );
  END COMPONENT sem_v4_0;
  ATTRIBUTE X_CORE_INFO : STRING;
  ATTRIBUTE X_CORE_INFO OF sem_0_arch: ARCHITECTURE IS "sem_v4_0,Vivado 2013.4";
  ATTRIBUTE CHECK_LICENSE_TYPE : STRING;
  ATTRIBUTE CHECK_LICENSE_TYPE OF sem_0_arch : ARCHITECTURE IS "sem_0,sem_v4_0,{}";
  ATTRIBUTE CORE_GENERATION_INFO : STRING;
  ATTRIBUTE CORE_GENERATION_INFO OF sem_0_arch: ARCHITECTURE IS "sem_0,sem_v4_0,{x_ipProduct=Vivado 2013.4,x_ipVendor=xilinx.com,x_ipLibrary=ip,x_ipName=sem,x_ipVersion=4.0,x_ipCoreRevision=3,x_ipLanguage=VERILOG,c_xdevice=zynq,c_xpackage=clg484,c_xspeedgrade=-1,c_xdevicefamily=zynq,c_family=zynq,c_device_array=67108866,c_icapwidth=32,c_eipwidth=40,c_farwidth=26,c_component_name=sem_0,c_clock_per=125000,c_feature_set=14,c_hardware_cfg=5,c_software_cfg=7,b_debug=0,b_cosim=0,b_dfset=0,b_gen_user_app=false}";
BEGIN
  U0 : sem_v4_0
    GENERIC MAP (
      c_xdevice => "zynq",
      c_xpackage => "clg484",
      c_xspeedgrade => "-1",
      c_xdevicefamily => "zynq",
      c_family => "zynq",
      c_device_array => 67108866,
      c_icapwidth => 32,
      c_eipwidth => 40,
      c_farwidth => 26,
      c_component_name => "sem_0",
      c_clock_per => 125000,
      c_feature_set => 14,
      c_hardware_cfg => 5,
      c_software_cfg => 7,
      b_debug => 0,
      b_cosim => 0,
      b_dfset => 0,
      b_gen_user_app => false
    )
    PORT MAP (
      status_heartbeat => status_heartbeat,
      status_initialization => status_initialization,
      status_observation => status_observation,
      status_correction => status_correction,
      status_classification => status_classification,
      status_injection => status_injection,
      status_essential => status_essential,
      status_uncorrectable => status_uncorrectable,
      fetch_txfull => '0',
      fetch_rxdata => STD_LOGIC_VECTOR(TO_UNSIGNED(0, 8)),
      fetch_rxempty => '0',
      fetch_tbladdr => STD_LOGIC_VECTOR(TO_UNSIGNED(0, 32)),
      monitor_txdata => monitor_txdata,
      monitor_txwrite => monitor_txwrite,
      monitor_txfull => monitor_txfull,
      monitor_rxdata => monitor_rxdata,
      monitor_rxread => monitor_rxread,
      monitor_rxempty => monitor_rxempty,
      inject_strobe => inject_strobe,
      inject_address => inject_address,
      icap_o => icap_o,
      icap_csib => icap_csib,
      icap_rdwrb => icap_rdwrb,
      icap_i => icap_i,
      icap_clk => icap_clk,
      icap_request => icap_request,
      icap_grant => icap_grant,
      fecc_crcerr => fecc_crcerr,
      fecc_eccerr => fecc_eccerr,
      fecc_eccerrsingle => fecc_eccerrsingle,
      fecc_syndromevalid => fecc_syndromevalid,
      fecc_syndrome => fecc_syndrome,
      fecc_far => fecc_far,
      fecc_synbit => fecc_synbit,
      fecc_synword => fecc_synword
    );
END sem_0_arch;
